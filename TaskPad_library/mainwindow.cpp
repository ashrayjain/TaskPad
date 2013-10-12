#include <QMouseEvent>
#include <QMessageBox>
#include <QTextBlock>
#include <QShortcut>
#include <QDateTime>
#include <QGraphicsOpacityEffect>
#include "Enum.h"
#include "mainwindow.h"
#include "Manager.h"
#include "lastColumnDelegate.h"
#include "CommandBar.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent) 
{
	ui.setupUi(this);
	customisedUi();
	QObject::connect(ui.CloseButton, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(ui.MinimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
	QObject::connect(ui.AboutButton, SIGNAL(clicked()), this, SLOT(about()));
	QObject::connect(ui.HelpButton, SIGNAL(clicked()), this, SLOT(help()));
	(void) new QShortcut(QKeySequence(tr("Ctrl+N", "New Task")), this, SLOT(createNewTaskTemplate()));
	//ui.CommandBar->installEventFilter(this);//filter RETURN
	ui.CloseButton->installEventFilter(this);//filter MOUSE MOVE
	ui.MinimizeButton->installEventFilter(this);//filter MOUSE MOVE
	ui.HelpButton->installEventFilter(this);//filter MOUSE MOVE
	ui.AboutButton->installEventFilter(this);//filter MOUSE MOVE
	ui.cmdBar->installEventFilter(this);

	scheduler = new Manager();
	getToday();
}

MainWindow::~MainWindow()
{
	delete scheduler;
	scheduler = NULL;
}
void MainWindow::help(){
	QMessageBox msgBox;
	msgBox.setText("Geek doesn't need help from us :p");
	msgBox.exec();
}
void MainWindow::test(){
	int tc = 8;//select test case here

	Task t1;
	t1.setName("task 1 task 1");
	t1.setLocation("NUS LT27");
	t1.setDueDate(time(NULL));

	Task t2;
	t2.setName("task 2 task 2");
	t2.setLocation("COM1 B1-13");
	t2.setFromDate(time(NULL));
	t2.setToDate(time(NULL));

	Task mod_t2;
	mod_t2.setName("task 2 task 2___MOD");
	mod_t2.setLocation("COM1 B1-13");

	Task t3;
	t3.setName("task 3 task 3");
	t3.setLocation("NTU + NUS somewhere lol");
	t3.setFromDate(time(NULL));
	t3.setToDate(time(NULL));
	t3.setPriority(TP::HIGH);
	t3.setState(TP::DONE);

	Task t4;
	t4.setName("task 4 task 4");
	t4.setLocation("PGP DR4");
	t4.setFromDate(time(NULL));
	t4.setToDate(time(NULL));

	list<Task> taskList;
	taskList.push_back(t1);//0
	taskList.push_back(t2);//1
	taskList.push_back(t3);//2
	taskList.push_back(t4);//3

	list<Task> taskList2;
	taskList2.push_back(t3);
	taskList2.push_back(t2);
	taskList2.push_back(t1);

	list<Task> taskList3;
	taskList3.push_back(t3);

	//************************************************************

	//TC1: (ERROR)
	if(tc == -1){
		Messenger tc1msg;
		tc1msg.setStatus(TP::ERROR);
		tc1msg.setCommandType(TP::FIND);
		tc1msg.setList(taskList);
		tc1msg.setErrorMsg("I wanna sleep zzz..");
		handleMessenger(tc1msg);
	}
	//TC1: get Today
	if(tc == 0){
		Messenger tc1msg;
		tc1msg.setStatus(TP::SUCCESS);
		tc1msg.setCommandType(TP::FIND);
		tc1msg.setList(taskList);
		handleGetToday(tc1msg);
	}
	//TC2: add a simple task (SUCCESS)
	else if(tc == 1){
		Messenger tc1msg;
		tc1msg.setStatus(TP::SUCCESS);
		tc1msg.setCommandType(TP::ADD);
		tc1msg.setList(taskList2);//the first task is t3
		handleMessenger(tc1msg);
	}
	//TC3: modify a task (SUCCESS)
	else if(tc == 2){
		Messenger tc1msg;
		tc1msg.setStatus(TP::SUCCESS);
		tc1msg.setCommandType(TP::MOD);
		tc1msg.setList(taskList2);//the first task is t3
		handleMessenger(tc1msg);
	}
	//TC4: modify a task (SUCCESS)
	else if(tc == 3){
		Messenger tc1msg;
		tc1msg.setStatus(TP::SUCCESS);
		tc1msg.setCommandType(TP::DEL);
		tc1msg.setList(taskList2);//the first task is t3
		handleMessenger(tc1msg);
	}
	//TC5: modify a task (INTERMEDIATE)
	else if(tc == 4){
		Messenger tc1msg;
		tc1msg.setStatus(TP::INTERMEDIATE);
		tc1msg.setCommandType(TP::MOD);
		tc1msg.setList(taskList2);
		handleMessenger(tc1msg);
	}
	//TC6: find a task (SUCCESS)
	else if(tc == 5){
		Messenger tc1msg;
		tc1msg.setStatus(TP::SUCCESS);
		tc1msg.setCommandType(TP::FIND);
		tc1msg.setList(taskList2);
		handleMessenger(tc1msg);
	}
	//TC7: display a task (DISPLAY)
	else if(tc == 6){
		//get today
		Messenger tc0msg;
		tc0msg.setStatus(TP::SUCCESS);
		tc0msg.setCommandType(TP::FIND);
		tc0msg.setList(taskList);
		handleGetToday(tc0msg);
		//display 1 (start from 0)
		Messenger tc1msg;
		tc1msg.setStatus(TP::DISPLAY);
		tc1msg.setInt(1);
		handleMessenger(tc1msg);
	}
	//TC8: CWI mod (SUCCESS_INDEXED_COMMAND)
	else if(tc == 7){
		//get today
		Messenger tc0msg;
		tc0msg.setStatus(TP::SUCCESS);
		tc0msg.setCommandType(TP::FIND);
		tc0msg.setList(taskList);
		handleGetToday(tc0msg);
		//mod 1
		Messenger tc1msg;
		tc1msg.setCommandType(TP::MOD);
		tc1msg.setStatus(TP::SUCCESS_INDEXED_COMMAND);
		tc1msg.setInt(1);
		list<Task> tmp;
		tmp.push_back(mod_t2);
		tc1msg.setList(tmp);
		handleMessenger(tc1msg);
	}
	//TC9: CWI DEL (SUCCESS_INDEXED_COMMAND)
	else if(tc == 8){
		//get today
		Messenger tc0msg;
		tc0msg.setStatus(TP::SUCCESS);
		tc0msg.setCommandType(TP::FIND);
		tc0msg.setList(taskList);
		handleGetToday(tc0msg);
		//mod 1
		Messenger tc1msg;
		tc1msg.setCommandType(TP::DEL);
		tc1msg.setStatus(TP::SUCCESS_INDEXED_COMMAND);
		tc1msg.setInt(1);
		handleMessenger(tc1msg);
	}
	//TC10: (ERROR_INTERMEDIATE)
	else if(tc == 9){
		//get today
		Messenger tc0msg;
		tc0msg.setStatus(TP::ERROR_INTERMEDIATE);
		tc0msg.setCommandType(TP::FIND);
		tc0msg.setList(taskList);
		handleMessenger(tc0msg);
	}
}

void MainWindow::keyPressEvent(QKeyEvent* event){
	if(event->key() == Qt::Key_Escape)
	{
		reset();
	}
}

void MainWindow::reset(){
	scheduler->resetStatus();
	getToday();
}

void MainWindow::getToday(){
	QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect(this);
	opacity->setOpacity(qreal(40)/100);
	ui.DetailsView->setGraphicsEffect(opacity);
	/*ui.DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
"	background-image:url(:/MainWindow/Resources/details_default_bg.png);\n"
"}"));*/
	Messenger msg = scheduler->getTodayTasks();
	handleGetToday(msg);
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
	else if(watched == ui.cmdBar)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->key() == Qt::Key_Escape){
				reset();
			}
			else if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			{
				QString currentInput = ui.cmdBar->getCurrentLine();//TODO: can shrink into one API
				ui.cmdBar->pushCurrentLine();
				/*QMessageBox msgBox;
				msgBox.setText(currentInput);
				msgBox.exec();*/
				if(!currentInput.isEmpty()){
					Messenger msg = scheduler->processCommand(currentInput.toLocal8Bit().constData());
					handleMessenger(msg);
				}
				return true;//stop Key return or Key enter
			}
		}
	}
	return QObject::eventFilter(watched, event);//normal processing
}

