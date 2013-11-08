/*
 * =====================================================================================
 *
 *       Filename:  QuickAddWindow.h
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

#ifndef QUICKADD_WINDOW_H
#define QUICKADD_WINDOW_H

#include "./GeneratedFiles/ui_quickadd_window.h"
#include "MainWindow.h"

class QuickAddWindow: public QDialog{

	Q_OBJECT

public:
	QuickAddWindow(QWidget *parent = 0);

	void setupUi();

signals:
	void requestSubmitted(QString requestStr);
	void windowClosed();

private:
	void customisedUi();
	bool eventFilter(QObject* watched, QEvent* event);
	bool handleKeyEvent( bool &FILTERED, QObject* watched, QEvent* event);
	void handleKeyEnter();
	void handleKeyEscape();
	bool isCommandAdd(QString requestStr);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void emitRequest(QString requestStr);
	void emitWindowClosed();

	Ui::QuickAddWindowClass ui;
	QPoint windowPosition;
	QPoint mousePressPosition;
	QPoint mouseMovePosition;
};

#endif // QUICKADD_WINDOW_H