/*
 * =====================================================================================
 *
 *       Filename:  mainwindow.cpp
 *
 *           Note:  MainWindow class handles all UI logic for main window of TaskPad
 *        Version:  1.0
 *        Created:  09/21/13 16:13:09
 *
 *         Author:  XIE KAI (A0102016E), gigikie@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "MainWindow.h"
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTextBlock>
#include <QShortcut>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <cassert>
#include "CommandBar.h"
#include "Enum.h"
#include "libqxt/qxtglobalshortcut.h"
#include "ListItemDelegate.h"
#include "QuickAddWindow.h"
#include "Manager.h"

const double MainWindow::DESIRED_TRANSPARENT_OPACITY	= 0.4;
const int   MainWindow::LENGTH_TOO_LONG					= 77;
const int   MainWindow::STEP							= 1;
const char* MainWindow::NEGLECTED						= "...";
const char* MainWindow::TODAY_VIEW						= "Today";
const char* MainWindow::INBOX_VIEW						= "Inbox";
const char* MainWindow::REMINDERS_VIEW					= "Reminders";
const char* MainWindow::DATE_DD_MM						= "dd/MM";
const char* MainWindow::DATE_DD_MM_YY					= "dd/MM/yy";
const char* MainWindow::DATE_DD_MM_YYYY					= "dd/MM/yyyy";
const char* MainWindow::DATE_HH_MM						= "hh:mm";
const char* MainWindow::DATE_DD_MM_HH_MM				= "dd/MM hh:mm";
const char* MainWindow::DATE_DD_MM_YYYY_HH_MM			= "dd/MM/yyyy hh:mm";
const char* MainWindow::DATE_YEAR						= "yyyy";
const char* MainWindow::EMPTY                           = "";
const char* MainWindow::DUE_STR							= "Due ";
const char* MainWindow::FROM_STR						= "From ";
const char* MainWindow::TO_STR							= "To ";
const char* MainWindow::FROM_TO_STR						= " to ";
const char* MainWindow::NEW_LINE                        = "\n";
const char* MainWindow::DEFAULT_WIN_TITLE               = "TaskPad";
const char* MainWindow::DEFAULT_STATUS_BAR_TEXT         = "Ready";
const char* MainWindow::DEFAULT_DETAILS_VIEW_TEXT       = "Task's Details";
const char* MainWindow::SUCCESS_DETAILS_ADD_TEXT        = "Added Task's Details";
const char* MainWindow::SUCCESS_DETAILS_MOD_TEXT        = "Modified Task's Details";
const char* MainWindow::SUCCESS_DETAILS_DEL_TEXT        = "Deleted Task's Details";
const char* MainWindow::SUCCESS_DETAILS_UNDO_TEXT       = "Undo Task's Details";
const char* MainWindow::SUCCESS_DETAILS_REDO_TEXT       = "Redo Task's Details";
const char* MainWindow::SUCCESS_STATUS_BAR_ADD_TEXT     = "Task added successfully";
const char* MainWindow::SUCCESS_STATUS_BAR_DEL_TEXT     = "Task deleted successfully";
const char* MainWindow::SUCCESS_STATUS_BAR_MOD_TEXT     = "Task modified successfully";
const char* MainWindow::SUCCESS_STATUS_BAR_FIND_TEXT    = "Searched successfully";
const char* MainWindow::SUCCESS_STATUS_BAR_UNDO_TEXT    = "Undo successfully";
const char* MainWindow::SUCCESS_STATUS_BAR_REDO_TEXT    = "Redo successfully";
const char* MainWindow::SUCCESS_STATUS_BAR_DISPLAY_TEXT = "Task displayed successfully";
const char* MainWindow::SUCCESS_NAV_LABEL_FIND_CMD		= "Search Results";
const char* MainWindow::INTERMEDIATE_NAV_LABEL_INTERMEDIATE_STAGE = "Select a task by typing its index";
const char* MainWindow::INTERMEDIATE_STATUS_BAR_INTERMEDIATE_STAGE = "Multiple results were found";
const char* MainWindow::ERROR_NAV_LABLE_INTERMEDIATE_STAGE = "Select a task by typing its index";
const char* MainWindow::ERROR_STATUS_BAR_INTERMEDIATE_STAGE = "Wrong input. Kindly press ECS to cancel";
const char* MainWindow::ERROR_ONLY_SUPPORT_ADD_N_DISPLAY_ONE_CMD = "Only Add Command and Display 1 are supported";

/************************************************************************/
/*                        INITIALIZATION                                */
/************************************************************************/

MainWindow::MainWindow(QWidget *parent)
:QMainWindow(parent){
	setupUI();
	setupDependency();
	getToday();
}

void MainWindow::setupUI(){
	ui.setupUi(this);
	customisedUi();
	installEventFilter();
}


//************************************
// Method: customisedUi
// Description: 1. make window metro
// style (borderless). 2. beautify tasklist
// column width
//************************************
void MainWindow::customisedUi(){
	const int _1ST_COLUMN = 0;
	const int _2ND_COLUMN = 1;
	const int DESIRED_1ST_COL_WIDTH = 70;
	const int DESIRED_2ND_COL_WIDTH = 220;
	detailsViewOpacity = DESIRED_TRANSPARENT_OPACITY;
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.errorBgWidget->setHidden(true);
	ui.TaskList->header()->resizeSection(_1ST_COLUMN, DESIRED_1ST_COL_WIDTH);
	ui.TaskList->header()->resizeSection(_2ND_COLUMN, DESIRED_2ND_COL_WIDTH);
}

void MainWindow::installEventFilter(){
	ui.CloseButton->installEventFilter(this);
	ui.MinimizeButton->installEventFilter(this);
	ui.HelpButton->installEventFilter(this);
	ui.AboutButton->installEventFilter(this);
	ui.cmdBar->installEventFilter(this);
	ui.TaskList->installEventFilter(this);
}

void MainWindow::setupDependency(){
	setupTimer();
	setupTrayIcon();
	setupConnection();
	setupHotkeys();
	setupScheduler();
}

void MainWindow::setupTimer(){
	const int CYCLE_ONE_MIN = 60000;
	timer = new QTimer(this);
	timer->start(CYCLE_ONE_MIN);
}

void MainWindow::setupTrayIcon(){
	const char* LOGO_PATH = ":/MainWindow/Resources/logo.png";
	isQuickAddOpen = false;

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon(LOGO_PATH));
	trayIcon->show();
	trayIcon->setToolTip(DEFAULT_WIN_TITLE);
}

