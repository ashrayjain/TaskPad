#ifndef QUICKADD_WINDOW_H
#define QUICKADD_WINDOW_H

#include "./GeneratedFiles/ui_quickadd_window.h"
#include "mainwindow.h"

class QuickAddWindow: public QDialog
{
	Q_OBJECT

public:
	QuickAddWindow(QWidget *parent = 0);
	QString getInputString();
	bool hasResult();

private:
	void customisedUi();
	bool eventFilter(QObject* watched, QEvent* event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	Ui::QuickAddWindowClass ui;
	QPoint windowPosition;
	QPoint mousePressPosition;
	QPoint mouseMovePosition;
	bool isInputSuccessful;
	QString inputStr;
	MainWindow* parentWindow;
};

#endif // QUICKADD_WINDOW_H