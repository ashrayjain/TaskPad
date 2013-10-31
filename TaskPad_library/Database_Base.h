/*
 * =====================================================================================
 *
 *       Filename:  Database_Base.h
 *
 *        Version:  1.0
 *        Created:  10/28/13 15:47:28
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#ifndef TASKPAD_DATABASE_BASE_H
#define TASKPAD_DATABASE_BASE_H

#include <list>
#include "Task.h"

class Database_Base {
public:
	Database_Base(){}
	virtual ~Database_Base() = 0;

	class iterator
    {
        public:
            virtual void operator++();
            virtual void operator++(int junk);
			virtual void operator--();
            virtual void operator--(int junk);
            virtual Task& operator*();
            virtual Task* operator->();
            virtual bool operator==(const iterator& rhs);
            virtual bool operator!=(const iterator& rhs);
    };


	virtual std::list<Task> getAllTasks();
	virtual void			addTask(Task newTask);
	virtual void			deleteTask(iterator i);
	iterator				begin();
	iterator				end();
	virtual Task&			front();
	virtual Task&			back();
};

#endif