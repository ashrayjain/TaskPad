/*
 * =====================================================================================
 *
 *       Filename:  Executor.cpp
 *
 *        Version:  1.0
 *        Created:  09/25/13 00:22:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Executor.h"

using namespace TP;

const unsigned		Executor::EMPTY_LIST_SIZE			= 0;
const unsigned		Executor::SINGLE_RESULT_LIST_SIZE	= 1;
const std::string	Executor::NAME_NOT_FOUND_ERROR		= "No results for name: ";
const std::string	Executor::INVALID_INDEX_ERROR		= " is not a valid index!";

void Executor::rebuildHashes() {
	rebuildIndexHash();
	rebuildHashTagsHash();
}

void Executor::rebuildIndexHash() {
	for (std::list<Task>::iterator i = _data->begin(); i != _data->end(); i++)
		_indexHash[i->getIndex()] = &(*i);
}

void Executor::rebuildHashTagsHash() {
	for (std::list<Task>::iterator i = _data->begin(); i != _data->end(); i++)
		if(i->getFlagTags())
			handleHashTagPtrs(*i, i->getTags());
}

void Executor::executeCommand(Command* cmd, Messenger &response) {
	switch (cmd->getCommandType()) {
	case COMMAND_TYPE::ADD:
		executeAdd (dynamic_cast<Command_Add*>(cmd), response);
		break;
	case COMMAND_TYPE::DEL:
		executeDel (dynamic_cast<Command_Del*>(cmd), response);
		break;
	case COMMAND_TYPE::MOD:
		executeMod (dynamic_cast<Command_Mod*>(cmd), response);
		break;
	case COMMAND_TYPE::FIND:
		executeFind(dynamic_cast<Command_Find*>(cmd), response);
		break;
	default:
		break;
	}
}

// Add functions

void Executor::executeAdd (Command_Add* cmd, Messenger &response) {
	Task newTask = Task();
	formTaskFromAddCmd(cmd, newTask);
	_data->push_back(newTask);
	_indexHash[newTask.getIndex()] = &(_data->back());
	handleHashTagPtrs(_data->back(), _data->back().getTags());
	setOpSuccessTask(newTask, response);
}

void Executor::formTaskFromAddCmd(Command_Add* cmd, Task &newTask) {
	if(cmd->getFlagName())
		newTask.setName(cmd->getName());
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
	if(cmd->getFlagDue())
		newTask.setDueDate(cmd->getDueDate());
	if(cmd->getFlagFrom())
		newTask.setFromDate(cmd->getFromDate());
	if(cmd->getFlagTo())
		newTask.setToDate(cmd->getToDate());
	if(cmd->getFlagTags())
		newTask.setTags(cmd->getTags());
}

void Executor::handleHashTagPtrs(Task &newTask, list<string> &hashTagsList) {
	list<list<Task*>::iterator> newHashTagPtrs;
	for (list<string>::iterator i = hashTagsList.begin(); i != hashTagsList.end(); i++) {
		std::unordered_map<std::string, list<Task*>>::iterator foundHashTag = _hashTagsHash.find(*i);
		if (foundHashTag != _hashTagsHash.end())
			handleExistingHashTag(newHashTagPtrs, newTask, foundHashTag->second);
		else
			handleNewHashTag(newHashTagPtrs, newTask, i);
	}
	newTask.setHashTagPtrs(newHashTagPtrs);
}

void Executor::handleExistingHashTag(list<list<Task*>::iterator> &newHashTagPtrs, Task &newTask, list<Task*> &hashTag) {
	hashTag.push_back(&newTask);
	newHashTagPtrs.push_back(--hashTag.end());
}

void Executor::handleNewHashTag(list<list<Task*>::iterator> &newHashTagPtrs, Task &newTask, list<string>::iterator &hashTag) {
	_hashTagsHash[*hashTag] = list<Task*>(1, &newTask);
	newHashTagPtrs.push_back(--(_hashTagsHash[*hashTag].end()));
}

// Delete Functions

void Executor::executeDel (Command_Del* cmd, Messenger &response) {
	if (cmd->getFlagCreatedTime())
		deleteTaskByIndex(cmd->getCreatedTime(), response);
	else
		deleteTaskByName(cmd->getName(), response, cmd->getFlagExact());
}

void Executor::deleteTaskByIndex(const unsigned long long &index, Messenger &response) {
	bool indexFound = false;
	for(list<Task>::iterator i = _data->begin(); i != _data->end() && !indexFound; ++i)
		if (i->getIndex() == index) {
			setOpSuccessTask(*i, response);
			deleteTask(i);
			indexFound = true;
			break;
		}

	if (!indexFound)
		setIndexNotFound(index, response);
}

void Executor::deleteTaskByName(const string &name, Messenger &response, const bool &exactFlag) {
	if (exactFlag)
		deleteByExactName(name, response);
	else
		deleteByApproxName(name, response);
}

void Executor::deleteByExactName(const string &name, Messenger &response) {
	bool nameFound = false;
	for(list<Task>::iterator i = _data->begin(); i != _data->end() && !nameFound; ++i)
		if (i->getName() == name) {
			setOpSuccessTask(*i, response);
			deleteTask(i);			
			nameFound = true;
			break;
		}

	if (!nameFound)
		setNameNotFound(name, response);
}

void Executor::deleteByApproxName(const string &name, Messenger &response) {
	list<Task> matchingResults;
	for(list<Task>::iterator i = _data->begin(); i != _data->end(); ++i)
		if (i->getName().find(name) != string::npos)
			matchingResults.push_back(Task(*i));

	if (matchingResults.size() == EMPTY_LIST_SIZE)
		setNameNotFound(name, response);
	else if (matchingResults.size() == SINGLE_RESULT_LIST_SIZE)
		deleteTaskByIndex(matchingResults.front().getIndex(), response);
	else
		setOpIntermediateTaskList(matchingResults, response);
}

void Executor::deleteTask(list<Task>::iterator &i) {
	_indexHash.erase(i->getIndex());
	deleteHashTags(*i);
	_data->erase(i);
}

void Executor::deleteHashTags(Task &task) {
	list<string> tags = task.getTags();
	list<list<Task*>::iterator> tagPtrs = task.getHashTagPtrs();
	list<string>::iterator k = tags.begin();
	for (list<list<Task*>::iterator>::iterator j = tagPtrs.begin(); j != tagPtrs.end(); j++, k++)
		_hashTagsHash[*k].erase(*j);
}

// Modify Functions

void Executor::executeMod (Command_Mod* cmd, Messenger &response) {
	if(cmd->getFlagCreatedTime())
		modifyByIndex(cmd, response);
	else
		modifyByName(cmd, response);
}

void Executor::modifyByIndex(Command_Mod* cmd, Messenger &response) {
	unordered_map<unsigned long long, Task*>::iterator result = _indexHash.find(cmd->getCreatedTime());
	if (result != _indexHash.end()) {
		modifyTaskTo(*(result->second), cmd);
		setOpSuccessTask(*(result->second), response);
	}
	else
		setIndexNotFound(cmd->getCreatedTime(), response);
}

void Executor::modifyByName(Command_Mod* cmd, Messenger &response) {
	if (cmd->getFlagExact())
		modifyByExactName(cmd, response);
	else
		modifyByApproxName(cmd, response);
}

void Executor::modifyByExactName(Command_Mod* cmd, Messenger &response) {
	bool nameFound = false;
	for(list<Task>::iterator i = _data->begin(); i != _data->end() && !nameFound; ++i)
		if (i->getName() == cmd->getName()) {
			modifyTaskTo(*i, cmd);
			setOpSuccessTask(Task(*i), response);
			nameFound = true;
		}

	if (!nameFound)
		setNameNotFound(cmd->getName(), response);
}

void Executor::modifyByApproxName(Command_Mod* cmd, Messenger &response) {
	list<Task> matchingResults;
	for(list<Task>::iterator i = _data->begin(); i != _data->end(); ++i)
		if (i->getName().find(cmd->getName()) != string::npos)
			matchingResults.push_back(Task(*i));

	if (matchingResults.size() == EMPTY_LIST_SIZE)
		setNameNotFound(cmd->getName(), response);
	else if(matchingResults.size() == SINGLE_RESULT_LIST_SIZE){
		cmd->setCreatedTime(matchingResults.front().getIndex());
		modifyByIndex(cmd, response);
	}
	else
		setOpIntermediateTaskList(matchingResults, response);
}

void Executor::modifyTaskTo(Task &oldTask, Command_Mod* cmd) {
	if(cmd->getFlagOptName()) 
		oldTask.setName(cmd->getOptName());//need to set exception handler in case the new name is the name of an existing task.
	if(cmd->getFlagLocation())
		oldTask.setLocation(cmd->getLocation());
	if(cmd->getFlagNote())
		oldTask.setNote(cmd->getNote());
	if(cmd->getFlagRemindTimes())
		oldTask.setRemindTimes(cmd->getRemindTimes());
	if(cmd->getFlagParticipants())
		oldTask.setParticipants(cmd->getParticipants());
	if(cmd->getFlagTags())
		handleHashTagsModify(oldTask, cmd->getTags());
	if(cmd->getFlagPriority())
		oldTask.setPriority(cmd->getPriority());
	if(cmd->getFlagDue())
		oldTask.setDueDate(cmd->getDueDate());
	if(cmd->getFlagFrom())
		oldTask.setFromDate(cmd->getFromDate());
	if(cmd->getFlagTo())
		oldTask.setToDate(cmd->getToDate());
	if(cmd->getFlagTaskState())
		oldTask.setState(cmd->getTaskState());
}

void Executor::handleHashTagsModify(Task &oldTask, list<string> &newTags) {
	deleteHashTags(oldTask);
	oldTask.setTags(newTags);
	handleHashTagPtrs(oldTask, newTags);
}

// Find functions

void Executor::executeFind (Command_Find* cmd, Messenger &response) {
	if(cmd->getFlagIndex())
		findByIndex(cmd->getIndex(), response);
	else if(cmd->getFlagTags())
		findByTags(cmd, response);
	else
		findGeneral(cmd, response);
	if(cmd->getFlagTaskType())
		filterResponseListByType(response, list<TP::TASK_TYPE>(1, cmd->getTaskType()));
}

void Executor::formTaskFromFindCmd(Command_Find* cmd, Task &newTask) {
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

void Executor::findByIndex(const unsigned long long index, Messenger &response) {
	unordered_map<unsigned long long, Task*>::iterator result = _indexHash.find(index);
	if (result != _indexHash.end())
		setOpSuccessTask(*(result->second), response);
	else
		setIndexNotFound(index, response);
}

void Executor::findGeneral(Command_Find* cmd, Messenger &response) {
	Task taskToCompare;
	formTaskFromFindCmd(cmd, taskToCompare);
	list<Task> results;
	if (cmd->getOptName() != string())
		runSearchWithTask(taskToCompare, results, cmd->getOptName());
	else
		runSearchWithTask(taskToCompare, results);
	setOpSuccessTaskList(results, response);
}

void Executor::findByTags(Command_Find* cmd, Messenger &response) {
	Task taskToCompare;
	list<Task*> customDataRange;
	list<Task> results;
	formTaskFromFindCmd(cmd, taskToCompare);
	getCustomDataRangeByTags(customDataRange, cmd->getTags());
	runSearchWithTaskOnData(taskToCompare, results, customDataRange);
	setOpSuccessTaskList(results, response);
}

void Executor::getCustomDataRangeByTags(list<Task*> &customDataRange, list<string> &tags) {
	//tags.push_back("#TestHash");
	for(list<string>::iterator i = tags.begin(); i != tags.end(); ++i)
		customDataRange.insert(customDataRange.end(), _hashTagsHash[*i].begin(), _hashTagsHash[*i].end());
}

void Executor::filterResponseListByType(Messenger &response, list<TP::TASK_TYPE> &types) {
	list<Task> newResults, oldResults = response.getList();
	for (list<Task>::iterator i = oldResults.begin(); i != oldResults.end(); ++i)
		if (find(types.begin(), types.end(), i->getTaskType()) != types.end())
			newResults.push_back(*i);
	setOpSuccessTaskList(newResults, response);
}

void Executor::runSearchWithTask(const Task &taskToCompare, list<Task> &results) {
	for(list<Task>::iterator i = _data->begin(); i != _data->end(); ++i)
		if (taskMatch(*i, taskToCompare))
			results.push_back(Task(*i));
}

void Executor::runSearchWithTask(const Task &taskToCompare, list<Task> &results, string substringName) {
	for(list<Task>::iterator i = _data->begin(); i != _data->end(); ++i)
		if (i->getFlagName() && i->getName().find(substringName) != string::npos && taskMatch(*i, taskToCompare))
			results.push_back(Task(*i));
}

void Executor::runSearchWithTaskOnData(const Task &taskToCompare, list<Task> &results, list<Task*> &customData) {
	for(list<Task*>::iterator i = customData.begin(); i != customData.end(); ++i)
		if (taskMatch(**i, taskToCompare))
			results.push_back(Task(**i));
}

bool Executor::taskMatch(const Task& lhs, const Task& rhs) const {
	if (rhs.getFlagName() && (!lhs.getFlagName() || rhs.getName() != lhs.getName()))
		return false;
	else if (rhs.getFlagLocation() && (!lhs.getFlagLocation() || rhs.getLocation() != lhs.getLocation()))
		return false;
	else if (rhs.getFlagParticipants() && (!lhs.getFlagParticipants() || rhs.getParticipants() != lhs.getParticipants()))
		return false;
	else if (rhs.getFlagNote() && (!lhs.getFlagNote() || rhs.getNote() != lhs.getNote()))
		return false;
	else if (rhs.getFlagRemindTimes() && (!lhs.getFlagRemindTimes() || rhs.getRemindTimes() != lhs.getRemindTimes()))
		return false;
	else if (rhs.getFlagFromDate() && !chkFromDateBound(rhs.getFromDate(), lhs))
		return false;
	else if (rhs.getFlagToDate() && !chkToDateBound(rhs.getToDate(), lhs))
		return false;
	else if (rhs.getFlagPriority() && rhs.getPriority() != lhs.getPriority())
		return false;
	else if (rhs.getFlagState() && rhs.getState() != lhs.getState())
		return false;
	return true;
} 

bool Executor::chkFromDateBound(const time_t &fromTime, const Task &lhs) const {
	return (lhs.getFlagFromDate() && fromTime <= lhs.getFromDate()) || 
		(lhs.getFlagDueDate() && fromTime <= lhs.getDueDate());
}

bool Executor::chkToDateBound(const time_t &toTime, const Task &lhs) const {
	return (lhs.getFlagToDate() && toTime >= lhs.getToDate()) || 
		(lhs.getFlagDueDate() && toTime >= lhs.getDueDate());
}

// Status setting functions

void Executor::setOpSuccessTask(const Task &retTask, Messenger &response) {
	response.setStatus(TP::SUCCESS);
	response.setTask(Task(retTask));
}

void Executor::setOpSuccessTaskList(const list<Task>& results, Messenger &response) {
	response.setStatus(TP::SUCCESS);
	response.setList(results);
}

void Executor::setOpIntermediateTaskList(const list<Task>& results, Messenger &response) {
	response.setStatus(TP::INTERMEDIATE);
	response.setList(results);
}

void Executor::setIndexNotFound(const unsigned long long &index, Messenger &response) {
	response.setStatus(TP::ERROR);
	response.setErrorMsg(std::to_string(index) + INVALID_INDEX_ERROR);
}

void Executor::setNameNotFound(const string &name, Messenger &response) {
	response.setStatus(TP::ERROR);
	response.setErrorMsg(NAME_NOT_FOUND_ERROR + name);
}
