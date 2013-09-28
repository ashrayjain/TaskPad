#include <QMouseEvent>
#include <QMessageBox>
#include <QTextBlock>
#include "mainwindow.h"
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
	//ui.CommandBar->installEventFilter(this);//filter RETURN
	ui.CloseButton->installEventFilter(this);//filter MOUSE MOVE
	ui.MinimizeButton->installEventFilter(this);//filter MOUSE MOVE
	ui.HelpButton->installEventFilter(this);//filter MOUSE MOVE
	ui.AboutButton->installEventFilter(this);//filter MOUSE MOVE
	ui.cmdBar->installEventFilter(this);
}

MainWindow::~MainWindow()
{

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
				QString currentInput = ui.cmdBar->getCurrentLine();
				ui.cmdBar->pushCurrentLine();
				return true;//stop Key return or Key enter
			}
		}
	}
	return QObject::eventFilter(watched, event);//normal processing
}

void MainWindow::about()
{
	QMessageBox Msgbox;
	Msgbox.setWindowTitle("");
    Msgbox.setText("TaskPad is a product created by Team F12-1C.\n"
		"Members: ASHRAY, KAI, JIANGZE, THYAGESH, ZIXUAN.");
    Msgbox.exec();
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