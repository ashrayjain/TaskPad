#include <QMouseEvent>
#include <qlist.h>
#include "mainwindow.h"
#include "lastColumnDelegate.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	customisedUi();
	QObject::connect(ui.CloseButton, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(ui.MinimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
	QObject::connect(ui.AboutButton, SIGNAL(clicked()), this, SLOT(about()));
}

MainWindow::~MainWindow()
{

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
	ui.TaskList->setGeometry(9, 135, 600, ui.TaskList->topLevelItemCount() * 48 + 10);
	ui.TaskList->header()->resizeSection(0, 70);
	ui.TaskList->header()->resizeSection(1, 230);
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