void MainWindow::setupConnection(){
	//call showReminder every 60 sec
	connect(timer, SIGNAL(timeout()),this, SLOT(showReminder()));
	//handleShowReminder when try msgbox is clicked in reminder mode
	connect(trayIcon, SIGNAL(messageClicked()),this, SLOT(handleShowReminder()));
	//show mainwindow when click tray icon
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, 
		SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
	//close mainwindow when click close button
	connect(ui.CloseButton, SIGNAL(clicked()), this, SLOT(close()));
	//minimize mainwindow when click minimum button
	connect(ui.MinimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
	//show about window
	connect(ui.AboutButton, SIGNAL(clicked()), this, SLOT(about()));
	//show help window
	connect(ui.HelpButton, SIGNAL(clicked()), this, SLOT(help()));
	//handle selected task in the list
	connect(ui.TaskList, SIGNAL(itemSelectionChanged()), this, SLOT(handleListSelection()));
}

void MainWindow::setupHotkeys(){
	setupGlobalHotkeys();
	setupWinDisplayHotkeys();
	setupDateNavHotkeys();
}

void MainWindow::setupGlobalHotkeys(){
	const char* OPEN_QUICK_ADD_WIN = "Alt+`";
	const char* OPEN_MAIN_WIN = "Ctrl+Alt+t";
	QxtGlobalShortcut * scOpenQuickAddWin = new QxtGlobalShortcut(QKeySequence(OPEN_QUICK_ADD_WIN), this);
	connect(scOpenQuickAddWin, SIGNAL(activated()),this, SLOT(showQuickAddWindow()));
	QxtGlobalShortcut * scOpenMainWin = new QxtGlobalShortcut(QKeySequence(OPEN_MAIN_WIN), this);
	connect(scOpenMainWin, SIGNAL(activated()),this, SLOT(showWindow()));
}

void MainWindow::setupWinDisplayHotkeys(){
	const char* SHOW_REMINDER_RIGHT_NOW = "F5";
	const char* MINIMIZE_MAIN_WIN = "Ctrl+H";
	const char* SHOW_TODAY_1 = "Ctrl+T";
	const char* SHOW_TODAY_2 = "Alt+1";
	const char* SHOW_INBOX_1 = "Ctrl+I";
	const char* SHOW_INBOX_2 = "Alt+2";
	(void) new QShortcut(QKeySequence(SHOW_REMINDER_RIGHT_NOW), this, SLOT(showReminder()));
	(void) new QShortcut(QKeySequence(MINIMIZE_MAIN_WIN), this, SLOT(showMinimized()));
	(void) new QShortcut(QKeySequence(SHOW_TODAY_1), this, SLOT(getToday()));
	(void) new QShortcut(QKeySequence(SHOW_TODAY_2), this, SLOT(getToday()));
	(void) new QShortcut(QKeySequence(SHOW_INBOX_1), this, SLOT(getInbox()));
	(void) new QShortcut(QKeySequence(SHOW_INBOX_2), this, SLOT(getInbox()));
}

void MainWindow::setupDateNavHotkeys(){
	const char* SHOW_NEXT_DAY = "Alt+D";
	const char* SHOW_NEXT_WEEK = "Alt+W";
	const char* SHOW_NEXT_MONTH = "Alt+M";
	const char* SHOW_PREV_DAY = "Alt+Shift+D";
	const char* SHOW_PREV_WEEK = "Alt+Shift+W";
	const char* SHOW_PREV_MONTH = "Alt+Shift+M";
	(void) new QShortcut(QKeySequence(SHOW_NEXT_DAY), this, SLOT(showNextDay()));
	(void) new QShortcut(QKeySequence(SHOW_PREV_DAY), this, SLOT(showPrevDay()));
	(void) new QShortcut(QKeySequence(SHOW_NEXT_WEEK), this, SLOT(showNextWeek()));
	(void) new QShortcut(QKeySequence(SHOW_PREV_WEEK), this, SLOT(showPrevWeek()));
	(void) new QShortcut(QKeySequence(SHOW_NEXT_MONTH), this, SLOT(showNextMonth()));
	(void) new QShortcut(QKeySequence(SHOW_PREV_MONTH), this, SLOT(showPrevMonth()));
}


//************************************
// Method:    setupScheduler
// Note: Business logic setup here
//************************************
void MainWindow::setupScheduler(){
	scheduler = new Manager();
}

//************************************
// Method:    setupQuickAddWindow
// Note: **no need to delete quickAddWindowPtr,
// it will be deleted automatically. Otherwise
// abort, since delete twice!**
//************************************
void MainWindow::setupQuickAddWindow(){
	quickAddWindowPtr = new QuickAddWindow();
	quickAddWindowPtr->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::setupQuickAddConnection(){
	QuickAddWindow *qa = (QuickAddWindow*) quickAddWindowPtr;
	connect(qa, SIGNAL(windowClosed()), this, SLOT(closeQuickAddWindow()));
	connect(qa, SIGNAL(requestSubmitted(QString)), this, SLOT(handleQuickAddRequest(QString)));
}

/************************************************************************/
/*                            DESTRUCTOR                                */
/************************************************************************/

MainWindow::~MainWindow(){
	dispose();
}

void MainWindow::dispose(){
	//must hide trayIcon before window exits
	trayIcon->hide();
	delete scheduler;
	scheduler = NULL;
}

void MainWindow::closeQuickAddWindow(){
	disposeQuickAddWindow();
}

void MainWindow::disposeQuickAddWindow(){
	disposeQuickAddConnection();
	quickAddWindowPtr->close();
	setIsQuickAddOpen(false);
}

void MainWindow::disposeQuickAddConnection(){
	QuickAddWindow *qa = (QuickAddWindow*) quickAddWindowPtr;
	disconnect(qa, SIGNAL(windowClosed()), this, SLOT(closeQuickAddWindow()));
	disconnect(qa, SIGNAL(requestSubmitted(QString)), this, SLOT(handleQuickAddRequest(QString)));
}

/************************************************************************/
/*                        WINDOW DISPLAY                                */
/************************************************************************/

void MainWindow::iconIsActived(QSystemTrayIcon::ActivationReason){
	showWindow();
}

void MainWindow::showWindow(){
	show();
	setWindowState(Qt::WindowActive);
	ui.cmdBar->setFocus();
}

void MainWindow::showReminder(){
	QString output;
	list<Task> reminderList = scheduler->getCurrentReminders();
	if(!reminderList.empty()){
		setCurrRemindTime();
		output = prepareTrayMsg(reminderList, output);
		showTrayMsg(output, REMINDERS_VIEW);
		setFromReminder(true);
	}
}

QString MainWindow::prepareTrayMsg(list<Task> &reminderList, QString output){
	const char* FIRST_ONE = "1. ";
	const char* SPLITTER = ". ";
	list<Task>::iterator iter = reminderList.begin();
	output += FIRST_ONE;
	output += iter->getName().c_str();
	advance(iter, 1);
	for(int i = 2; 
		iter != reminderList.end();
		advance(iter, 1), i++){
		output += NEW_LINE;
		output += QString::number(i) + SPLITTER;
		output += iter->getName().c_str();
	}
	return output;
}

void MainWindow::showQuickAddWindow(){
	if(!isQuickAddOpen){
		setIsQuickAddOpen(true);
		setupQuickAddWindow();
		setupQuickAddConnection();
		quickAddWindowPtr->show();
	}
}

void MainWindow::showNextDay(){
	const char* NEXT_DAY_STR = "Next Day: ";
	handleDateNavigation(DAY, NEXT_DAY_STR);
}

void MainWindow::showNextWeek(){
	const char* NEXT_WEEK_STR = "Next Week: ";
	handleDateNavigation(WEEK, NEXT_WEEK_STR);
}

void MainWindow::showNextMonth(){
	const char* NEXT_MONTH_STR = "Next Month: ";
	handleDateNavigation(MONTH, NEXT_MONTH_STR);
}

void MainWindow::showPrevDay(){
	const char* PREV_DAY_STR = "Prev. Day: ";
	handleDateNavigation(DAY, PREV_DAY_STR, true);
}

void MainWindow::showPrevWeek(){
	const char* PREV_WEEK_STR = "Prev. Week: ";
	handleDateNavigation(WEEK, PREV_WEEK_STR, true);
}

void MainWindow::showPrevMonth(){
	const char* PREV_MONTH_STR = "Prev. Month: ";
	handleDateNavigation(MONTH, PREV_MONTH_STR, true);
}

void MainWindow::showTrayMsg(QString msg, QString title){
	isFromReminder = false;
	trayIcon->showMessage(title, msg);
}

void MainWindow::about(){
	const QString DESCRIPTION = \
		"<b>TaskPad</b> is a simple, elegant and revolutionary task organiser designed specifically for power users. " 
		"It empowers the user with 4 basic yet comprehensive functions to Add, Modify, Find and Delete tasks. " 
		"Add and Modify offer a comprehensive list of parameters that neatly catalogues the specifications of" 
		"the tasks while Find is a flexible tool that can retrieve a task according to any of its parameters. "
		"Delete is just a one-line wonder that performs what it is meant to do. TaskPad also has Hot Key Templating: " 
		"all 4 functions are made simple with intuitive hotkeys that calls the full command template with clear " 
		"instructions.<br /><br />"
		"<b>TaskPad Team</b>: ASHRAY JAIN, XIE KAI, AN JIANGZE, THYAGESH M., LI ZIXUAN.";
	popMsgBox(DEFAULT_WIN_TITLE, DESCRIPTION);
}

void MainWindow::help(){
	const QString HOTKEY_LIST = \
		"<b>Global Hotkeys</b><br />"
		"Alt + ` 			        : open quick add window<br />"
		"Ctrl + Alt + T 		    : open main window<br />"
		"<br />"
		"<b>Local Hotkeys</b><br />"
		"<b>Quick Add window</b><br />"
		"ESC			            : exit window<br />"
		"<br />"
		"<b>Mainwindow</b><br />"
		"Ctrl + H 		            : hide main window<br />"
		"Ctrl + T 		            : show Today's view<br />"
		"Ctrl + I			        : show Inbox's view<br />"
		"Alt + 1 			        : show Today's view<br />"
		"Alt + 2 			        : show Inbox's view<br />"
		"// Date Navigation<br />"
		"Alt + D 		            : show next day<br />"
		"Alt + Shift + D 		    : show prev. day<br />"
		"Alt + W 		            : show next week<br />"
		"Alt + Shift + W 	        : show prev. day<br />"
		"Alt + M 		            : show next month<br />"
		"Alt + Shift + M 		    : show prev. month<br />"
		"<br />"
		"<b>CommandBar</b><br />"
		"// Hotkey Template<br />"
		"Ctrl + N		            : new deadline task<br />"
		"Ctrl + Shift + N 	        : new timed task<br />"
		"Ctrl + M 		            : modify task as done<br />"
		"Ctrl + Shift + M 	        : modify task by index<br />"
		"Ctrl + Alt + Shift + M 	: modify task by name<br />"
		"Ctrl + D 		            : delete task by index<br />"
		"Ctrl + Shift + D 	        : delete task by name<br />"
		"Ctrl + F 		            : find task<br />"
		"Ctrl + U 		            : undo<br />"
		"Ctrl + R 		            : redo<br />"
		"Tab/Space 		            : auto complete<br />"
		"<br />"
		"// in HotKey Template mode<br />"
		"Tab 			            : jump to next blank<br />"
		"Shift + Tab 		        : jump to prev. blank;<br /><br />"
		"For more information, kindly refer to our<br />"
		"<a href=\"http://bit.ly/HEdCw4\">Project Manual</a>";
	popMsgBox(DEFAULT_WIN_TITLE, HOTKEY_LIST);
}

void MainWindow::popMsgBox(QString title, QString description){
	QMessageBox msgBox;
	msgBox.setWindowTitle(title);
	msgBox.setText(description);
	msgBox.exec();
}

/************************************************************************/
/*                              HANDLERS                                */
/************************************************************************/

void MainWindow::getToday(){
	reset();
	Messenger msg = scheduler->getTodayTasks();
	handleGetToday(msg);
}

void MainWindow::getInbox(){
	const char* GET_INBOX_CMD_STR = "find floating undone";
	reset();
	Messenger msg = scheduler->processCommand(GET_INBOX_CMD_STR);
	handleGetInbox(msg);
}


//************************************
// Method:    reset
// Note: to avoid influence from Intermediate
// stage, we will call this function
//************************************
void MainWindow::reset(){
	scheduler->resetStatus();
	ui.cmdBar->clear();
	isIntermediateStage = false;
}

void MainWindow::handleDateNavigation(TP::PERIOD_TYPE periodType, QString listTitle, bool isPrevious){
	Messenger msg;
	if(isPrevious)
		msg = scheduler->getPrevPeriodTasks(periodType);
	else
		msg = scheduler->getNextPeriodTasks(periodType);
	if(msg.getStatus() == SUCCESS){
		pair<tm, tm> period = scheduler->getCurrentPeriod();
		listTitle += getTimePeriodStr(period);
		updateMainView(msg, listTitle);
		handleOneItemList(msg);
	}
}

//************************************
// Method:    getTimePeriodStr
// Note: result of this will be used in
// date navigation
//************************************
QString MainWindow::getTimePeriodStr(pair<tm, tm> period){
	const char* SPLITTER = " - ";
	QDateTime fromDate = QDateTime::fromTime_t(mktime(&period.first));
	QDateTime toDate = QDateTime::fromTime_t(mktime(&period.second));
	QString fromStr = fromDate.toString(DATE_DD_MM_YYYY);
	QString toStr = toDate.toString(DATE_DD_MM_YYYY);
	return fromStr + SPLITTER + toStr;
}

void MainWindow::handleGetToday(Messenger msg){
	updateMainView(msg, TODAY_VIEW);
}

void MainWindow::handleGetInbox(Messenger msg){
	updateMainView(msg, INBOX_VIEW);
}

void MainWindow::handleQuickAddRequest(QString requestStr){
	if(isCommandAdd(requestStr) || isEqualOne(requestStr)){
		getToday();
		Messenger msg = scheduler->processCommand(requestStr.toStdString());
		switch (msg.getStatus()){
		case TP::ERR:
			handleQA_ERROR(requestStr, msg);
			break;
		case TP::SUCCESS:
			handleQA_SUCCESS();
			break;
		case TP::DISPLAY:
			handleQA_DISPLAY(msg);
			break;
		}
	}
	else
		showTrayMsg(ERROR_ONLY_SUPPORT_ADD_N_DISPLAY_ONE_CMD);
}


//************************************
// Method:    isCommandAdd
// Note: only allow cmd add for quick 
// add window
//************************************
bool MainWindow::isCommandAdd(QString requestStr){
	const int CANT_FIND = -1;
	const QString COMMAND_ADD = "^add.*";
	const QRegExp REGEX_CMD_ADD(COMMAND_ADD);
	return REGEX_CMD_ADD.indexIn(requestStr) != CANT_FIND;
}

bool MainWindow::isEqualOne(QString &requestStr){
	return requestStr.toInt() == 1;
}

void MainWindow::handleShowReminder(){
	if(isFromReminder){
		reset();
		string findCurrRtTasks = getFindRtCmd();
		Messenger msg = scheduler->processCommand(findCurrRtTasks);
		updateMainView(msg, REMINDERS_VIEW);
		handleOneItemList(msg);
		showWindow();
		setFromReminder(false);
	}
}

std::string MainWindow::getFindRtCmd(){
	string currRemindTimeStr = currRemindTime.toString(DATE_DD_MM_YYYY_HH_MM).toStdString();
	const string FIND_CURR_REMIND_TASKS = "find rt `" + currRemindTimeStr + "` undone";
	return FIND_CURR_REMIND_TASKS;
}

void MainWindow::handleMessenger(Messenger msg){
	switch (msg.getStatus()){
	case ERR:
		handleMsg_ERROR(msg);
		break;
	case ERR_INTER:
		handleMsg_ERROR_INTERMEDIATE();
		break;
	case SUCCESS:
		handleMsg_SUCCESS(msg);
		break;
	case INTERMEDIATE:
		handleMsg_INTERMEDIATE(msg);
		break;
	case DISPLAY:
		handleDisplay(msg);
		break;
	case SUCCESS_INDEXED_COMMAND:
		handleMsg_SUCCESS_INDEXED_CMD(msg);
		break;
	}
}

void MainWindow::handleListSelection(){
	QList<QTreeWidgetItem*> list = ui.TaskList->selectedItems();
	if(!list.isEmpty()){
		QTreeWidgetItem* item = ui.TaskList->selectedItems().front();
		int listIndex = ui.TaskList->indexOfTopLevelItem(item);
		int actualIndex = listIndex + 1;
		if(!isIntermediateStage){
			QString indexStr = QString::number(actualIndex);
			Messenger msg = scheduler->processCommand(indexStr.toStdString());
			handleDisplay(msg, true);
		}
		else{//Intermediate Stage
			std::list<Task>::iterator iter = intermediateList.begin();
			advance(iter, listIndex);
			updateDetails(*iter);
		}
	}
}

void MainWindow::unselectAllItems(){
	for(int i = 0; i < ui.TaskList->topLevelItemCount(); i++){
		QTreeWidgetItem* itemToUnselect = ui.TaskList->topLevelItem(i);
		ui.TaskList->setItemSelected(itemToUnselect, false);
	}
}

void MainWindow::selectItemAt( int index ){
	int actualIndex = index;
	int listIndex = actualIndex - 1;
	QTreeWidgetItem* itemToShow = ui.TaskList->topLevelItem(listIndex);
	ui.TaskList->setItemSelected(itemToShow, true);
}

void MainWindow::handleDisplay(Messenger msg, bool callFromSignal){
	int actualIndex = msg.getIndex();
	int listIndex = actualIndex - 1;
	assert(actualIndex > 0);//need be positive and starts from 1

	list<Task> tmp_list = msg.getList();
	list<Task>::iterator iter = tmp_list.begin();
	advance(iter, listIndex);
	//in case of causing loop, check whether from signal event calling
	if(!callFromSignal){
		unselectAllItems();
		selectItemAt(actualIndex);
	}
	updateStatusBar(SUCCESS_STATUS_BAR_DISPLAY_TEXT);
	updateDetailsLabel();
	updateDetails(*iter);
}

void MainWindow::handleQA_ERROR(QString requestStr, Messenger &msg){
	if(!isEqualOne(requestStr)){
		showTrayMsg(msg.getErrorMsg().c_str());
		displayErrorView();
	}
	else{
		closeQuickAddWindow();
		showWindow();
	}
}

void MainWindow::handleQA_SUCCESS(){
	const char* SUCCESS_QA_MSG_ADDED = "Added";
	closeQuickAddWindow();
	refresh();
	showTrayMsg(SUCCESS_QA_MSG_ADDED);
}

void MainWindow::handleQA_DISPLAY(Messenger msg){
	closeQuickAddWindow();
	handleDisplay(msg);
	showWindow();
}

void MainWindow::handleMsg_ERROR(Messenger &msg){
	updateStatusBar(msg.getErrorMsg().c_str());
	displayErrorView();
}

void MainWindow::handleMsg_ERROR_INTERMEDIATE(){
	updateNavLabel(ERROR_NAV_LABLE_INTERMEDIATE_STAGE);
	updateStatusBar(ERROR_STATUS_BAR_INTERMEDIATE_STAGE);
	displayErrorView();
}

void MainWindow::handleMsg_SUCCESS(Messenger &msg){
	switch(msg.getCommandType()){
	case TP::ADD:
		updateForCmdExec(SUCCESS_STATUS_BAR_ADD_TEXT, SUCCESS_DETAILS_ADD_TEXT, msg);
		break;
	case TP::DEL:
		updateForCmdExec(SUCCESS_STATUS_BAR_DEL_TEXT, SUCCESS_DETAILS_DEL_TEXT, msg);
		break;
	case TP::MOD:
		updateForCmdExec(SUCCESS_STATUS_BAR_MOD_TEXT, SUCCESS_DETAILS_MOD_TEXT, msg);
		break;
	case TP::FIND:
		updateMainView(msg, SUCCESS_NAV_LABEL_FIND_CMD, SUCCESS_STATUS_BAR_FIND_TEXT);
		handleOneItemList(msg);
		break;
	case TP::UNDO:
		updateForCmdExec(SUCCESS_STATUS_BAR_UNDO_TEXT, SUCCESS_DETAILS_UNDO_TEXT, msg);
		break;
	case TP::REDO:
		updateForCmdExec(SUCCESS_STATUS_BAR_REDO_TEXT, SUCCESS_DETAILS_REDO_TEXT, msg);
		break;
	}
	isIntermediateStage = false;
}

void MainWindow::handleMsg_INTERMEDIATE(Messenger msg){
	updateMainView(msg, INTERMEDIATE_NAV_LABEL_INTERMEDIATE_STAGE, 
		INTERMEDIATE_STATUS_BAR_INTERMEDIATE_STAGE);
	isIntermediateStage = true;
	intermediateList = msg.getList();
}

void MainWindow::handleMsg_SUCCESS_INDEXED_CMD(Messenger &msg){
	switch (msg.getCommandType()){
	case TP::MOD:
		updateForCmdExec(SUCCESS_STATUS_BAR_MOD_TEXT, SUCCESS_DETAILS_MOD_TEXT, msg);
		break;
	case TP::DEL:
		updateForCmdExec(SUCCESS_STATUS_BAR_DEL_TEXT, SUCCESS_DETAILS_DEL_TEXT, msg);
		break;
	}
}

void MainWindow::refresh(){
	Messenger refreshedMsg = scheduler->refreshList();
	updateList(refreshedMsg.getList());
}

//************************************
// Method:    handleOneItemList
// Note: if list has only one item,
// display it directly
//************************************
void MainWindow::handleOneItemList(Messenger &msg, QString detailsLabel /*= "Task's Details" */){
	const int ONLY_ONE = 1;
	if(msg.getList().size() == ONLY_ONE){
		msg.setTask(msg.getList().front());
		scheduler->syncTask(msg.getList().front());
		updateDetailsView(msg, detailsLabel);
	}
}

/************************************************************************/
/*                              UPDATERS                                */
/************************************************************************/

void MainWindow::updateNavLabel(QString str){
	if(str != INTERMEDIATE_NAV_LABEL_INTERMEDIATE_STAGE)
		navTitleOfLastTime = str;
	ui.Navigation_taskList->setText(str);
}

void MainWindow::updateDetailsLabel(QString str /*= "Task's Details"*/){
	ui.Navigation_detailsView->setText(str);
}

void MainWindow::updateList(std::list<Task> result){
	QTreeWidgetItem* item = NULL;
	ui.TaskList->clear();
	int count = 1;
	for(std::list<Task>::const_iterator iter = result.begin();
		iter != result.end();
		std::advance(iter, STEP)){
		item = extractTask(count, *iter);
		ui.TaskList->addTopLevelItem(item);
		count++;
	}
}

void MainWindow::updateDetails(Task task){
	setDetailsViewOpacity100();
	setNameLabel(task);
	setPriorityLabel(task);
	setDueOrFromToLabel(task);
	setLocationLabel(task);
	setParticipantsLabel(task);
	setTagsLabel(task);
	setRemindTimesLabel(task);
	setNoteLabel(task);
}

void MainWindow::clearDetails(){
	setDetailsViewOpacity40();
	clearStrikeOut();
	updateDetailsLabel();
	setDetailsViewEmpty();
}

void MainWindow::clearStrikeOut(){
	QFont nameFont = ui.name->font();
	nameFont.setStrikeOut(false);
	ui.name->setFont(nameFont);
}

void MainWindow::updateStatusBar(QString str){
	ui.StatusBar->setText(str);
}

void MainWindow::updateMainView(Messenger &msg, QString listTitle, QString statusBarLabel /*= "Ready" */){
	clearDetails();
	updateStatusBar(statusBarLabel);
	updateNavLabel(listTitle);
	updateList(msg.getList());
}

void MainWindow::updateDetailsView(Messenger &msg, QString label /*= "Task's Details"*/){
	updateDetails(msg.getTask());
	updateDetailsLabel(label);
}

void MainWindow::updateForCmdExec(QString statusBarTxt, QString DetailsLabel, Messenger msg){
	updateNavLabel(navTitleOfLastTime);
	updateStatusBar(statusBarTxt);
	updateDetailsView(msg, DetailsLabel);
	refresh();
}

/************************************************************************/
/*                            EXTRACTORS                                */
/************************************************************************/

QTreeWidgetItem* MainWindow::extractTask(int index, Task task){
	QStringList strList;
	switch (task.getTaskType()){
	case DEADLINE:
		strList = extractDueDate(task, strList, index);
		break;
	case TIMED:
		strList = extractTimedDate(task, strList, index);
		break;
	case FLOATING:
		strList = extractFloating(index, task);
		break;
	}
	setListItemDelegate(task, index);
	return new QTreeWidgetItem(strList);
}

QStringList MainWindow::extractFloating(int index, Task &task){
	return QStringList() << QString::number(index) << task.getName().c_str() << EMPTY;
}

QStringList MainWindow::extractTimedDate(Task &task, QStringList strList, int index){
	QString fromTimeStr, toTimeStr;
	if(task.getFlagFromDate())
		fromTimeStr = getFromTimeStr(task, fromTimeStr);
	if(task.getFlagToDate())
		toTimeStr = getToDateStr(task, toTimeStr);
	strList = QStringList() << QString::number(index) << task.getName().c_str() << \
		fromTimeStr + toTimeStr;
	return strList;
}

QString MainWindow::getFromTimeStr(Task &task, QString fromTimeStr){
	QDateTime fromTime = QDateTime::fromTime_t(task.getFromDate());
	fromTimeStr = FROM_STR;
	if(ui.Navigation_taskList->text() == TODAY_VIEW && 
		fromTime.toString(DATE_DD_MM_YY) == QDateTime::currentDateTime().toString(DATE_DD_MM_YY)){
		fromTimeStr += fromTime.toString(DATE_HH_MM);
	}
	else{
		if(fromTime.toString(DATE_YEAR) == QDateTime::currentDateTime().toString(DATE_YEAR))
			fromTimeStr += fromTime.toString(DATE_DD_MM_HH_MM);
		else
			fromTimeStr += fromTime.toString(DATE_DD_MM_YYYY);
	}
	return fromTimeStr;
}

QString MainWindow::getToDateStr(Task &task, QString toTimeStr){
	QDateTime toTime = QDateTime::fromTime_t(task.getToDate());
	if(task.getFlagFromDate())
		toTimeStr = FROM_TO_STR;
	else
		toTimeStr = TO_STR;
	if(ui.Navigation_taskList->text() == TODAY_VIEW && 
		toTime.toString(DATE_DD_MM_YY) == QDateTime::currentDateTime().toString(DATE_DD_MM_YY)){
		toTimeStr += toTime.toString(DATE_HH_MM);
	}
	else{
		if(toTime.toString(DATE_YEAR) == QDateTime::currentDateTime().toString(DATE_YEAR))
			toTimeStr += toTime.toString(DATE_DD_MM_HH_MM);
		else
			toTimeStr += toTime.toString(DATE_DD_MM_YYYY);
	}
	return toTimeStr;
}

QString MainWindow::getDueDateStr( QDateTime &time, QString dueTimeStr ){
	const char* MIDNIGHT = "00:00";
	if(ui.Navigation_taskList->text() == TODAY_VIEW){
		if(time.toString(DATE_HH_MM) != MIDNIGHT)
			dueTimeStr = time.toString(DATE_HH_MM);
		else
			dueTimeStr = QString(TODAY_VIEW).toLower();
	}
	else{
		if(time.toString(DATE_YEAR) == QDateTime::currentDateTime().toString(DATE_YEAR))
			if(time.toString(DATE_HH_MM) != MIDNIGHT)
				dueTimeStr = time.toString(DATE_DD_MM_HH_MM);
			else
				dueTimeStr = time.toString(DATE_DD_MM);
		else
			dueTimeStr = time.toString(DATE_DD_MM_YYYY);
	}
	return dueTimeStr;
}

QStringList MainWindow::extractDueDate(Task &task, QStringList strList, int index){
	QString dueTimeStr;
	QDateTime time = QDateTime::fromTime_t(task.getDueDate());
	dueTimeStr = getDueDateStr(time, dueTimeStr);
	strList = QStringList() << QString::number(index) << task.getName().c_str() << \
		DUE_STR + dueTimeStr;
	return strList;
}

/************************************************************************/
/*                               SETTERS                                */
/************************************************************************/

void MainWindow::setCurrRemindTime(){
	currRemindTime = QDateTime::currentDateTime();
}

void MainWindow::setFromReminder(bool toggle){
	isFromReminder = toggle;
}

void MainWindow::setIsQuickAddOpen(bool toggle){
	isQuickAddOpen = toggle;
}

void MainWindow::setHighPriorityDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index, new ListItemDelegate(HIGH, UNDONE, ui.TaskList));
}

