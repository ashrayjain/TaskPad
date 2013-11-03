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

void Executor_Redo::executeCommand(Command_Redo* cmd, Messenger &response, Datastore &ds) {
	if (ds.isRedoStackEmpty())
		setErrorWithErrMsg(response, REDOSTACK_EMPTY_MSG);
	else {
		//executeCommandWithoutUndoRedo(_redoStack.top().first, response);		
		ds.popTopRedoStackToUndoStack();
	}
}