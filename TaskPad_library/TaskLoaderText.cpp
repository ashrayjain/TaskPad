#ifndef _TASKLOADERTEXT_CPP_
#define _TASKLOADERTEXT_CPP_

#include<list>
#include <sstream>
#include "TaskLoaderText.h"
#include "Task.h"
#include "Enum.h"
#include "Logger.h"

using namespace TP;
using namespace std;

const std::string TaskLoaderText::TASK_DIRECTORY = "Tasks\\";
const std::string TaskLoaderText::RECORD_MODIFIED_FILE_NAME = "savedTasks.txt";
const std::string TaskLoaderText::RECORD_DELETED_FILE_NAME = "deletedTasks.txt";

void TaskLoaderText::load (list<Task>& taskList, const string& fileName)
{
	this->recoverUnsavedChanges(taskList);
	this->openFile(fileName);
	this->loadTaskList(taskList);
	this->closeFile();
	return;
}

void TaskLoaderText::openFile(const std::string& fileName,  std::ios_base::openmode)
{
	this->_fileReader.open(fileName);
	return;
}

void TaskLoaderText::closeFile()
{
	this->_fileReader.close();
	return;
}

void TaskLoaderText::recoverUnsavedChanges(list<Task>& taskList)
{
	this->loadDeletedIndices();
	this->loadModifiedTasks(taskList);
}

void TaskLoaderText::loadDeletedIndices()
{
	Logger::getLogger()->log("TaskLoaderText","entering loadDeletedIndices");
	ifstream record(RECORD_DELETED_FILE_NAME);
	std::string nextTaskIndex;

	while(record.good())
	{
		getline(record, nextTaskIndex);
		if(nextTaskIndex!= "")
		{
			this->recoveredIndices.insert(nextTaskIndex);
			Logger::getLogger()->log("TaskLoaderText","reading deleted tasks file: " + nextTaskIndex,NOTICELOG);
			nextTaskIndex = "";
		}
	}
	record.close();
}

void TaskLoaderText::loadModifiedTasks(list<Task>& taskList)
{
	Logger::getLogger()->log("TaskLoaderText","entering loadModifiedTasks");
	ifstream recoverFile(RECORD_MODIFIED_FILE_NAME);
	std::string nextTaskFile;
	Task nextTask;

	while(recoverFile.good())
	{
		getline(recoverFile,nextTaskFile);
		if(nextTaskFile != "")
		{
			this->openFile(nextTaskFile);

			nextTask = this->getNextTask();
			if(nextTask.getFlagIndex())
			{
				taskList.push_back(nextTask);
			}
			else
			{
				Logger::getLogger()->log("TaskLoaderText","flagIndex false for task with fileName "+nextTaskFile,NOTICELOG);
			}

			this->closeFile();
		}
	}

	return;
}


void TaskLoaderText::loadTaskList(list<Task>& taskList)
{
	Logger::getLogger()->log("TaskLoaderText","entering loadTaskList");
	while(_fileReader.good() && hasNextTask())
	{
		Task nextTask = this->getNextTask();

		if(nextTask.getFlagIndex())
		{
			Logger::getLogger()->log("TaskLoaderText","created proper task",NOTICELOG);
			taskList.push_back(nextTask);
		}
		this->getNextLineFromFile();
	}
	return;
}

Task TaskLoaderText::getNextTask()
{
	Logger::getLogger()->log("TaskLoaderText","entering getNextTask");
	string newLine;
	string newLabel;
	string newData;
	Task newTask;

	while(_fileReader.good()) {

		newLine = getNextLineFromFile();
		newLabel = getNewLabel(newLine);
		newData = getNewData(newLine);

		if(newLabel == LABEL_INDEX) {
			bool taskHasBeenDeleted = recoveredIndices.find(newData) != recoveredIndices.end();

			if(taskHasBeenDeleted) {
				Logger::getLogger()->log("TaskLoaderText","deleted task found: "+newData, NOTICELOG);
				this->skipThisTask();
				newTask = Task();
				break;
			}
			else {
				try {
					int newIndex = getTaskIndex(newData);
					newTask = createNewTask(newIndex);
					Logger::getLogger()->log("TaskLoaderText","created new task with index: "+newData);
				}
				catch (exception e) {
					Logger::getLogger()->log("TaskLoaderText","recovered task duplicate in masterFile: "+newData, NOTICELOG);
					newTask = Task();
					return newTask;
				}
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

void TaskLoaderText::skipThisTask()
{
	bool hasEnded = false;

	while(_fileReader.good())
	{
		hasEnded = (getNextLineFromFile() == LABEL_END_OF_TASK);

		if(hasEnded)
		{
			break;
		}
	}
}

bool TaskLoaderText::hasNextTask()
{
	bool hasNextTask = false;

	while(_fileReader.good())
	{
		hasNextTask = (getNextLineFromFile() == LABEL_START_OF_TASK);

		if(hasNextTask)
		{
			break;
		}
	}

	return hasNextTask;
}

string TaskLoaderText::getNewLabel(string newLine)
{
	string label;
	stringstream tempStream(newLine);

	tempStream >> label;

	return label;
}

string TaskLoaderText::getNewData(string newLine)
{
	int pos = newLine.find_first_of(" ");
	if(pos != string::npos) {
		return newLine.substr(pos+1);
	}
	else {
		return "";
	}
}

unsigned long long TaskLoaderText::getTaskIndex(string indexStr)
{
	return getIndexFromString(indexStr);
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

string TaskLoaderText::getNextLineFromFile()
{
	string nextLine;
	getline(_fileReader,nextLine);

	return nextLine;
}

// From String converters

int	TaskLoaderText::getIntFromString (string attribute) {
	stringstream tempStream(attribute);
	int returnValue;

	tempStream >> returnValue;

	return returnValue;
}
unsigned long long TaskLoaderText::getIndexFromString		(string attribute){
	stringstream tempStream(attribute);
	unsigned long long returnValue;

	tempStream >> returnValue;

	return returnValue;
}
time_t TaskLoaderText::getTimeFromString (string attribute){
	stringstream tempStream(attribute);
	time_t returnValue;

	tempStream >> returnValue;

	return returnValue;
}
PRIORITY TaskLoaderText::getPriorityFromString	(string attribute){
	PRIORITY returnValue;
	for (PRIORITY prio = HIGH; prio <= LOW; prio = static_cast<PRIORITY>(prio + 1))
	{
		if(attribute == PRIORITY_STRING[prio])
		{
			returnValue = prio;
			break;
		}
	}
	return returnValue;
}
TASK_STATE TaskLoaderText::getTaskStateFromString	(string attribute) {
	TASK_STATE returnValue;
	for (TASK_STATE state = UNDONE; state <= DONE; state = static_cast<TASK_STATE>(state + 1))
	{
		if(attribute == TASK_STATE_STRING[state])
		{
			returnValue = state;
			break;
		}
	}

	return returnValue;
}


#endif