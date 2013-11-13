#ifndef _STORAGE_DS_STUB_H_
#define _STORAGE_DS_STUB_H_

#include <list>
#include "../TaskPad_library/StorableTaskDatastore.h"

class Storage_Datastore_stub : public StorableTaskDatastore {
	private:
		std::list<Task> _db;
	public:
		class const_iterator: public StorableTaskDatastore::const_iterator {
			public:	
				const_iterator(std::list<Task>::const_iterator _it): i(_it)			{ }
				void operator++()													{ i.operator++(); }
				void operator++(int junk)											{ i.operator++(junk); }
				void operator--()													{ i.operator--(); }
				void operator--(int junk)											{ i.operator--(junk); }
				const Task& operator*()												{ return i.operator*(); }
				const Task* operator->()											{ return i.operator->(); }
				bool operator==(const StorableTaskDatastore::const_iterator* rhs)	{ return i==dynamic_cast<const Storage_Datastore_stub::const_iterator*>(rhs)->i; }
				bool operator!=(const StorableTaskDatastore::const_iterator* rhs)	{ return i!=dynamic_cast<const Storage_Datastore_stub::const_iterator*>(rhs)->i; }
				std::list<Task>::const_iterator getIterator()						{ return i; }
			private:
				std::list<Task>::const_iterator i;
		};

		Storage_Datastore_stub::const_iterator*	cbeginPtr	()						{ return new Storage_Datastore_stub::const_iterator(_db.cbegin()); }
		Storage_Datastore_stub::const_iterator*	cendPtr		()						{ return new Storage_Datastore_stub::const_iterator(_db.cend()); }
		void									addTask		(const Task& newTask)	{ _db.push_back(newTask); }
		std::list<Task>							getTaskList	()						{ return _db; }
		void									emptyStore	();
};
#endif
