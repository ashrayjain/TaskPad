#include "stdafx.h"
#include "Executor_Datastore_stub.h"

using namespace std;

void Executor_Datastore_stub::addTask(const Task &newTask) {
	_ds.push_back(newTask); 
	Command_Del* tmp_del = new Command_Del();
	Command_Add* tmp_add = new Command_Add();
	tmp_del->setName("Test Task");
	delete undoCmd.first;
	delete redoCmd.first;
	undoCmd = pair<Command*, Task>(tmp_del, Task());
	redoCmd = pair<Command*, Task>(tmp_add, Task());
}
