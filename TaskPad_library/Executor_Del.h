/*
 * =====================================================================================
 *
 *       Filename:  Executor_Del.h
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

class Executor_Del: public Executor_Base {
public:
	void executeCommand						(Command_Del* cmd, Messenger &response, Datastore &ds);
private:
	void deleteTaskByIndex					(const unsigned long long &index, Messenger &reponse, Datastore &ds);	
	void deleteTaskByName					(const string &name, Messenger &reponse, const bool &exactFlag, Datastore &ds);
	void deleteByExactName					(const string &name, Messenger &response, Datastore &ds);
	void deleteByApproxName					(const string &name, Messenger &response, Datastore &ds);
	void selectAppropriateDeleteResponse	(const list<Task> &matchingResults, const list<Task> &caseInsensitiveResults, 
											 const string &name, Messenger &response, Datastore &ds);
};
