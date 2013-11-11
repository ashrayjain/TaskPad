/*
 * =====================================================================================
 *
 *		Filename:  StorableTaskDatastore.h
 *
 *      Version:  1.0
 *
 *      Authors:  Thyaegsh Manikandan (A0100124J), Ashray Jain (A0105199B)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file defines the necessary functions that a TaskDataStore needs to 
  * have in order to be saved to and loaded from the file system. It also 
  * helps to hide the rest of the APIs of a Datastore implemenentation
  * from the Storage Component.
 */

#ifndef _STORABLE_DSTORE_H_
#define _STORABLE_DSTORE_H_

#include <list>
#include "Task.h"

// @author A0105199B

class StorableTaskDatastore {
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

		virtual StorableTaskDatastore::const_iterator* cbeginPtr	()=0;
		virtual StorableTaskDatastore::const_iterator* cendPtr		()=0;


		virtual ~StorableTaskDatastore()=0 {};
};
#endif