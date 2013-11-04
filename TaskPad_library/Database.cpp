/*
 * =====================================================================================
 *
 *       Filename:  Database.cpp
 *
 *        Version:  1.0
 *        Created:  10/25/13 00:22:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Database.h"

using namespace std;

void Database_Restricted::addTask(Task newTask) {
	_db->addTask(newTask);
	_indexHash[newTask.getIndex()] = &(_db->back());
	//handleHashTagPtrs(_db->back(), _db->back().getTags());
	//handleRemindTimesPtrs(_db->back(), _db->back().getRemindTimes());
}

