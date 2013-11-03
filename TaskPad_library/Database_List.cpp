/*
 * =====================================================================================
 *
 *       Filename:  Database_List.cpp
 *
 *        Version:  1.0
 *        Created:  10/28/13 15:47:28
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Database_List.h"

using namespace std;

list<Task> Database_List::getAllTasks() {
	list<Task> taskList;
	for (list<Task>::iterator i = _db.begin(); i != _db.end(); i++)
		taskList.push_back(Task(*i));
	return taskList;
}


