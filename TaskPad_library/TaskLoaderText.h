#ifndef _TASKLOADERTEXT_H_
#define _TASKLOADERTEXT_H_

#include "TaskLoader.h"

class TaskLoaderTest: public TaskLoader
{
	public:
		bool load (list<Task>& taskList, std::string fileName);
}

#endif