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


void Task::defaultTaskInit() {

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

	_taskName			= "";
	_taskDueDate		= 0;
	_taskFromDate		= 0;
	_taskToDate			= 0;
	_taskLocation		= "";
	_taskParticipants	= "";
	_taskNote			= "";
	_taskPriority		= MEDIUM;
	_taskTags			= "";
	_taskRemindTime		= 0;
	_taskState			= UNDONE;
	_taskIndex			= int(time(NULL));
}
