/*
 * =====================================================================================
 *
 *       Filename:  Datastore.h
 *
 *        Version:  1.0
 *        Created:  10/25/13 00:22:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#ifndef TASKPAD_DATASTORE_H
#define TASKPAD_DATASTORE_H

#include <list>
#include <stack>
#include <unordered_map>
#include "Task.h"
#include "Command.h"
#include "Messenger.h"
#include "ExecutableTaskDatastore.h"
#include "StorableTaskDatastore.h"
#include "Datastore_List.h"

class Datastore: public ExecutableTaskDatastore, public StorableTaskDatastore {
private:
	Datastore_Base*								_ds;
	unordered_map<unsigned long long, Task*>	_indexHash;
	unordered_map<string, list<Task*>>			_hashTagsHash;
	unordered_map<time_t, list<Task*>>			_remindTimesHash;
	stack<pair<Command*, Task>>					_undoStack;
	stack<pair<Command*, Task>>					_redoStack;
	Task										_interimTask;

	void			clearRedoStack();
	void			clearUndoStack();

	void			handleHashTagPtrs			(Task &newTask, const list<string> &hashTagsList);
	void			handleExistingHashTag		(list<list<Task*>::iterator> &newHashTagPtrs, 
												 Task &newTask, list<Task*> &hashTag);
	void			handleNewHashTag			(list<list<Task*>::iterator> &newHashTagPtrs, 
												 Task &newTask, list<string>::const_iterator &hashTag);
	void			handleRemindTimesPtrs		(Task &newTask, const list<time_t> &remindTimesList);
	void			handleExistingRemindTime	(list<list<Task*>::iterator> &newRemindTimesPtrs, 
												 Task &newTask, list<Task*> &remindTime);
	void			handleNewRemindTime			(list<list<Task*>::iterator> &newRemindTimesPtrs, 
												 Task &newTask, list<time_t>::const_iterator &remindTime);
	
	void			deleteHashTags				(Task &task);
	void			deleteRemindTimes			(Task &task);

	void			modifyTaskWithPtr			(Task &oldTask, Command_Mod* cmd);
	void			handleHashTagsModify		(Task &oldTask, const list<string> &newTags);
	void			handleRemindTimesModify		(Task &oldTask, const list<time_t> &newRemindTimes);
	void			handleAddRemoveParticipants	(Task &task, list<string> &participants, TP::LIST_OP op);
	void			handleAddRemoveRemindTimes	(Task &task, list<time_t> &remindTimes, TP::LIST_OP op);
	list<string>	getTagsListDifference		(const list<string> &taskTags, const list<string> &tagsToRemove) const;

	void			stackModCmdForUndo			(Command* cmd, Messenger &response);
	void			stackAddCmdForUndo			(Command* cmd, Messenger &response);
	void			stackDelCmdForUndo			(Command* cmd, Messenger &response);



public:
	Datastore()		{ _ds = new Datastore_List();									}
	~Datastore()	{ clearRedoStack(); clearUndoStack(); delete _ds; _ds = NULL;	}

	list<Task>	getTaskList(){ return _ds->getAllTasks();	}

	void					addTask						(const Task &newTask);
	void					deleteTask					(const unsigned &pos);
	Task					modifyTask					(const unsigned &pos, Command_Mod* cmd);
	Task					modifyTaskWithIndex			(const unsigned long long index, Command_Mod* cmd);
	Task					indexHashSearch				(unsigned long long indexToSearch);
	bool					isIndexPresent				(unsigned long long indexToSearch);
	list<Task>				getTasksWithHash			(string hash);
	list<Task>				getTasksWithRemindTimes		(time_t remindTime);
	void					stackCmdForUndo				(Command* cmd, Messenger &response);
	bool					isUndoStackEmpty			() { return _undoStack.empty(); }
	bool					isRedoStackEmpty			() { return _redoStack.empty(); }
	pair<Command*, Task>	undoStackTop				() { return _undoStack.top();	}
	pair<Command*, Task>	redoStackTop				() { return _redoStack.top();	}
	void					popTopRedoStackToUndoStack	(); 
	void					popTopUndoStackToRedoStack	();
	
};

#endif
