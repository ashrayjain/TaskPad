/*
 * =====================================================================================
 *
 *       Filename:  Executor_Add.cpp
 *
 *        Version:  1.0
 *        Created:  11/02/13 13:47:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Executor_Add.h"

const unsigned			Executor_Add::RT_MIN_H_ARR[]				= {5, 15, 30, 60};
const unsigned			Executor_Add::RT_MIN_M_ARR[]				= {15, 30};
const unsigned			Executor_Add::RT_MIN_L_ARR[]				= {60};

void Executor_Add::executeCommand(Command* cmd, Messenger &response, Datastore &ds) {
	Command_Add* addCmd = dynamic_cast<Command_Add*>(cmd);
	if(validAddCmd(addCmd, response)) {
		Task newTask = formTaskFromAddCmd(addCmd);
		ds.addTask(newTask);
		setOpSuccessTask(newTask, response);
	}
}

bool Executor_Add::validAddCmd(Command_Add* cmd, Messenger &response) {
	if (cmd->getFlagFrom() && cmd->getFlagTo() && cmd->getFromDate() > cmd->getToDate()) {
		setErrorWithErrMsg(response, INVALID_FROMDATE_TODATE_ERROR);
		return false;
	}
	if (!cmd->getFlagName()) {
		setErrorWithErrMsg(response, NAME_NOT_SPECIFIED_ERROR);
		return false;
	}
	return true;
}

Task Executor_Add::formTaskFromAddCmd(Command_Add* cmd) {
	Task newTask;
	if(cmd->getFlagCreatedTime()) {
		newTask = Task(cmd->getCreatedTime());
		newTask.setName(cmd->getName());
	}
	else {
		newTask = Task(cmd->getName());
		cmd->setCreatedTime(newTask.getIndex());
	}

	if(cmd->getFlagLocation())
		newTask.setLocation(cmd->getLocation());
	if(cmd->getFlagNote())
		newTask.setNote(cmd->getNote());
	if(cmd->getFlagParticipants())
		newTask.setParticipants(cmd->getParticipants());
	if(cmd->getFlagPriority())
		newTask.setPriority(cmd->getPriority());
	if(cmd->getFlagFrom())
		newTask.setFromDate(cmd->getFromDate());
	if(cmd->getFlagTo())
		newTask.setToDate(cmd->getToDate());
	if(cmd->getFlagDue())
		newTask.setDueDate(cmd->getDueDate());
	if(cmd->getFlagTags())
		newTask.setTags(cmd->getTags());

	if(cmd->getFlagRemindTimes())
		newTask.setRemindTimes(cmd->getRemindTimes());
	else if (newTask.getFlagToDate())
		setDefaultRemindTimes(newTask);

	return newTask;
}

void Executor_Add::setDefaultRemindTimes(Task &task) {
	switch(task.getPriority()) {
	case TP::PRIORITY::HIGH:	setDefaultRemindTimesPriorityH(task); break;
	case TP::PRIORITY::MEDIUM:	setDefaultRemindTimesPriorityM(task); break;
	case TP::PRIORITY::LOW:		setDefaultRemindTimesPriorityL(task); break;
	}
}

void Executor_Add::setDefaultRemindTimesPriorityH(Task &task) {
	int n = sizeof(RT_MIN_H_ARR) / sizeof(RT_MIN_H_ARR[0]);
	task.setRemindTimes(getRemindTimesFromMinutesBefore(RT_MIN_H_ARR, n, task.getToDate()));
}

void Executor_Add::setDefaultRemindTimesPriorityM(Task &task) {
	int n = sizeof(RT_MIN_M_ARR) / sizeof(RT_MIN_M_ARR[0]);
	task.setRemindTimes(getRemindTimesFromMinutesBefore(RT_MIN_M_ARR, n, task.getToDate()));
}

void Executor_Add::setDefaultRemindTimesPriorityL(Task &task) {
	int n = sizeof(RT_MIN_L_ARR) / sizeof(RT_MIN_L_ARR[0]);
	task.setRemindTimes(getRemindTimesFromMinutesBefore(RT_MIN_L_ARR, n, task.getToDate()));
}

list<time_t> Executor_Add::getRemindTimesFromMinutesBefore(const unsigned minutesBeforeList[], const int listSize, const time_t &deadline) const{
	list<time_t> remindTimesList;
	struct tm * deadlineTime = localtime(&deadline);
	for(int i = 0; i < listSize; i++) {
		deadlineTime->tm_min -= minutesBeforeList[i];
		remindTimesList.push_back(mktime(deadlineTime));
		deadlineTime->tm_min += minutesBeforeList[i];
		mktime(deadlineTime);
	}
	return remindTimesList;
}