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

#include <list>
#include "Datastore.h"

class Task;
class Messenger;
class Executor_Base;
class Command;

class Executor
{
public:
	Executor(Datastore &ds):_ds(ds),_executor(NULL)		{ }
	void executeCommand	(Command* &cmd, Messenger &response);
	std::list<Task>	getCurrentReminders	();

protected:
	Executor_Base* _executor;
	Datastore	&_ds;
	void		executeCommandWithoutUndoRedo	(Command* cmd, Messenger &response);
	bool		isCmdSuccessful					(const Messenger &response) const;	
};