void MainWindow::handleGetToday(Messenger msg){
	updateNavLabel("Inbox");
	updateStatusBar("Ready");
	updateDetailsLabel("Details");
	clearDetails();
	updateList(msg.getList());
	lastTimeList = msg.getList();//TODO: do i need to sync this lastTimeList? or manager does it? like after delete some items..
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
			lastTimeList = msg.getList();
			break;
		}
	}
	else if(msg.getStatus() == TP::INTERMEDIATE)
	{
		updateNavLabel("Select a task by typing its index");
		updateStatusBar("Intermediate stage...");
		updateList(msg.getList());
		lastTimeList = msg.getList();
	}
	else if(msg.getStatus() == TP::DISPLAY)
	{
		int index = msg.getIndex();
		list<Task> tmp_list = msg.getList();
		list<Task>::iterator iter = tmp_list.begin();
		advance(iter, index - 1);
		
		updateStatusBar("Task displayed successfully");
		updateDetailsLabel("Task's Details");
		updateDetails(*iter);
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

		updateList(msg.getList());
		updateDetails(msg.getTask());
	}
}

void MainWindow::about()
{
	QMessageBox Msgbox;
	Msgbox.setWindowTitle("TaskPad");
    Msgbox.setText("TaskPad is a product created by Team F12-1C.\n"
		"Members: ASHRAY, KAI, JIANGZE, THYAGESH, ZIXUAN.");
    Msgbox.exec();
}

