/*
 * =====================================================================================
 *
 *		Filename:  TaskLoaderText.cpp
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file implements all of the functions defined in TaskLoaderText.h file
  * 
 */

#ifndef _TASKLOADERTEXT_CPP_
#define _TASKLOADERTEXT_CPP_

#include <list>
#include <sstream>
#include <QDir>
#include <QStringList>
#include <QString>
#include "TaskLoaderText.h"
#include "Task.h"
#include "Enum.h"
#include "Logger.h"
#include "StorableTaskDatastore.h"

using namespace TP;
using namespace std;
// @author A0100124J

const string TaskLoaderText::FILTER_FOR_TASK_FILES				= "*.task";
const string TaskLoaderText::FILTER_FOR_DELETED_TASK_FILES		= "*.deltask";
const string TaskLoaderText::DEFAULT_KEY_VALUE_DELIMITER		= " ";
const string TaskLoaderText::DEFAULT_DATA_VALUE					= "";

TaskLoaderText::TaskLoaderText(StorableTaskDatastore* taskDS) {
	_logger = Logger::getLogger();
	_taskDS = taskDS;
	mkTaskDir();
}

void TaskLoaderText::mkTaskDir() {
	QDir temp;
	if (!temp.cd(QString("Tasks")))
	{
		temp.mkdir("Tasks");
	}
}

/****************************************************/
/**************** Public Load API *******************/
/****************************************************/

void TaskLoaderText::load (const string& fileName) {
	recoverUnsavedChanges();

	openFile(fileName);
	loadTaskDS();
	closeFile();
	return;
}

/****************************************************/
/*************** Recovery Methods *******************/
/****************************************************/

void TaskLoaderText::recoverUnsavedChanges() {
	loadDeletedIndices();
	loadModifiedTasks();
}

void TaskLoaderText::loadDeletedIndices() {
	_logger->log("TaskLoaderText","entering loadDeletedIndices");

	//initialise file iterators
	QDir temp(QString::fromStdString(TASK_DIRECTORY));
	QStringList filters;
	filters << FILTER_FOR_DELETED_TASK_FILES.c_str();
	temp.setNameFilters(filters);
	QStringList taskFiles = temp.entryList();
	QStringList::iterator taskFilesIt= taskFiles.begin();

	string nextTaskIndexStr;
	string nextTaskFile;
	Task nextTask;

	//iterate through the files and amass deleted indices
	while(taskFilesIt != taskFiles.end()) {
		std::string nextTaskFile = temp.filePath(*taskFilesIt).toUtf8().constData();
		_logger->log("TaskLoaderText","found deleted task file: "+nextTaskFile,NOTICELOG);

		openFile(nextTaskFile);
		nextTaskIndexStr = getNextLineFromFile();
		closeFile();

		recoveredDeletedIndices.insert(nextTaskIndexStr);
		_logger->log("TaskLoaderText","reading deleted tasks file: " + nextTaskIndexStr,NOTICELOG);

		taskFilesIt++;
		nextTaskIndexStr = "";
	}	
}

void TaskLoaderText::loadModifiedTasks() {
	_logger->log("TaskLoaderText","entering loadModifiedTasks");

	std::string nextTaskFile;
	Task nextTask;

	//initialise file iterators
	QDir temp(QString::fromStdString(TASK_DIRECTORY));
	QStringList filters;
	filters << FILTER_FOR_TASK_FILES.c_str();
	temp.setNameFilters(filters);
	QStringList taskFiles = temp.entryList();
	QStringList::iterator taskFilesIt= taskFiles.begin();

	//iterate through files and add Tasks
	while(taskFilesIt != taskFiles.end()) {
		nextTaskFile = temp.filePath(*taskFilesIt).toUtf8().constData();
		_logger->log("TaskLoaderText","found task file: "+nextTaskFile,NOTICELOG);

		openFile(nextTaskFile);

		nextTask = getNextTask();
		validateAndAddTaskToList(nextTask);

		closeFile();

		taskFilesIt++;
	}
	return;
}

/****************************************************/
/************** Private Load Method *****************/
/****************************************************/

