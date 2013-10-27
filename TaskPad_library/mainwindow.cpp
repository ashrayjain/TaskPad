#include <QMouseEvent>
#include <QMessageBox>
#include <QTextBlock>
#include <QShortcut>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <cassert>
#include "libqxt/qxtglobalshortcut.h"
#include "Enum.h"
#include "mainwindow.h"
#include "quickadd_window.h"
#include "Manager.h"
#include "lastColumnDelegate.h"
#include "HighPriorityDelegate.h"
#include "CommandBar.h"


MainWindow::MainWindow(QWidget *parent)
:QMainWindow(parent){
	setupUI();
	setupDependency();
	getToday();
}

MainWindow::~MainWindow(){
	dispose();
}

void MainWindow::iconIsActived(QSystemTrayIcon::ActivationReason){
	showWindow();
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

void MainWindow::handleDateNavigation(TP::PERIOD_TYPE periodType, QString listTitle, bool isPrevious){
	Messenger msg;
	if(isPrevious)
		msg = scheduler->getPrevPeriodTasks(periodType);
	else
		msg = scheduler->getNextPeriodTasks(periodType);
	pair<tm, tm> period = scheduler->getCurrentPeriod();
	listTitle += getTimePeriodStr(period);
	updateMainView(msg, listTitle);
	scheduler->syncTaskList(msg.getList());
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

void MainWindow::showQuickAddWindow(){
	QString input;
	if(!isQuickAddOpen){
		setIsQuickAddOpen(true);
		setupQuickAddWindow();
		setupQuickAddConnection();
		quickAddWindowObj->show();
	}
}

void MainWindow::closeQuickAddWindow(){
	disposeQuickAddWindow();
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
	else{
		showTrayMsg("Only Add Command and Display 1 are supported");
	}
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

bool MainWindow::isCommandAdd(QString requestStr){
	const int CANT_FIND = -1;
	const QString COMMAND_ADD = "^add.*";
	const QRegExp REGEX_CMD_ADD(COMMAND_ADD);
	return REGEX_CMD_ADD.indexIn(requestStr) != CANT_FIND;
}

void MainWindow::help(){
	popMsgBox("TaskPad", "Geek doesn't need help from us :p");
}

void MainWindow::keyPressEvent(QKeyEvent* event){
	ui.cmdBar->setFocus();
	if(event->key() == Qt::Key_Escape)
		getToday();
	QMainWindow::keyPressEvent(event);
}

void MainWindow::reset(){
	scheduler->resetStatus();
	ui.cmdBar->clear();
}

void MainWindow::getToday(){
	reset();
	Messenger msg = scheduler->getTodayTasks();
	handleGetToday(msg);
}

void MainWindow::handleGetToday(Messenger msg){
	updateMainView(msg, "Today");
}

void MainWindow::getInbox(){
	reset();
	Messenger msg = scheduler->processCommand("find floating undone");
	handleGetInbox(msg);
}

void MainWindow::handleGetInbox(Messenger msg){
	updateMainView(msg, "Inbox");
}

QString MainWindow::prepareTrayMsg( list<Task> &reminderList, QString output ){
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

void MainWindow::showTrayMsg(QString msg, QString title){
	isFromReminder = false;
	trayIcon->showMessage(title, msg);
}

void MainWindow::changeEvent(QEvent* event){
	runInBackground(event);
	QMainWindow::changeEvent(event);
}

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

void MainWindow::handleDisplay(Messenger msg){
	int index = msg.getIndex();
	assert(index > 0);
	list<Task> tmp_list = msg.getList();
	list<Task>::iterator iter = tmp_list.begin();
	advance(iter, index - 1);

	updateStatusBar("Task displayed successfully");
	updateDetailsLabel();
	updateDetails(*iter);
}

void MainWindow::about(){
	popMsgBox("TaskPad", "TaskPad is a product created by Team F12-1C.\n"
		"Members: ASHRAY, KAI, JIANGZE, THYAGESH, ZIXUAN.");
}

void MainWindow::updateNavLabel(QString str){
	ui.Navigation_taskList->setText(str);
}

void MainWindow::updateDetailsLabel( QString str /*= "Task's Details"*/ ){
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

QTreeWidgetItem* MainWindow::extractTask(int index, Task task){
	QStringList strList;
	if(task.getTaskType() == TP::DEADLINE){
		strList = extractDueDate(task, strList, index);
	}
	else if(task.getTaskType() == TP::TIMED ||
		task.getFlagFromDate() ||
		task.getFlagToDate()){
		strList = extractTimedDate(task, strList, index);
	}
	else{//TaskType == TP::FLOATING
		strList = extractFloating(index, task);
	}
	if(task.getPriority() == HIGH)
		setHighPriorityDelegate(index);
	else
		setNormalDelegate(index);
	return new QTreeWidgetItem(strList);
}

void MainWindow::clearDetails(){
	setDetailsViewOpacity40();
	updateDetailsLabel("Task's Details");
	setDetailsViewEmpty();
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

void MainWindow::updateStatusBar(QString str){
	ui.StatusBar->setText(str);
}

QString MainWindow::getTimePeriodStr(pair<tm, tm> period){
	QDateTime fromDate = QDateTime::fromTime_t(mktime(&period.first));
	QDateTime toDate = QDateTime::fromTime_t(mktime(&period.second));
	QString fromStr = fromDate.toString("dd/MM/yyyy");
	QString toStr = toDate.toString("dd/MM/yyyy");
	return fromStr + " - " + toStr;
}

void MainWindow::customisedUi(){
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.TaskList->header()->resizeSection(0, 70);
	ui.TaskList->header()->resizeSection(1, 220);
	ui.TaskList->setItemDelegateForColumn(2, new LastColumnDelegate(ui.TaskList));
}

void MainWindow::mousePressEvent(QMouseEvent *event){
	this->windowPosition = this->pos();
	this->mousePressPosition = event->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
	this->mouseMovePosition = event->globalPos();
	QPoint distanceToMove = this->mouseMovePosition - this->mousePressPosition + this->windowPosition;
	this->move(distanceToMove);   
}

void MainWindow::installEventFilter(){
	ui.CloseButton->installEventFilter(this);
	ui.MinimizeButton->installEventFilter(this);
	ui.HelpButton->installEventFilter(this);
	ui.AboutButton->installEventFilter(this);
	ui.cmdBar->installEventFilter(this);
	ui.TaskList->installEventFilter(this);
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
}

void MainWindow::setupHotkeys(){
	QxtGlobalShortcut * scOpenQuickAddWin = new QxtGlobalShortcut(QKeySequence("Alt+`"), this);
	connect(scOpenQuickAddWin, SIGNAL(activated()),this, SLOT(showQuickAddWindow()));
	QxtGlobalShortcut * scOpenMainWin = new QxtGlobalShortcut(QKeySequence("Ctrl+Alt+t"), this);
	connect(scOpenMainWin, SIGNAL(activated()),this, SLOT(showWindow()));
	(void) new QShortcut(QKeySequence(tr("F5", "RemainderTesting")), this, SLOT(showReminder()));
	(void) new QShortcut(QKeySequence(tr("Ctrl+H", "Minimize")), this, SLOT(showMinimized()));
	(void) new QShortcut(QKeySequence(tr("Ctrl+T", "Today")), this, SLOT(getToday()));
	(void) new QShortcut(QKeySequence(tr("Alt+1", "Today")), this, SLOT(getToday()));
	(void) new QShortcut(QKeySequence(tr("Ctrl+I", "Inbox")), this, SLOT(getInbox()));
	(void) new QShortcut(QKeySequence(tr("Alt+2", "Inbox")), this, SLOT(getInbox()));
	(void) new QShortcut(QKeySequence(tr("Alt+D", "Show Next Day")), this, SLOT(showNextDay()));
	(void) new QShortcut(QKeySequence(tr("Alt+Shift+D", "Show Previous Day")), this, SLOT(showPrevDay()));
	(void) new QShortcut(QKeySequence(tr("Alt+W", "Show Next Week")), this, SLOT(showNextWeek()));
	(void) new QShortcut(QKeySequence(tr("Alt+Shift+W", "Show Previous Week")), this, SLOT(showPrevWeek()));
	(void) new QShortcut(QKeySequence(tr("Alt+M", "Show Next Month")), this, SLOT(showNextMonth()));
	(void) new QShortcut(QKeySequence(tr("Alt+Shift+M", "Show Previous Month")), this, SLOT(showPrevMonth()));
}

void MainWindow::setupTimer(){
	timer = new QTimer(this);
	timer->start(60000);
}

void MainWindow::setupTrayIcon(){
	isQuickAddOpen = false;
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon(":/MainWindow/Resources/logo.png"));
	trayIcon->show();
	trayIcon->setToolTip("TaskPad");
}

void MainWindow::setupScheduler(){
	scheduler = new Manager();
}

void MainWindow::setupDependency(){
	setupTimer();
	setupTrayIcon();
	setupConnection();
	setupHotkeys();
	setupScheduler();
}

void MainWindow::setupUI(){
	ui.setupUi(this);
	customisedUi();
	installEventFilter();
}

void MainWindow::dispose(){
	trayIcon->hide();
	delete scheduler;
	scheduler = NULL;
}

void MainWindow::updateMainView( Messenger &msg, QString listTitle, QString statusBarLabel /*= "Ready" */ ){
	clearDetails();
	updateStatusBar(statusBarLabel);
	updateNavLabel(listTitle);
	updateList(msg.getList());
}

void MainWindow::setCurrRemindTime(){
	currRemindTime = QDateTime::currentDateTime();
}

void MainWindow::setFromReminder( bool toggle ){
	isFromReminder = toggle;
}

void MainWindow::setIsQuickAddOpen( bool toggle ){
	isQuickAddOpen = toggle;
}

void MainWindow::setupQuickAddWindow(){
	quickAddWindowObj = new QuickAddWindow();
	quickAddWindowObj->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::setupQuickAddConnection(){
	QuickAddWindow *qa = (QuickAddWindow*) quickAddWindowObj;
	connect(qa, SIGNAL(windowClosed()), this, SLOT(closeQuickAddWindow()));
	connect(qa, SIGNAL(requestSubmitted(QString)), this, SLOT(handleQuickAddRequest(QString)));
}

void MainWindow::disposeQuickAddWindow(){
	QuickAddWindow *qa = (QuickAddWindow*) quickAddWindowObj;
	disconnect(qa, SIGNAL(windowClosed()), this, SLOT(closeQuickAddWindow()));
	disconnect(qa, SIGNAL(requestSubmitted(QString)), this, SLOT(handleQuickAddRequest(QString)));
	quickAddWindowObj->close();
	setIsQuickAddOpen(false);
}

bool MainWindow::isEqualOne( QString &requestStr ){
	return requestStr.toInt() == 1;
}

void MainWindow::handleQA_ERROR( QString requestStr, Messenger &msg ){
	if(!isEqualOne(requestStr))
		showTrayMsg(msg.getErrorMsg().c_str());
	else
		closeQuickAddWindow();
	showWindow();
}

void MainWindow::handleQA_SUCCESS(){
	closeQuickAddWindow();
	showTrayMsg("Added");
}

void MainWindow::handleQA_DISPLAY( Messenger msg ){
	closeQuickAddWindow();
	handleDisplay(msg);
	showWindow();
}

std::string MainWindow::getFindRtCmd(){
	string currRemindTimeStr = currRemindTime.toString("dd/MM/yy hh:mm").toStdString();
	string findCurrRtTasks = "find rt `" + currRemindTimeStr + "` undone";
	return findCurrRtTasks;
}

void MainWindow::updateDetailsView( Messenger &msg, QString label /*= "Task's Details"*/ ){
	updateDetails(msg.getList().front());
	updateDetailsLabel(label);
}

void MainWindow::updateDetailsView_GetTask( Messenger msg, QString detailsLabel /*= "Task's Details"*/ ){
	updateDetails(msg.getTask());
	updateDetailsLabel(detailsLabel);
}

void MainWindow::handleOneItemList( Messenger &msg, QString detailsLabel /*= "Task's Details" */ ){
	if(msg.getList().size() == 1){
		scheduler->syncTask(msg.getList().front());
		updateDetailsView(msg, detailsLabel);
	}
}

void MainWindow::popMsgBox( QString title, QString description ){
	QMessageBox msgBox;
	msgBox.setWindowTitle(title);
	msgBox.setText(description);
	msgBox.exec();
}

void MainWindow::runInBackground( QEvent* event ){
	if(event->type()==QEvent::WindowStateChange){
		if(windowState() == Qt::WindowMinimized){
			QTimer::singleShot(0, this, SLOT(hide()));
		}
	}
}

void MainWindow::refresh(){
	Messenger refreshedMsg = scheduler->refreshList();
	updateList(refreshedMsg.getList());
}

void MainWindow::updateForCmdExec( QString statusBarTxt, QString DetailsLabel, Messenger msg ){
	updateStatusBar(statusBarTxt);
	updateDetailsView_GetTask(msg, DetailsLabel);
	refresh();
}

void MainWindow::handleMsg_ERROR( Messenger &msg ){
	updateStatusBar(msg.getErrorMsg().c_str());
}

void MainWindow::handleMsg_ERROR_INTERMEDIATE(){
	updateNavLabel("Select a task by typing its index");
	updateStatusBar("Wrong input. Press ECS to cancel");
}

void MainWindow::handleMsg_SUCCESS( Messenger &msg ){
	switch(msg.getCommandType()){
	case TP::ADD:
		updateForCmdExec("Task added successfully", "Added Task's Details", msg);
		break;
	case TP::DEL:
		updateForCmdExec("Task deleted successfully", "Deleted Task's Details", msg);
		break;
	case TP::MOD:
		updateForCmdExec("Task modified successfully", "Modified Task's Details", msg);
		break;
	case TP::FIND:
		updateMainView(msg, "Search Results", "Searched successfully");
		handleOneItemList(msg);
		break;
	case TP::UNDO:
		updateForCmdExec("Undo successfully", "Undo Task's Details", msg);
		break;
	case TP::REDO:
		updateForCmdExec("Redo successfully", "Redo Task's Details", msg);
		break;
	}
}

void MainWindow::handleMsg_INTERMEDIATE( Messenger msg ){
	updateMainView(msg, "Select a task by typing its index", "Intermediate stage...");
}

void MainWindow::handleMsg_SUCCESS_INDEXED_CMD( Messenger &msg ){
	switch (msg.getCommandType()){
	case TP::MOD:
		updateForCmdExec("Task modified successfully","Modified Task's Details", msg);
		break;
	case TP::DEL:
		updateForCmdExec("Task deleted successfully", "Deleted Task's Details", msg);
		break;
	}
}

QStringList MainWindow::extractFloating( int index, Task &task ){
	return QStringList() << QString::number(index) << task.getName().c_str() << "";
}

QStringList MainWindow::extractTimedDate( Task &task, QStringList strList, int index ){
	QString fromTimeStr, toTimeStr;
	if(task.getFlagFromDate()){
		QDateTime fromTime = QDateTime::fromTime_t(task.getFromDate());
		fromTimeStr = "From " + fromTime.toString("dd/MM/yyyy");
	}
	if(task.getFlagToDate()){
		QDateTime toTime = QDateTime::fromTime_t(task.getToDate());
		if(task.getFlagFromDate())
			toTimeStr = " to " + toTime.toString("dd/MM/yyyy");
		else
			toTimeStr = "To " + toTime.toString("dd/MM/yyyy");
	}
	strList = QStringList() << QString::number(index) << task.getName().c_str() << \
		fromTimeStr + toTimeStr;	return strList;
}

QStringList MainWindow::extractDueDate( Task &task, QStringList strList, int index ){
	QDateTime time = QDateTime::fromTime_t(task.getDueDate());
	strList = QStringList() << QString::number(index) << task.getName().c_str() << \
		"Due " + time.toString("dd/MM/yyyy");	return strList;
}

void MainWindow::setHighPriorityDelegate( int index ){
	ui.TaskList->setItemDelegateForRow(index - 1, new HighPriorityDelegate(ui.TaskList));
}

void MainWindow::setNormalDelegate( int index ){
	ui.TaskList->setItemDelegateForRow(index - 1, NULL);
}

void MainWindow::setDetailsViewOpacity40(){
	QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect(this);
	opacity->setOpacity(qreal(40)/100);
	ui.DetailsView->setGraphicsEffect(opacity);
}

void MainWindow::setDetailsViewEmpty(){
	ui.name->setText("");
	ui.dueOrFromTo->setText("");
	ui.location->setText("");
	ui.participants->setText("");
	ui.tags->setText("");
	ui.remindTime->setText("");
	ui.note->setPlainText("");
}

void MainWindow::setDetailsViewOpacity100(){
	ui.DetailsView->setGraphicsEffect(NULL);
}

void MainWindow::setNameLabel( Task &task ){
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

void MainWindow::setPriorityLabel( Task &task ){
	if(task.getPriority() == TP::HIGH){
		ui.DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
			"	background-image:url(:/MainWindow/Resources/details_high_bg.png);\n"
			"}"));
	}
	else if(task.getPriority() == TP::MEDIUM){
		ui.DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
			"	background-image:url(:/MainWindow/Resources/details_normal_bg.png);\n"
			"}"));
	}
	else{
		ui.DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
			"	background-image:url(:/MainWindow/Resources/details_low_bg.png);\n"
			"}"));
	}
}

void MainWindow::setDueDateLabel( Task &task ){
	QDateTime time = QDateTime::fromTime_t(task.getDueDate());
	QTime hour_n_min = time.time();
	if(hour_n_min.hour() == 0 & hour_n_min.minute() == 0){
		ui.dueOrFromTo->setText("Due  " + time.toString("dd/MM/yyyy"));
	}
	else{
		ui.dueOrFromTo->setText("Due  " + time.toString("dd/MM/yyyy  hh:mm"));
	}
}

void MainWindow::setTimedLabel( Task &task ){
	QString fromTimeStr, toTimeStr;
	if(task.getFlagFromDate()){
		QDateTime fromTime = QDateTime::fromTime_t(task.getFromDate());
		fromTimeStr = "From " + fromTime.toString("dd/MM/yyyy  hh:mm");
	}
	if(task.getFlagToDate()){
		QDateTime toTime = QDateTime::fromTime_t(task.getToDate());
		if(task.getFlagFromDate()){
			toTimeStr = " to " + toTime.toString("dd/MM/yyyy  hh:mm");
		}
		else{
			toTimeStr = "To " + toTime.toString("dd/MM/yyyy  hh:mm");
		}
	}
	ui.dueOrFromTo->setText(fromTimeStr + toTimeStr);
}

void MainWindow::setFloatingLabel(){
	ui.dueOrFromTo->setText("");
}

void MainWindow::setDueOrFromToLabel( Task &task ){
	if(task.getTaskType() == TP::DEADLINE){
		setDueDateLabel(task);
	}
	else if(task.getTaskType() == TP::TIMED ||
		task.getFlagFromDate() ||
		task.getFlagToDate()){
			setTimedLabel(task);
	}
	else{//TaskType == TP::FLOATING
		setFloatingLabel();
	}
}

void MainWindow::setLocationLabel( Task &task ){
	if(task.getFlagLocation()){
		ui.location->setText(("@" + task.getLocation()).c_str());
	}
	else{
		ui.location->setText("");
	}
}

void MainWindow::setParticipantsLabel( Task &task ){
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
	else{
		ui.participants->setText("");
	}
}

void MainWindow::setTagsLabel( Task &task ){
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
	else{
		ui.tags->setText("");
	}
}

void MainWindow::setRemindTimesLabel( Task &task ){
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
	{
		ui.remindTime->setText("Remind me : none");
	}
}

void MainWindow::setNoteLabel( Task &task ){
	if(task.getFlagNote()){
		ui.note->setPlainText(task.getNote().c_str());
	}
	else{
		ui.note->setPlainText("");
	}
}
