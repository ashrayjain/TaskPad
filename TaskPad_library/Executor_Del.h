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

#ifndef TASKPAD_EXECUTOR_DEL_H
#define TASKPAD_EXECUTOR_DEL_H

#include "Executor_Base.h"

// @author A0105199B

class Executor_Del: public Executor_Base {
public:
	void executeCommand						(Command* cmd, Messenger &response, Datastore &ds);
private:
	void deleteTaskByIndex					(const unsigned long long &index, Messenger &reponse, Datastore &ds);	
	void deleteTaskByName					(const std::string &name, Messenger &reponse, const bool &exactFlag, Datastore &ds);
	void deleteByExactName					(const std::string &name, Messenger &response, Datastore &ds);
	void deleteByApproxName					(const std::string &name, Messenger &response, Datastore &ds);
	void selectAppropriateDeleteResponse	(const std::list<Task> &matchingResults, const std::list<Task> &caseInsensitiveResults, 
											 const std::string &name, Messenger &response, Datastore &ds);
};
#endif