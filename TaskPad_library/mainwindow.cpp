#include <QMouseEvent>
#include <qlist.h>
#include "mainwindow.h"
#include "lastColumnDelegate.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), inputHistory_undo(), inputHistory_redo() 
{
	ui.setupUi(this);
	customisedUi();
	QObject::connect(ui.CloseButton, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(ui.MinimizeButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
	QObject::connect(ui.AboutButton, SIGNAL(clicked()), this, SLOT(about()));
	ui.CommandBar->installEventFilter(this);
	ui.CloseButton->installEventFilter(this);
	ui.MinimizeButton->installEventFilter(this);
	ui.HelpButton->installEventFilter(this);
	ui.AboutButton->installEventFilter(this);
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
	else if(watched == ui.CommandBar)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->key() == Qt::Key_Up)
			{
				if(!inputHistory_undo.isEmpty())
				{
					QString lastInput = inputHistory_undo.pop();
					inputHistory_redo.push(lastInput);

					ui.CommandBar->clear();
					ui.CommandBar->insertHtml(inputHistory_redo.top());
				}
			}
			else if(keyEvent->key() == Qt::Key_Down)
			{
				if(!inputHistory_redo.isEmpty())
				{
					QString prevInput = inputHistory_redo.pop();
					inputHistory_undo.push(prevInput);

					ui.CommandBar->clear();
					if(!inputHistory_redo.isEmpty())
					{
						ui.CommandBar->insertHtml(inputHistory_redo.top());
					}
					else
					{
						ui.CommandBar->insertHtml("");
					}
				}
			}
			else if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			{
				QTextCursor cursor = ui.CommandBar->textCursor();
				QString currentInput = cursor.block().text().trimmed();
				if(currentInput != QString(""))
				{
					while(!inputHistory_redo.isEmpty())
					{
						QString input = inputHistory_redo.pop();
						inputHistory_undo.push(input);
					}

					inputHistory_undo.push(currentInput);
					ui.CommandBar->clear();
				}
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