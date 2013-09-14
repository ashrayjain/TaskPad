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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *action_Exit;
    QWidget *centralWidget;
    QLabel *Navigation;
    QTreeWidget *TaskList;
    QTextEdit *CommandBar;
    QLabel *StatusBar;
    QPushButton *CloseButton;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->setWindowModality(Qt::NonModal);
        MainWindowClass->resize(800, 572);
        MainWindowClass->setStyleSheet(QLatin1String("QMainWindow#MainWindowClass {\n"
"	Background-color: #fff;\n"
"}"));
        action_Exit = new QAction(MainWindowClass);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setStyleSheet(QLatin1String("QWidget#centralWidget {\n"
"	background-color: transparent;\n"
"	background-image: url(./Resources/TaskPad_bg.png);\n"
"}"));
        Navigation = new QLabel(centralWidget);
        Navigation->setObjectName(QStringLiteral("Navigation"));
        Navigation->setGeometry(QRect(9, 9, 280, 120));
        QFont font;
        font.setFamily(QStringLiteral("Segoe UI Light"));
        font.setPointSize(18);
        font.setBold(false);
        font.setWeight(50);
        Navigation->setFont(font);
        Navigation->setStyleSheet(QLatin1String("QLabel#Navigation {\n"
"	margin-top: 88px;\n"
"	margin-left: 72px;\n"
"	color: #565656;\n"
"}"));
        TaskList = new QTreeWidget(centralWidget);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        new QTreeWidgetItem(TaskList);
        TaskList->setObjectName(QStringLiteral("TaskList"));
        TaskList->setGeometry(QRect(9, 135, 782, 154));
        TaskList->setMaximumSize(QSize(16777215, 298));
        TaskList->setStyleSheet(QLatin1String("QTreeWidget#TaskList {\n"
"	margin-top: 10px;\n"
"	margin-left:65px;\n"
"	margin-right:65px;\n"
"	background: transparent;\n"
"	border: none;\n"
"	font-size: 18px;\n"
"	height: 100%;\n"
"}\n"
"\n"
"QTreeWidget#TaskList:Item {\n"
"	color: #fff;\n"
"	background-color:#9ac3fa;\n"
"	margin-bottom: 1px;\n"
"	padding: 10px 10px 10px 10px;\n"
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
        CommandBar->setGeometry(QRect(9, 454, 782, 35));
        QFont font1;
        font1.setFamily(QStringLiteral("Segoe UI Light"));
        font1.setPointSize(14);
        CommandBar->setFont(font1);
        CommandBar->setStyleSheet(QLatin1String("QTextEdit#CommandBar {\n"
"	background-color: #fff;\n"
"	border: 1px solid #9ac3fa;\n"
"	border-color: #9ac3fa;\n"
"	padding-top: 4px;\n"
"	padding-bottom: 4px;\n"
"	padding-left: 5px;\n"
"	padding-right: 5px;\n"
"	margin-left: 65px;\n"
"	margin-right: 65px;\n"
"	max-height: 33px;\n"
"}"));
        CommandBar->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        CommandBar->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        CommandBar->setLineWrapMode(QTextEdit::NoWrap);
        StatusBar = new QLabel(centralWidget);
        StatusBar->setObjectName(QStringLiteral("StatusBar"));
        StatusBar->setGeometry(QRect(9, 497, 72, 66));
        StatusBar->setStyleSheet(QLatin1String("QLabel#StatusBar {\n"
"	color: #fff;\n"
"	font-size: 14px;\n"
"	margin-left:25px;\n"
"	margin-top: 22px;\n"
"	margin-bottom:27px;\n"
"}"));
        CloseButton = new QPushButton(centralWidget);
        CloseButton->setObjectName(QStringLiteral("CloseButton"));
        CloseButton->setGeometry(QRect(742, 25, 32, 32));
        CloseButton->setStyleSheet(QLatin1String("QPushButton#CloseButton {\n"
"	background-image: url(Resources/WinButton_Close.png);\n"
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
        Navigation->setText(QApplication::translate("MainWindowClass", "Inbox", 0));
        QTreeWidgetItem *___qtreewidgetitem = TaskList->headerItem();
        ___qtreewidgetitem->setText(3, QApplication::translate("MainWindowClass", "Location", 0));
        ___qtreewidgetitem->setText(2, QApplication::translate("MainWindowClass", "Due Date", 0));
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWindowClass", "Task Name", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindowClass", "ID", 0));

        const bool __sortingEnabled = TaskList->isSortingEnabled();
        TaskList->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = TaskList->topLevelItem(0);
        ___qtreewidgetitem1->setText(3, QApplication::translate("MainWindowClass", "LT27, NUS", 0));
        ___qtreewidgetitem1->setText(2, QApplication::translate("MainWindowClass", "08/09/12", 0));
        ___qtreewidgetitem1->setText(1, QApplication::translate("MainWindowClass", "Project A", 0));
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindowClass", "1", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = TaskList->topLevelItem(1);
        ___qtreewidgetitem2->setText(1, QApplication::translate("MainWindowClass", "CG2222 HW", 0));
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWindowClass", "2", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = TaskList->topLevelItem(2);
        ___qtreewidgetitem3->setText(3, QApplication::translate("MainWindowClass", "ENG E4", 0));
        ___qtreewidgetitem3->setText(1, QApplication::translate("MainWindowClass", "CS2121", 0));
        ___qtreewidgetitem3->setText(0, QApplication::translate("MainWindowClass", "3", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = TaskList->topLevelItem(3);
        ___qtreewidgetitem4->setText(3, QApplication::translate("MainWindowClass", "FASS", 0));
        ___qtreewidgetitem4->setText(2, QApplication::translate("MainWindowClass", "01/09/14", 0));
        ___qtreewidgetitem4->setText(1, QApplication::translate("MainWindowClass", "Project DD2", 0));
        ___qtreewidgetitem4->setText(0, QApplication::translate("MainWindowClass", "4", 0));
        QTreeWidgetItem *___qtreewidgetitem5 = TaskList->topLevelItem(4);
        ___qtreewidgetitem5->setText(1, QApplication::translate("MainWindowClass", "CCD", 0));
        ___qtreewidgetitem5->setText(0, QApplication::translate("MainWindowClass", "5", 0));
        QTreeWidgetItem *___qtreewidgetitem6 = TaskList->topLevelItem(5);
        ___qtreewidgetitem6->setText(3, QApplication::translate("MainWindowClass", "BIZ2 B1-17", 0));
        ___qtreewidgetitem6->setText(2, QApplication::translate("MainWindowClass", "02/09/13", 0));
        ___qtreewidgetitem6->setText(1, QApplication::translate("MainWindowClass", "Project AC", 0));
        ___qtreewidgetitem6->setText(0, QApplication::translate("MainWindowClass", "6", 0));
        QTreeWidgetItem *___qtreewidgetitem7 = TaskList->topLevelItem(6);
        ___qtreewidgetitem7->setText(3, QApplication::translate("MainWindowClass", "COM1 0113", 0));
        ___qtreewidgetitem7->setText(2, QApplication::translate("MainWindowClass", "02/09/13", 0));
        ___qtreewidgetitem7->setText(1, QApplication::translate("MainWindowClass", "Project AB", 0));
        ___qtreewidgetitem7->setText(0, QApplication::translate("MainWindowClass", "7", 0));
        TaskList->setSortingEnabled(__sortingEnabled);

        StatusBar->setText(QApplication::translate("MainWindowClass", "Ready", 0));
        CloseButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
