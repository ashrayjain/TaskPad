#ifndef _EXECUTOR_DS_STUB_H_
#define _EXECUTOR_DS_STUB_H_

#include <list>
#include "../TaskPad_library/Datastore.h"

class Executor_Datastore_stub : public Datastore {
private:
	std::list<Task> _ds;
	std::pair<Command*, Task> undoCmd;
	std::pair<Command*, Task> redoCmd;
public:
	Executor_Datastore_stub() {
		undoCmd = std::pair<Command*, Task>(new Command_Add(), Task());
		redoCmd = std::pair<Command*, Task>(new Command_Add(), Task());
	}
	~Executor_Datastore_stub() {
		delete undoCmd.first;
		delete redoCmd.first;
	}

	class const_iterator: public Datastore::const_iterator {
		public:	
			const_iterator(std::list<Task>::const_iterator _it): i(_it)			{ }
			~const_iterator()													{ }
			const_iterator(Executor_Datastore_stub::const_iterator &rhs)		{ this->i = rhs.i; }
			void operator++()													{ i.operator++(); }
			void operator++(int junk)											{ i.operator++(junk); }
			void operator--()													{ i.operator--(); }
			void operator--(int junk)											{ i.operator--(junk); }
			const Task& operator*()												{ return i.operator*(); }
			const Task* operator->()											{ return i.operator->(); }
			bool operator==(const Executor_Datastore_stub::const_iterator& rhs)	{ return i==rhs.i; }
			bool operator!=(const Executor_Datastore_stub::const_iterator& rhs)	{ return i!=rhs.i; }
			std::list<Task>::const_iterator getIterator()						{ return i; }
		private:
			std::list<Task>::const_iterator i;
	};

	std::list<Task>	 		 getTaskList			()												{ return _ds; }
	void					 addTask				(const Task &newTask);
	void					 deleteTask				(Executor_Datastore_stub::const_iterator i)		{ _ds.erase(i.getIterator()); }
	Task					 modifyTask				(Datastore::const_iterator i, Command_Mod *cmd)	{ return *i;}
	Task					 indexHashSearch		(unsigned long long indexToSearch)				{ return _ds.back(); }
	bool					 isIndexPresent			(unsigned long long indexToSearch)				{ return false; }
	std::list<Task>			 getTasksWithHash		(std::string hash)								{ return std::list<Task>(); }
	std::list<Task>			 getTasksWithRemindTimes(std::time_t remindTime)						{ return std::list<Task>(); }
	void					 addCmdForUndo			(Command* cmd, Messenger &response)				{ return; }
	std::pair<Command*, Task>getNextUndoCmd			()												{ return undoCmd;	}	
	std::pair<Command*, Task>getNextRedoCmd			()												{ return redoCmd;	}
	void					 transferRedoCmdToUndo	()												{ return; }
	void					 transferUndoCmdToRedo	()												{ return; }

	Executor_Datastore_stub::const_iterator  cbegin()	{ return Executor_Datastore_stub::const_iterator(_ds.cbegin()); }
	Executor_Datastore_stub::const_iterator  cend()		{ return Executor_Datastore_stub::const_iterator(_ds.cend()); }

	unsigned size() { 
		int test = _ds.size();
		return _ds.size(); }
};
#endif
