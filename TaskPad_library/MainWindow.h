/*
 * =====================================================================================
 *
 *       Filename:  mainwindow.h
 *
 *           Note:  MainWindow class handles all UI logic for main window of TaskPad
 *        Version:  1.0
 *        Created:  09/21/13 16:13:09
 *
 *         Author:  XIE KAI (A0102016E), gigikie@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

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
	MainWindow                                 (QWidget *parent = 0);
	~MainWindow                                (); 

protected slots:
	//Pop MsgBox*********************************
	void          about                        ();
	void          help                         ();
	//Window Display*****************************
	void          showWindow                   ();
	void          iconIsActived                (QSystemTrayIcon::ActivationReason);
	void          showReminder                 ();
	void          handleShowReminder           ();
	void          showQuickAddWindow           ();
	void          closeQuickAddWindow          ();
	void          handleQuickAddRequest        (QString requestStr);
	void          handleListSelection          ();
	//Date Navigation****************************
	void          getToday                     ();
	void          getInbox                     ();
	void          showNextDay                  ();
	void          showNextWeek                 ();
	void          showNextMonth                ();
	void          showPrevDay                  ();
	void          showPrevWeek                 ();
	void          showPrevMonth                ();

private:
	//Static Val*********************************
	static        const double DESIRED_TRANSPARENT_OPACITY;
	static        const int	  LENGTH_TOO_LONG;
	static        const int   STEP;
	static        const char* MIDNIGHT;
	static        const char* DELIMITTER;
	static        const char* NEGLECTED;
	static        const char* TODAY_VIEW;
	static        const char* INBOX_VIEW;
	static        const char* REMINDERS_VIEW;
	static        const char* DATE_DD_MM;
	static        const char* DATE_DD_MM_YY;
	static        const char* DATE_DD_MM_YYYY;
	static        const char* DATE_HH_MM;
	static        const char* DATE_DD_MM_HH_MM;
	static        const char* DATE_DD_MM_YYYY_HH_MM;
	static        const char* DATE_YEAR;
	static        const char* EMPTY;
	static        const char* DUE_STR;
	static        const char* FROM_STR;
	static        const char* TO_STR;
	static        const char* FROM_TO_STR;
	static        const char* NEW_LINE;
	static        const char* DEFAULT_WIN_TITLE;
	static        const char* DEFAULT_STATUS_BAR_TEXT;
	static        const char* DEFAULT_DETAILS_VIEW_TEXT;
	static        const char* SUCCESS_DETAILS_ADD_TEXT;
	static        const char* SUCCESS_DETAILS_MOD_TEXT;
	static        const char* SUCCESS_DETAILS_DEL_TEXT;
	static        const char* SUCCESS_DETAILS_UNDO_TEXT;
	static        const char* SUCCESS_DETAILS_REDO_TEXT;
	static        const char* SUCCESS_STATUS_BAR_ADD_TEXT;
	static        const char* SUCCESS_STATUS_BAR_MOD_TEXT;
	static        const char* SUCCESS_STATUS_BAR_DEL_TEXT;
	static        const char* SUCCESS_STATUS_BAR_FIND_TEXT;
	static        const char* SUCCESS_STATUS_BAR_UNDO_TEXT;
	static        const char* SUCCESS_STATUS_BAR_REDO_TEXT;
	static        const char* SUCCESS_STATUS_BAR_DISPLAY_TEXT;
	static        const char* SUCCESS_NAV_LABEL_FIND_CMD;
	static        const char* INTERMEDIATE_NAV_LABEL_INTERMEDIATE_STAGE;
	static        const char* INTERMEDIATE_STATUS_BAR_INTERMEDIATE_STAGE;
	static        const char* ERROR_NAV_LABLE_INTERMEDIATE_STAGE;
	static        const char* ERROR_STATUS_BAR_INTERMEDIATE_STAGE;
	static        const char* ERROR_ONLY_SUPPORT_ADD_N_DISPLAY_ONE_CMD;
	//Init***************************************
	void          setupUI                      ();
	void          setupDependency              ();
	void          setupScheduler               ();
	void          setupTrayIcon                ();
	void          setupTimer                   ();
	void          setupHotkeys                 ();
	void          setupWinDisplayHotkeys       ();
	void          setupDateNavHotkeys          ();
	void          setupGlobalHotkeys           ();
	void          setupConnection              ();
	void          installEventFilter           ();
	void          customisedUi                 ();
	//Destroy************************************
	void          dispose                      ();
	//Tray Related*******************************
	QString       prepareTrayMsg               (std::list<Task> &reminderList, QString output);
	void          showTrayMsg                  (QString msg, QString title = DEFAULT_WIN_TITLE);
	bool          isCommandAdd                 (QString requestStr);
	bool          isEqualOne                   (QString &requestStr);
	//View Related*******************************
	void          reset                        ();
	void          updateStatusBar              (QString str);
	void          displayErrorView             ();
	//--Task List Related************************
	void          updateMainView               (Messenger &msg, QString listTitle, QString statusBarLabel = DEFAULT_STATUS_BAR_TEXT);
	void          updateNavLabel               (QString str);
	void          updateList                   (std::list<Task> result);
	QTreeWidgetItem* extractTask               (int index, Task task);
	void          setListItemDelegate          (Task &task, int index);
	void          setForLowPrioItem            ( Task &task, int listIndex );
	void          setForMediumPrioItem         ( Task &task, int listIndex );
	void          setForHighPrioItem           ( Task &task, int listIndex );
	QStringList   extractFloating              (int index, Task &task);
	QStringList   extractTimedDate             (Task &task, QStringList strList, int index);
	QString       getToDateStr                 (Task &task, QString toTimeStr);
	QString       getFromTimeStr               (Task &task, QString fromTimeStr);
	QStringList   extractDueDate               (Task &task, QStringList strList, int index);
	QString       getDueDateStr                (QDateTime &time, QString dueTimeStr);
	void          setNormalDelegate            (int index);
	void          setNormalDoneDelegate        (int index);
	void          setNormalOverdueDelegate     (int index);
	void          setHighPriorityDelegate      (int index);
	void          setHighPriorityDoneDelegate  (int index);
	void          setHighPriorityOverdueDelegate (int index);
	void          setMediumPriorityDelegate(int index);
	void          setMediumPriorityDoneDelegate  (int index);
	void          setMediumPriorityOverdueDelegate(int index);
	//--Details Related**************************
	void          updateDetailsView            (Messenger &msg, QString detailsLabel = DEFAULT_DETAILS_VIEW_TEXT);
	void          handleOneItemList            (Messenger &msg, QString detailsLabel = DEFAULT_DETAILS_VIEW_TEXT);
	void          updateDetailsLabel           (QString str = DEFAULT_DETAILS_VIEW_TEXT);
	void          clearDetails                 ();
	void          clearStrikeOut               ();
	void          setDetailsViewEmpty          ();
	void          setDetailsViewOpacity40      ();
	void          setDetailsViewOpacity100     ();
	void          updateDetails                (Task task);
	void          setNoteLabel                 (Task &task);
	void          setRemindTimesLabel          (Task &task);
	void          setTagsLabel                 (Task &task);
	void          setParticipantsLabel         (Task &task);
	void          setLocationLabel             (Task &task);
	void          setDueOrFromToLabel          (Task &task);
	void          setFloatingLabel             ();
	void          setTimedLabel                (Task &task);
	void          setDueDateLabel              (Task &task);
	void          setPriorityLabel             (Task &task);
	void          setNameLabel                 (Task &task);
	void          setLabelText                 (QLineEdit *label, QString text);
	void          setLabelEmpty                (QLineEdit *label);
	void          setLabelText                 (QPlainTextEdit *label, QString text);
	void          setLabelEmpty                (QPlainTextEdit *label);
	//Msg Handling*******************************
	void          handleGetToday               (Messenger msg);
	void          handleGetInbox               (Messenger msg);
	void          handleMessenger              (Messenger msg);
	void          handleMsg_SUCCESS_INDEXED_CMD(Messenger &msg);
	void          handleMsg_INTERMEDIATE       (Messenger msg);
	void          handleMsg_SUCCESS            (Messenger &msg);
	void          handleMsg_ERROR_INTERMEDIATE ();
	void          handleMsg_ERROR              (Messenger &msg);
	void          handleDisplay                (Messenger msg, bool callFromSignal = false);
	void          selectItemAt                 (int index);
	void          unselectAllItems             ();
	void          handleDateNavigation         (TP::PERIOD_TYPE period, QString listTitle, bool isPrevious = false);
	void          updateForCmdExec             (QString statusBarTxt, QString DetailsLabel, Messenger msg);
	void          refresh                      ();
	//Quick Add Window Related*******************
	void          setupQuickAddConnection      ();
	void          setupQuickAddWindow          ();
	void          setIsQuickAddOpen            (bool toggle);
	void          setFromReminder              (bool toggle);
	void          setCurrRemindTime            ();
	std::string   getFindRtCmd                 ();
	void          disposeQuickAddWindow        ();
	void          disposeQuickAddConnection    ();
	void          handleQA_DISPLAY             (Messenger msg);
	void          handleQA_SUCCESS             ();
	void          handleQA_ERROR               (QString requestStr, Messenger &msg);
	QString       getTimePeriodStr             (std::pair<tm, tm> period);
	//Other**************************************
	void          popMsgBox                    (QString title, QString description);
	void          keyPressEvent                (QKeyEvent*event);
	void          changeEvent                  (QEvent* event);
	void          runInBackground              (QEvent* event);
	bool          eventFilter                  (QObject* watched, QEvent* event);
	bool          handleKeyPressInCmdBar       ( bool ret, QObject* watched, QEvent* event );
	void          handleKeyEnter               ();
	void          mousePressEvent              (QMouseEvent *event);
	void          mouseMoveEvent               (QMouseEvent *event);
	void          logEnd                       ();
	void          logStart                     ();
	//Main Widgets******************************
	Ui::MainWindowClass ui;
	QSystemTrayIcon*    trayIcon;
	//State*************************************
	QTimer*             timer;
	QDateTime           currRemindTime;
	QDialog*            quickAddWindowPtr;
	QPoint              windowPosition;
	QPoint              mousePressPosition;
	QPoint              mouseMovePosition;
	double              detailsViewOpacity;
	std::list<Task>     intermediateList;
	QString             navTitleOfLastTime;
	//State Flag********************************
	bool                isQuickAddOpen;
	bool                isFromReminder;
	bool                isIntermediateStage;
	//Logic's entrance**************************
	Manager*            scheduler;
};

#endif // MAINWINDOW_H
