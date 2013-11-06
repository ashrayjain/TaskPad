#ifndef _STORABLE_DSTORE_
#define _STORABLE_DSTORE_

#include <list>
#include "Task.h"

class StorableTaskDatastore
{
	public:
		virtual std::list<Task>	getTaskList	()=0;
		virtual void			addTask		(const Task&)=0;

		class const_iterator {
			public:
				typedef const_iterator self_type;
				typedef Task value_type;
				typedef Task& reference;
				typedef Task* pointer;
				typedef std::bidirectional_iterator_tag iterator_category;
				typedef int difference_type;
				virtual ~const_iterator()=0 {};
				virtual void operator++()=0;
				virtual void operator++(int junk)=0;
				virtual void operator--()=0;
				virtual void operator--(int junk)=0;
				virtual const Task& operator*()=0;
				virtual const Task* operator->()=0;
				virtual bool operator==(const const_iterator* rhs)=0;
				virtual bool operator!=(const const_iterator* rhs)=0;
		};

		virtual StorableTaskDatastore::const_iterator* cbegin	()=0;
		virtual StorableTaskDatastore::const_iterator* cend		()=0;


		virtual ~StorableTaskDatastore()=0 {};
};
#endif