void MainWindow::setHighPriorityDoneDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index, new ListItemDelegate(HIGH, DONE, ui.TaskList));
}

void MainWindow::setHighPriorityOverdueDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index, new ListItemDelegate(HIGH, OVERDUE, ui.TaskList));
}

void MainWindow::setMediumPriorityDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index, new ListItemDelegate(MEDIUM, UNDONE, ui.TaskList));
}

void MainWindow::setMediumPriorityDoneDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index, new ListItemDelegate(MEDIUM, DONE, ui.TaskList));
}

void MainWindow::setMediumPriorityOverdueDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index, new ListItemDelegate(MEDIUM, OVERDUE, ui.TaskList));
}

void MainWindow::setNormalDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index, new ListItemDelegate(LOW, UNDONE, ui.TaskList));
}

void MainWindow::setNormalDoneDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index, new ListItemDelegate(LOW, DONE, ui.TaskList));
}

void MainWindow::setNormalOverdueDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index, new ListItemDelegate(LOW, OVERDUE, ui.TaskList));
}

void MainWindow::setListItemDelegate(Task &task, int index){
	int listIndex = index - 1;
	//for HIGH prio
	setForHighPrioItem(task, listIndex);
	//for MEDIUM
	setForMediumPrioItem(task, listIndex);
	//for LOW
	setForLowPrioItem(task, listIndex);
}

