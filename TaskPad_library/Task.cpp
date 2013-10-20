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

std::set<unsigned long long>				Task::listOfAllIndices;

const std::string							Task::INDEX_INVALID_ERROR	= "is not a valid index!";

const unsigned long long					Task::DEFAULT_INDEX			= 0;
const std::string							Task::DEFAULT_NAME			= "";
const std::string							Task::DEFAULT_LOCATION		= "";
const std::string							Task::DEFAULT_NOTE			= "";
const TP::PRIORITY							Task::DEFAULT_PRIORITY		= TP::MEDIUM;
const std::list<std::string>				Task::DEFAULT_PARTICIPANTS	= std::list<std::string>();
const std::list<std::string>				Task::DEFAULT_TAGS			= std::list<std::string>();
const std::list<std::list<Task*>::iterator>	Task::DEFAULT_HASHTAG_PTRS	= std::list<std::list<Task*>::iterator>();
const std::list<std::time_t>				Task::DEFAULT_REMINDTIMES	= std::list<std::time_t>();
const std::time_t							Task::DEFAULT_FROMDATE		= 0;
const std::time_t							Task::DEFAULT_TODATE		= 0;
const TP::TASK_STATE						Task::DEFAULT_STATE			= TP::UNDONE;	
const TP::TASK_TYPE							Task::DEFAULT_TYPE			= TP::FLOATING;

void Task::defaultTaskInit(bool createIndex)
{
	initFlags();
    initTaskAttributes();
    if (createIndex) {
		_taskIndex = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		listOfAllIndices.insert(_taskIndex);
	}
}

void Task::defaultTaskInit(unsigned long long indexToPut)
{
	initFlags();
    initTaskAttributes();
    if (isIndexInList(indexToPut))
    {
        _taskIndex = indexToPut;
        listOfAllIndices.insert(indexToPut);
    }
    else
        throw std::to_string(indexToPut) + INDEX_INVALID_ERROR;
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
    _taskFromDate		= DEFAULT_FROMDATE;
    _taskToDate			= DEFAULT_TODATE;
    _taskState			= DEFAULT_STATE;
    _taskType			= DEFAULT_TYPE;
	_taskIndex			= DEFAULT_INDEX;
}

void Task::setRemindTimes(std::time_t newRemindTime, TP::LIST_OP op) {
    if (op == TP::ADD_ELEMENT)
        _taskRemindTimes.push_back(newRemindTime);
    else if (op == TP::REMOVE_ELEMENT)
        _taskRemindTimes.remove(newRemindTime);
	flagRemindTimes = _taskRemindTimes != DEFAULT_REMINDTIMES;
}

void Task::setParticipants(std::string newParticipant, TP::LIST_OP op) {
    if (op == TP::ADD_ELEMENT)
        _taskParticipants.push_back(newParticipant);
    else if (op == TP::REMOVE_ELEMENT)
        _taskParticipants.remove(newParticipant);
	flagParticipants = _taskParticipants != DEFAULT_PARTICIPANTS;
}

void Task::setTags(std::string newTag, TP::LIST_OP op) {
    if (op == TP::ADD_ELEMENT)
        _taskTags.push_back(newTag);
    else if (op == TP::REMOVE_ELEMENT)
        _taskTags.remove(newTag);
	flagTags = _taskTags !=DEFAULT_TAGS;
}

void Task::setFromDate(std::time_t newFromDate) {
    _taskFromDate = newFromDate;
	flagFromDate = newFromDate != DEFAULT_FROMDATE;
    handleDatesChange();
}

void Task::setToDate(std::time_t newToDate) {
    _taskToDate = newToDate;
	flagToDate = newToDate != DEFAULT_TODATE;
    handleDatesChange();
}

void Task::setDueDate(std::time_t newDueDate) {
	_taskFromDate = _taskToDate = newDueDate;
	flagFromDate = flagToDate = newDueDate != DEFAULT_FROMDATE;
	handleDatesChange();
}

void Task::handleDatesChange() {
    if (getFlagDueDate() == true)
        setTaskType(TP::DEADLINE);
	else if (getFlagFromDate() || getFlagToDate())
        setTaskType(TP::TIMED);
    else
        setTaskType(TP::FLOATING);
}





