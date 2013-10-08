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

std::set<unsigned> Task::listOfAllIndices;

void Task::defaultTaskInit()
{
    _taskIndex = int(time(NULL));
    listOfAllIndices.insert(_taskIndex);
    initFlags();
    initTaskAttributes();
}

void Task::defaultTaskInit(unsigned indexToPut)
{
    if (isIndexInList(indexToPut))
    {
        _taskIndex = indexToPut;
        listOfAllIndices.insert(indexToPut);
    }
    else
        throw std::to_string(indexToPut) + INDEX_INVALID_ERROR;
    
    initFlags();
    initTaskAttributes();
}

void Task::initFlags()
{
    flagName		 = false;
    flagDueDate		 = false;
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
    _taskName			= "";
    _taskLocation		= "";
    _taskNote			= "";
    _taskPriority		= TP::MEDIUM;
    _taskParticipants	= std::list<std::string>();
    _taskTags			= std::list<std::string>();
    _taskRemindTimes	= std::list<std::time_t>();
    _taskDueDate		= 0;
    _taskFromDate		= 0;
    _taskToDate			= 0;
    _taskState			= TP::UNDONE;
    _taskType			= TP::FLOATING;
}

void Task::setRemindTimes(std::time_t newRemindTime, TP::LIST_OP op) {
    if (op == TP::ADD_ELEMENT)
        _taskRemindTimes.push_back(newRemindTime);
    else if (op == TP::REMOVE_ELEMENT)
        _taskRemindTimes.remove(newRemindTime);
    flagRemindTimes = true;
}

void Task::setParticipants(std::string newParticipant, TP::LIST_OP op) {
    if (op == TP::ADD_ELEMENT)
        _taskParticipants.push_back(newParticipant);
    else if (op == TP::REMOVE_ELEMENT)
        _taskParticipants.remove(newParticipant);
    flagParticipants = true;
}

void Task::setTags(std::string newTag, TP::LIST_OP op) {
    if (op == TP::ADD_ELEMENT)
        _taskTags.push_back(newTag);
    else if (op == TP::REMOVE_ELEMENT)
        _taskTags.remove(newTag);
    flagTags = true;
}

void Task::setFromDate(std::time_t newFromDate) {
    _taskFromDate = newFromDate;
	flagFromDate = newFromDate!=0;
    handleDatesChange();
}

void Task::setToDate(std::time_t newToDate) {
    _taskToDate = newToDate;
    flagToDate = newToDate!=0;
    handleDatesChange();
}

void Task::setDueDate(std::time_t newDueDate) {	
    _taskDueDate = newDueDate;
	flagDueDate = newDueDate!=0;
    handleDatesChange();
}

void Task::handleDatesChange() {
    if (getFlagDueDate() == true)
        setTaskType(TP::DEADLINE);
    else if (getFlagFromDate() == true && getFlagToDate() == true)
        setTaskType(TP::TIMED);
    else
        setTaskType(TP::FLOATING);
}