void MainWindow::setDetailsViewEmpty(){
	ui.name->setText(EMPTY);
	ui.name->setToolTip(EMPTY);
	ui.dueOrFromTo->setText(EMPTY);
	ui.dueOrFromTo->setToolTip(EMPTY);
	ui.location->setText(EMPTY);
	ui.location->setToolTip(EMPTY);
	ui.participants->setText(EMPTY);
	ui.participants->setToolTip(EMPTY);
	ui.tags->setText(EMPTY);
	ui.tags->setToolTip(EMPTY);
	ui.remindTime->setText(EMPTY);
	ui.remindTime->setToolTip(EMPTY);
	ui.note->setPlainText(EMPTY);
	ui.note->setToolTip(EMPTY);
}

//************************************
//    SETTERS FOR DETAILS (BELOW)
//************************************
void MainWindow::setDetailsViewOpacity40(){
	const char* PROPERTY_OPACITY = "opacity";
	const int ANIMATE_DURATION = 330;

	QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
	opacityEffect->setOpacity(detailsViewOpacity);
	ui.DetailsView->setGraphicsEffect(opacityEffect);
	QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, PROPERTY_OPACITY);
	animation->setDuration(ANIMATE_DURATION);
	animation->setStartValue(detailsViewOpacity);
	animation->setEndValue(DESIRED_TRANSPARENT_OPACITY);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
	detailsViewOpacity = DESIRED_TRANSPARENT_OPACITY;
}

