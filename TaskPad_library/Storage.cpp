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
	return false;
}

void Storage::saveString(string line)
{
	return;
}

void Storage::saveDeadlineTask(Task* tempTask)
{
	DeadlineTask* tempTaskDeadline = (DeadlineTask *) tempTask;
	this->saveTaskType(tempTaskDeadline);
	this->saveIndex(tempTaskDeadline);
	this->saveName(tempTaskDeadline);
	this->saveDueDate(tempTaskDeadline);
	this->saveLocation(tempTaskDeadline);
	this->saveParticipants(tempTaskDeadline);
	this->saveNote(tempTaskDeadline);
	this->savePriority(tempTaskDeadline);
	this->saveTags(tempTaskDeadline);
	this->saveReminderTimes(tempTaskDeadline);
	this->saveState(tempTaskDeadline);
}

void Storage::saveTimedTask(Task* tempTask)
{
	TimedTask* tempTaskTimed = (TimedTask *) tempTask;
	this->saveTaskType(tempTaskTimed);
	this->saveIndex(tempTaskTimed);
	this->saveName(tempTaskTimed);
	this->saveFromDate(tempTaskTimed);
	this->saveToDate(tempTaskTimed);
	this->saveLocation(tempTaskTimed);
	this->saveParticipants(tempTaskTimed);
	this->saveNote(tempTaskTimed);
	this->savePriority(tempTaskTimed);
	this->saveTags(tempTaskTimed);
	this->saveReminderTimes(tempTaskTimed);
	this->saveState(tempTaskTimed);
}

void Storage::saveFloatingTask(Task* tempTask)
{
	FloatingTask* tempTaskFloating = (FloatingTask *) tempTask;
	this->saveTaskType(tempTaskFloating);
	this->saveIndex(tempTaskFloating);
	this->saveName(tempTaskFloating);
	this->saveLocation(tempTaskFloating);
	this->saveParticipants(tempTaskFloating);
	this->saveNote(tempTaskFloating);
	this->savePriority(tempTaskFloating);
	this->saveTags(tempTaskFloating);
	this->saveReminderTimes(tempTaskFloating);
	this->saveState(tempTaskFloating);
}

template <class taskTypePointer>
void Storage::saveTaskType(taskTypePointer tempTask)
{
	string taskTypeStr = convertToString(tempTask->getTaskType());
	this->writeLineToFile(taskTypeStr);
}

template <class taskTypePointer>
void Storage::saveIndex(taskTypePointer tempTask)
{
	this->writeLineToFile(convertToString(tempTask->getIndex()));
}

template <class taskTypePointer>
void Storage::saveName(taskTypePointer tempTask)
{
	if(tempTask->getFlagName())
	{
		this->writeLineToFile(tempTask->getName());
	}
	else
	{
		this->writeLineToFile("");
	}
}

template <class taskTypePointer>
void Storage::saveDueDate(taskTypePointer tempTask)
{
	if(tempTask->getFlagDueDate())
	{
		string dueDateStr = convertToString(tempTask->getDueDate());
		this->writeLineToFile(dueDateStr);
	}
	else
	{
		this->writeLineToFile("");
	}
}

template <class taskTypePointer>
void Storage::saveFromDate(taskTypePointer tempTask)
{
	if(tempTask->getFlagFromDate())
	{
		string fromDateStr = convertToString(tempTask->getFromDate());
		this->writeLineToFile(fromDateStr);
	}
	else
	{
		this->writeLineToFile("");
	}
}

template <class taskTypePointer>
void Storage::saveToDate(taskTypePointer tempTask)
{
	if(tempTask->getFlagToDate())
	{
		string toDateStr = convertToString(tempTask->getToDate());
		this->writeLineToFile(toDateStr);
	}
	else
	{
		this->writeLineToFile("");
	}
}

template <class taskTypePointer>
void Storage::saveLocation(taskTypePointer tempTask)
{
	if(tempTask->getFlagLocation())
	{
		this->writeLineToFile(tempTask->getLocation());
	}
	else
	{
		this->writeLineToFile("");
	}
}

template <class taskTypePointer>
void Storage::saveParticipants(taskTypePointer tempTask)
{
	if(tempTask->getFlagParticipant())
	{
		string participantStr = tempTask->getParticipants();//convertToString(tempTask->getParticipants());
		this->writeLineToFile(participantStr);
	}
	else
	{
		this->writeLineToFile("");
	}
}

template <class taskTypePointer>
void Storage::saveNote(taskTypePointer tempTask)
{
	if(tempTask->getFlagNote())
	{
		this->writeLineToFile(tempTask->getNote());
	}
	else
	{
		this->writeLineToFile("");
	}
}

template <class taskTypePointer>
void Storage::savePriority(taskTypePointer tempTask)
{
	throw "priority conversion needed!";
	if(tempTask->getFlagPriority())
	{
		string priorityStr = convertToString(tempTask->getPriority());
		this->writeLineToFile(priorityStr);
	}
	else
	{
		this->writeLineToFile("");
	}
}	

template <class taskTypePointer>
void Storage::saveTags(taskTypePointer tempTask)
{
	if(tempTask->getFlagTags())
	{
		string tagStr = tempTask->getTags();//convertToString(tempTask->getTags());
		this->writeLineToFile(tagStr);
	}
	else
	{
		this->writeLineToFile("");
	}
}

template <class taskTypePointer>
void Storage::saveReminderTimes(taskTypePointer tempTask)
{
	if(tempTask->getFlagRemindTime())
	{
		string reminderStr = convertToString(tempTask->getRemindTime());
		this->writeLineToFile(reminderStr);
	}
	else
	{
		this->writeLineToFile("");
	}
}

template <class taskTypePointer>
void Storage::saveState(taskTypePointer tempTask)
{
	if(tempTask->getFlagState())
	{
		string stateStr = convertToString(tempTask->getState());
		this->writeLineToFile(stateStr);
	}
	else
	{
		this->writeLineToFile("");
	}
}

/****************************************************/
/************** To String Converters ****************/
/****************************************************/

string Storage::convertToString(unsigned index)
{
	char *stringVal = NULL;
	itoa(index,stringVal,10);
	return string(stringVal);
}

string Storage::convertToString(TASK_TYPE type)
{
	return TASK_TYPE_STRING[type];
}

string Storage::convertToString(time_t time)
{
	stringstream ss(time);
	return ss.str();
}

string Storage::convertToString(list<time_t> timeList)
{
	list<time_t>::iterator it = timeList.begin();
	stringstream tempStream;
	while( it != timeList.end())
	{
		tempStream << "rt: " << convertToString(*it);
		it++;
	}

	return tempStream.str();
}

string Storage::convertToString(list<string> strList)
{
	stringstream tempStream;
	list<string>::iterator it = strList.begin();

	while(it != strList.end())
	{
		tempStream << "&&" << (*it);
		it++;
	}

	return tempStream.str();

}

string Storage::convertToString(PRIORITY priority)
{
	return PRIORITY_STRING[priority];
}

string Storage::convertToString(TASK_STATE state)
{
	return TASK_STATE_STRING[state];
}

bool Storage::writeLineToFile(string line)
{
	return false;
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