#ifndef _TASKSAVERTEXT_CPP_
#define _TASKSAVERTEXT_CPP_

#include <fstream>
#include <sstream>
#include "TaskSaverText.h"
#include "Enum.h"
#include "Task.h"

using namespace TP;
using namespace std;

void TaskSaverText::save(const list<Task>& taskList, const std::string& fileName)
{
	this->openFile(fileName);
	this->saveTaskList(taskList);
	this->closeFile();
	this->removeTaskFiles();
	return;
}

void TaskSaverText::removeTaskFiles()
{
	return;
}

void TaskSaverText::save(const Task& task)
{
	std::string fileName = convertToString(task.getIndex()) + ".task";
	this->openFile(fileName);
	this->saveTask(task);
	this->closeFile();
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

void TaskSaverText::emptyTheFile()
{
	//throw "storage empty the file not implemented";
}

#endif