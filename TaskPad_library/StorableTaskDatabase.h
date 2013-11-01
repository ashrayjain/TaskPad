#ifndef _STORABLE_DB_
#define _STORABLE_DB_

#include <list>

class Task;

class StorableTaskDatabase
{
	public:
		virtual std::list<Task>	getTaskList	()=0;
		virtual void			addTask		(const Task&)=0;

		virtual	~StorableTaskDatabase	()=0;
};
#endif