/*
 * =====================================================================================
 *
 *       Filename:  Database_List.h
 *
 *        Version:  1.0
 *        Created:  10/28/13 15:47:28
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#ifndef TASKPAD_DATABASE_LIST_H
#define TASKPAD_DATABASE_LIST_H

#include "Database_Base.h"

class Database_List: public Database_Base {
private:
	std::list<Task> _db;

public:
	~Database_List() {};

	class iterator: public Database_Base::iterator {
	public:
		iterator()											{ }
		iterator(std::list<Task>::iterator _it): i(_it)		{ }
		void operator++()									{ i.operator++(); }
		void operator++(int junk)							{ i.operator++(junk); }
		void operator--()									{ i.operator--(); }
		void operator--(int junk)							{ i.operator--(junk); }
		Task& operator*()									{ return i.operator*(); }
		Task* operator->()									{ return i.operator->(); }
		bool operator==(const iterator& rhs)				{ return i==rhs.i; }
		bool operator!=(const iterator& rhs)				{ return i!=rhs.i; }
		const std::list<Task>::iterator getIterator() const	{ return i; }
	private:
		std::list<Task>::iterator i;
	};

	std::list<Task>				getAllTasks();
	void						addTask(Task newTask)	{ _db.push_back(newTask);		}
	void						deleteTask(iterator i)	{ _db.erase(i.getIterator());	}

	Database_List::iterator		begin()	{ return _db.begin();	}
	Database_List::iterator		end()	{ return _db.end();		}
	Task&						front() { return _db.front();	}
	Task&						back()	{ return _db.back();	}
};

#endif