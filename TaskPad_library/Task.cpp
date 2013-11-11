/*
 * =====================================================================================
 *
 *       Filename:  Task.cpp
 *
 *        Version:  1.0
 *        Created:  09/23/13 17:32:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Task.h"

using namespace std;

//@ASHRAY JAIN A0105199B

set<unsigned long long>				Task::listOfAllIndices;

const string						Task::INDEX_INVALID_ERROR	= "is not a valid index!";

const unsigned long long			Task::DEFAULT_INDEX				= 0;
const string						Task::DEFAULT_NAME				= "";
const string						Task::DEFAULT_LOCATION			= "";
const string						Task::DEFAULT_NOTE				= "";
const TP::PRIORITY					Task::DEFAULT_PRIORITY			= TP::MEDIUM;
const list<string>					Task::DEFAULT_PARTICIPANTS		= list<string>();
const list<string>					Task::DEFAULT_TAGS				= list<string>();
const list<list<Task*>::iterator>	Task::DEFAULT_HASHTAG_PTRS		= list<list<Task*>::iterator>();
const list<time_t>					Task::DEFAULT_REMINDTIMES		= list<time_t>();
const list<list<Task*>::iterator>	Task::DEFAULT_REMINDTIMES_PTRS	= list<list<Task*>::iterator>();
const time_t						Task::DEFAULT_FROMDATE			= 0;
const time_t						Task::DEFAULT_TODATE			= 0;
const TP::TASK_STATE				Task::DEFAULT_STATE				= TP::UNDONE;	
const TP::TASK_TYPE					Task::DEFAULT_TYPE				= TP::FLOATING;

void Task::defaultTaskInit(bool createIndex)
{
	initFlags();
    initTaskAttributes();
    if (createIndex) {
		_taskIndex = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
		listOfAllIndices.insert(_taskIndex);
	}
}

void Task::defaultTaskInit(unsigned long long indexToPut)
{
	initFlags();
    initTaskAttributes();
    if (!isIndexInList(indexToPut))
    {
        _taskIndex = indexToPut;
        listOfAllIndices.insert(indexToPut);
    }
    else
        throw to_string(indexToPut) + INDEX_INVALID_ERROR;
}

void Task::initFlags()
{
    flagName		 = false;
    flagFromDate	 = false;
    flagToDate		 = false;
    flagLocation	 = false;
    flagParticipants = false;
    flagNote		 = false;
    flagPriority	 = false;
    flagTags		 = false;
    flagRemindTimes	 = false;
    flagState		 = false;
}

void Task::initTaskAttributes()
{
	_taskName			= DEFAULT_NAME;
	_taskLocation		= DEFAULT_LOCATION;
	_taskNote			= DEFAULT_NOTE;
	_taskPriority		= DEFAULT_PRIORITY;
    _taskParticipants	= DEFAULT_PARTICIPANTS;
    _taskTags			= DEFAULT_TAGS;
	_hashTagPtrs		= DEFAULT_HASHTAG_PTRS;
    _taskRemindTimes	= DEFAULT_REMINDTIMES;
    _remindTimesPtrs	= DEFAULT_REMINDTIMES_PTRS;
	_taskFromDate		= DEFAULT_FROMDATE;
    _taskToDate			= DEFAULT_TODATE;
    _taskState			= DEFAULT_STATE;
    _taskType			= DEFAULT_TYPE;
	_taskIndex			= DEFAULT_INDEX;
}

void Task::setRemindTimes(time_t newRemindTime, TP::LIST_OP op) {
    if (op == TP::ADD_ELEMENT)
        _taskRemindTimes.push_back(newRemindTime);
    else if (op == TP::REMOVE_ELEMENT)
		removeSingleRTOccurrence(newRemindTime);
	flagRemindTimes = _taskRemindTimes != DEFAULT_REMINDTIMES;
}

void Task::setParticipants(string newParticipant, TP::LIST_OP op) {
    if (op == TP::ADD_ELEMENT)
        _taskParticipants.push_back(newParticipant);
    else if (op == TP::REMOVE_ELEMENT)
		removeSingleParticipansOccurrence(newParticipant);
	flagParticipants = _taskParticipants != DEFAULT_PARTICIPANTS;
}

void Task::setTags(string newTag, TP::LIST_OP op) {
    if (op == TP::ADD_ELEMENT)
        _taskTags.push_back(newTag);
    else if (op == TP::REMOVE_ELEMENT)
		removeSingleTagOccurrence(newTag);
	flagTags = _taskTags !=DEFAULT_TAGS;
}

void Task::setFromDate(time_t newFromDate) {
    _taskFromDate = newFromDate;
	flagFromDate = newFromDate != DEFAULT_FROMDATE;
    handleDatesChange();
}

void Task::setToDate(time_t newToDate) {
    _taskToDate = newToDate;
	flagToDate = newToDate != DEFAULT_TODATE;
    handleDatesChange();
}

void Task::setDueDate(time_t newDueDate) {
	_taskFromDate = _taskToDate = newDueDate;
	flagFromDate = flagToDate = newDueDate != DEFAULT_FROMDATE;
	handleDatesChange();
}

void Task::handleDatesChange() {
    if (getFlagDueDate() == true)
		setTaskType(TP::TASK_TYPE::DEADLINE);
	else if (getFlagFromDate() || getFlagToDate())
        setTaskType(TP::TASK_TYPE::TIMED);
    else
        setTaskType(TP::TASK_TYPE::FLOATING);

	if (getFlagToDate() && getState() != TP::TASK_STATE::DONE) {
		if (time(NULL) > getToDate())
			_taskState = TP::TASK_STATE::OVERDUE;
		else if (time(NULL) <= getToDate())
			_taskState = TP::TASK_STATE::UNDONE;
	}
}

void Task::removeSingleRTOccurrence(time_t &toRemove) {
	for(list<time_t>::iterator i = _taskRemindTimes.begin(); i != _taskRemindTimes.end(); ++i)
		if (*i == toRemove) {
			_taskRemindTimes.erase(i);
			break;
		}
}

void Task::removeSingleParticipansOccurrence(string &toRemove) {
	for(list<string>::iterator i = _taskParticipants.begin(); i != _taskParticipants.end(); ++i)
		if (*i == toRemove) {
			_taskParticipants.erase(i);
			break;
		}
}

void Task::removeSingleTagOccurrence(string &toRemove) {
	for(list<string>::iterator i = _taskTags.begin(); i != _taskTags.end(); ++i)
		if (*i == toRemove) {
			_taskTags.erase(i);
			break;
		}
}




