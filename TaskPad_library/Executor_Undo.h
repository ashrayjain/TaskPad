/*
 * =====================================================================================
 *
 *       Filename:  Executor_Undo.h
 *
 *        Version:  1.0
 *        Created:  11/02/13 13:47:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#pragma once

#include "Executor_Base.h"

class Executor_Undo: public Executor_Base {
public:
	Executor_Undo()	{ _undoCommandToExecute = NULL; }
	~Executor_Undo(){ delete _undoCommandToExecute; }

	void		executeCommand					(Command* cmd, Messenger &response, Datastore &ds);
	Command*	getUndoCommandToExecute			();
private:
	Command*	getTransposeCommand				(Command* cmd, Task &task);
	Command*	getTransposeCommand				(Command_Add* cmd, Task &task);
	Command*	getTransposeCommand				(Command_Del* cmd, Task &task);
	Command*	getTransposeCommand				(Command_Mod* cmd, Task &task);
	void		getCmdForSubtractingCmdFromTask	(Command_Mod* subtractCmd, Command_Mod* cmd, Task &task);
	void		formAddCmdFromTask				(Task &task, Command_Add* cmd);
	Command*	_undoCommandToExecute;
};
