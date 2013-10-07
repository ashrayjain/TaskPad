#include <QMouseEvent>
#include <QMessageBox>
#include <QTextBlock>
#include <QShortcut>
#include <QDateTime>
#include "Enum.h"
#include "mainwindow.h"
#include "Manager.h"
#include "lastColumnDelegate.h"
#include "CommandBar.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent) 
{
	scheduler = new Manager();
	/*Messenger msg = scheduler->getToday();
	handleMessenger(msg);*/

	ui.setupUi(this);
	customisedUi();
	QObject::connect(ui.CloseButton, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(ui.MinimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
	QObject::connect(ui.AboutButton, SIGNAL(clicked()), this, SLOT(about()));
	(void) new QShortcut(QKeySequence(tr("Ctrl+N", "New Task")), this, SLOT(createNewTaskTemplate()));
	//ui.CommandBar->installEventFilter(this);//filter RETURN
	ui.CloseButton->installEventFilter(this);//filter MOUSE MOVE
	ui.MinimizeButton->installEventFilter(this);//filter MOUSE MOVE
	ui.HelpButton->installEventFilter(this);//filter MOUSE MOVE
	ui.AboutButton->installEventFilter(this);//filter MOUSE MOVE
	ui.cmdBar->installEventFilter(this);
}

MainWindow::~MainWindow()
{
	delete scheduler;
	scheduler = NULL;
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
			if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			{
				QString currentInput = ui.cmdBar->getCurrentLine();//TODO: can shrink into one API
				ui.cmdBar->pushCurrentLine();
				Messenger msg = scheduler->processCommand(currentInput.toLocal8Bit().constData());
				handleMessenger(msg);
				return true;//stop Key return or Key enter
			}
		}
	}
	return QObject::eventFilter(watched, event);//normal processing
}

void MainWindow::handleMessenger(Messenger msg){
	if(msg.getStatus() == TP::ERROR)
	{
		updateStatusBar(msg.getErrorMsg().c_str());
	}
	else if(msg.getStatus() == TP::SUCCESS)
	{
		//no command type yet in response object
		updateStatusBar("what what successfully");
	}
	else if(msg.getStatus() == TP::INTERMEDIATE)
	{
		//no command type yet in response object
		updateStatusBar("...");
	}
	else if(msg.getStatus() == TP::SUCCESS_INDEXED_COMMAND)
	{
		//no command type yet in response object
		updateStatusBar("what what successfully");
	}
	else if(msg.getStatus() == TP::DISPLAY)
	{
		//no command type yet in response object
		updateStatusBar("xxx in details");
	}
}

void MainWindow::about()
{
	QMessageBox Msgbox;
	Msgbox.setWindowTitle("");
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
	ui.TaskList->clear();

	int count = 1;
	for(std::list<Task>::const_iterator iter = result.begin();
		iter != result.end();
		std::advance(iter, 1))
	{
		extractTask(count, *iter);
		count++;
	}
}

void MainWindow::extractTask(int index, Task task){
	QStringList strList;
	if(task.getTaskType() == TP::DEADLINE){
		DeadlineTask* d_task = dynamic_cast<DeadlineTask*>(&task);
		QDateTime time = QDateTime::fromTime_t(d_task->getDueDate());
		strList = QStringList() << QString::number(index) << d_task->getName().c_str() << \
			"Due " + time.toString();
	}
	else if(task.getTaskType() == TP::TIMED){
		TimedTask* d_task = dynamic_cast<TimedTask*>(&task);
		QDateTime fromTime = QDateTime::fromTime_t(d_task->getFromDate());
		QDateTime toTime = QDateTime::fromTime_t(d_task->getToDate());
		strList = QStringList() << QString::number(index) << d_task->getName().c_str() << \
			"From " + fromTime.toString() + " to " + toTime.toString();
	}
	else{//TaskType == TP::FLOATING
		strList = QStringList() << QString::number(index) << task.getName().c_str() << "";
	}
	(void) new QTreeWidgetItem(ui.TaskList, strList);
}

void MainWindow::updateDetails(std::list<Task> result){
	Task task_showDetails = *(result.begin());
	//set label name
	ui.name->setText(task_showDetails.getName().c_str());
	//set label dueOrFromTo
	if(task_showDetails.getTaskType() == TP::DEADLINE){
		DeadlineTask* d_task = dynamic_cast<DeadlineTask*>(&task_showDetails);
		QDateTime time = QDateTime::fromTime_t(d_task->getDueDate());
		ui.dueOrFromTo->setText("Due " + time.toString());
	}
	else if(task_showDetails.getTaskType() == TP::TIMED){
		TimedTask* d_task = dynamic_cast<TimedTask*>(&task_showDetails);
		QDateTime fromTime = QDateTime::fromTime_t(d_task->getFromDate());
		QDateTime toTime = QDateTime::fromTime_t(d_task->getToDate());
		ui.dueOrFromTo->setText("From " + fromTime.toString() + " to " + toTime.toString());
	}
	else{//TaskType == TP::FLOATING
		ui.dueOrFromTo->setText("");
	}
	//set label location
	ui.location->setText(task_showDetails.getLocation().c_str());
	//set label participants
	ui.participants->setText(task_showDetails.getParticipants().c_str());
	//set tags label
	ui.tags->setText(task_showDetails.getTags().c_str());
	//set remind time
	if(task_showDetails.getFlagRemindTime()){
		QDateTime remindTime = QDateTime::fromTime_t(task_showDetails.getRemindTime());
		ui.remindTime->setText("Remind me : " + remindTime.toString());
	}
	else
	{
		ui.remindTime->setText("Remind me : none");
	}
	//set textBox note
	ui.note->setPlainText(task_showDetails.getNote().c_str());
}

void MainWindow::updateStatusBar(QString str){
	ui.StatusBar->setText(str);
}

void MainWindow::customisedUi(){
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);

	//magic number
	ui.TaskList->setGeometry(9, 135, 595, ui.TaskList->topLevelItemCount() * 48 + 10);
	ui.TaskList->header()->resizeSection(0, 70);
	ui.TaskList->header()->resizeSection(1, 225);
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