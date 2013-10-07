#include "Storage.h"
#include "Task.h"
#include "Enum.h"
#include <list>
#include <sstream>

using namespace TP;

const string Storage::_fileName = "TaskPad.txt";

Storage::Storage(list<Task>& taskList)
{
}

bool Storage::save(list<Task>& taskList)
{
	this->openTheFileToWrite(std::ios_base::trunc);
	this->emptyTheFile();
	this->saveTaskCount(taskList.size());
	this->saveTaskList(taskList);
	this->closeTheWrittenFile();
	return true;
}

void Storage::openTheFileToWrite(std::ios_base::openmode mode)
{
	this->_fileWriter.open(this->_fileName,mode);
	return;
}

void Storage::openTheFileToRead()
{
	this->_fileReader.open(this->_fileName);
	return;
}

void Storage::closeTheWrittenFile()
{
	this->_fileWriter.close();
	return;
}

void Storage::closeTheReadFile()
{
	this->_fileReader.close();
	return;
}

void Storage::saveTaskList(list<Task>& taskList)
{
	list<Task>::iterator it = taskList.begin();
	while(it != taskList.end())
	{
		this->saveTask(*it);
		it++;
	}
}

void Storage::saveTask(Task& tempTask)
{
	this->saveTaskType(tempTask);
	this->saveIndex(tempTask);
	this->saveName(tempTask);
	this->saveDueDate(tempTask);
	this->saveFromDate(tempTask);
	this->saveToDate(tempTask);
	this->saveLocation(tempTask);
	this->saveParticipants(tempTask);
	this->saveNote(tempTask);
	this->savePriority(tempTask);
	this->saveTags(tempTask);
	this->saveReminderTimes(tempTask);
	this->saveState(tempTask);
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

void Storage::saveTaskType(Task& tempTask)
{
	string taskTypeStr = convertToString(tempTask.getTaskType());
	this->writeLineToFile(taskTypeStr);
}

void Storage::saveIndex(Task& tempTask)
{
	this->writeLineToFile(convertToString(tempTask.getIndex()));
}

void Storage::saveName(Task& tempTask)
{
	if(tempTask.getFlagName())
	{
		this->writeLineToFile(tempTask.getName());
	}
	else
	{
		this->writeLineToFile("");
	}
}

void Storage::saveLocation(Task& tempTask)
{
	if(tempTask.getFlagLocation())
	{
		this->writeLineToFile(tempTask.getLocation());
	}
	else
	{
		this->writeLineToFile("");
	}
}

void Storage::saveParticipants(Task& tempTask)
{
	if(tempTask.getFlagParticipants())
	{
		list<std::string> participantList = tempTask.getParticipants();
		list<std::string>::iterator pit = participantList.begin();
		string participant = "";

		while(pit != participantList.end())
		{
			participant = (*pit);//convertToString(tempTask.getParticipants());
			this->writeLineToFile(participant);
			pit++;
		}
	}
	else
	{
		this->writeLineToFile("");
	}
}

void Storage::saveNote(Task& tempTask)
{
	if(tempTask.getFlagNote())
	{
		this->writeLineToFile(tempTask.getNote());
	}
	else
	{
		this->writeLineToFile("");
	}
}

void Storage::savePriority(Task& tempTask)
{
	string priorityStr = convertToString(tempTask.getPriority());
	this->writeLineToFile(priorityStr);
}	

void Storage::saveTags(Task& tempTask)
{
	if(tempTask.getFlagTags())
	{
		list<std::string> tagsList = tempTask.getTags();
		list<std::string>::iterator tagit = tagsList.begin();
		string tag = "";

		while(tagit != tagsList.end())
		{
			tag = (*tagit);
			this->writeLineToFile(tag);
			tagit++;
		}
	}
	else
	{
		this->writeLineToFile("");
	}
}

void Storage::saveReminderTimes(Task& tempTask)
{
	if(tempTask.getFlagRemindTimes())
	{
		list<time_t> reminderList = tempTask.getRemindTimes();
		list<time_t>::iterator rtit = reminderList.begin();

		while(rtit != reminderList.end())
		{
		string reminderStr = convertToString(*rtit);
		this->writeLineToFile(reminderStr);
		rtit++;
		}
	}
	else
	{
		this->writeLineToFile("");
	}
}

void Storage::saveState(Task& tempTask)
{
	string stateStr = convertToString(tempTask.getState());
	this->writeLineToFile(stateStr);
}

void Storage::saveDueDate(Task& tempTask)
{
	if(tempTask.getFlagDueDate())
	{
		string dueDateStr = convertToString(tempTask.getDueDate());
		this->writeLineToFile(dueDateStr);
	}
	else
	{
		this->writeLineToFile("");
	}
}

void Storage::saveFromDate(Task& tempTask)
{
	if(tempTask.getFlagFromDate())
	{
		string fromDateStr = convertToString(tempTask.getFromDate());
		this->writeLineToFile(fromDateStr);
	}
	else
	{
		this->writeLineToFile("");
	}
}

void Storage::saveToDate(Task& tempTask)
{
	if(tempTask.getFlagToDate())
	{
		string toDateStr = convertToString(tempTask.getToDate());
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
	//throw "storage writeLineToFile not implemented";
}

void Storage::emptyTheFile()
{
	//throw "storage empty the file not implemented";
}

bool Storage::save(const Command&)
{
	return false;
}

Storage::~Storage()
{
	/*
	this->_fileReader.close();
	this->_fileWriter.close();
	*/
}