void MainWindow::displayErrorView(){
	const char* PROPERTY_OPACITY = "opacity";
	const double NON_TRANSPARENT = 1.0;
	const double TRANSPARENT = 0.0;
	const int ANIMATE_DURATION = 1200;

	QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
	opacityEffect->setOpacity(NON_TRANSPARENT);
	ui.errorBgWidget->setGraphicsEffect(opacityEffect);
	ui.errorBgWidget->setHidden(false);
	QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, PROPERTY_OPACITY);
	animation->setDuration(ANIMATE_DURATION);
	animation->setStartValue(NON_TRANSPARENT);
	animation->setEndValue(TRANSPARENT);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::setDetailsViewOpacity100(){
	const double NON_TRANSPARENT = 1.0;
	ui.DetailsView->setGraphicsEffect(NULL);
	detailsViewOpacity = NON_TRANSPARENT;
}

void MainWindow::setNameLabel(Task &task){
	const char* OVERDUE_SIGN = "!! ";
	ui.name->setText(task.getName().c_str());
	QFont nameFont = ui.name->font();
	if(task.getState() == TP::UNDONE){
		nameFont.setStrikeOut(false);
	}
	else if(task.getState() == TP::OVERDUE){
		nameFont.setStrikeOut(false);
		ui.name->setText(OVERDUE_SIGN + ui.name->text());
	}
	else{//DONE already
		nameFont.setStrikeOut(true);
	}
	ui.name->setFont(nameFont);
	if(ui.name->text().length() < LENGTH_TOO_LONG)
		ui.name->setToolTip(ui.name->text());
	else
		ui.name->setToolTip(ui.name->text().left(LENGTH_TOO_LONG) + NEGLECTED);
}

