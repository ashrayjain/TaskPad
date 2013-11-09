/*
 * =====================================================================================
 *
 *       Filename:  Executor_Find.cpp
 *
 *        Version:  1.0
 *        Created:  11/02/13 13:47:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Executor_Find.h"

void Executor_Find::executeCommand(Command* cmd, Messenger &response, Datastore &ds) {
	Command_Find* findCmd = dynamic_cast<Command_Find*>(cmd);
	if(findCmd->getFlagIndex())
		findByIndex(findCmd->getIndex(), response, ds);
	else
		findGeneral(findCmd, response, ds);
	if(findCmd->getFlagTaskType())
		filterResponseListByType(response, list<TP::TASK_TYPE>(1, findCmd->getTaskType()));
	if(findCmd->getFlagFrom())
		response.setList(getSortListByFromTime(response.getList()));
	response.setList(getSortListByPriority(response.getList()));
}


void Executor_Find::formTaskFromFindCmd(Command_Find* cmd, Task &newTask) {
	if(cmd->getFlagExact() && cmd->getFlagOptName())
		newTask.setName(cmd->getOptName());
	if(cmd->getFlagLocation())
		newTask.setLocation(cmd->getLocation());
	if(cmd->getFlagNote())
		newTask.setNote(cmd->getNote());
	if(cmd->getFlagRemindTimes())
		newTask.setRemindTimes(cmd->getRemindTimes());
	if(cmd->getFlagParticipants())
		newTask.setParticipants(cmd->getParticipants());
	if(cmd->getFlagPriority())
		newTask.setPriority(cmd->getPriority());
	if(cmd->getFlagFrom())
		newTask.setFromDate(cmd->getFromDate());
	if(cmd->getFlagTo())
		newTask.setToDate(cmd->getToDate());
	if(cmd->getFlagTaskState())
		newTask.setState(cmd->getTaskState());
}

void Executor_Find::findByIndex(const unsigned long long index, Messenger &response, Datastore &ds) {
	if (ds.isIndexPresent(index))
		setOpSuccessTask(ds.indexHashSearch(index), response);
	else
		setIndexNotFound(index, response);
}

void Executor_Find::findGeneral(Command_Find* cmd, Messenger &response, Datastore &ds) {
	Task taskToCompare;
	list<Task> results;
	set<Task> customDataRange;
	formTaskFromFindCmd(cmd, taskToCompare);
	if (cmd->getFlagTags())
		getCustomDataRangeByTags(customDataRange, cmd->getTags(), ds);
	if (cmd->getFlagRemindTimes())
		getCustomDataRangeByRT(customDataRange, cmd->getRemindTimes(), ds);

	runSearch(taskToCompare, results, getLowerStr(cmd->getOptName()), customDataRange, 
			  cmd->getFlagTags() || cmd->getFlagRemindTimes(), ds);		

	setOpSuccessTaskList(results, response);
}

void Executor_Find::runSearch(const Task &taskToCompare, list<Task> &results, string substringName, 
							  set<Task> &customData, bool customDataSet, Datastore &ds) {
	if (customDataSet) {
		if (substringName != string())
			runSearchWithTask(taskToCompare, results, substringName, customData);
		else
			runSearchWithTask(taskToCompare, results, customData);
	}
	else {
		if (substringName != string())
			runSearchWithTask(taskToCompare, results, substringName, ds);
		else
			runSearchWithTask(taskToCompare, results, ds);
	}
}

/*
void Executor_Find::findByTags(Command_Find* cmd, Messenger &response) {
	Task taskToCompare;
	list<Task*> customDataRange;
	list<Task> results;
	formTaskFromFindCmd(cmd, taskToCompare);
	getCustomDataRangeByTags(customDataRange, cmd->getTags());
	runSearchWithTaskOnData(taskToCompare, results, customDataRange);
	setOpSuccessTaskList(results, response);
}

void Executor_Find::findByRemindTimes(Command_Find* cmd, Messenger &response) {
	Task taskToCompare;
	list<Task*> customDataRange;
	list<Task> results;
	formTaskFromFindCmd(cmd, taskToCompare);
	getCustomDataRangeByTags(customDataRange, cmd->getTags());
	runSearchWithTaskOnData(taskToCompare, results, customDataRange);
	setOpSuccessTaskList(results, response);
}
*/
 
void Executor_Find::getCustomDataRangeByTags(set<Task> &customDataRange, list<string> &tags, Datastore &ds) {
	for(list<string>::iterator i = tags.begin(); i != tags.end(); ++i) {
		list<Task> results = ds.getTasksWithHash(*i);
		customDataRange.insert(results.begin(), results.end());
	}
}

void Executor_Find::getCustomDataRangeByRT(set<Task> &customDataRange, list<time_t> &remindTimes, Datastore &ds) {
	for(list<time_t>::iterator i = remindTimes.begin(); i != remindTimes.end(); ++i) {
		list<Task> results = ds.getTasksWithRemindTimes(*i);
		customDataRange.insert(results.begin(), results.end());
	}
}

void Executor_Find::filterResponseListByType(Messenger &response, list<TP::TASK_TYPE> &types) {
	list<Task> newResults, oldResults = response.getList();
	for (list<Task>::iterator i = oldResults.begin(); i != oldResults.end(); ++i)
		if (find(types.begin(), types.end(), i->getTaskType()) != types.end())
			newResults.push_back(*i);
	setOpSuccessTaskList(newResults, response);
}

