/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *action_Exit;
    QWidget *centralWidget;
    QLabel *Navigation_taskList;
    QTreeWidget *TaskList;
    QTextEdit *CommandBar;
    QLabel *StatusBar;
    QWidget *DetailsView;
    QPlainTextEdit *note;
    QLineEdit *name;
    QLineEdit *dueOrFromTo;
    QLineEdit *location;
    QLineEdit *participants;
    QLineEdit *tags;
    QLineEdit *remindTime;
    QLabel *Navigation_detailsView;
    QToolButton *CloseButton;
    QToolButton *MinimizeButton;
    QToolButton *AboutButton;
    QToolButton *HelpButton;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->setWindowModality(Qt::NonModal);
        MainWindowClass->resize(1100, 600);
        MainWindowClass->setStyleSheet(QLatin1String("QMainWindow#MainWindowClass {\n"
"	Background-color: #fff;\n"
"}"));
        action_Exit = new QAction(MainWindowClass);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setStyleSheet(QLatin1String("QWidget#centralWidget {\n"
"	background-color: transparent;\n"
"	background-image: url(:/MainWindow/Resources/mainwindow_bg.png);\n"
"}"));
        Navigation_taskList = new QLabel(centralWidget);
        Navigation_taskList->setObjectName(QStringLiteral("Navigation_taskList"));
        Navigation_taskList->setGeometry(QRect(83, 89, 280, 50));
        QFont font;
        font.setFamily(QStringLiteral("Segoe UI Light"));
        Navigation_taskList->setFont(font);
        Navigation_taskList->setStyleSheet(QLatin1String("QLabel#Navigation_taskList {\n"
"	font-family: \"Segoe UI Light\";\n"
"	font-size: 24px;\n"
"	color: #565656;\n"
"}"));
        TaskList = new QTreeWidget(centralWidget);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        TaskList->setObjectName(QStringLiteral("TaskList"));
        TaskList->setGeometry(QRect(9, 135, 595, 248));
        TaskList->setMaximumSize(QSize(16777215, 330));
        TaskList->setStyleSheet(QLatin1String("QTreeWidget#TaskList {\n"
"	margin-top: 10px;\n"
"	margin-left: 37px;\n"
"	background: transparent;\n"
"	border: none;\n"
"	height: 100%;\n"
"	font-size: 18px;\n"
"}\n"
"\n"
"QTreeWidget#TaskList:Item {\n"
"	color: #fff;\n"
"	background-color:#9ac3fa;\n"
"	margin-bottom: 1px;\n"
"	padding: 10px 0 10px 10px;\n"
"}\n"
"\n"
"QTreeWidget#TaskList:Item:last {\n"
"	color: #9ac3fa;\n"
"	padding: 10px 18px 10px 10px;\n"
"}\n"
"\n"
"QTreeWidget#TaskList QScrollBar:vertical {\n"
"	opacity: 0.6;\n"
"	border: none;\n"
"	background-color: transparent;\n"
"	margin-left: 1px;\n"
"	width: 5px;\n"
"}\n"
"\n"
"QTreeWidget#TaskList QScrollBar::handle:vertical {\n"
"	border: none;\n"
"	background-color: #CAE1FF;\n"
"}\n"
"\n"
"QTreeWidget#TaskList QScrollBar::add-line:vertical {\n"
"	visibility: hidden;\n"
"	background-color: transparent;\n"
"}\n"
"\n"
"QTreeWidget#TaskList QScrollBar::sub-line:vertical {\n"
"	visibility: hidden;\n"
"	background-color: transparent;\n"
"}"));
        TaskList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        TaskList->header()->setVisible(false);
        TaskList->header()->setDefaultSectionSize(105);
        CommandBar = new QTextEdit(centralWidget);
        CommandBar->setObjectName(QStringLiteral("CommandBar"));
        CommandBar->setGeometry(QRect(9, 485, 595, 35));
        CommandBar->setFont(font);
        CommandBar->setStyleSheet(QLatin1String("QTextEdit#CommandBar {\n"
"	font-family: \"Segoe UI Light\";\n"
"	font-size: 18px;\n"
"	background-color: #fff;\n"
"	border: 1px solid #9ac3fa;\n"
"	border-color: #9ac3fa;\n"
"	padding-top: 4px;\n"
"	padding-bottom: 4px;\n"
"	padding-left: 5px;\n"
"	padding-right: 5px;\n"
"	margin-left: 57px;\n"
"	max-height: 33px;\n"
"}\n"
"\n"
"QTextEdit#CommandBar:hover {\n"
"	border-color: #565656;\n"
"}"));
        CommandBar->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        CommandBar->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        CommandBar->setLineWrapMode(QTextEdit::NoWrap);
        StatusBar = new QLabel(centralWidget);
        StatusBar->setObjectName(QStringLiteral("StatusBar"));
        StatusBar->setGeometry(QRect(10, 525, 72, 66));
        StatusBar->setStyleSheet(QLatin1String("QLabel#StatusBar {\n"
"	color: #fff;\n"
"	font-size: 14px;\n"
"	margin-left:25px;\n"
"	margin-top: 22px;\n"
"	margin-bottom:27px;\n"
"}"));
        DetailsView = new QWidget(centralWidget);
        DetailsView->setObjectName(QStringLiteral("DetailsView"));
        DetailsView->setGeometry(QRect(640, 145, 391, 372));
        DetailsView->setStyleSheet(QLatin1String("QWidget#DetailsView{\n"
"	background-image:url(:/MainWindow/Resources/details_normal_bg.png);\n"
"}\n"
"\n"
"QLineEdit#name{\n"
"	font-family: \"Segoe UI\";\n"
"	font-size: 18px;\n"
"	color: #565656;\n"
"	background-color: transparent;\n"
"	border: none;\n"
"}\n"
" QLineEdit#dueOrFromTo{\n"
"	font-family: \"Segoe UI\";\n"
"	font-size: 15px;\n"
"	color: #fff;\n"
"	background-color: transparent;\n"
"	border: none;\n"
"}\n"
"\n"
"QLineEdit#location, QLineEdit#participants, QLineEdit#tags, QLineEdit#remindTime, QLineEdit#note, QPlainTextEdit#note{\n"
"	font-family: \"Segoe UI\";\n"
"	font-size: 15px;\n"
"	color: #565656;\n"
"	background-color: transparent;\n"
"	border: none;\n"
"}"));
        note = new QPlainTextEdit(DetailsView);
        note->setObjectName(QStringLiteral("note"));
        note->setGeometry(QRect(10, 250, 361, 101));
        note->setStyleSheet(QStringLiteral(""));
        note->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        note->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        note->setReadOnly(true);
        name = new QLineEdit(DetailsView);
        name->setObjectName(QStringLiteral("name"));
        name->setEnabled(true);
        name->setGeometry(QRect(10, 14, 371, 20));
        name->setReadOnly(true);
        dueOrFromTo = new QLineEdit(DetailsView);
        dueOrFromTo->setObjectName(QStringLiteral("dueOrFromTo"));
        dueOrFromTo->setEnabled(true);
        dueOrFromTo->setGeometry(QRect(10, 63, 371, 20));
        dueOrFromTo->setReadOnly(true);
        location = new QLineEdit(DetailsView);
        location->setObjectName(QStringLiteral("location"));
        location->setEnabled(true);
        location->setGeometry(QRect(10, 111, 371, 20));
        location->setReadOnly(true);
        participants = new QLineEdit(DetailsView);
        participants->setObjectName(QStringLiteral("participants"));
        participants->setEnabled(true);
        participants->setGeometry(QRect(10, 161, 171, 20));
        participants->setReadOnly(true);
        tags = new QLineEdit(DetailsView);
        tags->setObjectName(QStringLiteral("tags"));
        tags->setEnabled(true);
        tags->setGeometry(QRect(205, 161, 171, 20));
        tags->setReadOnly(true);
        remindTime = new QLineEdit(DetailsView);
        remindTime->setObjectName(QStringLiteral("remindTime"));
        remindTime->setEnabled(true);
        remindTime->setGeometry(QRect(10, 209, 371, 20));
        remindTime->setReadOnly(true);
        Navigation_detailsView = new QLabel(centralWidget);
        Navigation_detailsView->setObjectName(QStringLiteral("Navigation_detailsView"));
        Navigation_detailsView->setGeometry(QRect(660, 89, 280, 50));
        Navigation_detailsView->setFont(font);
        Navigation_detailsView->setStyleSheet(QLatin1String("QLabel#Navigation_detailsView {\n"
"	font-family: \"Segoe UI Light\";\n"
"	font-size: 24px;\n"
"	color: #565656;\n"
"}"));
        CloseButton = new QToolButton(centralWidget);
        CloseButton->setObjectName(QStringLiteral("CloseButton"));
        CloseButton->setGeometry(QRect(1049, 27, 26, 30));
        CloseButton->setStyleSheet(QLatin1String("QToolButton#CloseButton {\n"
"	background-image: url(:/MainWindow/Resources/window_buttons.png);\n"
"	background-position: top right;\n"
"	border-style: flat;\n"
"}\n"
"\n"
"QToolButton#CloseButton::hover {\n"
"	background-image: url(:/MainWindow/Resources/window_buttons_hovered.png);\n"
"	background-position: top right;\n"
"	border-style: flat;\n"
"}"));
        MinimizeButton = new QToolButton(centralWidget);
        MinimizeButton->setObjectName(QStringLiteral("MinimizeButton"));
        MinimizeButton->setGeometry(QRect(1024, 27, 26, 30));
        MinimizeButton->setStyleSheet(QLatin1String("QToolButton#MinimizeButton {\n"
"	background-image: url(:/MainWindow/Resources/window_buttons.png);\n"
"	background-position: top left;\n"
"	border-style: flat;\n"
"}\n"
"\n"
"QToolButton#MinimizeButton::hover {\n"
"	background-image: url(:/MainWindow/Resources/window_buttons_hovered.png);\n"
"	background-position: top left;\n"
"	border-style: flat;\n"
"}"));
        AboutButton = new QToolButton(centralWidget);
        AboutButton->setObjectName(QStringLiteral("AboutButton"));
        AboutButton->setGeometry(QRect(905, 27, 55, 33));
        AboutButton->setStyleSheet(QLatin1String("QToolButton#AboutButton\n"
"{\n"
"	background-image: url(:/MainWindow/Resources/top_buttons.png);\n"
"	background-position: top right;\n"
"	background-color: transparent;\n"
"	border-style: flat;\n"
"}\n"
"\n"
"QToolButton#AboutButton:hover\n"
"{\n"
"	background-image: url(:/MainWindow/Resources/top_buttons_hovered.png);\n"
"	background-position: top right;\n"
"	background-color: transparent;\n"
"	border-style: flat;\n"
"}"));
        HelpButton = new QToolButton(centralWidget);
        HelpButton->setObjectName(QStringLiteral("HelpButton"));
        HelpButton->setGeometry(QRect(860, 27, 44, 33));
        HelpButton->setStyleSheet(QLatin1String("QToolButton#HelpButton\n"
"{\n"
"	background-image: url(:/MainWindow/Resources/top_buttons.png);\n"
"	background-position: top left;\n"
"	background-color: transparent;\n"
"	border-style: flat;\n"
"}\n"
"\n"
"QToolButton#HelpButton:hover\n"
"{\n"
"	background-image: url(:/MainWindow/Resources/top_buttons_hovered.png);\n"
"	background-position: top left;\n"
"	background-color: transparent;\n"
"	border-style: flat;\n"
"}"));
        MainWindowClass->setCentralWidget(centralWidget);
        QWidget::setTabOrder(CommandBar, TaskList);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0));
        action_Exit->setText(QApplication::translate("MainWindowClass", "&Exit", 0));
        Navigation_taskList->setText(QApplication::translate("MainWindowClass", "<span style=\\\"font-size:18px; \\\">Today</span>", 0));
        QTreeWidgetItem *___qtreewidgetitem = TaskList->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("MainWindowClass", "Due|From-to", 0));
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWindowClass", "Task Name", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindowClass", "ID", 0));

        const bool __sortingEnabled = TaskList->isSortingEnabled();
        TaskList->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = TaskList->topLevelItem(0);
        ___qtreewidgetitem1->setText(2, QApplication::translate("MainWindowClass", "due 08/09/12", 0));
        ___qtreewidgetitem1->setText(1, QApplication::translate("MainWindowClass", "Project A", 0));
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindowClass", "1", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = TaskList->topLevelItem(1);
        ___qtreewidgetitem2->setText(2, QApplication::translate("MainWindowClass", "from 02/09/13 to 02/09/13", 0));
        ___qtreewidgetitem2->setText(1, QApplication::translate("MainWindowClass", "CG2222 HW", 0));
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWindowClass", "2", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = TaskList->topLevelItem(2);
        ___qtreewidgetitem3->setText(1, QApplication::translate("MainWindowClass", "CS2121", 0));
        ___qtreewidgetitem3->setText(0, QApplication::translate("MainWindowClass", "3", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = TaskList->topLevelItem(3);
        ___qtreewidgetitem4->setText(2, QApplication::translate("MainWindowClass", "due 01/09/14", 0));
        ___qtreewidgetitem4->setText(1, QApplication::translate("MainWindowClass", "Project DD2", 0));
        ___qtreewidgetitem4->setText(0, QApplication::translate("MainWindowClass", "4", 0));
        QTreeWidgetItem *___qtreewidgetitem5 = TaskList->topLevelItem(4);
        ___qtreewidgetitem5->setText(1, QApplication::translate("MainWindowClass", "CCD", 0));
        ___qtreewidgetitem5->setText(0, QApplication::translate("MainWindowClass", "5", 0));
        QTreeWidgetItem *___qtreewidgetitem6 = TaskList->topLevelItem(5);
        ___qtreewidgetitem6->setText(2, QApplication::translate("MainWindowClass", "due 02/09/13", 0));
        ___qtreewidgetitem6->setText(1, QApplication::translate("MainWindowClass", "Project AC", 0));
        ___qtreewidgetitem6->setText(0, QApplication::translate("MainWindowClass", "6", 0));
        TaskList->setSortingEnabled(__sortingEnabled);

        StatusBar->setText(QApplication::translate("MainWindowClass", "Ready", 0));
        note->setPlainText(QApplication::translate("MainWindowClass", "Note here", 0));
        name->setText(QApplication::translate("MainWindowClass", "Project A", 0));
        dueOrFromTo->setText(QApplication::translate("MainWindowClass", "From  09/02/13 to 09/02/13", 0));
        location->setText(QApplication::translate("MainWindowClass", "@NUS Lecture Theater 27, Science", 0));
        participants->setText(QApplication::translate("MainWindowClass", "Colin, Halim", 0));
        tags->setText(QApplication::translate("MainWindowClass", "#CG2271, #CS2010", 0));
        remindTime->setText(QApplication::translate("MainWindowClass", "Remind me: 08/02/13", 0));
        Navigation_detailsView->setText(QApplication::translate("MainWindowClass", "<span style=\\\"font-size:18px; \\\">Details</span>", 0));
        CloseButton->setText(QString());
        MinimizeButton->setText(QString());
        AboutButton->setText(QString());
        HelpButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
