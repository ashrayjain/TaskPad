#include "quickadd_window.h"

QuickAddWindow::QuickAddWindow(QWidget *parent)
{
	parentWindow = (MainWindow*) parent;
	ui.setupUi(this);
	customisedUi();
	ui.cmdBar->installEventFilter(this);
}

bool QuickAddWindow::eventFilter(QObject* watched, QEvent* event)
{
	if(watched == ui.cmdBar)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->key() == Qt::Key_Escape){
				isInputSuccessful = false;
				parentWindow->isQuickAddOpen = false;
				close();
			}
			else if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			{
				QString currentInput = ui.cmdBar->getCurrentLine();
				ui.cmdBar->pushCurrentLine();
				isInputSuccessful = true;
				inputStr = currentInput;
				parentWindow->reset();
				parentWindow->handleInput(inputStr, true);//true --> FROM_QUICK_ADD
				//wait for mainwindow to decide whether close it or not
				return true;
			}
		}
	}
	return QObject::eventFilter(watched, event);//normal processing
}

QString QuickAddWindow::getInputString(){
	return inputStr;
}

bool QuickAddWindow::hasResult(){
	return isInputSuccessful;
}

void QuickAddWindow::customisedUi(){
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog | Qt::Popup);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowState(Qt::WindowActive);
}

void QuickAddWindow::mousePressEvent(QMouseEvent *event){
	this->windowPosition = this->pos();
	this->mousePressPosition = event->globalPos();
}

void QuickAddWindow::mouseMoveEvent(QMouseEvent *event){
	this->mouseMovePosition = event->globalPos();
	QPoint distanceToMove = this->mouseMovePosition - this->mousePressPosition + this->windowPosition;
	this->move(distanceToMove);   
}