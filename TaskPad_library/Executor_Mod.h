/*
 * =====================================================================================
 *
 *       Filename:  Executor_Mod.h
 *
 *        Version:  1.0
 *        Created:  11/02/13 13:47:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#ifndef TASKPAD_EXECUTOR_MOD_H
#define TASKPAD_EXECUTOR_MOD_H

#include "Executor_Base.h"

//@ASHRAY JAIN A0105199B

class Executor_Mod: public Executor_Base {
public:
	void executeCommand					(Command* cmd, Messenger &response, Datastore &ds);
private:
	void modifyByIndex					(Command_Mod* cmd, Messenger &response, Datastore &ds);
	void modifyByName					(Command_Mod* cmd, Messenger &response, Datastore &ds);
	void modifyByExactName				(Command_Mod* cmd, Messenger &response, Datastore &ds);
	void modifyByApproxName				(Command_Mod* cmd, Messenger &response, Datastore &ds);
	void selectAppropriateModifyResponse(const std::list<Task> &matchingResults, 
										 const std::list<Task> &caseInsensitiveResults,
										 Command_Mod* cmd, Messenger &response, Datastore &ds);
	bool isModCmdValid					(Command_Mod* cmd, const Task& task, Messenger &response);
};
#endif