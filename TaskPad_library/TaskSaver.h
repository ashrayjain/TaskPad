#ifndef _TASKSAVER_H_
#define _TASKSAVER_H_

#include <list>
#include <fstream>
#include "FileHandler.h"

class Task;

class TaskSaver :public FileHandler
{
	public:
		virtual void save (const std::list<Task>& taskList, const std::string& fileName) =0;
		virtual void save (const Task& task) =0;
	protected:
		void openFile(const std::string& fileName, std::ios_base::openmode oMode = std::ios::trunc)	
							{ _fileWriter.open(fileName,oMode); }
		
		void closeFile()	{ _fileWriter.close(); }

		std::ofstream _fileWriter;
};

#endif