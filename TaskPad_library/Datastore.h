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
#include <algorithm>
#include <unordered_map>
#include "Task.h"
#include "Command.h"
#include "Messenger.h"
#include "StorableTaskDatastore.h"
#include "Datastore_List.h"

class Datastore: public StorableTaskDatastore {
private:
	typedef Datastore_List Datastore_Type;
	Datastore_Base*										_ds;
	std::unordered_map<unsigned long long, Task*>		_indexHash;
	std::unordered_map<std::string, std::list<Task*>>	_hashTagsHash;
	std::unordered_map<std::time_t, std::list<Task*>>	_remindTimesHash;
	std::stack<std::pair<Command*, Task>>				_undoStack;
	std::stack<std::pair<Command*, Task>>				_redoStack;
	Task												_interimTask;

	void clearRedoStack();
	void clearUndoStack();

	void					handleHashTagPtrs			(Task &newTask, const std::list<std::string> &hashTagsList);
	void					handleExistingHashTag		(std::list<std::list<Task*>::iterator> &newHashTagPtrs, 
														 Task &newTask, std::list<Task*> &hashTag);
	void					handleNewHashTag			(std::list<std::list<Task*>::iterator> &newHashTagPtrs, 
														 Task &newTask, std::list<std::string>::const_iterator &hashTag);
	void					handleRemindTimesPtrs		(Task &newTask, const std::list<std::time_t> &remindTimesList);
	void					handleExistingRemindTime	(std::list<std::list<Task*>::iterator> &newRemindTimesPtrs, 
														 Task &newTask, std::list<Task*> &remindTime);
	void					handleNewRemindTime			(std::list<std::list<Task*>::iterator> &newRemindTimesPtrs, 
														 Task &newTask, std::list<std::time_t>::const_iterator &remindTime);
	void					deleteHashTags				(Task &task);
	void					deleteRemindTimes			(Task &task);
	void					modifyTaskWithPtr			(Task &oldTask, Command_Mod* cmd);
	void					handleHashTagsModify		(Task &oldTask, const std::list<std::string> &newTags);
	void					handleRemindTimesModify		(Task &oldTask, const std::list<std::time_t> &newRemindTimes);
	void					handleAddRemoveParticipants	(Task &task, std::list<std::string> &participants,
														 TP::LIST_OP op);
	void					handleAddRemoveRemindTimes	(Task &task, std::list<std::time_t> &remindTimes,
														 TP::LIST_OP op);
	std::list<std::string>	getTagsListDifference		(const std::list<std::string> &taskTags,
														 const std::list<std::string> &tagsToRemove) const;

	void	stackModCmdForUndo			(Command* cmd, Messenger &response);
	void	stackAddCmdForUndo			(Command* cmd, Messenger &response);
	void	stackDelCmdForUndo			(Command* cmd, Messenger &response);



public:
	Datastore()		{ _ds = new Datastore_Type();								}
	~Datastore()	{ clearRedoStack(); clearUndoStack(); delete _ds; _ds = NULL;	}

	// Utility functions
	static std::string				getLowerStr		(std::string str);
	static std::list<std::string>	getLowerStrList	(std::list<std::string> strList);

	class const_iterator: public StorableTaskDatastore::const_iterator {
        public:
			const_iterator()											{ }
			~const_iterator()											{ delete i; i = NULL; }
			const_iterator(Datastore_Type::const_iterator* it): i(it)	{ }
			const_iterator(Datastore::const_iterator &rhs)				{ this->i = new Datastore_Type::const_iterator(*(rhs.i)); }
			typedef const_iterator self_type;
            typedef Task value_type;
            typedef Task& reference;
            typedef Task* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;
            typedef int difference_type;
			void operator++()													{ i->operator++(); }
			void operator++(int junk)											{ i->operator++(junk); }
			void operator--()													{ i->operator--(); }
			void operator--(int junk)											{ i->operator--(junk); }
			const Task& operator*()												{ return i->operator*(); }
			const Task* operator->()											{ return i->operator->(); }
			bool operator==(const StorableTaskDatastore::const_iterator* rhs)	{ return *i==dynamic_cast<const Datastore::const_iterator*>(rhs)->i; }
			bool operator!=(const StorableTaskDatastore::const_iterator* rhs)	{ return *i!=dynamic_cast<const Datastore::const_iterator*>(rhs)->i; }
			bool operator==(const Datastore::const_iterator* rhs)				{ return *i==(rhs)->i; }
			bool operator!=(const Datastore::const_iterator* rhs)				{ return *i!=(rhs)->i; }
			bool operator==(const Datastore::const_iterator rhs)				{ return *i==rhs.i; }
			bool operator!=(const Datastore::const_iterator rhs)				{ return *i!=rhs.i; }


	private:
		Datastore_Type::const_iterator* i;
    };

	std::list<Task>					getTaskList(){ return _ds->getAllTasks();	}

	void							addTask						(const Task &newTask);
	void							deleteTask					(const unsigned &pos);
	void							deleteTask					(Datastore::const_iterator i);
	Task							modifyTask					(const unsigned &pos, Command_Mod* cmd);
	Task							modifyTask					(Datastore::const_iterator i, Command_Mod *cmd);
	Task							modifyTaskWithIndex			(const unsigned long long index, Command_Mod* cmd);
	Task							indexHashSearch				(unsigned long long indexToSearch);
	bool							isIndexPresent				(unsigned long long indexToSearch);
	std::list<Task>					getTasksWithHash			(std::string hash);
	std::list<Task>					getTasksWithRemindTimes		(std::time_t remindTime);
	void							stackCmdForUndo				(Command* cmd, Messenger &response);
	bool							isUndoStackEmpty			() { return _undoStack.empty(); }
	bool							isRedoStackEmpty			() { return _redoStack.empty(); }
	std::pair<Command*, Task>		undoStackTop				() { return _undoStack.top();	}
	std::pair<Command*, Task>		redoStackTop				() { return _redoStack.top();	}
	void							popTopRedoStackToUndoStack	(); 
	void							popTopUndoStackToRedoStack	();

	Datastore::const_iterator* cbeginPtr();
	Datastore::const_iterator* cendPtr	();
	Datastore::const_iterator  cbegin	();
	Datastore::const_iterator  cend		();
	
	unsigned size() { return _ds->size(); }
	
};

#endif
