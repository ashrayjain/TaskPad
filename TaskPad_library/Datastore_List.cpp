/*
 * =====================================================================================
 *
 *       Filename:  Datastore_List.cpp
 *
 *        Version:  1.0
 *        Created:  10/28/13 15:47:28
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Datastore_List.h"

using namespace std;
using namespace TP;

//@ASHRAY JAIN A0105199B

list<Task> Datastore_List::getAllTasks() {
	list<Task> taskList;
	for (list<Task>::iterator i = _db.begin(); i != _db.end(); i++)
		taskList.push_back(Task(*i));
	return taskList;
}


