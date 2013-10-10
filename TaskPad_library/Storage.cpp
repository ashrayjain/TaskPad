#include "Storage.h"
#include "Task.h"
#include "Enum.h"
#include <list>
#include <sstream>
#include <string>

using namespace TP;

const string Storage::_fileName = "TaskPad.txt";
const string Storage::LABEL_TASK_COUNT = "Task Count:";
const string Storage::LABEL_NAME = "Name:";
const string Storage::LABEL_INDEX = "Index:";
const string Storage::LABEL_DUE_DATE = "Due:";
const string Storage::LABEL_FROM_DATE = "StartTime:";
const string Storage::LABEL_TO_DATE = "EndTime:";
const string Storage::LABEL_LOCATION = "Location:";
const string Storage::LABEL_PARTICIPANT = "Participant:";
const string Storage::LABEL_NOTE = "Note:";
const string Storage::LABEL_PRIORITY = "Priority:";
const string Storage::LABEL_TAG = "Tag:";
const string Storage::LABEL_REMINDER_TIME = "Reminder:";
const string Storage::LABEL_STATE = "State:";
const string Storage::LABEL_END_OF_TASK = "EndOfTask";

Storage::Storage(list<Task>& taskList)
{
	this->load(taskList);
}

bool Storage::save(const list<Task>& taskList)
{
	this->openTheFileToWrite(std::ios_base::trunc);
	this->saveTaskList(taskList);
	this->closeTheWrittenFile();
	return true;
}

bool Storage::save(const Task& task)
{
	this->openTheFileToWrite(std::ios_base::app);
	this->saveTask(task);
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

void Storage::saveTaskList(const list<Task>& taskList)
{
	/*
	this->saveTaskCount(taskList.size());
	writeLineToFile("");
	*/
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
	this->saveTaskAttributes(task);
	writeLineToFile(this->LABEL_END_OF_TASK,false);
	writeLineToFile("");
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

void Storage::saveLabel(string LabelStr)
{
	writeLineToFile(LabelStr,false);
}

void Storage::saveCount(unsigned long long count)
{
	std::string countStr = convertToString(count);
	this->writeLineToFile(countStr);
}

void Storage::saveTaskCount(unsigned long long taskCount)
{
	saveLabel(LABEL_TASK_COUNT);
	saveCount(taskCount);
}

void Storage::saveIndex(const Task& tempTask)
{
	saveLabel(LABEL_INDEX);
	this->writeLineToFile(convertToString(tempTask.getIndex()));
}

void Storage::saveName(const Task& tempTask)
{
	if(tempTask.getFlagName())
	{
		saveLabel(LABEL_NAME);
		this->writeLineToFile(tempTask.getName());
	}
}

void Storage::saveLocation(const Task& tempTask)
{
	if(tempTask.getFlagLocation())
	{
		saveLabel(LABEL_LOCATION);
		this->writeLineToFile(tempTask.getLocation());
	}
}

void Storage::saveParticipants(const Task& tempTask)
{
	if(tempTask.getFlagParticipants())
	{
		list<std::string> participantList = tempTask.getParticipants();
		list<std::string>::iterator pit = participantList.begin();
		string participant = "";

		while(pit != participantList.end())
		{
			saveLabel(LABEL_PARTICIPANT);
			participant = (*pit);//convertToString(tempTask.getParticipants());
			this->writeLineToFile(participant);
			pit++;
		}
	}
}

void Storage::saveNote(const Task& tempTask)
{
	if(tempTask.getFlagNote())
	{
		saveLabel(LABEL_NOTE);
		this->writeLineToFile(tempTask.getNote());
	}
}

void Storage::savePriority(const Task& tempTask)
{
	saveLabel(LABEL_PRIORITY);
	string priorityStr = convertToString(tempTask.getPriority());
	this->writeLineToFile(priorityStr);
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
			saveLabel(LABEL_TAG);
			tag = (*tagit);
			this->writeLineToFile(tag);
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
			saveLabel(LABEL_REMINDER_TIME);
			string reminderStr = convertToString(*rtit);
			this->writeLineToFile(reminderStr);
			rtit++;
		}
	}
}

void Storage::saveState(const Task& tempTask)
{
	saveLabel(LABEL_STATE);
	string stateStr = convertToString(tempTask.getState());
	this->writeLineToFile(stateStr);
}

void Storage::saveDueDate(const Task& tempTask)
{
	if(tempTask.getFlagDueDate())
	{
		saveLabel(LABEL_DUE_DATE);
		string dueDateStr = convertToString(tempTask.getDueDate());
		this->writeLineToFile(dueDateStr);
	}
}

void Storage::saveFromDate(const Task& tempTask)
{
	if(tempTask.getFlagFromDate())
	{
		saveLabel(LABEL_FROM_DATE);
		string fromDateStr = convertToString(tempTask.getFromDate());
		this->writeLineToFile(fromDateStr);
	}
}

