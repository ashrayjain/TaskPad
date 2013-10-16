#include <list>
#include <sstream>
#include <string>
#include "Storage.h"
#include "Task.h"
#include "Enum.h"
#include "Logger.h"
#include "TaskLoaderText.h"

using namespace TP;
const string Storage::_fileName = "TaskPad.txt";

const string Storage::LABEL_START_OF_TASK = "StartOfTask";
const string Storage::LABEL_NAME = "name:";
const string Storage::LABEL_INDEX = "index:";
const string Storage::LABEL_DUE_DATE = "due:";
const string Storage::LABEL_FROM_DATE = "from:";
const string Storage::LABEL_TO_DATE = "to:";
const string Storage::LABEL_LOCATION = "at:";
const string Storage::LABEL_PARTICIPANT = "ppl:";
const string Storage::LABEL_NOTE = "note:";
const string Storage::LABEL_PRIORITY = "impt:";
const string Storage::LABEL_TAG = "#:";
const string Storage::LABEL_REMINDER_TIME = "rt:";
const string Storage::LABEL_STATE = "state:";
const string Storage::LABEL_END_OF_TASK = "EndOfTask";

Storage::Storage(list<Task>& taskList)
{
	_logger = Logger::getLogger();
	_logger->log("Storage","called constructor!");

	_loader = NULL;
	this->load(taskList);
}

bool Storage::save(const list<Task>& taskList)
{
	this->openTheFileToWrite();
	this->saveTaskList(taskList);
	this->closeTheWrittenFile();
	this->removeTaskFiles();
	return true;
}

void Storage::removeTaskFiles()
{
	return;
}

bool Storage::save(const Task& task)
{
	std::string fileName = convertToString(task.getIndex()) + ".task";
	this->openTheFileToWrite(fileName);
	this->saveTask(task);
	this->closeTheWrittenFile();
	return true;
}

void Storage::openTheFileToWrite(std::string fileName, std::ios_base::openmode mode)
{
	this->_fileWriter.open(fileName,mode);
	return;
}

void Storage::closeTheWrittenFile()
{
	this->_fileWriter.flush();
	this->_fileWriter.close();
	return;
}

void Storage::saveTaskList(const list<Task>& taskList)
{
	list<Task>::const_iterator it = taskList.begin();
	while(it != taskList.end())
	{
		this->saveTask(*it);
		it++;
	}
}

void Storage::saveTask(const Task& task)
{
	writeLineToFile("");

	saveTaskLevelLabel(this->LABEL_START_OF_TASK);
	saveTaskAttributes(task);
	saveTaskLevelLabel(this->LABEL_END_OF_TASK);

	return;
}

void Storage::saveTaskAttributes(const Task& tempTask)
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

void Storage::saveTaskLevelLabel(std::string LabelStr)
{
	writeLineToFile(LabelStr);
}

void Storage::saveAttributeLevelLabel(string LabelStr)
{
	writeLineToFile(LabelStr,false);
}

void Storage::saveIndex(const Task& tempTask)
{
	saveAttributeLevelLabel(LABEL_INDEX);
	writeLineToFile(convertToString(tempTask.getIndex()));
}

void Storage::saveName(const Task& tempTask)
{
	if(tempTask.getFlagName())
	{
		saveAttributeLevelLabel(LABEL_NAME);
		writeLineToFile(tempTask.getName());
	}
}

void Storage::saveLocation(const Task& tempTask)
{
	if(tempTask.getFlagLocation())
	{
		saveAttributeLevelLabel(LABEL_LOCATION);
		writeLineToFile(tempTask.getLocation());
	}
}

void Storage::saveParticipants(const Task& tempTask)
{
	if(tempTask.getFlagParticipants())
	{
		list<std::string> participantList = tempTask.getParticipants();
		list<std::string>::iterator pit = participantList.begin();
		string participant;

		while(pit != participantList.end())
		{
			saveAttributeLevelLabel(LABEL_PARTICIPANT);

			participant = (*pit);
			writeLineToFile(participant);

			pit++;
		}
	}
}

void Storage::saveNote(const Task& tempTask)
{
	if(tempTask.getFlagNote())
	{
		saveAttributeLevelLabel(LABEL_NOTE);
		writeLineToFile(tempTask.getNote());
	}
}

void Storage::savePriority(const Task& tempTask)
{
	saveAttributeLevelLabel(LABEL_PRIORITY);

	string priorityStr = convertToString(tempTask.getPriority());
	writeLineToFile(priorityStr);
}	

void Storage::saveTags(const Task& tempTask)
{
	if(tempTask.getFlagTags())
	{
		list<std::string> tagsList = tempTask.getTags();
		list<std::string>::iterator tagit = tagsList.begin();
		string tag = "";

		while(tagit != tagsList.end())
		{
			saveAttributeLevelLabel(LABEL_TAG);

			tag = (*tagit);
			writeLineToFile(tag);
			tagit++;
		}
	}
}

void Storage::saveReminderTimes(const Task& tempTask)
{
	if(tempTask.getFlagRemindTimes())
	{
		list<time_t> reminderList = tempTask.getRemindTimes();
		list<time_t>::iterator rtit = reminderList.begin();

		while(rtit != reminderList.end())
		{
			saveAttributeLevelLabel(LABEL_REMINDER_TIME);

			string reminderStr = convertToString(*rtit);
			writeLineToFile(reminderStr);
			rtit++;
		}
	}
}

void Storage::saveState(const Task& tempTask)
{
	saveAttributeLevelLabel(LABEL_STATE);

	string stateStr = convertToString(tempTask.getState());
	writeLineToFile(stateStr);
}

void Storage::saveDueDate(const Task& tempTask)
{
	if(tempTask.getFlagDueDate())
	{
		saveAttributeLevelLabel(LABEL_DUE_DATE);

		string dueDateStr = convertToString(tempTask.getDueDate());
		writeLineToFile(dueDateStr);
	}
}

void Storage::saveFromDate(const Task& tempTask)
{
	if(tempTask.getFlagFromDate())
	{
		saveAttributeLevelLabel(LABEL_FROM_DATE);

		string fromDateStr = convertToString(tempTask.getFromDate());
		writeLineToFile(fromDateStr);
	}
}

void Storage::saveToDate(const Task& tempTask)
{
	if(tempTask.getFlagToDate())
	{
		saveAttributeLevelLabel(LABEL_TO_DATE);
		
		string toDateStr = convertToString(tempTask.getToDate());
		writeLineToFile(toDateStr);
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

void Storage::writeLineToFile(string line, bool newLine)
{
	if(newLine)
		_fileWriter << line << endl;
	else
		_fileWriter << line << " ";
}

void Storage::emptyTheFile()
{
	//throw "storage empty the file not implemented";
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
	/*
	this->_fileReader.close();
	this->_fileWriter.close();
	*/
}