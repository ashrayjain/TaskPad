/*
 * =====================================================================================
 *
 *		Filename:  TaskSaverText.cpp
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file implements all of the functions defined in TaskSaverText.h file
  * 
 */

#ifndef _TASKSAVERTEXT_CPP_
#define _TASKSAVERTEXT_CPP_

#include <fstream>
#include <sstream>
#include <assert.h>
#include <QDir>
#include <QString>
#include "TaskSaverText.h"
#include "Enum.h"
#include "Task.h"
#include "Logger.h"
#include "StorableTaskDatastore.h"

using namespace TP;
using namespace std;

TaskSaverText::TaskSaverText() {
	_logger = Logger::getLogger();
}

/****************************************************/
/*************** Public Save APIs *******************/
/****************************************************/

void TaskSaverText::save(StorableTaskDatastore* taskDS, const std::string& fileName) {
	openFile(fileName);
	saveTaskDS(taskDS);
	closeFile();

	// remove the quick save files
	removeTaskFiles();
	return;
}

// quick save that creates a new .task file for each task that was edited/added
// deleted tasks' IDs are recorded in a delete record
void TaskSaverText::save(const Task& task, const COMMAND_TYPE& cType) {
	assert(isSaveableCommandType(cType));
	switch(cType) {
		case DEL:
			saveDeleteCommand(task);
			break;
		default:
			saveNonDeleteCommands(task);
			break;
	}
	return;
}

void TaskSaverText::saveNonDeleteCommands (const Task& task) {
	std::string taskFilePath = getTaskFilePath(task);

	openFile(taskFilePath);
	saveTask(task);
	closeFile();

	removeDeletedTaskFile(task);
}
void TaskSaverText::saveDeleteCommand(const Task& task) {
	std::string deletedTaskFilePath = getDeletedTaskFilePath(task);

	openFile(deletedTaskFilePath);
	writeLineToFile(convertToString(task.getIndex()));
	closeFile();

	removeTaskFile(task);
}

void TaskSaverText::removeTaskFile	(const Task& task) {
	string taskFileName = convertToString(task.getIndex()) + TASK_FILE_EXTENSION;
	QDir curDir(QString::fromStdString(TASK_DIRECTORY));

	bool test = curDir.remove(QString::fromStdString(taskFileName));
}

void TaskSaverText::removeDeletedTaskFile (const Task& task) {
	string taskFileName = convertToString(task.getIndex())+ TASK_DELETED_FILE_EXTENSION;
	QDir curDir(QString::fromStdString(TASK_DIRECTORY));

	bool test = curDir.remove(QString::fromStdString(taskFileName));
}

/****************************************************/
/************** Private Save Methods ****************/
/****************************************************/

void TaskSaverText::saveTaskDS(StorableTaskDatastore* taskDS) {
	return saveTaskList(taskDS->getTaskList());
	/*StorableTaskDatastore::const_iterator* it = taskDS->cbeginPtr();

	while(it != taskDS->cendPtr()) {
		saveTask(**it);
		it++;
	}
	delete it;
	*/
}

void TaskSaverText::saveTaskList(const list<Task>& taskList) {
	list<Task>::const_iterator it = taskList.begin();
	while(it != taskList.end()) {
		saveTask(*it);
		it++;
	}
}

void TaskSaverText::saveTask(const Task& task) {
	writeLineToFile("");

	saveTaskLevelLabel(LABEL_START_OF_TASK);
	saveTaskAttributes(task);
	saveTaskLevelLabel(LABEL_END_OF_TASK);

	return;
}

/*****************************************************/
/************** Task Attribute Savers ****************/
/*****************************************************/

void TaskSaverText::saveTaskAttributes(const Task& tempTask) {
	saveIndex(tempTask);
	saveName(tempTask);
	saveDueDate(tempTask);
	saveFromDate(tempTask);
	saveToDate(tempTask);
	saveLocation(tempTask);
	saveParticipants(tempTask);
	saveNote(tempTask);
	savePriority(tempTask);
	saveTags(tempTask);
	saveReminderTimes(tempTask);
	saveState(tempTask);
}

void TaskSaverText::saveIndex(const Task& tempTask) {
	saveAttributeLevelLabel(LABEL_INDEX);
	writeLineToFile(convertToString(tempTask.getIndex()));
}

void TaskSaverText::saveName(const Task& tempTask) {
	_logger->log("TaskSaverText","entering function to saving name");
	if(tempTask.getFlagName()) {
		_logger->log("TaskSaverText","saving name attribute",NOTICELOG);
		saveAttributeLevelLabel(LABEL_NAME);
		writeLineToFile(tempTask.getName());
	}
}

void TaskSaverText::saveDueDate(const Task& tempTask) {
	if(tempTask.getFlagDueDate()) {
		saveAttributeLevelLabel(LABEL_DUE_DATE);

		string dueDateStr = convertToString(tempTask.getDueDate());
		writeLineToFile(dueDateStr);
	}
}