void TaskLoaderText::loadTaskDS() {
	_logger->log("TaskLoaderText","entering loadTaskList");

	while(_fileReader.good() && hasNextTask()) {
		Task nextTask = getNextTask();
		validateAndAddTaskToList(nextTask);
		getNextLineFromFile();
	}
	return;
}

void TaskLoaderText::validateAndAddTaskToList(const Task& nextTask) {
	string taskFilePath = getTaskFilePath(nextTask);
	if(nextTask.getFlagIndex()) {
		_taskDS->addTask(nextTask);
		_logger->log("TaskLoaderText","created proper task from "+taskFilePath,NOTICELOG);
	}
	else {
		_logger->log("TaskLoaderText","flagIndex false for task with fileName "+taskFilePath,NOTICELOG);
	}
}

/*****************************************************/
/************** Task Attribute Loaders ***************/
/*****************************************************/

Task TaskLoaderText::getNextTask() {
	_logger->log("TaskLoaderText","entering getNextTask");
	string newLine;
	string newLabel;
	string newData;
	Task newTask;

	while(_fileReader.good()) {

		newLine		= getNextLineFromFile();
		newLabel	= getNewLabel(newLine);
		newData		= getNewData(newLine);

		if(newLabel == LABEL_INDEX) {
			if(!validateAndCreateTask(newTask, newData)) {
				break;
			}
		}
		else if(newLabel == LABEL_NAME) {
			setTaskName(newTask, newData);
		}
		else if(newLabel == LABEL_DUE_DATE)	{
			setTaskDueDate(newTask,newData);
		}
		else if(newLabel == LABEL_FROM_DATE) {
			setTaskFromDate(newTask,newData);
		}
		else if(newLabel == LABEL_TO_DATE) {
			setTaskToDate(newTask,newData);
		}
		else if(newLabel == LABEL_LOCATION)	{
			setTaskLocation(newTask,newData);
		}
		else if(newLabel == LABEL_PARTICIPANT) {
			setTaskParticipant(newTask,newData);
		}
		else if(newLabel == LABEL_NOTE) {
			setTaskNote(newTask,newData);
		}
		else if(newLabel == LABEL_PRIORITY)	{
			setTaskPriority(newTask,newData);
		}
		else if(newLabel == LABEL_TAG) {
			setTaskTag(newTask,newData);
		}
		else if(newLabel == LABEL_REMINDER_TIME) {
			setTaskReminderTime(newTask,newData);
		}
		else if(newLabel == LABEL_STATE) {
			setTaskState(newTask,newData);
		}
		else if(newLabel == LABEL_END_OF_TASK) {
			break;
		}
	}

	return newTask;
}

bool TaskLoaderText::validateAndCreateTask (Task& newTask, const std::string& newData) {
	bool taskHasBeenDeleted = recoveredDeletedIndices.find(newData) != recoveredDeletedIndices.end();

	if(taskHasBeenDeleted) {
		_logger->log("TaskLoaderText","deleted task found: " + newData, NOTICELOG);
		skipThisTask();
		newTask = Task();
		return false;
	}
	else {
		try {
			unsigned long long newIndex = getTaskIndex(newData);
			newTask = createNewTask(newIndex);
			_logger->log("TaskLoaderText","created new task with index: " + newData);
			return true;
		}
		catch (string e) {
			_logger->log("TaskLoaderText","recovered task duplicate in masterFile: " + newData, NOTICELOG);
			newTask = Task();
			return false;
		}
	}
}

Task TaskLoaderText::createNewTask(unsigned long long index) {
	return Task(index);
}

void TaskLoaderText::setTaskName	(Task& task,string taskName) {
	task.setName(taskName);
}

void TaskLoaderText::setTaskDueDate	(Task& task, string dueDateStr) {
	time_t dueDate = getTimeFromString(dueDateStr);
	task.setDueDate(dueDate);
}

void TaskLoaderText::setTaskFromDate(Task& task, string fromDateStr) {
	time_t fromDate = getTimeFromString(fromDateStr);
	task.setFromDate(fromDate);
}

