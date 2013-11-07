#include "QuickAddWindow.h"

QuickAddWindow::QuickAddWindow(QWidget *parent)
:QDialog(parent)
{
	ui.setupUi(this);
	customisedUi();
	ui.cmdBar->installEventFilter(this);
	ui.cmdBar->setQuickAddMode();
}

bool QuickAddWindow::eventFilter(QObject* watched, QEvent* event){
	const bool FILTERED = true;

	if(watched == ui.cmdBar)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if(keyEvent->key() == Qt::Key_Escape){
				emitWindowClosed();
				//wait for mainwindow to close it
				return FILTERED;
			}
			else if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			{
				QString currentInput = ui.cmdBar->getCurrentLine();
				ui.cmdBar->pushCurrentLine();
				if(!currentInput.isEmpty()){
					inputStr = currentInput;
					emitRequest(currentInput);
					//wait for mainwindow to decide whether close it or not
				}
				return FILTERED;
			}
		}
	}
	return QObject::eventFilter(watched, event);//normal processing
}

void QuickAddWindow::customisedUi(){
	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog | Qt::Popup);
	setAttribute(Qt::WA_TranslucentBackground);
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

void QuickAddWindow::emitRequest(QString requestStr){
	emit requestSubmitted(requestStr);
}

void QuickAddWindow::emitWindowClosed(){
	emit windowClosed();
}