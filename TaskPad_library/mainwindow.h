#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSystemTrayIcon>
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
	void help();
	void createNewTaskTemplate();
	void getToday();
	void getInbox();
	void showWindow();
	void showQuickAddWindow();
	void iconIsActived(QSystemTrayIcon::ActivationReason);

private:
	void showReminder();
	void showTrayMsg(QString msg);
	void keyPressEvent(QKeyEvent*event);
	void reset();
	void updateNavLabel(QString str);
	void updateDetailsLabel(QString str);
	void updateList(std::list<Task> result, bool isToday = false);
	QTreeWidgetItem* extractTask(int index, Task task);
	QTreeWidgetItem* extractTaskForToday(int index, Task task);
	void clearDetails();
	void updateDetails(Task t);
	void updateStatusBar(QString str);
	void handleInput(QString input, bool isFromQuickAdd = false);
	void handleGetToday(Messenger msg);
	void handleGetInbox(Messenger msg);
	void handleMessenger(Messenger msg, bool isFromQuickAdd = false);
	void customisedUi();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	QSystemTrayIcon* trayIcon;
	Ui::MainWindowClass ui;
	QPoint windowPosition;
	QPoint mousePressPosition;
	QPoint mouseMovePosition;
	void changeEvent(QEvent* event);
	bool eventFilter(QObject* watched, QEvent* event);

	bool isQuickAddOpen;
	QDialog* quickAddWindow;
	Manager* scheduler;

	friend class QuickAddWindow;
};

#endif // MAINWINDOW_H
