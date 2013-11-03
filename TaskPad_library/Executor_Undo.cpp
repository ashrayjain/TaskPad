/*
 * =====================================================================================
 *
 *       Filename:  Executor_Undo.cpp
 *
 *        Version:  1.0
 *        Created:  11/02/13 13:47:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Executor_Undo.h"

void Executor_Undo::executeCommand(Command_Undo* cmd, Messenger &response, Datastore &ds) {
	if (ds.isUndoStackEmpty())
		setErrorWithErrMsg(response, UNDOSTACK_EMPTY_MSG);
	else {
		Command* undoCmd = getTransposeCommand(ds.undoStackTop().first, ds.undoStackTop().second);
		//executeCommandWithoutUndoRedo(undoCmd, response);
		delete undoCmd;
		ds.popTopUndoStackToRedoStack();
	}
}

Command* Executor_Undo::getTransposeCommand(Command* cmd, Task &task) {
	switch (cmd->getCommandType()) {
	case COMMAND_TYPE::ADD:
		return getTransposeCommand(dynamic_cast<Command_Add*>(cmd), task);
		break;
	case COMMAND_TYPE::DEL:
		return getTransposeCommand(dynamic_cast<Command_Del*>(cmd), task);
		break;
	case COMMAND_TYPE::MOD:
		return getTransposeCommand(dynamic_cast<Command_Mod*>(cmd), task);
		break;
	default:
		break;
	}
	return cmd;
}

Command* Executor_Undo::getTransposeCommand(Command_Add* cmd, Task &task) {
	Command_Del* transposeCmd = new Command_Del();
	transposeCmd->setCreatedTime(cmd->getCreatedTime());
	return transposeCmd;
}

Command* Executor_Undo::getTransposeCommand(Command_Del* cmd, Task &task) {
	Command_Add* transposeCmd = new Command_Add();
	formAddCmdFromTask(task, transposeCmd);
	return transposeCmd;
}

void Executor_Undo::formAddCmdFromTask(Task &task, Command_Add* cmd) {
	cmd->setCreatedTime(task.getIndex());
	if(task.getFlagName())
		cmd->setName(task.getName());
	if(task.getFlagLocation())
		cmd->setLocation(task.getLocation());
	if(task.getFlagNote())
		cmd->setNote(task.getNote());
	if(task.getFlagRemindTimes())
		cmd->setRemindTimes(task.getRemindTimes());
	if(task.getFlagParticipants())
		cmd->setParticipants(task.getParticipants());
	if(task.getFlagPriority())
		cmd->setPriority(task.getPriority());
	if(task.getFlagDueDate())
		cmd->setDueDate(task.getDueDate());
	if(task.getFlagFromDate())
		cmd->setFromDate(task.getFromDate());
	if(task.getFlagToDate())
		cmd->setToDate(task.getToDate());
	if(task.getFlagTags())
		cmd->setTags(task.getTags());
}

Command* Executor_Undo::getTransposeCommand(Command_Mod* cmd, Task &task) {
	Command_Mod* transposeCmd = new Command_Mod();
	getCmdForSubtractingCmdFromTask(transposeCmd, cmd, task);
	return transposeCmd;
}

void Executor_Undo::getCmdForSubtractingCmdFromTask(Command_Mod* subtractCmd, Command_Mod* cmd, Task &task) {
	subtractCmd->setCreatedTime(task.getIndex());
	if(cmd->getFlagOptName()) 
		subtractCmd->setOptName(task.getName());
	if(cmd->getFlagLocation())
		subtractCmd->setLocation(task.getLocation());
	if(cmd->getFlagNote())
		subtractCmd->setNote(task.getNote());
	if(cmd->getFlagRemindTimes() || 
		cmd->getFlagAddRemindTimes() || 
		cmd->getFlagRemoveRemindTimes() || 
		cmd->getFlagRemoveAllRemindTimes())
		subtractCmd->setRemindTimes(task.getRemindTimes());
	if(cmd->getFlagParticipants() || 
		cmd->getFlagAddParticipants() || 
		cmd->getFlagRemoveParticipants() || 
		cmd->getFlagRemoveAllParticipants())
		subtractCmd->setParticipants(task.getParticipants());
	if(cmd->getFlagTags() || 
		cmd->getFlagAddTags() ||
		cmd->getFlagRemoveTags() || 
		cmd->getFlagRemoveAllTags())
		subtractCmd->setTags(task.getTags());
	if(cmd->getFlagPriority())
		subtractCmd->setPriority(task.getPriority());
	if(cmd->getFlagDue() || cmd->getFlagRemoveDue())
		subtractCmd->setDueDate(task.getDueDate());
	if(cmd->getFlagFrom() || cmd->getFlagRemoveFrom())
		subtractCmd->setFromDate(task.getFromDate());
	if(cmd->getFlagTo() || cmd->getFlagRemoveTo())
		subtractCmd->setToDate(task.getToDate());
	if(cmd->getFlagTaskState())
		subtractCmd->setTaskState(task.getState());
}