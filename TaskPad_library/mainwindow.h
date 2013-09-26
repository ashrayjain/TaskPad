#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStack>
#include "./GeneratedFiles/ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow(); 

protected slots:
	void about();

private:
	void customisedUi();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	Ui::MainWindowClass ui;
	QPoint windowPosition;
	QPoint mousePressPosition;
	QPoint mouseMovePosition;
	bool eventFilter(QObject* watched, QEvent* event);

	QStack<QString> inputHistory_undo;
	QStack<QString> inputHistory_redo;
};

#endif // MAINWINDOW_H