void MainWindow::setPriorityLabel(Task &task){
	const char* HIGH_PRIO_BG = "QWidget#DetailsView{\n"
		"	background-image:url(:/MainWindow/Resources/details_high_bg.png);\n"
		"}";
	const char* MEDIUM_PRIO_BG = "QWidget#DetailsView{\n"
		"	background-image:url(:/MainWindow/Resources/details_normal_bg.png);\n"
		"}";
	const char* LOW_PRIO_BG = "QWidget#DetailsView{\n"
		"	background-image:url(:/MainWindow/Resources/details_low_bg.png);\n"
		"}";

	switch (task.getPriority()){
	case HIGH:
		ui.DetailsView->setStyleSheet(QLatin1String(HIGH_PRIO_BG));
		break;
	case MEDIUM:
		ui.DetailsView->setStyleSheet(QLatin1String(MEDIUM_PRIO_BG));
		break;
	case LOW:
		ui.DetailsView->setStyleSheet(QLatin1String(LOW_PRIO_BG));
		break;
	}
}

void MainWindow::setDueDateLabel(Task &task){
	const char* MIDNIGHT = "00:00";

	QDateTime time = QDateTime::fromTime_t(task.getDueDate());
	if(time.toString(DATE_HH_MM) == MIDNIGHT)
		ui.dueOrFromTo->setText(DUE_STR + time.toString(DATE_DD_MM_YYYY));
	else
		ui.dueOrFromTo->setText(DUE_STR + time.toString(DATE_DD_MM_YYYY_HH_MM));
}

