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
#ifndef _STORAGE_CPP_
#define _STORAGE_CPP_

#include <list>
#include <string>
#include <QDir>
#include <QString>
#include "Storage.h"
#include "Task.h"
#include "Logger.h"
#include "TaskLoaderText.h"
#include "TaskSaverText.h"
#include "StorableTaskDatastore.h"

using namespace std;
// @author A0100124J

const string Storage::_fileName = "TaskPad.txt";

Storage::Storage(StorableTaskDatastore* taskDS) {
	_logger = Logger::getLogger();
	_logger->log("Storage","in constructor");
	QDir temp;
	if (!temp.cd(QString("Tasks")))
	{
		temp.mkdir("Tasks");
	}

	_loader = NULL;
	_saver = NULL;
	this->load(taskDS);
}

bool Storage::save (StorableTaskDatastore* taskDS) {
	_saver = new TaskSaverText;

	_saver->save(taskDS,_fileName);

	delete _saver;
	_saver = NULL;

	return true;
}

bool Storage::save(const Task& task, const TP::COMMAND_TYPE& cType) {
	_saver = new TaskSaverText;

	_saver->save(task,cType);

	delete _saver;
	_saver = NULL;

	return true;
}

void Storage::load (StorableTaskDatastore* taskDS) {
	_loader = new TaskLoaderText(taskDS);

	_loader->load(_fileName);

	delete _loader;
	_loader = NULL;

	return;
}

Storage::~Storage() {
	//Empty
	QDir temp(QString::fromStdString("Tasks"));
	temp.removeRecursively();
}
#endif