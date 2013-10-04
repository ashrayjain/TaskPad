#include "Storage.h"
#include "Task.h"
#include "Enum.h"
#include <list>
#include <sstream>

using namespace TP;

const string Storage::_fileName = "TaskPad.txt";

Storage::Storage(list<Task>&)
{
	throw "Storage Constructor not implemented!";
}

bool Storage::save(list<Task>& taskList)
{
	string singleTaskEntry = "";
	if(taskList.empty())
	{
		this->emptyTheFile();
		return true;
	}

	list<Task>::iterator it = taskList.begin();

	this->saveTaskCount(taskList.size());

	while(it != taskList.end())
	{
		this->saveGenericAttributes(&*it);
		switch(it->getTaskType())
		{
			case DEADLINE:
				this->saveDeadlineTaskSpecificAttributes(&(*it));
				break;
			case TIMED:
				this->saveTimedTaskSpecificAttributes(&(*it));
				break;
			case FLOATING:
				this->saveFloatingTaskSpecificAttributes(&(*it));
				break;
		}
		return false;
		it++;
	}
	return false;
}

void Storage::saveGenericAttributes(Task* tempTask)
{
	this->saveTaskType(tempTask);
	this->saveIndex(tempTask);
	this->saveName(tempTask);
	this->saveLocation(tempTask);
	this->saveParticipants(tempTask);
	this->saveNote(tempTask);
	this->savePriority(tempTask);
	this->saveTags(tempTask);
	this->saveReminderTimes(tempTask);
	this->saveState(tempTask);
}

void Storage::saveDeadlineTaskSpecificAttributes(Task* tempTask)
{
	DeadlineTask* tempTaskDeadline = (DeadlineTask *) tempTask;
	this->saveDueDate(tempTaskDeadline);
}

void Storage::saveTimedTaskSpecificAttributes(Task* tempTask)
{
	TimedTask* tempTaskTimed = (TimedTask *) tempTask;
	this->saveFromDate(tempTaskTimed);
	this->saveToDate(tempTaskTimed);
}

void Storage::saveFloatingTaskSpecificAttributes(Task* tempTask)
{
	// Nothing specific to save for floating task (yet)
	return;
}

void Storage::saveCount(unsigned count)
{
	std::string countStr = convertToString(count);
	this->writeLineToFile(countStr);
}

void Storage::saveTaskCount(unsigned taskCount)
{
	saveCount(taskCount);
}

void Storage::saveParticipantCount(unsigned participantCount)
{
	saveCount(participantCount);
}

void Storage::saveTagCount(unsigned tagCount)
{
	saveCount(tagCount);
}

void Storage::saveReminderCount(unsigned reminderCount)
{
	saveCount(reminderCount);
}

void Storage::saveTaskType(Task* tempTask)
{
	string taskTypeStr = convertToString(tempTask->getTaskType());
	this->writeLineToFile(taskTypeStr);
}

void Storage::saveIndex(Task* tempTask)
{
	this->writeLineToFile(convertToString(tempTask->getIndex()));
}

void Storage::saveName(Task* tempTask)
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

void Storage::saveLocation(Task* tempTask)
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

void Storage::saveParticipants(Task* tempTask)
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

void Storage::saveNote(Task* tempTask)
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

void Storage::savePriority(Task* tempTask)
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

void Storage::saveTags(Task* tempTask)
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

void Storage::saveReminderTimes(Task* tempTask)
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

void Storage::saveState(Task* tempTask)
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
		tempStream << convertToString(*it) << endl;
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

void Storage::writeLineToFile(string line)
{
	throw "storage writeLineToFile not implemented";
}

void Storage::emptyTheFile()
{
	throw "storage empty the file not implemented";
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