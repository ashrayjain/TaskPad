#include "Storage.h"
#include "Task.h"
#include "Enum.h"
#include <list>
#include <sstream>

using namespace TP;

const string Storage::_fileName = "TaskPad.txt";
const string Storage::LABEL_TASK_COUNT = "Task Count: ";
const string Storage::LABEL_NAME = "Name: ";
const string Storage::LABEL_INDEX = "Index: ";
const string Storage::LABEL_DUE_DATE = "Due: ";
const string Storage::LABEL_FROM_DATE = "StartTime: ";
const string Storage::LABEL_TO_DATE = "EndTime: ";
const string Storage::LABEL_LOCATION = "Location: ";
const string Storage::LABEL_PARTICIPANT = "Participant: ";
const string Storage::LABEL_NOTE = "Note: ";
const string Storage::LABEL_PRIORITY = "Priority: ";
const string Storage::LABEL_TAG = "Tag: ";
const string Storage::LABEL_REMINDER_TIME = "Reminder: ";
const string Storage::LABEL_STATE = "State: ";
const string Storage::LABEL_END_OF_TASK = "END OF TASK";

Storage::Storage(list<Task>& taskList)
{
}

bool Storage::save(list<Task>& taskList)
{
	this->openTheFileToWrite(std::ios_base::trunc);
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
	this->_fileWriter.flush();
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
	this->saveTaskCount(taskList.size());
	writeLineToFile("");

	list<Task>::iterator it = taskList.begin();
	while(it != taskList.end())
	{
		this->saveTask(*it);
		writeLineToFile(this->LABEL_END_OF_TASK);
		it++;
	}
}

void Storage::saveTask(Task& tempTask)
{
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

void Storage::saveHeader(string headerStr)
{
	writeLineToFile(headerStr,false);
}

void Storage::saveCount(unsigned count)
{
	std::string countStr = convertToString(count);
	this->writeLineToFile(countStr);
}

void Storage::saveTaskCount(unsigned taskCount)
{
	saveHeader(LABEL_TASK_COUNT);
	saveCount(taskCount);
}

void Storage::saveIndex(Task& tempTask)
{
	saveHeader(LABEL_INDEX);
	this->writeLineToFile(convertToString(tempTask.getIndex()));
}

void Storage::saveName(Task& tempTask)
{
	if(tempTask.getFlagName())
	{
		saveHeader(LABEL_NAME);
		this->writeLineToFile(tempTask.getName());
	}
}

void Storage::saveLocation(Task& tempTask)
{
	if(tempTask.getFlagLocation())
	{
		saveHeader(LABEL_LOCATION);
		this->writeLineToFile(tempTask.getLocation());
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
			saveHeader(LABEL_PARTICIPANT);
			participant = (*pit);//convertToString(tempTask.getParticipants());
			this->writeLineToFile(participant);
			pit++;
		}
	}
}

void Storage::saveNote(Task& tempTask)
{
	if(tempTask.getFlagNote())
	{
		saveHeader(LABEL_NOTE);
		this->writeLineToFile(tempTask.getNote());
	}
}

void Storage::savePriority(Task& tempTask)
{
	saveHeader(LABEL_PRIORITY);
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
			saveHeader(LABEL_TAG);
			tag = (*tagit);
			this->writeLineToFile(tag);
			tagit++;
		}
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
			saveHeader(LABEL_REMINDER_TIME);
			string reminderStr = convertToString(*rtit);
			this->writeLineToFile(reminderStr);
			rtit++;
		}
	}
}

void Storage::saveState(Task& tempTask)
{
	saveHeader(LABEL_STATE);
	string stateStr = convertToString(tempTask.getState());
	this->writeLineToFile(stateStr);
}

void Storage::saveDueDate(Task& tempTask)
{
	if(tempTask.getFlagDueDate())
	{
		saveHeader(LABEL_DUE_DATE);
		string dueDateStr = convertToString(tempTask.getDueDate());
		this->writeLineToFile(dueDateStr);
	}
}

void Storage::saveFromDate(Task& tempTask)
{
	if(tempTask.getFlagFromDate())
	{
		saveHeader(LABEL_FROM_DATE);
		string fromDateStr = convertToString(tempTask.getFromDate());
		this->writeLineToFile(fromDateStr);
	}
}

void Storage::saveToDate(Task& tempTask)
{
	if(tempTask.getFlagToDate())
	{
		saveHeader(LABEL_TO_DATE);
		string toDateStr = convertToString(tempTask.getToDate());
		this->writeLineToFile(toDateStr);
	}
}

/****************************************************/
/************** To String Converters ****************/
/****************************************************/

string Storage::convertToString(int num)
{
	stringstream tmpstream;
	tmpstream << (num);
	return tmpstream.str();
}

string Storage::convertToString(unsigned index)
{
	stringstream tmpstream;
	tmpstream << (index);
	return tmpstream.str();
}

string Storage::convertToString(unsigned long long index)
{
	stringstream tmpstream;
	tmpstream << (index);
	return tmpstream.str();
}

string Storage::convertToString(time_t time)
{
	stringstream ss;
	ss << (time);
	return ss.str();
}

string Storage::convertToString(PRIORITY priority)
{
	return PRIORITY_STRING[priority];
}

string Storage::convertToString(TASK_STATE state)
{
	return TASK_STATE_STRING[state];
}

/****************************************************/
/***************** Actual Writers ****************/
/****************************************************/

void Storage::writeLineToFile(string line)
{
	writeLineToFile(line,true);
}

void Storage::writeLineToFile(string line, bool newLine)
{
	if(newLine)
		_fileWriter << line << endl;
	else
		_fileWriter << line;
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