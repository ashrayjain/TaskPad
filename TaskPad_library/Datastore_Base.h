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
	virtual ~Datastore_Base() = 0;

	class iterator {
        public:
			typedef iterator self_type;
            typedef Task value_type;
            typedef Task& reference;
            typedef Task* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;
            typedef int difference_type;
            virtual void operator++();
            virtual void operator++(int junk);
			virtual void operator--();
            virtual void operator--(int junk);
            virtual Task& operator*();
            virtual Task* operator->();
            virtual bool operator==(const iterator& rhs);
            virtual bool operator!=(const iterator& rhs);
    };


	std::list<Task>				getAllTasks();
	void						addTask(Task newTask);
	void						deleteTask(iterator i);
	Task&						front();
	Task&						back();
	Datastore_Base::iterator	begin();
	Datastore_Base::iterator	end();
};

#endif