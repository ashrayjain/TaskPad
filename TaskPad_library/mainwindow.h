#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSystemTrayIcon>
#include <QDateTime>
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
	void getToday();
	void getInbox();
	void showWindow();
	void showReminder();
	void showQuickAddWindow();
	void closeQuickAddWindow();
	void handleQuickAddRequest(QString requestStr);
	void handleShowReminder();
	bool isCommandAdd(QString requestStr);
	void iconIsActived(QSystemTrayIcon::ActivationReason);
	void showNextDay();
	void showNextWeek();
	void showNextMonth();
	void showPrevDay();
	void showPrevWeek();
	void showPrevMonth();

private:
	void showTrayMsg(QString msg, QString title = "TaskPad");
	void keyPressEvent(QKeyEvent*event);
	void reset();
	void updateNavLabel(QString str);
	void updateDetailsLabel(QString str);
	void updateList(std::list<Task> result);
	QTreeWidgetItem* extractTask(int index, Task task);
	void clearDetails();
	void updateDetails(Task t);
	void updateStatusBar(QString str);
	void handleGetToday(Messenger msg);//TODO: many handlers r redundant
	void handleGetInbox(Messenger msg);
	void handleMessenger(Messenger msg);
	void handleDisplay(Messenger msg);
	void handleDateNavigation(TP::PERIOD_TYPE period, QString listTitle, bool isPrevious = false);
	QString getTimePeriod(pair<tm, tm> period);
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
	bool isFromReminder;
	QDateTime currRemindTime;
	QDialog* quickAddWindow;
	Manager* scheduler;
	QTimer* timer;

	friend class QuickAddWindow;
};

#endif // MAINWINDOW_H
