#ifndef QUICKADD_WINDOW_H
#define QUICKADD_WINDOW_H

#include "./GeneratedFiles/ui_quickadd_window.h"
#include "MainWindowPPP.h"

class QuickAddWindow: public QDialog
{
	Q_OBJECT

public:
	QuickAddWindow(QWidget *parent = 0);

signals:
	void requestSubmitted(QString requestStr);
	void windowClosed();

private:
	void customisedUi();
	bool eventFilter(QObject* watched, QEvent* event);
	bool isCommandAdd(QString requestStr);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void emitRequest(QString requestStr);
	void emitWindowClosed();

	Ui::QuickAddWindowClass ui;
	QPoint windowPosition;
	QPoint mousePressPosition;
	QPoint mouseMovePosition;
	QString inputStr;
};

#endif // QUICKADD_WINDOW_H