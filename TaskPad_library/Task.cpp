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
	flagName		= false;
	flagDueDate		= false;
	flagFromDate	= false;
	flagToDate		= false;
	flagLocation	= false;
	flagParticipant = false;
	flagNote		= false;
	flagPriority	= false;
	flagTags		= false;
	flagRemindTime	= false;
	flagState		= false;
}

void Task::initTaskAttributes()
{
	_taskName			= "";
	_taskDueDate		= 0;
	_taskFromDate		= 0;
	_taskToDate			= 0;
	_taskLocation		= "";
	_taskParticipants	= "";
	_taskNote			= "";
	_taskPriority		= TP::MEDIUM;
	_taskTags			= "";
	_taskRemindTime		= 0;
	_taskState			= TP::UNDONE;
}



