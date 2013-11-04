/*
 * =====================================================================================
 *
 *       Filename:  Executor_Redo.cpp
 *
 *        Version:  1.0
 *        Created:  11/02/13 13:47:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Executor_Redo.h"

void Executor_Redo::executeCommand(Command* cmd, Messenger &response, Datastore &ds) {
	if (ds.isRedoStackEmpty())
		setErrorWithErrMsg(response, REDOSTACK_EMPTY_MSG);
	else {
		_redoCommandToExecute = ds.undoStackTop().first;
		ds.popTopRedoStackToUndoStack();
	}
}

Command* Executor_Redo::getRedoCommandToExecute() {
	return _redoCommandToExecute;
}