void Executor_Find::runSearchWithTask(const Task &taskToCompare, list<Task> &results, Datastore &ds) {
	list<Task> tasks = ds.getTaskList();
	for(Datastore::const_iterator i = ds.cbegin(); i != ds.cend(); i++)
	//for(list<Task>::iterator i = tasks.begin(); i != tasks.end(); ++i)
		if (taskMatch(*i, taskToCompare))
			results.push_back(Task(*i));
}

void Executor_Find::runSearchWithTask(const Task &taskToCompare, list<Task> &results, 
									  string substringName, Datastore &ds) {
	list<Task> tasks = ds.getTaskList();
	for(Datastore::const_iterator i = ds.cbegin(); i != ds.cend(); i++)
	//for(list<Task>::iterator i =tasks.begin(); i != tasks.end(); ++i)
		if (getLowerStr(i->getName()).find(substringName) != string::npos && taskMatch(*i, taskToCompare))
			results.push_back(Task(*i));
}

void Executor_Find::runSearchWithTask(const Task &taskToCompare, list<Task> &results, set<Task> &customData) {
	for(set<Task>::iterator i = customData.begin(); i != customData.end(); ++i)
		if (taskMatch(*i, taskToCompare))
			results.push_back(Task(*i));
}

void Executor_Find::runSearchWithTask(const Task &taskToCompare, list<Task> &results, string substringName, set<Task> &customData) {
	for(set<Task>::iterator i = customData.begin(); i != customData.end(); ++i)
		if (getLowerStr(i->getName()).find(substringName) != string::npos && taskMatch(*i, taskToCompare))
			results.push_back(Task(*i));
}

bool Executor_Find::taskMatch(const Task& lhs, const Task& rhs) {
	if (rhs.getFlagName() && strcmpi(rhs.getName().c_str(), lhs.getName().c_str()))
		return false;
	else if (rhs.getFlagLocation() && (!lhs.getFlagLocation() || rhs.getLocation() != lhs.getLocation()))
		return false;
	else if (rhs.getFlagParticipants() && (!lhs.getFlagParticipants() || !participantsMatchFound(getLowerStrList(rhs.getParticipants()), getLowerStrList(lhs.getParticipants()))))
		return false;
	else if (rhs.getFlagNote() && (!lhs.getFlagNote() || rhs.getNote() != lhs.getNote()))
		return false;
	else if (invalidDateChk(lhs, rhs))
		return false;
	else if (rhs.getFlagPriority() && rhs.getPriority() != lhs.getPriority())
		return false;
	else if (rhs.getFlagState() && taskStatesNotEqual(lhs.getState(), rhs.getState()))
		return false;
	return true;
} 

bool Executor_Find::taskStatesNotEqual(const TP::TASK_STATE &lhs, const TP::TASK_STATE &rhs) const {
	return ((rhs==TP::TASK_STATE::UNDONE && lhs==TP::TASK_STATE::DONE)||(rhs!=TP::TASK_STATE::UNDONE && lhs!=rhs));
}

bool Executor_Find::participantsMatchFound(const list<string> &rhsParticipants, const list<string> &lhsParticipants) const {
	for(list<string>::const_iterator i = lhsParticipants.begin(); i != lhsParticipants.end(); i++)
		for(list<string>::const_iterator j = rhsParticipants.begin(); j != rhsParticipants.end(); j++)
			if(i->find(*j) != string::npos)
				return true;
	return false;
}

bool Executor_Find::invalidDateChk(const Task &lhs, const Task &rhs) const {
	bool retVal = true;
	if (!rhs.getFlagFromDate() && !rhs.getFlagToDate())
		retVal = false;
	else if (rhs.getFlagFromDate() && rhs.getFlagToDate()) {
		if (validDateBound(rhs.getFromDate(), rhs.getToDate(), lhs))
			retVal = false;
	}
	else if (rhs.getFlagFromDate()) {
		if (chkFromDateBound(rhs.getFromDate(), lhs))
			retVal = false;
	}
	else if (chkToDateBound(rhs.getToDate(), lhs))
		retVal = false;
	return retVal;
}

bool Executor_Find::validDateBound(const time_t &fromTime, const time_t &toTime, const Task &lhs) const {
	return !((!lhs.getFlagToDate() && !lhs.getFlagFromDate()) || 
		(lhs.getFlagToDate() && fromTime > lhs.getToDate()) || 
		(lhs.getFlagFromDate() && toTime < lhs.getFromDate()));
}

bool Executor_Find::chkFromDateBound(const time_t &fromTime, const Task &lhs) const {
	bool retVal = true;
	if (lhs.getFlagToDate()) {
		if (lhs.getToDate() < fromTime)
			retVal = false;
	}
	else if (!lhs.getFlagFromDate())
		retVal = false;
	return retVal;
}

bool Executor_Find::chkToDateBound(const time_t &toTime, const Task &lhs) const {
	bool retVal = true;
	if (lhs.getFlagFromDate()) {
		if (lhs.getFromDate() < toTime)
			retVal = false;
	}
	else if (!lhs.getFlagToDate())
		retVal = false;
	return retVal;
}

list<Task> Executor_Find::getSortListByPriority(list<Task> &taskList) {
	taskList.sort(sortTaskByPriorityComparator);
	return taskList;
}

list<Task> Executor_Find::getSortListByFromTime(list<Task> &taskList) {
	taskList.sort(sortTaskByFromTimeComparator);
	return taskList;
}

bool Executor_Find::sortTaskByPriorityComparator(const Task &first, const Task &second) {
	return (first.getPriority() < second.getPriority());
}

bool Executor_Find::sortTaskByFromTimeComparator(const Task &first, const Task &second) {
	return (first.getFromDate() < second.getFromDate());
}
