#include "Storage.h"
#include "Task.h"
#include "Enum.h"
#include <list>

using namespace TD;

const string Storage::_fileName = "TaskPad.txt";

Storage::Storage(list<Task>&)
{
}

bool Storage::save(list<Task>& taskList)
{
	string singleTaskEntry = "";
	if(taskList.empty())
	{
		// EMPTY THE WHOLE FILE!!
		return true;
	}

	list<Task>::iterator it = taskList.begin();

	while(it != taskList.end())
	{
		switch(it->getTaskType())
		{
			case DEADLINE:
				this->saveDeadlineTask(&(*it));
				break;
			case TIMED:
				this->saveTimedTask(&(*it));
				break;
			case FLOATING:
				this->saveFloatingTask(&(*it));
				break;
		}
		return false;


		it++;
	}
}

void Storage::saveDeadlineTask(Task* tempTask)
{
	DeadlineTask* tempTaskDeadline = (DeadlineTask *) tempTask;
}

void Storage::saveTimedTask(Task* tempTask)
{
}

void Storage::saveFloatingTask(Task* tempTask)
{
}

bool Storage::save(const Command&)
{
	return false;
}

Storage::~Storage()
{
	this->_fileReader.close();
	this->_fileWriter.close();
}