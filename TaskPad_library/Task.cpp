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
	time(&_taskCreatedTime);
	
	_taskName = "";
	_taskDueDate = _taskCreatedTime;
	_taskFromDate = _taskCreatedTime;
	_taskToDate = _taskCreatedTime;
	_taskLocation = "";
	_taskParticipants = "";
	_taskNote = "";
	_taskPriority = MEDIUM;
	_taskTags = "";
	_taskRemindTime = _taskCreatedTime;
	_taskState = UNDONE;
	_taskIndex = 0;
}
