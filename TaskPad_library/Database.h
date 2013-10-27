/*
 * =====================================================================================
 *
 *       Filename:  Database.h
 *
 *        Version:  1.0
 *        Created:  10/25/13 00:22:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#ifndef TASKPAD_DATABASE_H
#define TASKPAD_DATABASE_H

#include <list>
#include "Task.h"

/*
 *=======================
 
	Database Base class
 
 *=======================
 */

class Database
{
public:
	virtual	~Database() = 0;
};

#endif
