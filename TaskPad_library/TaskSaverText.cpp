#ifndef _TASKSAVERTEXT_CPP_
#define _TASKSAVERTEXT_CPP_

#include <fstream>
#include <sstream>
#include "TaskSaverText.h"
#include "Enum.h"
#include "Task.h"
#include "Logger.h"

using namespace TP;
using namespace std;

const std::string TaskSaverText::TASK_DIRECTORY = "Tasks\\";
const std::string TaskSaverText::RECORD_MODIFIED_FILE_NAME = "savedTasks.txt";
const std::string TaskSaverText::RECORD_DELETED_FILE_NAME = "deletedTasks.txt";

TaskSaverText::TaskSaverText()
{
	this->_logger = Logger::getLogger();
}

void TaskSaverText::save(const list<Task>& taskList, const std::string& fileName)
{
	this->openFile(fileName);
	this->saveTaskList(taskList);
	this->closeFile();
	this->removeTaskFiles();
	this->removeSaveRecord();
	this->removeDeleteRecord();
	return;
}

void TaskSaverText::removeTaskFiles()
{
	ifstream record(RECORD_MODIFIED_FILE_NAME);
	std::string nextTaskFile;

	while(record.good())
	{
		getline(record, nextTaskFile);
		if(nextTaskFile!= "")
		{
			_logger->log("TaskSaverText","removing file: "+nextTaskFile,NOTICELOG);

			if(int ret = std::remove(nextTaskFile.c_str()) != 0)
			{
				_logger->log("TaskSaverText","error num: " + convertToString(ret),ERRORLOG);
			}
			nextTaskFile = "";
		}
	}
	record.close();
}

void TaskSaverText::save(const Task& task, const COMMAND_TYPE& cType)
{
	if (cType == DEL)
	{
		ofstream record(RECORD_DELETED_FILE_NAME, ios_base::app);
		record << task.getIndex() << endl;
		record.close();
	}
	else
	{
		std::string fileName = convertToString(task.getIndex()) + ".task";
		std::string taskFileName = TASK_DIRECTORY + fileName;

		this->openFile(taskFileName);
		this->saveTask(task);
		this->closeFile();

		this->updateSaveRecord(taskFileName);
	}
	return;
}

void TaskSaverText::saveTaskList(const list<Task>& taskList)
{
	list<Task>::const_iterator it = taskList.begin();
	while(it != taskList.end())
	{
		this->saveTask(*it);
		it++;
	}
}

void TaskSaverText::saveTask(const Task& task)
{
	writeLineToFile("");

	saveTaskLevelLabel(this->LABEL_START_OF_TASK);
	saveTaskAttributes(task);
	saveTaskLevelLabel(this->LABEL_END_OF_TASK);

	return;
}

void TaskSaverText::saveTaskAttributes(const Task& tempTask)
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

void TaskSaverText::saveTaskLevelLabel(std::string LabelStr)
{
	writeLineToFile(LabelStr);
}

void TaskSaverText::saveAttributeLevelLabel(string LabelStr)
{
	writeLineToFile(LabelStr,false);
}

void TaskSaverText::saveIndex(const Task& tempTask)
{
	saveAttributeLevelLabel(LABEL_INDEX);
	writeLineToFile(convertToString(tempTask.getIndex()));
}

void TaskSaverText::saveName(const Task& tempTask)
{
	if(tempTask.getFlagName())
	{
		saveAttributeLevelLabel(LABEL_NAME);
		writeLineToFile(tempTask.getName());
	}
}

void TaskSaverText::saveLocation(const Task& tempTask)
{
	if(tempTask.getFlagLocation())
	{
		saveAttributeLevelLabel(LABEL_LOCATION);
		writeLineToFile(tempTask.getLocation());
	}
}

void TaskSaverText::saveParticipants(const Task& tempTask)
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

void TaskSaverText::saveNote(const Task& tempTask)
{
	if(tempTask.getFlagNote())
	{
		saveAttributeLevelLabel(LABEL_NOTE);
		writeLineToFile(tempTask.getNote());
	}
}

void TaskSaverText::savePriority(const Task& tempTask)
{
	saveAttributeLevelLabel(LABEL_PRIORITY);

	string priorityStr = convertToString(tempTask.getPriority());
	writeLineToFile(priorityStr);
}	

void TaskSaverText::saveTags(const Task& tempTask)
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

void TaskSaverText::saveReminderTimes(const Task& tempTask)
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

void TaskSaverText::saveState(const Task& tempTask)
{
	saveAttributeLevelLabel(LABEL_STATE);

	string stateStr = convertToString(tempTask.getState());
	writeLineToFile(stateStr);
}

void TaskSaverText::saveDueDate(const Task& tempTask)
{
	if(tempTask.getFlagDueDate())
	{
		saveAttributeLevelLabel(LABEL_DUE_DATE);

		string dueDateStr = convertToString(tempTask.getDueDate());
		writeLineToFile(dueDateStr);
	}
}

void TaskSaverText::saveFromDate(const Task& tempTask)
{
	if(tempTask.getFlagFromDate())
	{
		saveAttributeLevelLabel(LABEL_FROM_DATE);

		string fromDateStr = convertToString(tempTask.getFromDate());
		writeLineToFile(fromDateStr);
	}
}

void TaskSaverText::saveToDate(const Task& tempTask)
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

string TaskSaverText::convertToString(int num)
{
	stringstream tmpstream;
	tmpstream << (num);
	return tmpstream.str();
}

string TaskSaverText::convertToString(unsigned long long index)
{
	stringstream tmpstream;
	tmpstream << (index);
	return tmpstream.str();
}

string TaskSaverText::convertToString(time_t time)
{
	stringstream ss;
	ss << (time);
	return ss.str();
}

string TaskSaverText::convertToString(PRIORITY priority)
{
	return PRIORITY_STRING[priority];
}

string TaskSaverText::convertToString(TASK_STATE state)
{
	return TASK_STATE_STRING[state];
}

void TaskSaverText::updateSaveRecord	(std::string entry)
{
	ofstream record(RECORD_MODIFIED_FILE_NAME,std::ios_base::app);
	record<<entry<<endl;
	record.close();
	return;
}

void TaskSaverText::removeSaveRecord	()
{
	//empty the file just in case
	ofstream record(RECORD_MODIFIED_FILE_NAME,ios_base::trunc);
	record.close();

	//delete the file
	std::remove(RECORD_MODIFIED_FILE_NAME.c_str());
	return;
}

void TaskSaverText::removeDeleteRecord	()
{
	//empty the file just in case
	ofstream record(RECORD_DELETED_FILE_NAME,ios_base::trunc);
	record.close();

	//delete the file
	std::remove(RECORD_DELETED_FILE_NAME.c_str());
	return;
}

/****************************************************/
/***************** Actual Writers ****************/
/****************************************************/

void TaskSaverText::writeLineToFile(string line, bool newLine)
{
	if(newLine)
		_fileWriter << line << endl;
	else
		_fileWriter << line << " ";
}

#endif