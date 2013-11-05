/*
 * =====================================================================================
 *
 *       Filename:  ExecutableTaskDatastore.h
 *
 *        Version:  1.0
 *        Created:  11/02/13 10:14:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#ifndef _EXECUTABLE_DSTORE_
#define _EXECUTABLE_DSTORE_

#include <list>
#include "Task.h"
#include "Datastore_Base.h"

class ExecutableTaskDatastore
{
	public:
		virtual void addTask(const Task&)=0;
		virtual void deleteTask(const unsigned &pos)=0;
		
		virtual ~ExecutableTaskDatastore()=0 {};
};
#endif