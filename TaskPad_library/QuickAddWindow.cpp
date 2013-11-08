/*
 * =====================================================================================
 *
 *       Filename:  QuickAddWindow.cpp
 *
 *    Description:	Quick Add window allows user to add a new task quickly, without opening
 *	the mainwindow.
 *
 *        Version:  1.0
 *        Created:  10/21/13 16:13:09
 *
 *         Author:  XIE KAI (A0102016E), gigikie@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "QuickAddWindow.h"

QuickAddWindow::QuickAddWindow(QWidget *parent):QDialog(parent){
	setupUi();
	customisedUi();
}

bool QuickAddWindow::eventFilter(QObject* watched, QEvent* event){
	bool FILTERED = true;

	if(watched == ui.cmdBar &&
		event->type() == QEvent::KeyPress){
		FILTERED = handleKeyEvent(FILTERED, watched, event);
	}
	else{
		//normal processing
		FILTERED = QObject::eventFilter(watched, event);
	}
	return FILTERED;
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

void QuickAddWindow::setupUi()
{
	ui.setupUi(this);
	ui.cmdBar->installEventFilter(this);
	ui.cmdBar->setQuickAddMode();
}

bool QuickAddWindow::handleKeyEvent( bool &FILTERED, QObject* watched, QEvent* event ){
	QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
	if(keyEvent->key() == Qt::Key_Escape){
		handleKeyEscape();
	}
	else if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter){
		handleKeyEnter();
	}
	else{
		//normal processing
		FILTERED = QObject::eventFilter(watched, event);
	}
	return FILTERED;
}

void QuickAddWindow::handleKeyEscape(){
	emitWindowClosed();
	//wait for parent window to close it
}

void QuickAddWindow::handleKeyEnter(){
	QString currentInput = ui.cmdBar->getCurrentLine();
	ui.cmdBar->pushCurrentLine();
	if(!currentInput.isEmpty()){
		emitRequest(currentInput);
		//wait for parent window to decide whether close it or not
	}
}
