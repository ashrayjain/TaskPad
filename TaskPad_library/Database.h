/*
 * =====================================================================================
 *
 *       Filename:  Database.h
 *
 *        Version:  1.0
 *        Created:  10/25/13 00:22:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#ifndef TASKPAD_DATABASE_H
#define TASKPAD_DATABASE_H

#include <list>
#include <stack>
#include <unordered_map>
#include <iterator>
#include "Task.h"
#include "Command.h"
#include "Messenger.h"

/*
 *=======================
 
	Database Base class
 
 *=======================
 */

class Database
{
public:
	Database	()	{ rebuildHashes(); }
	~Database	()	{ clearRedoStack(); clearUndoStack(); }

protected:
	std::unordered_map<unsigned long long, Task*>		_indexHash;
	std::unordered_map<std::string, std::list<Task*>>	_hashTagsHash;
	std::unordered_map<std::time_t, std::list<Task*>>	_remindTimesHash;
	std::stack<std::pair<Command*, Task>>				_undoStack;
	std::stack<std::pair<Command*, Task>>				_redoStack;

	void	rebuildHashes();
	void	rebuildIndexHash();
	void	rebuildHashTagsHash();
	void	rebuildRemindTimesHash();

	void	clearRedoStack();
	void	clearUndoStack();

	// Setters for Messenger to return
	void	setOpSuccessTask			(const Task &retTask, Messenger &response);
	void	setOpSuccessTaskList		(const list<Task>& results, Messenger &response);
	void	setOpIntermediateTaskList	(const list<Task> &results, Messenger &response);
	void	setIndexNotFound			(const unsigned long long &index, Messenger &response);
	void	setNameNotFound				(const string &name, Messenger &response);
	void	setErrorWithErrMsg			(Messenger &response, const string errMsg);

	



};

#endif