void Storage::saveToDate(const Task& tempTask)
{
	if(tempTask.getFlagToDate())
	{
		saveLabel(LABEL_TO_DATE);
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
		_fileWriter << line << " ";
}

void Storage::emptyTheFile()
{
	//throw "storage empty the file not implemented";
}

void Storage::loadTaskList(list<Task>& taskList)
{
	while(_fileReader.good())
	{
		taskList.push_back(this->getNextTask());
		this->getNextLineFromFile();
	}
	return;
}

Task Storage::getNextTask()
{
	std::string newLine;
	std::string newLabel;
	std::string newData;
	Task newTask;

	while(_fileReader.good()) {
		newLine = getNextLineFromFile();
		newLabel = getNewLabel(newLine);
		newData = getNewData(newLine);

		if(newLabel == LABEL_END_OF_TASK) {
			break;
		}
		else if(newLabel == LABEL_INDEX) {
			newTask = createNewTask(getTaskIndex(newData));
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
	}

	return newTask;
}

std::string Storage::getNewLabel(std::string newLine)
{
	std::string label;
	stringstream tempStream(newLine);

	tempStream >> label;

	return label;
}

std::string Storage::getNewData(std::string newLine)
{
	int pos = newLine.find_first_of(" ");
	return newLine.substr(pos+1);
}

unsigned long long Storage::getTaskIndex(std::string indexStr)
{
	return getIndexFromString(indexStr);
}

Task Storage::createNewTask(unsigned long long index) {
	return Task(index);
}
void Storage::setTaskName	(Task& task,std::string taskName) {
	task.setName(taskName);
}
void Storage::setTaskDueDate	(Task& task, std::string dueDateStr) {
	time_t dueDate = getTimeFromString(dueDateStr);
	task.setDueDate(dueDate);
}
void Storage::setTaskFromDate(Task& task, std::string fromDateStr) {
	time_t fromDate = getTimeFromString(fromDateStr);
	task.setFromDate(fromDate);
}
void Storage::setTaskToDate(Task& task, std::string toDateStr) {
	time_t toDate = getTimeFromString(toDateStr);
	task.setToDate(toDate);
}
void Storage::setTaskLocation(Task& task, std::string location) {
	task.setLocation(location);
}
void Storage::setTaskParticipant(Task& task, std::string participant) {
	task.setParticipants(participant,ADD_ELEMENT);
}
void Storage::setTaskNote(Task& task, std::string note) {
	task.setNote(note);
}
void Storage::setTaskPriority(Task& task, std::string taskPriorityStr) {
	PRIORITY taskPriority = getPriorityFromString(taskPriorityStr);
	task.setPriority(taskPriority);
}
void Storage::setTaskTag(Task& task, std::string tag) {
	task.setTags(tag,ADD_ELEMENT);
}
void Storage::setTaskReminderTime(Task& task, std::string reminderTimeStr) {
	time_t reminderTime = getTimeFromString(reminderTimeStr);
	task.setRemindTimes(reminderTime,ADD_ELEMENT);
}
void Storage::setTaskState(Task& task, std::string taskStateStr) {
	TASK_STATE taskState = getTaskStateFromString(taskStateStr);
	task.setState(taskState);
}

std::string Storage::getNextLineFromFile()
{
	std::string nextLine;
	getline(_fileReader,nextLine);

	return nextLine;
}

void Storage::load (list<Task>& taskList)
{
	this->openTheFileToRead();
	this->getNextLineFromFile();
	this->loadTaskList(taskList);
	this->closeTheReadFile();
	this->save(taskList);
	return;
}


// From String converters

int	Storage::getIntFromString (std::string attribute) {
	stringstream tempStream(attribute);
	int returnValue;

	tempStream >> returnValue;

	return returnValue;
}
unsigned long long Storage::getIndexFromString		(std::string attribute){
	stringstream tempStream(attribute);
	unsigned long long returnValue;

	tempStream >> returnValue;

	return returnValue;
}
time_t Storage::getTimeFromString (std::string attribute){
	stringstream tempStream(attribute);
	time_t returnValue;

	tempStream >> returnValue;

	return returnValue;
}
PRIORITY Storage::getPriorityFromString	(std::string attribute){
	for (PRIORITY prio = HIGH; prio <= LOW; prio = static_cast<PRIORITY>(prio + 1))
	{
		if(attribute == PRIORITY_STRING[prio])
		{
			return prio;
		}
	}
}
TASK_STATE Storage::getTaskStateFromString	(std::string attribute) {
	for (TASK_STATE state = UNDONE; state <= DONE; state = static_cast<TASK_STATE>(state + 1))
	{
		if(attribute == TASK_STATE_STRING[state])
		{
			return state;
		}
	}
}

Storage::~Storage()
{
	/*
	this->_fileReader.close();
	this->_fileWriter.close();
	*/
}