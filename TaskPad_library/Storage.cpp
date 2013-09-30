#include "Storage.h"
#include "Task.h"
#include "Enum.h"
#include <list>
#include <sstream>

using namespace TP;

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

void Storage::saveString(string line)
{
	return;
}

string Storage::convertTimeToString(time_t time)
{
	stringstream ss;
	ss<<time;
	return ss.str();
}

void Storage::saveDeadlineTask(Task* tempTask)
{
	DeadlineTask* tempTaskDeadline = (DeadlineTask *) tempTask;
	if(tempTaskDeadline->getFlagName())
	{
		this->saveString(tempTaskDeadline->getName());
	}
	else
	{
		this->saveString("");
	}
	

	if(tempTaskDeadline->getFlagDueDate())
	{
		this->saveString(convertTimeToString(tempTaskDeadline->getDueDate()));
	}
	else
	{
		this->saveString("");
	}

	if(tempTaskDeadline->getFlagLocation())
	{
		this->saveString(tempTaskDeadline->getLocation());
	}
	else
	{
		this->saveString("");
	}
	if(tempTaskDeadline->getFlagParticipant())
	{
		this->saveString(tempTaskDeadline->getParticipants());
	}
	else
	{
		this->saveString("");
	}
	
	if(tempTaskDeadline->getFlagNote())
	{
		this->saveString(tempTaskDeadline->getNote());
	}
	else
	{
		this->saveString("");
	}

	if(tempTaskDeadline->getFlagPriority())
	{
		this->saveString(PRIORITY_STRING[tempTaskDeadline->getPriority()]);
	}
	else
	{
		this->saveString("");
	}

	if(tempTaskDeadline->getFlagTags())
	{

	}
	else
	{
		this->saveString("");
	}

	if(tempTaskDeadline->getFlagRemindTime())
	{
	}
	else
	{
		this->saveString("");
	}

	if(tempTaskDeadline->getFlagState())	
	{
		this->saveString(TASK_STATE_STRING[tempTaskDeadline->getState()]);
	}
	else
	{
		this->saveString("");
	}
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