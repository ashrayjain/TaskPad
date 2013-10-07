#include "Storage.h"
#include "Task.h"
#include "Enum.h"
#include <list>
#include <sstream>

using namespace TP;

const string Storage::_fileName = "TaskPad.txt";
const string Storage::HEADER_TASK_COUNT = "Task Count: ";
const string Storage::HEADER_NAME = "Name: ";
const string Storage::HEADER_TYPE = "Type: ";
const string Storage::HEADER_INDEX = "Index: ";
const string Storage::HEADER_DUE_DATE = "Due: ";
const string Storage::HEADER_FROM_DATE = "StartTime: ";
const string Storage::HEADER_TO_DATE = "EndTime: ";
const string Storage::HEADER_LOCATION = "Location: ";
const string Storage::HEADER_PARTICIPANT_COUNT = "ParticipantCount: ";
const string Storage::HEADER_PARTICIPANT = ": ";
const string Storage::HEADER_NOTE = "Note: ";
const string Storage::HEADER_PRIORITY = "Priority: ";
const string Storage::HEADER_TAG_COUNT = "TagCount: ";
const string Storage::HEADER_TAG = ": ";
const string Storage::HEADER_REMINDER_TIME_COUNT = "ReminderTimeCount: ";
const string Storage::HEADER_REMINDER_TIME = ": ";
const string Storage::HEADER_STATE = "State: ";

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
		writeLineToFile("");
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
	saveHeader(HEADER_TASK_COUNT);
	saveCount(taskCount);
}

void Storage::saveParticipantCount(unsigned participantCount)
{
	saveHeader(HEADER_PARTICIPANT_COUNT);
	saveCount(participantCount);
}

void Storage::saveTagCount(unsigned tagCount)
{
	saveHeader(HEADER_TAG_COUNT);
	saveCount(tagCount);
}

void Storage::saveReminderCount(unsigned reminderCount)
{
	saveHeader(HEADER_REMINDER_TIME_COUNT);
	saveCount(reminderCount);
}

void Storage::saveTaskType(Task& tempTask)
{
	string taskTypeStr = convertToString(tempTask.getTaskType());

	saveHeader(HEADER_TYPE);
	this->writeLineToFile(taskTypeStr);
}

void Storage::saveIndex(Task& tempTask)
{
	saveHeader(HEADER_INDEX);
	this->writeLineToFile(convertToString(tempTask.getIndex()));
}

void Storage::saveName(Task& tempTask)
{
	if(tempTask.getFlagName())
	{
		saveHeader(HEADER_NAME);
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
		saveHeader(HEADER_LOCATION);
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

		this->saveParticipantCount(participantList.size());

		for(int i = 1; pit != participantList.end(); i++,pit++)
		{
			saveHeader(convertToString(i) + HEADER_PARTICIPANT);
			participant = (*pit);//convertToString(tempTask.getParticipants());
			this->writeLineToFile(participant);
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
		saveHeader(HEADER_NOTE);
		this->writeLineToFile(tempTask.getNote());
	}
	else
	{
		this->writeLineToFile("");
	}
}

void Storage::savePriority(Task& tempTask)
{
	saveHeader(HEADER_PRIORITY);
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

		this->saveTagCount(tagsList.size());

		for(int i = 1; tagit != tagsList.end();i++, tagit++)
		{
			saveHeader(convertToString(i) + HEADER_TAG);
			tag = (*tagit);
			this->writeLineToFile(tag);
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

		this->saveReminderCount(reminderList.size());

		for(int i = 0; rtit != reminderList.end(); i++,rtit++)
		{
			saveHeader(convertToString(i) + HEADER_REMINDER_TIME);
			string reminderStr = convertToString(*rtit);
			this->writeLineToFile(reminderStr);
		}
	}
	else
	{
		this->writeLineToFile("");
	}
}

void Storage::saveState(Task& tempTask)
{
	saveHeader(HEADER_STATE);
	string stateStr = convertToString(tempTask.getState());
	this->writeLineToFile(stateStr);
}

void Storage::saveDueDate(Task& tempTask)
{
	if(tempTask.getFlagDueDate())
	{
		saveHeader(HEADER_DUE_DATE);
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
		saveHeader(HEADER_FROM_DATE);
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
		saveHeader(HEADER_TO_DATE);
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

string Storage::convertToString(TASK_TYPE type)
{
	return TASK_TYPE_STRING[type];
}

string Storage::convertToString(time_t time)
{
	stringstream ss;
	ss << (time);
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