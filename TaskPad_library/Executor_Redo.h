/*
 * =====================================================================================
 *
 *       Filename:  Executor_Redo.h
 *
 *        Version:  1.0
 *        Created:  11/02/13 13:47:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#ifndef TASKPAD_EXECUTOR_REDO_H
#define TASKPAD_EXECUTOR_REDO_H

#include "Executor_Base.h"

// @author A0105199B

class Executor_Redo: public Executor_Base {
public:
	Executor_Redo()	{ _redoCommandToExecute = NULL; }
	~Executor_Redo(){ }

	void		executeCommand					(Command* cmd, Messenger &response, Datastore &ds);
	Command*	getRedoCommandToExecute			();
private:
	Command*	_redoCommandToExecute;
};
#endif