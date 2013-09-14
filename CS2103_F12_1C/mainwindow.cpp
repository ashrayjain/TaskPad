#include <QMouseEvent>
#include <qlist.h>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	customisedUi();
	QObject::connect(ui.CloseButton, SIGNAL(clicked()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::customisedUi(){
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	//magic number
	ui.TaskList->setGeometry(9, 135, 782, ui.TaskList->topLevelItemCount() * 48 + 10);
	ui.TaskList->setRootIsDecorated(false);
	ui.TaskList->setFrameStyle(QFrame::NoFrame);
	ui.TaskList->header()->setStretchLastSection(false);
	ui.TaskList->header()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.TaskList->header()->resizeSection(0, 50);
	ui.TaskList->header()->resizeSection(2, 150);
	ui.TaskList->header()->resizeSection(3, 150);
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