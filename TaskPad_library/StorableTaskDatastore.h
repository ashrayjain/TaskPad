#ifndef _STORABLE_DSTORE_
#define _STORABLE_DSTORE_

#include <list>
#include "Task.h"

class StorableTaskDatastore
{
	public:
		virtual std::list<Task>	getTaskList	()=0;
		virtual void			addTask		(const Task&)=0;

		virtual	~StorableTaskDatastore	()=0;
};
#endif