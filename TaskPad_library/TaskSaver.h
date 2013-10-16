#ifndef _TASKSAVER_H_
#define _TASKSAVER_H_

#include <list>
#include <fstream>

class Task;

class TaskSaver
{
	public:
		virtual bool save (std::list<Task>& taskList);
		virtual bool save (const Task& task);
	protected:
};

#endif