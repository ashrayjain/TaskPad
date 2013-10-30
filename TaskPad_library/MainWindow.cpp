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

const char* MainWindow::EMPTY                           = "";
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
	detailsViewOpacity = 0.4;
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.TaskList->header()->resizeSection(0, 70);
	ui.TaskList->header()->resizeSection(1, 220);
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
	const int TIMER_CYCLE = 60000;
	timer = new QTimer(this);
	timer->start(TIMER_CYCLE);
}

void MainWindow::setupTrayIcon(){
	isQuickAddOpen = false;
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon(":/MainWindow/Resources/logo.png"));
	trayIcon->show();
	trayIcon->setToolTip(DEFAULT_WIN_TITLE);
}

void MainWindow::setupConnection(){
	connect(timer, SIGNAL(timeout()),this, SLOT(showReminder()));
	connect(trayIcon, SIGNAL(messageClicked()),this, SLOT(handleShowReminder()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, 
		SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
	connect(ui.CloseButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.MinimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
	connect(ui.AboutButton, SIGNAL(clicked()), this, SLOT(about()));
	connect(ui.HelpButton, SIGNAL(clicked()), this, SLOT(help()));
	connect(ui.TaskList, SIGNAL(itemSelectionChanged()), this, SLOT(handleListSelection()));
}

void MainWindow::setupHotkeys(){
	setupGlobalHotkeys();
	setupWinDisplayHotkeys();
	setupDateNavHotkeys();
}

void MainWindow::setupGlobalHotkeys(){
	QxtGlobalShortcut * scOpenQuickAddWin = new QxtGlobalShortcut(QKeySequence("Alt+`"), this);
	connect(scOpenQuickAddWin, SIGNAL(activated()),this, SLOT(showQuickAddWindow()));
	QxtGlobalShortcut * scOpenMainWin = new QxtGlobalShortcut(QKeySequence("Ctrl+Alt+t"), this);
	connect(scOpenMainWin, SIGNAL(activated()),this, SLOT(showWindow()));
}

void MainWindow::setupWinDisplayHotkeys(){
	(void) new QShortcut(QKeySequence(tr("F5", "RemainderTesting")), this, SLOT(showReminder()));
	(void) new QShortcut(QKeySequence(tr("Ctrl+H", "Minimize")), this, SLOT(showMinimized()));
	(void) new QShortcut(QKeySequence(tr("Ctrl+T", "Today")), this, SLOT(getToday()));
	(void) new QShortcut(QKeySequence(tr("Alt+1", "Today")), this, SLOT(getToday()));
	(void) new QShortcut(QKeySequence(tr("Ctrl+I", "Inbox")), this, SLOT(getInbox()));
	(void) new QShortcut(QKeySequence(tr("Alt+2", "Inbox")), this, SLOT(getInbox()));
}

void MainWindow::setupDateNavHotkeys(){
	(void) new QShortcut(QKeySequence(tr("Alt+D", "Show Next Day")), this, SLOT(showNextDay()));
	(void) new QShortcut(QKeySequence(tr("Alt+Shift+D", "Show Previous Day")), this, SLOT(showPrevDay()));
	(void) new QShortcut(QKeySequence(tr("Alt+W", "Show Next Week")), this, SLOT(showNextWeek()));
	(void) new QShortcut(QKeySequence(tr("Alt+Shift+W", "Show Previous Week")), this, SLOT(showPrevWeek()));
	(void) new QShortcut(QKeySequence(tr("Alt+M", "Show Next Month")), this, SLOT(showNextMonth()));
	(void) new QShortcut(QKeySequence(tr("Alt+Shift+M", "Show Previous Month")), this, SLOT(showPrevMonth()));
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
		showTrayMsg(output, "Reminders");
		setFromReminder(true);
	}
}

QString MainWindow::prepareTrayMsg(list<Task> &reminderList, QString output){
	list<Task>::iterator iter = reminderList.begin();
	output += "1. ";
	output += iter->getName().c_str();
	advance(iter, 1);
	for(int i = 2; iter != reminderList.end(); advance(iter, 1), i++){
		output += "\n";
		output += QString::number(i) + ". ";
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
	handleDateNavigation(DAY, "Next Day: ");
}

void MainWindow::showNextWeek(){
	handleDateNavigation(WEEK, "Next Week: ");
}

void MainWindow::showNextMonth(){
	handleDateNavigation(MONTH, "Next Month: ");
}

void MainWindow::showPrevDay(){
	handleDateNavigation(DAY, "Prev. Day: ", true);
}

void MainWindow::showPrevWeek(){
	handleDateNavigation(WEEK, "Prev. Week: ", true);
}

void MainWindow::showPrevMonth(){
	handleDateNavigation(MONTH, "Prev. Month: ", true);
}

void MainWindow::showTrayMsg(QString msg, QString title){
	isFromReminder = false;
	trayIcon->showMessage(title, msg);
}

void MainWindow::about(){
	popMsgBox(DEFAULT_WIN_TITLE, "TaskPad is a product created by Team F12-1C.\n"
		"Members: ASHRAY, KAI, JIANGZE, THYAGESH, ZIXUAN.");
}

void MainWindow::help(){
	popMsgBox(DEFAULT_WIN_TITLE, "Geek doesn't need help from us :p");
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
	reset();
	Messenger msg = scheduler->processCommand("find floating undone");
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
	QDateTime fromDate = QDateTime::fromTime_t(mktime(&period.first));
	QDateTime toDate = QDateTime::fromTime_t(mktime(&period.second));
	QString fromStr = fromDate.toString("dd/MM/yyyy");
	QString toStr = toDate.toString("dd/MM/yyyy");
	return fromStr + " - " + toStr;
}

void MainWindow::handleGetToday(Messenger msg){
	updateMainView(msg, "Today");
}

void MainWindow::handleGetInbox(Messenger msg){
	updateMainView(msg, "Inbox");
}

void MainWindow::handleQuickAddRequest(QString requestStr){
	if(isCommandAdd(requestStr) || isEqualOne(requestStr)){
		getToday();
		Messenger msg = scheduler->processCommand(requestStr.toStdString());
		switch (msg.getStatus()){
		case TP::ERROR:
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
		showTrayMsg("Only Add Command and Display 1 are supported");
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
		updateMainView(msg, "Reminders");
		handleOneItemList(msg);
		showWindow();
		setFromReminder(false);
	}
}

std::string MainWindow::getFindRtCmd(){
	string currRemindTimeStr = currRemindTime.toString("dd/MM/yy hh:mm").toStdString();
	string findCurrRtTasks = "find rt `" + currRemindTimeStr + "` undone";
	return findCurrRtTasks;
}

void MainWindow::handleMessenger(Messenger msg){
	switch (msg.getStatus()){
	case ERROR:
		handleMsg_ERROR(msg);
		break;
	case ERROR_INTERMEDIATE:
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
		int index = ui.TaskList->indexOfTopLevelItem(item) + 1;
		if(!isIntermediateStage){
			QString indexStr = QString::number(index);
			Messenger msg = scheduler->processCommand(indexStr.toStdString());
			handleDisplay(msg, true);
		}
		else{
			std::list<Task>::iterator iter = intermediateList.begin();
			advance(iter, index - 1);
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
	QTreeWidgetItem* itemToShow = ui.TaskList->topLevelItem(index - 1);
	ui.TaskList->setItemSelected(itemToShow, true);
}

void MainWindow::handleDisplay(Messenger msg, bool callFromSignal){
	int index = msg.getIndex();
	assert(index > 0);
	list<Task> tmp_list = msg.getList();
	list<Task>::iterator iter = tmp_list.begin();
	advance(iter, index - 1);
	if(!callFromSignal){
		unselectAllItems();
		selectItemAt(index);
	}
	updateStatusBar(SUCCESS_STATUS_BAR_DISPLAY_TEXT);
	updateDetailsLabel();
	updateDetails(*iter);
}

void MainWindow::handleQA_ERROR(QString requestStr, Messenger &msg){
	if(!isEqualOne(requestStr)){
		showTrayMsg(msg.getErrorMsg().c_str());
	}
	else{
		closeQuickAddWindow();
		showWindow();
	}
}

void MainWindow::handleQA_SUCCESS(){
	closeQuickAddWindow();
	refresh();
	showTrayMsg("Added");
}

void MainWindow::handleQA_DISPLAY(Messenger msg){
	closeQuickAddWindow();
	handleDisplay(msg);
	showWindow();
}

void MainWindow::handleMsg_ERROR(Messenger &msg){
	updateStatusBar(msg.getErrorMsg().c_str());
}

void MainWindow::handleMsg_ERROR_INTERMEDIATE(){
	updateNavLabel("Select a task by typing its index");
	updateStatusBar("Wrong input. Kindly press ECS to cancel");
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
		updateMainView(msg, "Search Results", SUCCESS_STATUS_BAR_FIND_TEXT);
		handleOneItemList(msg);
		break;
	case TP::UNDO:
		updateForCmdExec(SUCCESS_STATUS_BAR_UNDO_TEXT, SUCCESS_DETAILS_UNDO_TEXT, msg);
		break;
	case TP::REDO:
		updateForCmdExec(SUCCESS_STATUS_BAR_REDO_TEXT, SUCCESS_DETAILS_REDO_TEXT, msg);
		break;
	}
}

void MainWindow::handleMsg_INTERMEDIATE(Messenger msg){
	updateMainView(msg, "Select a task by typing its index", "Multiple results were found");
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
	if(str != "Select a task by typing its index")
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
		std::advance(iter, 1)){
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
	updateDetailsLabel();
	setDetailsViewEmpty();
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
	fromTimeStr = "From " + fromTime.toString("dd/MM/yyyy");	return fromTimeStr;
}

QString MainWindow::getToDateStr(Task &task, QString toTimeStr){
	QDateTime toTime = QDateTime::fromTime_t(task.getToDate());
	if(task.getFlagFromDate())
		toTimeStr = " to " + toTime.toString("dd/MM/yyyy");
	else
		toTimeStr = "To " + toTime.toString("dd/MM/yyyy");	return toTimeStr;
}

QStringList MainWindow::extractDueDate(Task &task, QStringList strList, int index){
	QDateTime time = QDateTime::fromTime_t(task.getDueDate());
	strList = QStringList() << QString::number(index) << task.getName().c_str() << \
		"Due " + time.toString("dd/MM/yyyy");
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
	ui.TaskList->setItemDelegateForRow(index - 1, new ListItemDelegate(HIGH, UNDONE, ui.TaskList));
}

void MainWindow::setHighPriorityDoneDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index - 1, new ListItemDelegate(HIGH, DONE, ui.TaskList));
}

void MainWindow::setNormalDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index - 1, new ListItemDelegate(NON_HIGH, UNDONE, ui.TaskList));
}

void MainWindow::setNormalDoneDelegate(int index){
	ui.TaskList->setItemDelegateForRow(index - 1, new ListItemDelegate(NON_HIGH, DONE, ui.TaskList));
}

void MainWindow::setListItemDelegate(Task &task, int index){
	if(task.getPriority() == HIGH && task.getState() == UNDONE)
		setHighPriorityDelegate(index);
	else if(task.getPriority() == HIGH && task.getState() == DONE)
		setHighPriorityDoneDelegate(index);
	else if(task.getState() == UNDONE)
		setNormalDelegate(index);
	else
		setNormalDoneDelegate(index);
}

void MainWindow::setDetailsViewEmpty(){
	ui.name->setText(EMPTY);
	ui.dueOrFromTo->setText(EMPTY);
	ui.location->setText(EMPTY);
	ui.participants->setText(EMPTY);
	ui.tags->setText(EMPTY);
	ui.remindTime->setText(EMPTY);
	ui.note->setPlainText(EMPTY);
}

//************************************
//    SETTERS FOR DETAILS (BELOW)
//************************************
void MainWindow::setDetailsViewOpacity40(){
	QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
	opacityEffect->setOpacity(detailsViewOpacity);
	ui.DetailsView->setGraphicsEffect(opacityEffect);
	QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
	animation->setDuration(330);
	animation->setStartValue(detailsViewOpacity);
	animation->setEndValue(0.4);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
	detailsViewOpacity = 0.4;
}

void MainWindow::setDetailsViewOpacity100(){
	ui.DetailsView->setGraphicsEffect(NULL);
	detailsViewOpacity = 1.0;
}

void MainWindow::setNameLabel(Task &task){
	ui.name->setText(task.getName().c_str());
	QFont nameFont = ui.name->font();
	if(task.getState() == TP::UNDONE){
		nameFont.setStrikeOut(false);
	}
	else{//DONE already
		nameFont.setStrikeOut(true);
	}
	ui.name->setFont(nameFont);
}

void MainWindow::setPriorityLabel(Task &task){
	switch (task.getPriority()){
	case HIGH:
		ui.DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
			"	background-image:url(:/MainWindow/Resources/details_high_bg.png);\n"
			"}"));
		break;
	case MEDIUM:
		ui.DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
			"	background-image:url(:/MainWindow/Resources/details_normal_bg.png);\n"
			"}"));
		break;
	case LOW:
		ui.DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
			"	background-image:url(:/MainWindow/Resources/details_low_bg.png);\n"
			"}"));
		break;
	}
}