void TaskLoaderText::setTaskToDate(Task& task, string toDateStr) {
	time_t toDate = getTimeFromString(toDateStr);
	task.setToDate(toDate);
}

void TaskLoaderText::setTaskLocation(Task& task, string location) {
	task.setLocation(location);
}

void TaskLoaderText::setTaskParticipant(Task& task, string participant) {
	task.setParticipants(participant,ADD_ELEMENT);
}

void TaskLoaderText::setTaskNote(Task& task, string note) {
	task.setNote(note);
}

void TaskLoaderText::setTaskPriority(Task& task, string taskPriorityStr) {
	PRIORITY taskPriority = getPriorityFromString(taskPriorityStr);
	task.setPriority(taskPriority);
}

void TaskLoaderText::setTaskTag(Task& task, string tag) {
	task.setTags(tag,ADD_ELEMENT);
}

void TaskLoaderText::setTaskReminderTime(Task& task, string reminderTimeStr) {
	time_t reminderTime = getTimeFromString(reminderTimeStr);
	task.setRemindTimes(reminderTime,ADD_ELEMENT);
}

void TaskLoaderText::setTaskState(Task& task, string taskStateStr) {
	TASK_STATE taskState = getTaskStateFromString(taskStateStr);
	task.setState(taskState);
}

/****************************************************/
/****** Helper Functions to Naviagate the File ******/
/****************************************************/

void TaskLoaderText::skipThisTask() {
	bool hasEnded = false;

	while(_fileReader.good()) {
		hasEnded = (getNextLineFromFile() == LABEL_END_OF_TASK);

		if(hasEnded) {
			break;
		}
	}
}

bool TaskLoaderText::hasNextTask() {
	bool hasNextTask = false;

	while(_fileReader.good()) {
		hasNextTask = (getNextLineFromFile() == LABEL_START_OF_TASK);

		if(hasNextTask) {
			break;
		}
	}

	return hasNextTask;
}

string TaskLoaderText::getNewLabel(string newLine) {
	string label;
	stringstream tempStream(newLine);

	tempStream >> label;

	return label;
}

string TaskLoaderText::getNewData(string newLine) {
	int pos = newLine.find_first_of(DEFAULT_KEY_VALUE_DELIMITER);
	if(pos != string::npos) {
		return newLine.substr(pos+1);
	}
	else {
		return DEFAULT_DATA_VALUE;
	}
}

/****************************************************/
/************** From String Converters **************/
/****************************************************/

int	TaskLoaderText::getIntFromString (string attribute) {
	stringstream tempStream(attribute);
	int returnValue;

	tempStream >> returnValue;

	return returnValue;
}

unsigned long long TaskLoaderText::getTaskIndex(string indexStr) {
	return getIndexFromString(indexStr);
}

unsigned long long TaskLoaderText::getIndexFromString (string attribute) {
	stringstream tempStream(attribute);
	unsigned long long returnValue;

	tempStream >> returnValue;

	return returnValue;
}

time_t TaskLoaderText::getTimeFromString (string attribute) {
	stringstream tempStream(attribute);
	time_t returnValue;

	tempStream >> returnValue;

	return returnValue;
}

PRIORITY TaskLoaderText::getPriorityFromString	(string attribute) {
	PRIORITY returnValue;
	for (PRIORITY prio = HIGH; prio < PRIORITY_COUNT; prio = static_cast<PRIORITY>(prio + 1)) {
		if(attribute == PRIORITY_STRING[prio]) {
			returnValue = prio;
			break;
		}
	}
	return returnValue;
}

TASK_STATE TaskLoaderText::getTaskStateFromString	(string attribute) {
	TASK_STATE returnValue;
	for (TASK_STATE state = UNDONE; state < TASK_STATE_COUNT; state = static_cast<TASK_STATE>(state + 1)) {
		if(attribute == TASK_STATE_STRING[state]) {
			returnValue = state;
			break;
		}
	}

	return returnValue;
}

/***************************************************/
/***************** Actual Reader *******************/
/***************************************************/

string TaskLoaderText::getNextLineFromFile() {
	string nextLine;
	getline(_fileReader,nextLine);

	return nextLine;
}
#endif