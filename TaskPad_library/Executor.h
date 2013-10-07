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
#include <map>
#include <string>
#include "Task.h"
#include "Messenger.h"
#include "Command.h"

const unsigned		EMPTY_LIST_SIZE			= 0;
const std::string	NAME_NOT_FOUND_ERROR	= "No results for name: ";
const std::string	INVALID_INDEX_ERROR		= " is not a valid index!";

class Executor
{
public:
	Executor (std::list<Task>* data) { _data = data; }
	void executeCommand	(Command* cmd, Messenger &response);

protected:
	std::list<Task>*			_data;
	std::map<unsigned, Task*>	_indexHash;

	void executeAdd					(Command_Add* cmd,  Messenger &response);
	void executeDel					(Command_Del* cmd,  Messenger &response);
	void executeMod					(Command_Mod* cmd,  Messenger &response);
	void executeFind				(Command_Find* cmd, Messenger &response);

	void formTaskFromAddCmd			(Command_Add* cmd, Task &newTask);

	void deleteTaskByIndex			(const unsigned &index, Messenger &reponse);
	
	void deleteTaskByName			(const string &name, Messenger &reponse, const bool &exactFlag);
	void deleteByExactName			(const string &name, Messenger &response);
	void deleteByApproxName			(const string &name, Messenger &response);

	void formTaskFromFindCmd		(Command_Find* cmd, Task &newTask);

	void findByIndex				(const unsigned index, Messenger &response);
	void findByName					(Command_Find* cmd, Messenger &response);
	void runSearchWithTask			(const Task &taskToCompare,	list<Task> &results);
	void runSearchWithTask			(const Task &taskToCompare, list<Task> &results, string substringName);
	bool taskMatch					(const Task& lhs, const Task& rhs) const;
	
	// Setters for Messenger to return
	void setOpSuccessTask			(const Task &retTask, Messenger &response);
	void setOpSuccessTaskList		(const list<Task>& results, Messenger &response);
	void setOpIntermediateTaskList	(const list<Task> &results, Messenger &response);
	void setIndexNotFound			(const unsigned &index, Messenger &response);
	void setNameNotFound			(const string &name, Messenger &response);
};