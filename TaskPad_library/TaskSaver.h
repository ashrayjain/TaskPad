#ifndef _TASKSAVER_H_
#define _TASKSAVER_H_

#include <list>
#include <fstream>
#include "FileHandler.h"

class Task;

class TaskSaver :public FileHandler
{
	public:
		virtual bool save (std::list<Task>& taskList) =0;
		virtual bool save (const Task& task) =0;
	protected:
		virtual void openFile(const std::string& fileName, std::ios_base::openmode = std::ios::trunc) =0;
		virtual void closeFile() =0;

		std::ofstream _fileWriter;
};

#endif