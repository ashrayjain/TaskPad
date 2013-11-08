//
//	TEAM: F12-1C
//	MEMBER: ASHRAY JAIN, XIE KAI, AN JIANGZE, THYAGESH M., LI ZIXUAN
//	PROJECT NAME: TaskPad
//
//	TaskPad is a simple, elegant and revolutionary task organiser designed specifically for power users. 
//	It empowers the user with 4 basic yet comprehensive functions to Add, Modify, Find and Delete tasks. 
//	Add and Modify offer a comprehensive list of parameters that neatly catalogues the specifications of 
//	the tasks while Find is a flexible tool that can retrieve a task according to any of its parameters. 
//	Delete is just a one-line wonder that performs what it is meant to do. TaskPad also has Hot Key Templating: 
//	all 4 functions are made simple with intuitive hotkeys that calls the full command template with clear 
//	instructions. Some additional features include Natural Language Processing for date parsing and Arrow Keys (Up - Down) Navigation.
//
#include <QtWidgets/QApplication>
#include <QSharedMemory>
#include "../TaskPad_library/MainWindow.h"
#include "Natty.h"
//#include <vld.h>

int main(int argc, char *argv[])
{
	const int ONE_INSTANCE_MODE = 1;
	const QString APP_NAME = "TaskPad";

	int ret = 0;
	QSharedMemory singletonCheck(APP_NAME);

	singletonCheck.create(ONE_INSTANCE_MODE);
	if(singletonCheck.error() != QSharedMemory::AlreadyExists){
		QApplication app(argc, argv);
		Q_INIT_RESOURCE(images);
		MainWindow window;
		window.show();
		ret = app.exec();
	}
	return ret;
}