void MainWindow::createNewTaskTemplate()
{
	ui.cmdBar->createNewTaskTemplate();
}

void MainWindow::updateNavLabel(QString str){
	ui.Navigation_taskList->setText(str);
}

void MainWindow::updateDetailsLabel(QString str){
	ui.Navigation_detailsView->setText(str);
}

void MainWindow::updateList(std::list<Task> result){
	QTreeWidgetItem* item = NULL;

	ui.TaskList->clear();

	int count = 1;
	for(std::list<Task>::const_iterator iter = result.begin();
		iter != result.end();
		std::advance(iter, 1))
	{
		item = extractTask(count, *iter);
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
	else if(task.getTaskType() == TP::TIMED){
		QDateTime fromTime = QDateTime::fromTime_t(task.getFromDate());
		QDateTime toTime = QDateTime::fromTime_t(task.getToDate());
		strList = QStringList() << QString::number(index) << task.getName().c_str() << \
			"From " + fromTime.toString("dd/MM/yyyy") + " to " + toTime.toString("dd/MM/yyyy");
	}
	else{//TaskType == TP::FLOATING
		strList = QStringList() << QString::number(index) << task.getName().c_str() << "";
	}
	return new QTreeWidgetItem(strList);
}

void MainWindow::clearDetails(){
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
		ui.dueOrFromTo->setText("Due  " + time.toString("dd/MM/yyyy"));
	}
	else if(task_showDetails.getTaskType() == TP::TIMED){
		QDateTime fromTime = QDateTime::fromTime_t(task_showDetails.getFromDate());
		QDateTime toTime = QDateTime::fromTime_t(task_showDetails.getToDate());
		ui.dueOrFromTo->setText("From  " + fromTime.toString("dd/MM/yyyy") + "  to  " + toTime.toString("dd/MM/yyyy"));
	}
	else{//TaskType == TP::FLOATING
		ui.dueOrFromTo->setText("");
	}
	//set label location
	if(task_showDetails.getFlagLocation()){
		ui.location->setText(task_showDetails.getLocation().c_str());
	}
	else{
		ui.location->setText("");
	}
	//set label participants
	if(task_showDetails.getFlagParticipants()){
		QString participants;
		list<string> listOfParticipants =  task_showDetails.getParticipants();
		for(list<string>::iterator iter = listOfParticipants.begin();
			iter != listOfParticipants.end();
			advance(iter, 1)){
				participants += iter->c_str();
				participants += ", ";
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
		for(list<string>::iterator iter = listOfTags.begin();
			iter != listOfTags.end();
			advance(iter, 1)){
				tags += iter->c_str();
				tags += ", ";
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
		for(list<time_t>::iterator iter = listOfRemindTimes.begin();
			iter != listOfRemindTimes.end();
			advance(iter, 1)){
				remindTimes += QDateTime::fromTime_t(*iter).toString("dd/MM/yyyy");
				remindTimes += ", ";
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
	ui.TaskList->setItemDelegate(new LastColumnDelegate(2));
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