void MainWindow::setTimedLabel(Task &task){
	QString fromTimeStr, toTimeStr;
	if(task.getFlagFromDate()){
		QDateTime fromTime = QDateTime::fromTime_t(task.getFromDate());
		fromTimeStr = FROM_STR + fromTime.toString(DATE_DD_MM_YYYY_HH_MM);
	}
	if(task.getFlagToDate()){
		QDateTime toTime = QDateTime::fromTime_t(task.getToDate());
		if(task.getFlagFromDate())
			toTimeStr = FROM_TO_STR + toTime.toString(DATE_DD_MM_YYYY_HH_MM);
		else
			toTimeStr = TO_STR + toTime.toString(DATE_DD_MM_YYYY_HH_MM);
	}
	ui.dueOrFromTo->setText(fromTimeStr + toTimeStr);
}

void MainWindow::setFloatingLabel(){
	ui.dueOrFromTo->setText(EMPTY);
}

void MainWindow::setDueOrFromToLabel(Task &task){
	switch (task.getTaskType()){
	case DEADLINE:
		setDueDateLabel(task);
		break;
	case TIMED:
		setTimedLabel(task);
		break;
	case FLOATING:
		setFloatingLabel();
		break;
	}
	ui.dueOrFromTo->setToolTip(ui.dueOrFromTo->text());
}

void MainWindow::setLocationLabel(Task &task){
	const char* AT = "@";

	if(task.getFlagLocation()){
		ui.location->setText((AT + task.getLocation()).c_str());
		if(ui.location->text().length() < LENGTH_TOO_LONG)
			ui.location->setToolTip(ui.location->text());
		else
			ui.location->setToolTip(ui.location->text().left(LENGTH_TOO_LONG) + NEGLECTED);
	}
	else{
		ui.location->setText(EMPTY);
		ui.location->setToolTip(EMPTY);
	}
}

void MainWindow::setParticipantsLabel(Task &task){
	const char* DELIMITTER = ", ";

	if(task.getFlagParticipants()){
		QString participants;
		list<string> listOfParticipants =  task.getParticipants();
		list<string>::iterator iter = listOfParticipants.begin();
		participants += iter->c_str();
		iter++;
		for(;iter != listOfParticipants.end();
			advance(iter, STEP)){
				participants += DELIMITTER;
				participants += iter->c_str();
		}
		ui.participants->setText(participants);
		if(ui.participants->text().length() < LENGTH_TOO_LONG)
			ui.participants->setToolTip(ui.participants->text());
		else
			ui.participants->setToolTip(ui.participants->text().left(LENGTH_TOO_LONG) + NEGLECTED);
	}
	else{
		ui.participants->setText(EMPTY);
		ui.participants->setToolTip(EMPTY);
	}
}

