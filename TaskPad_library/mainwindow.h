#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <list>
#include "Task.h"
#include "Messenger.h"
#include "./GeneratedFiles/ui_mainwindow.h"

class CommandBar;
class Manager;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow(); 

protected slots:
	void about();
	void test();
	void createNewTaskTemplate();

private:
	void keyPressEvent(QKeyEvent*event);
	void reset();
	void getToday();
	void updateNavLabel(QString str);
	void updateDetailsLabel(QString str);
	void updateList(std::list<Task> result);
	QTreeWidgetItem* extractTask(int index,Task task);
	void clearDetails();
	void updateDetails(Task t);
	void updateStatusBar(QString str);
	void handleGetToday(Messenger msg);
	void handleMessenger(Messenger msg);
	void customisedUi();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	Ui::MainWindowClass ui;
	QPoint windowPosition;
	QPoint mousePressPosition;
	QPoint mouseMovePosition;
	bool eventFilter(QObject* watched, QEvent* event);

	Manager* scheduler;
	list<Task> lastTimeList;
};

#endif // MAINWINDOW_H
