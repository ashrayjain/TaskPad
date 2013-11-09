/*
 * =====================================================================================
 *
 *       Filename:  Executor.h
 *
 *        Version:  1.0
 *        Created:  09/25/13 00:22:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Task.h"
#include "Command.h"
#include "Messenger.h"
#include "Datastore.h"
#include "Executor_Base.h"
#include "Executor_Add.h"
#include "Executor_Del.h"
#include "Executor_Mod.h"
#include "Executor_Find.h"
#include "Executor_Undo.h"
#include "Executor_Redo.h"


class Executor
{
public:
	Executor(Datastore &ds):_ds(ds)			{ }
	void executeCommand	(Command* &cmd, Messenger &response);
	std::list<Task>	getCurrentReminders	();

protected:
	Datastore	&_ds;
	void		executeCommandWithoutUndoRedo	(Command* cmd, Messenger &response);
	bool		isCmdSuccessful					(const Messenger &response) const;	
};