void MainWindow::setTagsLabel(Task &task){
	const char* SHARP = "#";
	const char* DELIMITTER = ", ";

	if(task.getFlagTags()){
		QString tags;
		list<string> listOfTags = task.getTags();
		list<string>::iterator iter = listOfTags.begin();
		tags += SHARP;
		tags += iter->c_str();
		iter++;
		for(;iter != listOfTags.end();
			advance(iter, STEP)){
				tags += DELIMITTER;
				tags += SHARP;
				tags += iter->c_str();
		}
		ui.tags->setText(tags);
		if(ui.tags->text().length() < LENGTH_TOO_LONG)
			ui.tags->setToolTip(ui.tags->text());
		else
			ui.tags->setToolTip(ui.tags->text().left(LENGTH_TOO_LONG) + NEGLECTED);
	}
	else{
		ui.tags->setText(EMPTY);
		ui.tags->setToolTip(EMPTY);
	}
}

void MainWindow::setRemindTimesLabel(Task &task){
	const char* DELIMITTER = ", ";
	const char* REMIND_ME_STR = "Remind me : ";

	if(task.getFlagRemindTimes()){
		QString remindTimes;
		list<time_t> listOfRemindTimes = task.getRemindTimes();
		list<time_t>::iterator iter = listOfRemindTimes.begin();
		remindTimes += QDateTime::fromTime_t(*iter).toString(DATE_DD_MM_YYYY_HH_MM);
		iter++;
		for(;iter != listOfRemindTimes.end();
			advance(iter, STEP)){
				remindTimes += DELIMITTER;
				remindTimes += QDateTime::fromTime_t(*iter).toString(DATE_DD_MM_YYYY_HH_MM);
		}
		ui.remindTime->setText(REMIND_ME_STR + remindTimes);
		if(ui.remindTime->text().length() < LENGTH_TOO_LONG)
			ui.remindTime->setToolTip(ui.remindTime->text());
		else
			ui.remindTime->setToolTip(ui.remindTime->text().left(LENGTH_TOO_LONG) + NEGLECTED);
	}
	else{
		ui.remindTime->setText(EMPTY);
		ui.remindTime->setToolTip(EMPTY);
	}
}

void MainWindow::setNoteLabel(Task &task){
	if(task.getFlagNote()){
		ui.note->setPlainText(task.getNote().c_str());
		if(ui.note->toPlainText().length() < LENGTH_TOO_LONG)
			ui.note->setToolTip(ui.note->toPlainText());
		else
			ui.note->setToolTip(ui.note->toPlainText().left(LENGTH_TOO_LONG) + NEGLECTED);
	}
	else{
		ui.note->setPlainText(EMPTY);
		ui.note->setToolTip(EMPTY);
	}
}

/************************************************************************/
/*                        WINDOW EVENT RELATED                          */
/************************************************************************/

bool MainWindow::eventFilter(QObject* watched, QEvent* event){
	bool ret = true;
	if(watched == ui.CloseButton || watched == ui.MinimizeButton ||
		watched == ui.HelpButton || watched == ui.AboutButton){
		if(event->type() != QEvent::MouseMove){
			ret = QObject::eventFilter(watched, event);//normal processing
		}
	}
	else if(watched == ui.cmdBar && 
		event->type() == QEvent::KeyPress){
			ret = handleKeyPressInCmdBar(ret, watched, event);
	}
	else{
		ret = QObject::eventFilter(watched, event);//normal processing
	}
	return ret;
}

void MainWindow::keyPressEvent(QKeyEvent* event){
	ui.cmdBar->setFocus();
	if(event->key() == Qt::Key_Escape)
		getToday();
	QMainWindow::keyPressEvent(event);
}

void MainWindow::changeEvent(QEvent* event){
	runInBackground(event);
	QMainWindow::changeEvent(event);
}

void MainWindow::runInBackground(QEvent* event){
	const int RIGHT_AWAY = 0;
	if(event->type()==QEvent::WindowStateChange){
		if(windowState() == Qt::WindowMinimized){
			QTimer::singleShot(RIGHT_AWAY, this, SLOT(hide()));
		}
	}
}

//************************************
// Method:    mousePressEvent
// Note: impl darg & move for borderless
// window
//************************************
void MainWindow::mousePressEvent(QMouseEvent *event){
	this->windowPosition = this->pos();
	this->mousePressPosition = event->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
	this->mouseMovePosition = event->globalPos();
	QPoint distanceToMove = this->mouseMovePosition - this->mousePressPosition + this->windowPosition;
	this->move(distanceToMove);   
}

void MainWindow::setForHighPrioItem( Task &task, int listIndex ){
	if     (task.getPriority() == HIGH   && task.getState() == UNDONE)
		setHighPriorityDelegate(listIndex);
	else if(task.getPriority() == HIGH   && task.getState() == DONE)
		setHighPriorityDoneDelegate(listIndex);
	else if(task.getPriority() == HIGH   && task.getState() == OVERDUE)
		setHighPriorityOverdueDelegate(listIndex);
}

void MainWindow::setForMediumPrioItem( Task &task, int listIndex ){
	if     (task.getPriority() == MEDIUM && task.getState() == UNDONE)
		setMediumPriorityDelegate(listIndex);
	else if(task.getPriority() == MEDIUM && task.getState() == DONE)
		setMediumPriorityDoneDelegate(listIndex);
	else if(task.getPriority() == MEDIUM && task.getState() == OVERDUE)
		setMediumPriorityOverdueDelegate(listIndex);
}

void MainWindow::setForLowPrioItem( Task &task, int listIndex ){
	if     (task.getPriority() == LOW && task.getState() == UNDONE)
		setNormalDelegate(listIndex);
	else if(task.getPriority() == LOW && task.getState() == DONE)
		setNormalDoneDelegate(listIndex);
	else if(task.getPriority() == LOW && task.getState() == OVERDUE)
		setNormalOverdueDelegate(listIndex);
}

bool MainWindow::handleKeyPressInCmdBar( bool ret, QObject* watched, QEvent* event ){
	QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
	if(keyEvent->key() == Qt::Key_Escape){
		getToday();
	}
	else if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter){
		handleKeyEnter();
	}
	else{
		ret = QObject::eventFilter(watched, event);//normal processing
	}
	return ret;
}

void MainWindow::handleKeyEnter(){
	QString currentInput = ui.cmdBar->getCurrentLine();
	ui.cmdBar->pushCurrentLine();
	if(!currentInput.isEmpty()){
		string inputStdString = currentInput.toLocal8Bit().constData();
		Messenger msg = scheduler->processCommand(inputStdString);
		handleMessenger(msg);
	}
}
