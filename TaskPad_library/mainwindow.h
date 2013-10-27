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
	//Pop MsgBox
	void about();
	void help();
	//Window Display
	void showWindow();
	void iconIsActived(QSystemTrayIcon::ActivationReason);
	void showReminder();
	void handleShowReminder();
	void showQuickAddWindow();
	void closeQuickAddWindow();
	void handleQuickAddRequest(QString requestStr);
	//Date Navigation
	void getToday();
	void getInbox();
	void showNextDay();
	void showNextWeek();
	void showNextMonth();
	void showPrevDay();
	void showPrevWeek();
	void showPrevMonth();

private:
	//Init
	void setupUI();
	void setupDependency();
	void setupScheduler();
	void setupTrayIcon();
	void setupTimer();
	void setupHotkeys();
	void setupConnection();
	void installEventFilter();
	void customisedUi();
	//Destroy
	void dispose();
	//Tray Related
	QString prepareTrayMsg( list<Task> &reminderList, QString output );
	void showTrayMsg(QString msg, QString title = "TaskPad");
	bool isCommandAdd(QString requestStr);
	bool isEqualOne( QString &requestStr );
	//View Related
	void reset();
	void updateStatusBar(QString str);
	//--Task List Related
	void updateMainView(  Messenger &msg, QString listTitle, QString statusBarLabel = "Ready" );
	void updateNavLabel(QString str);
	void updateList(std::list<Task> result);
	QTreeWidgetItem* extractTask(int index, Task task);
	QStringList extractFloating( int index, Task &task );
	QStringList extractTimedDate( Task &task, QStringList strList, int index );
	QStringList extractDueDate( Task &task, QStringList strList, int index );
	void setNormalDelegate( int index );
	void setHighPriorityDelegate( int index );
	//--Details Related
	void updateDetailsView( Messenger &msg, QString detailsLabel = "Task's Details");
	void updateDetailsView_GetTask( Messenger msg, QString detailsLabel = "Task's Details");
	void handleOneItemList( Messenger &msg, QString detailsLabel = "Task's Details" );
	void updateDetailsLabel(QString str = "Task's Details");
	void clearDetails();
	void setDetailsViewEmpty();
	void setDetailsViewOpacity40();
	void setDetailsViewOpacity100();
	void updateDetails(Task task);
	void setNoteLabel( Task &task );
	void setRemindTimesLabel( Task &task );
	void setTagsLabel( Task &task );
	void setParticipantsLabel( Task &task );
	void setLocationLabel( Task &task );
	void setDueOrFromToLabel( Task &task );
	void setFloatingLabel();
	void setTimedLabel( Task &task );
	void setDueDateLabel( Task &task );
	void setPriorityLabel( Task &task );
	void setNameLabel( Task &task );
	//Msg Handling
	void handleGetToday(Messenger msg);//TODO: many handlers r redundant
	void handleGetInbox(Messenger msg);
	void handleMessenger(Messenger msg);
	void handleMsg_SUCCESS_INDEXED_CMD( Messenger &msg );
	void handleMsg_INTERMEDIATE( Messenger msg );
	void handleMsg_SUCCESS( Messenger &msg );
	void handleMsg_ERROR_INTERMEDIATE();
	void handleMsg_ERROR( Messenger &msg );
	void handleDisplay(Messenger msg);
	void handleDateNavigation(TP::PERIOD_TYPE period, QString listTitle, bool isPrevious = false);
	void updateForCmdExec( QString statusBarTxt, QString DetailsLabel, Messenger msg );
	void refresh();
	//Quick Add Window Related
	void setupQuickAddConnection();
	void setupQuickAddWindow();
	void setIsQuickAddOpen(bool toggle);
	void setFromReminder(bool toggle);
	void setCurrRemindTime();
	string getFindRtCmd();
	void disposeQuickAddWindow();
	void handleQA_DISPLAY( Messenger msg );
	void handleQA_SUCCESS();
	void handleQA_ERROR( QString requestStr, Messenger &msg );
	QString getTimePeriodStr(pair<tm, tm> period);
	//Other
	void popMsgBox(QString title, QString description);
	void keyPressEvent(QKeyEvent*event);
	void changeEvent(QEvent* event);
	void runInBackground(QEvent* event);
	bool eventFilter(QObject* watched, QEvent* event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	//Main Widgets
	Ui::MainWindowClass ui;
	QSystemTrayIcon* trayIcon;
	//State
	QTimer* timer;
	QDateTime currRemindTime;
	QDialog* quickAddWindowObj;
	QPoint windowPosition;
	QPoint mousePressPosition;
	QPoint mouseMovePosition;
	//State Flag
	bool isQuickAddOpen;
	bool isFromReminder;
	//Logic's entrance
	Manager* scheduler;
};

#endif // MAINWINDOW_H
