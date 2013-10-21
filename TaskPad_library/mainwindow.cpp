#include <QMouseEvent>
#include <QMessageBox>
#include <QTextBlock>
#include <QShortcut>
#include <QDateTime>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <cassert>
#include "libqxt/qxtglobalshortcut.h"
#include "Enum.h"
#include "mainwindow.h"
#include "quickadd_window.h"
#include "Manager.h"
#include "lastColumnDelegate.h"
#include "CommandBar.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent) 
{
	//TODO: make it SLAP
	isQuickAddOpen = false;
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon(":/MainWindow/Resources/logo.png"));
	trayIcon->show();
	trayIcon->setToolTip("TaskPad");
	ui.setupUi(this);
	customisedUi();
	QxtGlobalShortcut * sc = new QxtGlobalShortcut(QKeySequence("Alt+`"), this);
    connect(sc, SIGNAL(activated()),this, SLOT(showQuickAddWindow()));
	QxtGlobalShortcut * sc2 = new QxtGlobalShortcut(QKeySequence("Ctrl+Alt+t"), this);
    connect(sc2, SIGNAL(activated()),this, SLOT(showWindow()));
	QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, 
		SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
	QObject::connect(ui.CloseButton, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(ui.MinimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
	QObject::connect(ui.AboutButton, SIGNAL(clicked()), this, SLOT(about()));
	QObject::connect(ui.HelpButton, SIGNAL(clicked()), this, SLOT(help()));
	(void) new QShortcut(QKeySequence(tr("Ctrl+T", "Today")), this, SLOT(getToday()));
	(void) new QShortcut(QKeySequence(tr("Ctrl+I", "Inbox")), this, SLOT(getInbox()));
	//ui.CommandBar->installEventFilter(this);//filter RETURN
	ui.CloseButton->installEventFilter(this);//filter MOUSE MOVE
	ui.MinimizeButton->installEventFilter(this);//filter MOUSE MOVE
	ui.HelpButton->installEventFilter(this);//filter MOUSE MOVE
	ui.AboutButton->installEventFilter(this);//filter MOUSE MOVE
	ui.cmdBar->installEventFilter(this);
	ui.TaskList->installEventFilter(this);

	scheduler = new Manager();
	getToday();
}

MainWindow::~MainWindow()
{
	trayIcon->hide();
	delete scheduler;
	scheduler = NULL;
}

void MainWindow::iconIsActived(QSystemTrayIcon::ActivationReason){
	showWindow();
}

void MainWindow::showWindow(){
	show();
	setWindowState(Qt::WindowActive);
	ui.cmdBar->setFocus();
}

void MainWindow::showQuickAddWindow(){
	QString input;

	if(!isQuickAddOpen){
		isQuickAddOpen = true;
		//no need to delete quickAddWindow
		//since it's set (Qt::WA_DeleteOnClose)
		//refer to: http://qt-project.org/doc/qt-5.0/qtcore/qt.html#WidgetAttribute-enum
		quickAddWindow = new QuickAddWindow();
		quickAddWindow->setAttribute(Qt::WA_DeleteOnClose);
		QuickAddWindow *qa = (QuickAddWindow*) quickAddWindow;
		connect(qa, SIGNAL(windowClosed()), this, SLOT(closeQuickAddWindow()));
		connect(qa, SIGNAL(requestSubmitted(QString)), this, SLOT(handleQuickAddRequest(QString)));
		quickAddWindow->show();
	}
}

void MainWindow::closeQuickAddWindow(){
	QuickAddWindow *qa = (QuickAddWindow*) quickAddWindow;
	disconnect(qa, SIGNAL(windowClosed()), this, SLOT(closeQuickAddWindow()));
	disconnect(qa, SIGNAL(requestSubmitted(QString)), this, SLOT(handleQuickAddRequest(QString)));
	quickAddWindow->close();
	isQuickAddOpen = false;
}

void MainWindow::handleQuickAddRequest(QString requestStr){
	const int FIRST_ITEM = 1;
	if(isCommandAdd(requestStr) ||
		requestStr.toInt() == FIRST_ITEM){
			reset();
			string requestStdStr = requestStr.toLocal8Bit().constData();
			Messenger msg = scheduler->processCommand(requestStdStr);
			if(msg.getStatus() == TP::ERROR)
			{
				showTrayMsg(msg.getErrorMsg().c_str());
			}
			else if(msg.getStatus() == TP::SUCCESS)
			{
				getToday();
				closeQuickAddWindow();
				showTrayMsg("Added");
			}
			else if(msg.getStatus() == TP::DISPLAY)
			{
				closeQuickAddWindow();
				handleDisplay(msg);
				showWindow();
			}
	}
	else{
		showTrayMsg("Only Add Command and Display 1 are supported");
	}
}

bool MainWindow::isCommandAdd(QString requestStr){
	const int CANT_FIND = -1;
	const QString COMMAND_ADD = "^add.*";
	const QRegExp REGEX_CMD_ADD(COMMAND_ADD);
	return REGEX_CMD_ADD.indexIn(requestStr) != CANT_FIND;
}

void MainWindow::help(){
	QMessageBox msgBox;
	msgBox.setText("Geek doesn't need help from us :p");
	msgBox.exec();
}

void MainWindow::keyPressEvent(QKeyEvent* event){
	ui.cmdBar->setFocus();
	if(event->key() == Qt::Key_Escape)
	{
		reset();
		getToday();
	}
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
	bool isToday = true;
	updateNavLabel("Today");
	updateStatusBar("Ready");
	clearDetails();
	updateList(msg.getList(), isToday);
}

void MainWindow::getInbox(){
	reset();
	Messenger msg = scheduler->processCommand("find undone");
	handleGetInbox(msg);
}

void MainWindow::handleGetInbox(Messenger msg){
	updateNavLabel("Inbox");
	updateStatusBar("Ready");
	clearDetails();
	updateList(msg.getList());
}

void MainWindow::showReminder(){
	trayIcon->showMessage("TaskPad", "msg here");
}

void MainWindow::showTrayMsg(QString msg){
	trayIcon->showMessage("TaskPad", msg);
}

void MainWindow::changeEvent(QEvent* event){
	if(event->type()==QEvent::WindowStateChange){
		if(windowState() == Qt::WindowMinimized)
		{
			QTimer::singleShot(0, this, SLOT(hide()));
		}
	}
	QMainWindow::changeEvent(event);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
	if(watched == ui.CloseButton || watched == ui.MinimizeButton ||
		watched == ui.HelpButton || watched == ui.AboutButton)
	{
		if(event->type() == QEvent::MouseMove)
		{
			return true;
		}
	}
	/*else if(watched == ui.TaskList)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			QString currentStr = ui.cmdBar->getCurrentLine();
			QString appendStr(keyEvent->key());
			ui.cmdBar->setText(currentStr + appendStr);
			ui.cmdBar->setFocus();
			ui.cmdBar->moveCursor(QTextCursor::EndOfLine);
			return true;
		}
	}*/
	else if(watched == ui.cmdBar)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->key() == Qt::Key_Escape){
				reset();
				getToday();
			}
			else if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			{
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
	//not SLAP now
	if(msg.getStatus() == TP::ERROR)
	{
		updateStatusBar(msg.getErrorMsg().c_str());
	}
	else if(msg.getStatus() == TP::ERROR_INTERMEDIATE)
	{
		updateNavLabel("Select a task by typing its index");
		updateStatusBar("Wrong input. Press ECS to cancel");
		//QuickAdd cannot reach here, as it calls reset every time before submit
	}
	else if(msg.getStatus() == TP::SUCCESS)
	{
		Messenger td_msg;
		switch(msg.getCommandType()){
		case TP::ADD:
			getToday();
			updateStatusBar("Task added successfully");
			updateDetailsLabel("Added Task's Details");
			updateDetails(msg.getTask());
			break;
		case TP::DEL:
			getToday();
			updateStatusBar("Task deleted successfully");
			updateDetailsLabel("Deleted Task's Details");
			updateDetails(msg.getTask());
			break;
		case TP::MOD:
			getToday();
			updateStatusBar("Task modified successfully");
			updateDetailsLabel("Modified Task's Details");
			updateDetails(msg.getTask());
			break;
		case TP::FIND:
			updateNavLabel("Search Results");
			updateStatusBar("Searched successfully");
			clearDetails();
			updateList(msg.getList());
			if(msg.getList().size() == 1){
				updateDetails(msg.getList().front());
			}
			break;
		case TP::UNDO:
			getToday();
			updateStatusBar("Undo successfully");
			updateDetailsLabel("Undo Task's Details");
			updateDetails(msg.getTask());
			break;
		case TP::REDO:
			getToday();
			updateStatusBar("Redo successfully");
			updateDetailsLabel("Redo Task's Details");
			updateDetails(msg.getTask());
			break;
		}
	}
	else if(msg.getStatus() == TP::INTERMEDIATE)
	{
		updateNavLabel("Select a task by typing its index");
		updateStatusBar("Intermediate stage...");
		updateList(msg.getList());
	}
	else if(msg.getStatus() == TP::DISPLAY)
	{
		handleDisplay(msg);
	}
	else if(msg.getStatus() == TP::SUCCESS_INDEXED_COMMAND)
	{
		switch (msg.getCommandType()){
		case TP::MOD:
			updateStatusBar("Task modified successfully");
			updateDetailsLabel("Modified Task's Details");
			break;
		case TP::DEL:
			updateStatusBar("Task deleted successfully");//TODO: make it into function.. to reuse
			updateDetailsLabel("Deleted Task's Details");
			break;
		}

		if(ui.Navigation_taskList->text() == "Today")
			updateList(msg.getList(), true);
		else
			updateList(msg.getList());
		updateDetails(msg.getTask());
	}
}

void MainWindow::handleDisplay(Messenger msg){
	int index = msg.getIndex();
	assert(index > 0);
	list<Task> tmp_list = msg.getList();
	list<Task>::iterator iter = tmp_list.begin();
	advance(iter, index - 1);

	updateStatusBar("Task displayed successfully");
	updateDetailsLabel("Task's Details");
	updateDetails(*iter);
}

void MainWindow::about()
{
	QMessageBox Msgbox;
	Msgbox.setWindowTitle("TaskPad");
    Msgbox.setText("TaskPad is a product created by Team F12-1C.\n"
		"Members: ASHRAY, KAI, JIANGZE, THYAGESH, ZIXUAN.");
    Msgbox.exec();
}

void MainWindow::updateNavLabel(QString str){
	ui.Navigation_taskList->setText(str);
}

void MainWindow::updateDetailsLabel(QString str){
	ui.Navigation_detailsView->setText(str);
}

void MainWindow::updateList(std::list<Task> result, bool isToday){
	QTreeWidgetItem* item = NULL;

	ui.TaskList->clear();

	int count = 1;
	for(std::list<Task>::const_iterator iter = result.begin();
		iter != result.end();
		std::advance(iter, 1))
	{
		if(isToday){
			item = extractTaskForToday(count, *iter);
		}
		else{//not today
			item = extractTask(count, *iter);
		}
		ui.TaskList->addTopLevelItem(item);
		count++;
	}
}

QTreeWidgetItem* MainWindow::extractTask(int index, Task task){
	QStringList strList;
	if(task.getTaskType() == TP::DEADLINE){
		QDateTime time = QDateTime::fromTime_t(task.getDueDate());
		strList = QStringList() << QString::number(index) << task.getName().c_str() << \
			"Due " + time.toString("dd/MM/yyyy");
	}
	else if(task.getTaskType() == TP::TIMED ||
		task.getFlagFromDate() ||
		task.getFlagToDate()){
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
			fromTimeStr + toTimeStr;
	}
	else{//TaskType == TP::FLOATING
		strList = QStringList() << QString::number(index) << task.getName().c_str() << "";
	}
	return new QTreeWidgetItem(strList);
}

//TODO: can combine into one
QTreeWidgetItem* MainWindow::extractTaskForToday(int index, Task task){
	QStringList strList;
	if(task.getTaskType() == TP::DEADLINE){
		QDateTime time = QDateTime::fromTime_t(task.getDueDate());
		QTime due_hour_n_min = time.time();
		QString dueStr;
		if(due_hour_n_min.hour() == 0 && due_hour_n_min.minute() == 0){
			dueStr = "Due today";
		}
		else{
			dueStr = "Due " + time.toString("hh:mm");
		}

		strList = QStringList() << QString::number(index) << task.getName().c_str() << \
			dueStr;
	}
	else if(task.getTaskType() == TP::TIMED ||
		task.getFlagFromDate() ||
		task.getFlagToDate()){
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
			fromTimeStr + toTimeStr;
	}
	else{//TaskType == TP::FLOATING
		strList = QStringList() << QString::number(index) << task.getName().c_str() << "";
	}
	return new QTreeWidgetItem(strList);
}

void MainWindow::clearDetails(){
	QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect(this);
	opacity->setOpacity(qreal(40)/100);
	ui.DetailsView->setGraphicsEffect(opacity);
	updateDetailsLabel("Details");
	ui.name->setText("");
	ui.dueOrFromTo->setText("");
	ui.location->setText("");
	ui.participants->setText("");
	ui.tags->setText("");
	ui.remindTime->setText("");
	ui.note->setPlainText("");
}

void MainWindow::updateDetails(Task t){
	ui.DetailsView->setGraphicsEffect(NULL);
	Task task_showDetails = t;
	//set label name
	if(task_showDetails.getState() == TP::UNDONE){
		ui.name->setText(task_showDetails.getName().c_str());
	}
	else{//DONE already
		ui.name->setText(("(Finished) " + task_showDetails.getName()).c_str());
	}
	//set priority
	if(task_showDetails.getPriority() == TP::HIGH){
		ui.DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
"	background-image:url(:/MainWindow/Resources/details_high_bg.png);\n"
"}"));
	}
	else if(task_showDetails.getPriority() == TP::MEDIUM){
		ui.DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
"	background-image:url(:/MainWindow/Resources/details_normal_bg.png);\n"
"}"));
	}
	else{
		ui.DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
"	background-image:url(:/MainWindow/Resources/details_low_bg.png);\n"
"}"));
	}
	
	//set label dueOrFromTo
	if(task_showDetails.getTaskType() == TP::DEADLINE){
		QDateTime time = QDateTime::fromTime_t(task_showDetails.getDueDate());
		QTime hour_n_min = time.time();
		if(hour_n_min.hour() == 0 & hour_n_min.minute() == 0){
			ui.dueOrFromTo->setText("Due  " + time.toString("dd/MM/yyyy"));
		}
		else{
			ui.dueOrFromTo->setText("Due  " + time.toString("dd/MM/yyyy  hh:mm"));
		}
	}
	else if(task_showDetails.getTaskType() == TP::TIMED ||
		task_showDetails.getFlagFromDate() ||
		task_showDetails.getFlagToDate()){
		//TODO: redundent... make into one function
		QString fromTimeStr, toTimeStr;
		if(task_showDetails.getFlagFromDate()){
			QDateTime fromTime = QDateTime::fromTime_t(task_showDetails.getFromDate());
			QTime hour_n_min = fromTime.time();
			if(hour_n_min.hour() == 0 & hour_n_min.minute() == 0)
				fromTimeStr = "From " + fromTime.toString("dd/MM/yyyy");
			else
				fromTimeStr = "From " + fromTime.toString("dd/MM/yyyy  hh:mm");
		}
		if(task_showDetails.getFlagToDate()){
			QDateTime toTime = QDateTime::fromTime_t(task_showDetails.getToDate());
			QTime hour_n_min = toTime.time();
			if(task_showDetails.getFlagFromDate()){
				if(hour_n_min.hour() == 0 & hour_n_min.minute() == 0)
					toTimeStr = " to " + toTime.toString("dd/MM/yyyy");
				else
					toTimeStr = " to " + toTime.toString("dd/MM/yyyy  hh:mm");
			}
			else{
				if(hour_n_min.hour() == 0 & hour_n_min.minute() == 0)
					toTimeStr = "To " + toTime.toString("dd/MM/yyyy");
				else
					toTimeStr = "To " + toTime.toString("dd/MM/yyyy  hh:mm");
			}
		}
		ui.dueOrFromTo->setText(fromTimeStr + toTimeStr);
	}
	else{//TaskType == TP::FLOATING
		ui.dueOrFromTo->setText("");
	}
	//set label location
	if(task_showDetails.getFlagLocation()){
		ui.location->setText(("@" + task_showDetails.getLocation()).c_str());
	}
	else{
		ui.location->setText("");
	}
	//set label participants
	if(task_showDetails.getFlagParticipants()){
		QString participants;
		list<string> listOfParticipants =  task_showDetails.getParticipants();
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
	//set tags label
	if(task_showDetails.getFlagTags()){
		QString tags;
		list<string> listOfTags = task_showDetails.getTags();
		list<string>::iterator iter = listOfTags.begin();
		tags += iter->c_str();
		iter++;
		for(;iter != listOfTags.end();
			advance(iter, 1)){
				tags += ", ";
				tags += iter->c_str();
		}
		ui.tags->setText(tags);
	}
	else{
		ui.tags->setText("");
	}
	//set remind time
	if(task_showDetails.getFlagRemindTimes()){
		QString remindTimes;
		list<time_t> listOfRemindTimes = task_showDetails.getRemindTimes();
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
	//set textBox note
	if(task_showDetails.getFlagNote()){
		ui.note->setPlainText(task_showDetails.getNote().c_str());
	}
	else{
		ui.note->setPlainText("");
	}
}

void MainWindow::updateStatusBar(QString str){
	ui.StatusBar->setText(str);
}

void MainWindow::customisedUi(){
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	//magic number
	ui.TaskList->header()->resizeSection(0, 70);
	ui.TaskList->header()->resizeSection(1, 220);
	ui.TaskList->setItemDelegate(new LastColumnDelegate(2, ui.TaskList));
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