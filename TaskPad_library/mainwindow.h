#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./GeneratedFiles/ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow(); 

private:
	void customisedUi();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	Ui::MainWindowClass ui;
	QPoint windowPosition;
	QPoint mousePressPosition;
	QPoint mouseMovePosition;

protected slots:
	void about();
};

#endif // MAINWINDOW_H
