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
	if (isAddDeadlineTaskCommand(cmd))
		executeAddDeadlineTask(cmd, response);
	else if (isAddTimedTaskCommand(cmd))
		executeAddTimedTask(cmd, response);
	else
		executeAddFloatingTask(cmd, response);
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

}

void Executor::executeAddDeadlineTask(Command_Add* cmd, Messenger &response) {
	DeadlineTask* newTask = new DeadlineTask();
	formDeadlineTaskFromCmd(cmd, newTask);
	_data->push_back(*newTask);
	response.setList(list<Task>(1, *newTask));
}

void Executor::executeAddTimedTask(Command_Add* cmd, Messenger &response) {
	TimedTask* newTask = new TimedTask();
	formTimedTaskFromCmd(cmd, newTask);
	_data->push_back(*newTask);
	response.setList(list<Task>(1, *newTask));
}

void Executor::executeAddFloatingTask(Command_Add* cmd, Messenger &response) {
	FloatingTask* newTask = new FloatingTask();
	formFloatingTaskFromCmd(cmd, newTask);
	_data->push_back(*newTask);
	response.setList(list<Task>(1, *newTask));
}

void Executor::formDeadlineTaskFromCmd(Command_Add* cmd, DeadlineTask* newTask) {
	newTask->setDueDate(cmd->getDueDate());

	if(cmd->getFlagName())
		newTask->setName(cmd->getName());
	else if(cmd->getFlagLocation())
		newTask->setLocation(cmd->getLocation());
	else if(cmd->getFlagParticipants())
		newTask->setParticipants(cmd->getParticipants());
	else if(cmd->getFlagNote())
		newTask->setNote(cmd->getNote());
	else if(cmd->getFlagPriority())
		newTask->setPriority(cmd->getPriority());
	else if(cmd->getFlagTags())
		newTask->setTags(cmd->getTags());
	else if(cmd->getFlagRemindTime())
		newTask->setRemindTime(cmd->getRemindTime());
}

void Executor::formTimedTaskFromCmd(Command_Add* cmd, TimedTask* newTask) {
	newTask->setFromDate(cmd->getFromDate());
	newTask->setToDate(cmd->getToDate());

	if(cmd->getFlagName())
		newTask->setName(cmd->getName());
	else if(cmd->getFlagLocation())
		newTask->setLocation(cmd->getLocation());
	else if(cmd->getFlagParticipants())
		newTask->setParticipants(cmd->getParticipants());
	else if(cmd->getFlagNote())
		newTask->setNote(cmd->getNote());
	else if(cmd->getFlagPriority())
		newTask->setPriority(cmd->getPriority());
	else if(cmd->getFlagTags())
		newTask->setTags(cmd->getTags());
	else if(cmd->getFlagRemindTime())
		newTask->setRemindTime(cmd->getRemindTime());
}

void Executor::formFloatingTaskFromCmd(Command_Add* cmd, FloatingTask* newTask) {
	if(cmd->getFlagName())
		newTask->setName(cmd->getName());
	else if(cmd->getFlagLocation())
		newTask->setLocation(cmd->getLocation());
	else if(cmd->getFlagParticipants())
		newTask->setParticipants(cmd->getParticipants());
	else if(cmd->getFlagNote())
		newTask->setNote(cmd->getNote());
	else if(cmd->getFlagPriority())
		newTask->setPriority(cmd->getPriority());
	else if(cmd->getFlagTags())
		newTask->setTags(cmd->getTags());
	else if(cmd->getFlagRemindTime())
		newTask->setRemindTime(cmd->getRemindTime());
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

void Executor::setIndexNotFound(const unsigned &index, Messenger &response) {
	response.setStatus(TP::ERROR);
	response.setErrorMsg(std::to_string(index) + INDEX_INVALID_ERROR);
}

void Executor::deleteByExactName(const string &name, Messenger &response) {
	bool nameFound = false;
	for(list<Task>::iterator i = _data->begin(); i != _data->end() && !nameFound; ++i)
		if (i->getName() == name) {
			response.setList(list<Task>(1, *i));
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
			matchingResults.push_back(*i);


	if (matchingResults.size() == EMPTY_LIST_SIZE)
		setNameNotFound(name, response);
	else
		setApproxNameResultsFound(matchingResults, response);

}

void Executor::setNameNotFound(const string &name, Messenger &response) {
	response.setStatus(TP::ERROR);
	response.setErrorMsg(NAME_NOT_FOUND_ERROR + name);
}

void Executor::setApproxNameResultsFound(const list<Task>& results, Messenger &response) {
	response.setStatus(TP::INTERMEDIATE);
	response.setList(results);
}