void MainWindow::setDueDateLabel(Task &task){
	QDateTime time = QDateTime::fromTime_t(task.getDueDate());
	QTime hour_n_min = time.time();
	if(hour_n_min.hour() == 0 & hour_n_min.minute() == 0)
		ui.dueOrFromTo->setText("Due  " + time.toString("dd/MM/yyyy"));//TODO
	else
		ui.dueOrFromTo->setText("Due  " + time.toString("dd/MM/yyyy  hh:mm"));
}

void MainWindow::setTimedLabel(Task &task){
	QString fromTimeStr, toTimeStr;
	if(task.getFlagFromDate()){
		QDateTime fromTime = QDateTime::fromTime_t(task.getFromDate());
		fromTimeStr = "From  " + fromTime.toString("dd/MM/yyyy  hh:mm");
	}
	if(task.getFlagToDate()){
		QDateTime toTime = QDateTime::fromTime_t(task.getToDate());
		if(task.getFlagFromDate())
			toTimeStr = "  to  " + toTime.toString("dd/MM/yyyy  hh:mm");//TODO
		else
			toTimeStr = "To  " + toTime.toString("dd/MM/yyyy  hh:mm");
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
}

void MainWindow::setLocationLabel(Task &task){
	if(task.getFlagLocation())
		ui.location->setText(("@" + task.getLocation()).c_str());
	else
		ui.location->setText(EMPTY);
}

void MainWindow::setParticipantsLabel(Task &task){
	if(task.getFlagParticipants()){
		QString participants;
		list<string> listOfParticipants =  task.getParticipants();
		list<string>::iterator iter = listOfParticipants.begin();
		participants += iter->c_str();
		iter++;
		for(;iter != listOfParticipants.end();
			advance(iter, 1)){
				participants += ", ";
				participants += iter->c_str();
		}
		ui.participants->setText(participants);
	}
	else
		ui.participants->setText(EMPTY);
}

void MainWindow::setTagsLabel(Task &task){
	if(task.getFlagTags()){
		QString tags;
		list<string> listOfTags = task.getTags();
		list<string>::iterator iter = listOfTags.begin();
		tags += "#";
		tags += iter->c_str();
		iter++;
		for(;iter != listOfTags.end();
			advance(iter, 1)){
				tags += ", ";
				tags += "#";
				tags += iter->c_str();
		}
		ui.tags->setText(tags);
	}
	else
		ui.tags->setText(EMPTY);
}

void MainWindow::setRemindTimesLabel(Task &task){
	if(task.getFlagRemindTimes()){
		QString remindTimes;
		list<time_t> listOfRemindTimes = task.getRemindTimes();
		list<time_t>::iterator iter = listOfRemindTimes.begin();
		remindTimes += QDateTime::fromTime_t(*iter).toString("dd/MM/yyyy hh:mm");
		iter++;
		for(;iter != listOfRemindTimes.end();
			advance(iter, 1)){
				remindTimes += ", ";
				remindTimes += QDateTime::fromTime_t(*iter).toString("dd/MM/yyyy hh:mm");
		}
		ui.remindTime->setText("Remind me : " + remindTimes);
	}
	else
		ui.remindTime->setText(EMPTY);
}

void MainWindow::setNoteLabel(Task &task){
	if(task.getFlagNote()){
		ui.note->setPlainText(task.getNote().c_str());
	}
	else
		ui.note->setPlainText(EMPTY);
}

/************************************************************************/
/*                        WINDOW EVENT RELATED                          */
/************************************************************************/

bool MainWindow::eventFilter(QObject* watched, QEvent* event){
	if(watched == ui.CloseButton || watched == ui.MinimizeButton ||
		watched == ui.HelpButton || watched == ui.AboutButton){
		if(event->type() == QEvent::MouseMove){
			return true;
		}
	}
	else if(watched == ui.cmdBar){
		if(event->type() == QEvent::KeyPress){
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->key() == Qt::Key_Escape){
				getToday();
			}
			else if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter){
				QString currentInput = ui.cmdBar->getCurrentLine();//TODO: can shrink into one API
				ui.cmdBar->pushCurrentLine();
				if(!currentInput.isEmpty()){
					string inputStdString = currentInput.toLocal8Bit().constData();
					Messenger msg = scheduler->processCommand(inputStdString);
					handleMessenger(msg);
				}
				return true;//stop Key return or Key enter
			}
		}
	}
	return QObject::eventFilter(watched, event);//normal processing
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
	if(event->type()==QEvent::WindowStateChange){
		if(windowState() == Qt::WindowMinimized){
			QTimer::singleShot(0, this, SLOT(hide()));
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