void TaskSaverText::saveFromDate(const Task& tempTask) {
	if(tempTask.getFlagFromDate()) {
		saveAttributeLevelLabel(LABEL_FROM_DATE);

		string fromDateStr = convertToString(tempTask.getFromDate());
		writeLineToFile(fromDateStr);
	}
}

void TaskSaverText::saveToDate(const Task& tempTask) {
	if(tempTask.getFlagToDate()) {
		saveAttributeLevelLabel(LABEL_TO_DATE);
		
		string toDateStr = convertToString(tempTask.getToDate());
		writeLineToFile(toDateStr);
	}
}


void TaskSaverText::saveLocation(const Task& tempTask) {
	if(tempTask.getFlagLocation()) {
		saveAttributeLevelLabel(LABEL_LOCATION);
		writeLineToFile(tempTask.getLocation());
	}
}

void TaskSaverText::saveParticipants(const Task& tempTask) {
	if(tempTask.getFlagParticipants()) {
		list<std::string> participantList = tempTask.getParticipants();
		list<std::string>::iterator pit = participantList.begin();
		string participant = "";

		while(pit != participantList.end()) {
			saveAttributeLevelLabel(LABEL_PARTICIPANT);

			participant = (*pit);
			writeLineToFile(participant);

			pit++;
		}
	}
}

void TaskSaverText::saveNote(const Task& tempTask) {
	if(tempTask.getFlagNote()) {
		saveAttributeLevelLabel(LABEL_NOTE);
		writeLineToFile(tempTask.getNote());
	}
}

void TaskSaverText::savePriority(const Task& tempTask) {
	saveAttributeLevelLabel(LABEL_PRIORITY);

	string priorityStr = convertToString(tempTask.getPriority());
	writeLineToFile(priorityStr);
}	

void TaskSaverText::saveTags(const Task& tempTask) {
	if(tempTask.getFlagTags()) {
		list<std::string> tagsList = tempTask.getTags();
		list<std::string>::iterator tagit = tagsList.begin();
		string tag = "";

		while(tagit != tagsList.end()) {
			saveAttributeLevelLabel(LABEL_TAG);

			tag = (*tagit);
			writeLineToFile(tag);
			tagit++;
		}
	}
}

void TaskSaverText::saveReminderTimes(const Task& tempTask) {
	if(tempTask.getFlagRemindTimes()) {
		list<time_t> reminderList = tempTask.getRemindTimes();
		list<time_t>::iterator rtit = reminderList.begin();
		string reminderStr = "";

		while(rtit != reminderList.end()) {
			saveAttributeLevelLabel(LABEL_REMINDER_TIME);

			reminderStr = convertToString(*rtit);
			writeLineToFile(reminderStr);
			rtit++;
		}
	}
}

void TaskSaverText::saveState(const Task& tempTask) {
	saveAttributeLevelLabel(LABEL_STATE);

	string stateStr = convertToString(tempTask.getState());
	writeLineToFile(stateStr);
}

/****************************************************/
/************** To String Converters ****************/
/****************************************************/

string TaskSaverText::convertToString(int num) {
	stringstream tmpstream;
	tmpstream << (num);
	return tmpstream.str();
}

string TaskSaverText::convertToString(unsigned long long index) {
	stringstream tmpstream;
	tmpstream << (index);
	return tmpstream.str();
}

string TaskSaverText::convertToString(time_t time) {
	stringstream ss;
	ss << (time);
	return ss.str();
}

string TaskSaverText::convertToString(PRIORITY priority) {
	return PRIORITY_STRING[priority];
}

string TaskSaverText::convertToString(TASK_STATE state) {
	return TASK_STATE_STRING[state];
}

/*********************************************************/
/********** Quick Save/Delete Record Managers ************/
/*********************************************************/

void TaskSaverText::removeTaskFiles() {
	QDir temp(QString::fromStdString(TASK_DIRECTORY));
	temp.removeRecursively();
	QDir curDir;
	curDir.mkdir(TASK_DIRECTORY.c_str());
}

/****************************************************/
/***************** Actual Writers *******************/
/****************************************************/

void TaskSaverText::saveTaskLevelLabel(std::string LabelStr) {
	writeLineToFile(LabelStr);
}

void TaskSaverText::saveAttributeLevelLabel(string LabelStr) {
	writeLineToFile(LabelStr,false);
}

void TaskSaverText::writeLineToFile(string line, bool newLine) {
	if(newLine)
		_fileWriter << line << endl;
	else
		_fileWriter << line << " ";
}

/****************************************************/
/***************** Helper functions *****************/
/****************************************************/


bool TaskSaverText::isSaveableCommandType(COMMAND_TYPE cType) {
	return (cType == DEL || cType == MOD || cType == ADD);
}

#endif