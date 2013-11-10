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
#include "Executor_Add.h"
#include "Executor_Del.h"
#include "Executor_Mod.h"
#include "Executor_Find.h"
#include "Executor_Undo.h"
#include "Executor_Redo.h"

using namespace std;
using namespace TP;

list<Task> Executor::getCurrentReminders() {
	time_t now = time(NULL);
	struct tm* timeNow = localtime(&now);
	timeNow->tm_sec = 0;
	now = mktime(timeNow);
	return _ds.getTasksWithRemindTimes(now);
}

void Executor::executeCommand(Command* &cmd, Messenger &response) {
	switch (cmd->getCommandType()) {
	case COMMAND_TYPE::ADD:	_executor = new Executor_Add();
							_executor->executeCommand(cmd, response, _ds);
							if (isCmdSuccessful(response))
								_ds.addCmdForUndo(cmd, response);
							break;
	case COMMAND_TYPE::DEL:	_executor = new Executor_Del();
							_executor->executeCommand(cmd, response, _ds);
							if (isCmdSuccessful(response))
								_ds.addCmdForUndo(cmd, response);
							break;
	case COMMAND_TYPE::MOD:	_executor = new Executor_Mod();
							_executor->executeCommand(cmd, response, _ds);
							if (isCmdSuccessful(response))
								_ds.addCmdForUndo(cmd, response);
							break;
	case COMMAND_TYPE::FIND:_executor = new Executor_Find();
							_executor->executeCommand(cmd, response, _ds);
							break;
	case COMMAND_TYPE::UNDO:_executor = new Executor_Undo();
							_executor->executeCommand(cmd, response, _ds);
							if(response.getStatus() != TP::STATUS::ERR) {
								delete cmd;
								cmd = (dynamic_cast<Executor_Undo*>(_executor)->getUndoCommandToExecute());
								executeCommandWithoutUndoRedo(cmd, response);
							}
							break;
	case COMMAND_TYPE::REDO:_executor = new Executor_Redo();
							_executor->executeCommand(cmd, response, _ds);
							if(response.getStatus() != TP::STATUS::ERR) {
								delete cmd;
								cmd = (dynamic_cast<Executor_Redo*>(_executor)->getRedoCommandToExecute());
								executeCommandWithoutUndoRedo(cmd, response);
							}
							break;
	default:
		break;
	}
	if (_executor != NULL) {
		delete _executor;
		_executor = NULL;
	}
}

void Executor::executeCommandWithoutUndoRedo(Command* cmd, Messenger &response) {
	if (_executor != NULL) {
		delete _executor;
		_executor = NULL;
	}
	switch (cmd->getCommandType()) {
	case COMMAND_TYPE::ADD:	_executor = new Executor_Add();
							break;
	case COMMAND_TYPE::DEL:	_executor = new Executor_Del();
							break;
	case COMMAND_TYPE::MOD:	_executor = new Executor_Mod();
							break;
	}
	_executor->executeCommand(cmd, response, _ds);
	delete _executor;
	_executor = NULL;
}

bool Executor::isCmdSuccessful(const Messenger &response) const {
	return response.getStatus() == TP::STATUS::SUCCESS;
}
