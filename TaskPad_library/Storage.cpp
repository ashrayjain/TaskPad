#include <list>
#include <string>
#include "Storage.h"
#include "Task.h"
#include "Logger.h"
#include "TaskLoaderText.h"
#include "TaskSaverText.h"

using namespace std;

const string Storage::_fileName = "TaskPad.txt";

Storage::Storage(list<Task>& taskList)
{
	_logger = Logger::getLogger();
	_logger->log("Storage","called constructor!");

	_loader = NULL;
	this->load(taskList);
}

bool Storage::save(const list<Task>& taskList)
{
	_saver = new TaskSaverText;

	_saver->save(taskList,_fileName);

	delete _saver;
	_saver = NULL;

	return true;
}

bool Storage::save(const Task& task)
{
	_saver = new TaskSaverText;

	_saver->save(task);

	delete _saver;
	_saver = NULL;

	return true;
}

void Storage::load (list<Task>& taskList)
{
	_loader = new TaskLoaderText;

	_loader->load(taskList,_fileName);

	delete _loader;
	_loader = NULL;

	return;
}

Storage::~Storage()
{
	//Empty
}