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

list<Task> Executor::getCurrentReminders() {
	time_t now = time(NULL);
	struct tm* timeNow = localtime(&now);
	timeNow->tm_sec = 0;
	now = mktime(timeNow);
	return _ds.getTasksWithRemindTimes(now);
}

void Executor::executeCommand(Command* &cmd, Messenger &response) {
	Executor_Base* executor;
	switch (cmd->getCommandType()) {
	case COMMAND_TYPE::ADD:	executor = new Executor_Add();
							executor->executeCommand(cmd, response, _ds);
							if (isCmdSuccessful(response))
								_ds.stackCmdForUndo(cmd, response);
							break;
	case COMMAND_TYPE::DEL:	executor = new Executor_Del();
							executor->executeCommand(cmd, response, _ds);
							if (isCmdSuccessful(response))
								_ds.stackCmdForUndo(cmd, response);
							break;
	case COMMAND_TYPE::MOD:	executor = new Executor_Mod();
							executor->executeCommand(cmd, response, _ds);
							if (isCmdSuccessful(response))
								_ds.stackCmdForUndo(cmd, response);
							break;
	case COMMAND_TYPE::FIND:executor = new Executor_Find();
							executor->executeCommand(cmd, response, _ds);
							break;
	case COMMAND_TYPE::UNDO:executor = new Executor_Undo();
							executor->executeCommand(cmd, response, _ds);
							if(response.getStatus() != TP::STATUS::ERR) {
								delete cmd;
								cmd = (dynamic_cast<Executor_Undo*>(executor)->getUndoCommandToExecute());
								executeCommandWithoutUndoRedo(cmd, response);
							}
							break;
	case COMMAND_TYPE::REDO:executor = new Executor_Redo();
							executor->executeCommand(cmd, response, _ds);
							if(response.getStatus() != TP::STATUS::ERR) {
								delete cmd;
								cmd = (dynamic_cast<Executor_Redo*>(executor)->getRedoCommandToExecute());
								executeCommandWithoutUndoRedo(cmd, response);
							}
							break;
	default:
		break;
	}
	delete executor;
}

void Executor::executeCommandWithoutUndoRedo(Command* cmd, Messenger &response) {
	Executor_Base* executor;
	switch (cmd->getCommandType()) {
	case COMMAND_TYPE::ADD:	executor = new Executor_Add();
							break;
	case COMMAND_TYPE::DEL:	executor = new Executor_Del();
							break;
	case COMMAND_TYPE::MOD:	executor = new Executor_Mod();
							break;
	}
	executor->executeCommand(cmd, response, _ds);
	delete executor;
}

bool Executor::isCmdSuccessful(const Messenger &response) const {
	return response.getStatus() == TP::STATUS::SUCCESS;
}
