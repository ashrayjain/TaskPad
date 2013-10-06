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
#include <string>
#include "Task.h"
#include "Messenger.h"
#include "Command.h"

const unsigned		EMPTY_LIST_SIZE			= 0;
const std::string	NAME_NOT_FOUND_ERROR	= "No results for name: ";

class Executor
{
public:
	Executor (std::list<Task>* data) { _data = data; }
	void executeCommand	(Command* cmd, Messenger &response);

protected:
	std::list<Task>* _data;

	void executeAdd					(Command_Add* cmd,  Messenger &response);
	void executeDel					(Command_Del* cmd,  Messenger &response);
	void executeMod					(Command_Mod* cmd,  Messenger &response);
	void executeFind				(Command_Find* cmd, Messenger &response);

	void formTaskFromAddCmd			(Command_Add* cmd, Task &newTask);

	void deleteTaskByIndex			(const unsigned &index, Messenger &reponse);
	void deleteTaskByName			(const string &name, Messenger &reponse, const bool &exactFlag);

	void setIndexNotFound			(const unsigned &index, Messenger &response);

	void deleteByExactName			(const string &name, Messenger &response);
	void deleteByApproxName			(const string &name, Messenger &response);

	void setNameNotFound			(const string &name, Messenger &response);
	void setApproxNameResultsFound	(const list<Task>& results, Messenger &response);

};