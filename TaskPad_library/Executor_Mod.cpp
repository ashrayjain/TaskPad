/*
 * =====================================================================================
 *
 *       Filename:  Executor_Mod.cpp
 *
 *        Version:  1.0
 *        Created:  11/02/13 13:47:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Executor_Mod.h"

void Executor_Mod::executeCommand(Command* cmd, Messenger &response, Datastore &ds) {
	Command_Mod* modCmd = dynamic_cast<Command_Mod*>(cmd);
	if(modCmd->getFlagCreatedTime())
		modifyByIndex(modCmd, response, ds);
	else
		modifyByName(modCmd, response, ds);
}

void Executor_Mod::modifyByIndex(Command_Mod* cmd, Messenger &response, Datastore &ds) {
	if (ds.isIndexPresent(cmd->getCreatedTime())) {
		if(isModCmdValid(cmd, ds.indexHashSearch(cmd->getCreatedTime()), response))
			setOpSuccessTask(ds.modifyTaskWithIndex(cmd->getCreatedTime(), cmd), response);
	}
	else
		setIndexNotFound(cmd->getCreatedTime(), response);
}

void Executor_Mod::modifyByName(Command_Mod* cmd, Messenger &response, Datastore &ds) {
	if (cmd->getFlagExact())
		modifyByExactName(cmd, response, ds);
	else
		modifyByApproxName(cmd, response, ds);
}

void Executor_Mod::modifyByExactName(Command_Mod* cmd, Messenger &response, Datastore &ds) {
	bool nameFound = false;
	list<Task> tasks = ds.getTaskList();
	for(Datastore::const_iterator i = ds.cbegin(); i != ds.cend() && !nameFound; i++)
	//for(list<Task>::iterator i = tasks.begin(); i != tasks.end() && !nameFound; ++i)
		if (i->getName() == cmd->getName()) {
			if (isModCmdValid(cmd, *i, response))
				setOpSuccessTask(ds.modifyTask(i, cmd), response);
			nameFound = true;
		}

	if (!nameFound)
		setNameNotFound(cmd->getName(), response);
}

void Executor_Mod::modifyByApproxName(Command_Mod* cmd, Messenger &response, Datastore &ds) {
	list<Task> matchingResults;
	list<Task> caseInsensitiveResults;
	list<Task> tasks = ds.getTaskList();
	string lowerName = getLowerStr(cmd->getName());
	for(Datastore::const_iterator i = ds.cbegin(); i != ds.cend(); i++) {
	//for(list<Task>::iterator i = tasks.begin(); i != tasks.end(); ++i) {
		string currName = getLowerStr(i->getName());
		if (currName.find(cmd->getName()) != string::npos)
			matchingResults.push_back(Task(*i));
		else if (currName.find(lowerName) != string::npos)
			caseInsensitiveResults.push_back(Task(*i));
	}
	selectAppropriateModifyResponse(matchingResults, caseInsensitiveResults, cmd, response, ds);
}

void Executor_Mod::selectAppropriateModifyResponse(const list<Task> &matchingResults, const list<Task> &caseInsensitiveResults, 
												   Command_Mod* cmd, Messenger &response, Datastore &ds) {
	if (matchingResults.size() == EMPTY_LIST_SIZE) {
		if (caseInsensitiveResults.size() == EMPTY_LIST_SIZE)
			setNameNotFound(cmd->getName(), response);
		else if (caseInsensitiveResults.size() == SINGLE_RESULT_LIST_SIZE) {
			cmd->setCreatedTime(caseInsensitiveResults.front().getIndex());
			modifyByIndex(cmd, response, ds);
		}
		else
			setOpIntermediateTaskList(caseInsensitiveResults, response);
	}
	else if(matchingResults.size() == SINGLE_RESULT_LIST_SIZE){
		cmd->setCreatedTime(matchingResults.front().getIndex());
		modifyByIndex(cmd, response, ds);
	}
	else
		setOpIntermediateTaskList(matchingResults, response);
}

bool Executor_Mod::isModCmdValid(Command_Mod* cmd, const Task& task, Messenger &response) {
	if (cmd->getFlagOptName() && cmd->getOptName() == task.getName()) {
		setErrorWithErrMsg(response, MODIFY_SAME_NAME_ERROR);
		return false;
	}
	if (!(cmd->getFlagFrom() && cmd->getFlagTo())) {
		if (cmd->getFlagFrom() && task.getFlagToDate() && cmd->getFromDate() > task.getToDate()) {
			setErrorWithErrMsg(response, INVALID_FROMDATE_ERROR);
			return false;
		}
		if (cmd->getFlagTo() && task.getFlagFromDate() && cmd->getToDate() < task.getFromDate()) {
			setErrorWithErrMsg(response, INVALID_TODATE_ERROR);
			return false;
		}
	}
	else if(cmd->getFromDate() > cmd->getToDate()) {
		setErrorWithErrMsg(response, INVALID_FROMDATE_TODATE_ERROR);
		return false;
	}
	return true;
}