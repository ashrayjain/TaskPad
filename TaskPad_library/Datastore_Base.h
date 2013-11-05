/*
 * =====================================================================================
 *
 *       Filename:  Datastore_Base.h
 *
 *        Version:  1.0
 *        Created:  10/28/13 15:47:28
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#ifndef TASKPAD_DATASTORE_BASE_H
#define TASKPAD_DATASTORE_BASE_H

#include <list>
#include "Task.h"

class Datastore_Base {
public:
	virtual ~Datastore_Base()=0 {};
	class iterator {
        public:
			typedef iterator self_type;
            typedef Task value_type;
            typedef Task& reference;
            typedef Task* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;
            typedef int difference_type;
			virtual ~iterator()=0 {};
            virtual void operator++()=0;
            virtual void operator++(int junk)=0;
			virtual void operator--()=0;
            virtual void operator--(int junk)=0;
            virtual Task& operator*()=0;
            virtual Task* operator->()=0;
            virtual bool operator==(const iterator* rhs)=0;
            virtual bool operator!=(const iterator* rhs)=0;
    };

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


	virtual std::list<Task>getAllTasks()=0;
	virtual void			addTask(Task newTask)=0;
	virtual void			deleteTask(iterator* i)=0;
	virtual Task&			front()=0;
	virtual Task&			back()=0;
	virtual iterator*		begin()=0;
	virtual iterator*		end()=0;
	virtual const_iterator*	cbegin()=0;
	virtual const_iterator*	cend()=0;
	virtual unsigned		size()=0;
};

#endif