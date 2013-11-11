/*
 * =====================================================================================
 *
 *       Filename:  Datastore_List.h
 *
 *        Version:  1.0
 *        Created:  10/28/13 15:47:28
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#ifndef TASKPAD_DATASTORE_LIST_H
#define TASKPAD_DATASTORE_LIST_H

#include "Datastore_Base.h"

class Datastore_List: public Datastore_Base {
private:
	std::list<Task> _db;

public:
	~Datastore_List() {};

	class iterator: public Datastore_Base::iterator {
	public:
		iterator(std::list<Task>::iterator _it): i(_it)				{ }
		void operator++()											{ i.operator++(); }
		void operator++(int junk)									{ i.operator++(junk); }
		void operator--()											{ i.operator--(); }
		void operator--(int junk)									{ i.operator--(junk); }
		Task& operator*()											{ return i.operator*(); }
		Task* operator->()											{ return i.operator->(); }
		bool operator==(const Datastore_Base::iterator* rhs)		{ return i==dynamic_cast<const Datastore_List::iterator*>(rhs)->i; }
		bool operator!=(const Datastore_Base::iterator* rhs)		{ return i!=dynamic_cast<const Datastore_List::iterator*>(rhs)->i; }
		const std::list<Task>::iterator& getIterator() const		{ return i; }
	private:
		std::list<Task>::iterator i;
	};

	class const_iterator: public Datastore_Base::const_iterator {
	public:	
		const_iterator(std::list<Task>::const_iterator _it): i(_it)	{ }
		void operator++()											{ i.operator++(); }
		void operator++(int junk)									{ i.operator++(junk); }
		void operator--()											{ i.operator--(); }
		void operator--(int junk)									{ i.operator--(junk); }
		const Task& operator*()										{ return i.operator*(); }
		const Task* operator->()									{ return i.operator->(); }
		bool operator==(const Datastore_Base::const_iterator* rhs)	{ return i==dynamic_cast<const Datastore_List::const_iterator*>(rhs)->i; }
		bool operator!=(const Datastore_Base::const_iterator* rhs)	{ return i!=dynamic_cast<const Datastore_List::const_iterator*>(rhs)->i; }
		std::list<Task>::const_iterator getIterator()				{ return i; }
	private:
		std::list<Task>::const_iterator i;
	};
	
	std::list<Task>					getAllTasks();
	void							addTask(Task newTask)					{ _db.push_back(newTask); }
	void							deleteTask(Datastore_Base::iterator* i)	{ (*i)->freeIndex(); _db.erase(dynamic_cast<Datastore_List::iterator*>(i)->getIterator()); }
	Task&							front()									{ return _db.front(); }
	Task&							back()									{ return _db.back(); }
	Datastore_List::iterator*		begin()									{ return new Datastore_List::iterator(_db.begin()); }
	Datastore_List::iterator*		end()									{ return new Datastore_List::iterator(_db.end()); }
	Datastore_List::const_iterator*	cbegin()								{ return new Datastore_List::const_iterator(_db.cbegin()); }
	Datastore_List::const_iterator*	cend()									{ return new Datastore_List::const_iterator(_db.cend()); }
	unsigned						size()									{ return _db.size(); }
};

#endif