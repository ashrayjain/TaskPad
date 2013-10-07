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

void Executor::executeAdd (Command_Add* cmd, Messenger &response) {
	Task newTask = Task();
	formTaskFromAddCmd(cmd, newTask);
	_data->push_back(newTask);
	_indexHash[newTask.getIndex()] = &newTask;
	setOpSuccessTask(newTask, response);
}

void Executor::executeDel (Command_Del* cmd, Messenger &response) {
	if (cmd->getFlagIndex())
		deleteTaskByIndex(cmd->getIndex(), response);
	else
		deleteTaskByName(cmd->getName(), response, cmd->getFlagExact());
}

void Executor::executeMod (Command_Mod* cmd, Messenger &response) {

}

void Executor::executeFind (Command_Find* cmd, Messenger &response) {
	if(cmd->getFlagIndex())
		findByIndex(cmd->getIndex(), response);
	else 
		findByName(cmd, response);
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
	if(cmd->getFlagTags())
		newTask.setTags(cmd->getTags());
	if(cmd->getFlagPriority())
		newTask.setPriority(cmd->getPriority());
	if(cmd->getFlagDue())
		newTask.setDueDate(cmd->getDueDate());
	if(cmd->getFlagFrom())
		newTask.setFromDate(cmd->getFromDate());
	if(cmd->getFlagTo())
		newTask.setToDate(cmd->getToDate());
}

void Executor::deleteTaskByIndex(const unsigned &index, Messenger &response) {
	bool indexFound = false;
	for(list<Task>::iterator i = _data->begin(); i != _data->end() && !indexFound; ++i)
		if (i->getIndex() == index) {
			response.setList(list<Task>(1, *i));
			_data->erase(i);
			indexFound = true;
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
			setOpSuccessTask(Task(*i), response);
			_data->erase(i);
			nameFound = true;
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
	else
		setOpIntermediateTaskList(matchingResults, response);

}

void Executor::formTaskFromFindCmd(Command_Find* cmd, Task &newTask) {
	if(cmd->getFlagExact() && cmd->getFlagName())
		newTask.setName(cmd->getOptName());
	if(cmd->getFlagLocation())
		newTask.setLocation(cmd->getLocation());
	if(cmd->getFlagNote())
		newTask.setNote(cmd->getNote());
	if(cmd->getFlagRemindTimes())
		newTask.setRemindTimes(cmd->getRemindTimes());
	if(cmd->getFlagParticipants())
		newTask.setParticipants(cmd->getParticipants());
	if(cmd->getFlagTags())
		newTask.setTags(cmd->getTags());
	if(cmd->getFlagPriority())
		newTask.setPriority(cmd->getPriority());
	if(cmd->getFlagFrom())
		newTask.setFromDate(cmd->getFromDate());
	if(cmd->getFlagTo())
		newTask.setToDate(cmd->getToDate());
}

void Executor::findByIndex(const unsigned index, Messenger &response) {
	map<unsigned, Task*>::iterator result = _indexHash.find(index);
	if (result != _indexHash.end())
		setOpSuccessTask(*(result->second), response);
	else
		setIndexNotFound(index, response);
}

void Executor::findByName(Command_Find* cmd, Messenger &response) {
	Task taskToCompare;
	formTaskFromFindCmd(cmd, taskToCompare);
	list<Task> results;
	if (cmd->getFlagExact())
		runSearchWithTask(taskToCompare, results);
	else
		runSearchWithTask(taskToCompare, results, cmd->getOptName());
	setOpIntermediateTaskList(results, response);
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

bool Executor::taskMatch(const Task& lhs, const Task& rhs) const {
	if (rhs.getFlagName() && lhs.getFlagName() && rhs.getName() != lhs.getName())
		return false;
	else if (rhs.getFlagLocation() && lhs.getFlagLocation() && rhs.getLocation() != lhs.getLocation())
		return false;
	else if (rhs.getFlagParticipants() && lhs.getFlagParticipants() && rhs.getParticipants() != lhs.getParticipants())
		return false;
	else if (rhs.getFlagNote() && lhs.getFlagNote() && rhs.getNote() != lhs.getNote())
		return false;
	else if (rhs.getFlagRemindTimes() && lhs.getFlagRemindTimes() && rhs.getRemindTimes() != lhs.getRemindTimes())
		return false;
	else if (rhs.getFlagFromDate() && lhs.getFlagFromDate() && rhs.getFromDate() <= lhs.getFromDate())
		return false;
	else if (rhs.getFlagToDate() && lhs.getFlagToDate() && rhs.getToDate() >= lhs.getToDate())
		return false;
	else if (rhs.getFlagDueDate() && lhs.getFlagDueDate() && rhs.getDueDate() != lhs.getDueDate())
		return false;
	else if (rhs.getPriority() != lhs.getPriority())
		return false;
	else if (rhs.getState() != lhs.getState())
		return false;
	return true;
} 

void Executor::setOpSuccessTask(const Task &retTask, Messenger &response) {
	response.setStatus(TP::SUCCESS);
	response.setList(list<Task>(1, retTask));
}

void Executor::setOpIntermediateTaskList(const list<Task>& results, Messenger &response) {
	response.setStatus(TP::INTERMEDIATE);
	response.setList(results);
}

void Executor::setIndexNotFound(const unsigned &index, Messenger &response) {
	response.setStatus(TP::ERROR);
	response.setErrorMsg(std::to_string(index) + INDEX_INVALID_ERROR);
}

void Executor::setNameNotFound(const string &name, Messenger &response) {
	response.setStatus(TP::ERROR);
	response.setErrorMsg(NAME_NOT_FOUND_ERROR + name);
}

