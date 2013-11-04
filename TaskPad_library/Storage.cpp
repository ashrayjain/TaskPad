/*
 * =====================================================================================
 *
 *		Filename:  Storage.cpp
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file implements all of the functions defined in Storage.h file
  * 
 */

#include <list>
#include <string>
#include "Storage.h"
#include "Task.h"
#include "Logger.h"
#include "TaskLoaderText.h"
#include "TaskSaverText.h"
#include "StorableTaskDatastore.h"

using namespace std;

const string Storage::_fileName = "TaskPad.txt";

Storage::Storage(StorableTaskDatastore* taskDB) {
//	QDir myDir;
//	QMessageBox qdirTest;
//	qdirTest.setText("Storage::Storage -- simple QDir test " + myDir.absolutePath());
//	qdirTest.exec();
	_logger = Logger::getLogger();
	_logger->log("Storage","in constructor");

	_loader = NULL;
	_saver = NULL;
	this->load(taskDB);
}

bool Storage::save(const list<Task>& taskList) {
	_saver = new TaskSaverText;

	_saver->save(taskList,_fileName);

	delete _saver;
	_saver = NULL;

	return true;
}

bool Storage::save (StorableTaskDatastore* taskDB)
{
	return save(taskDB->getTaskList());
}

bool Storage::save(const Task& task, const TP::COMMAND_TYPE& cType) {
	_saver = new TaskSaverText;

	_saver->save(task,cType);

	delete _saver;
	_saver = NULL;

	return true;
}
/*
void Storage::load (list<Task>& taskList) {
	_loader = new TaskLoaderText();

	_loader->load(_fileName);

	delete _loader;
	_loader = NULL;

	return;
}*/

void Storage::load (StorableTaskDatastore* taskDB)
{
	_loader = new TaskLoaderText(taskDB);

	_loader->load(_fileName);

	delete _loader;
	_loader = NULL;

	return;
}

Storage::~Storage() {
	//Empty
}