#ifndef _TASKLOADER_H_
#define _TASKLOADER_H_

#include <list>
#include <fstream>
#include "FileHandler.h"

class Task;

class TaskLoader: public FileHandler
{
	public:
		virtual void load (std::list<Task>& taskList, const std::string& fileName) =0;
		~TaskLoader() {}
	protected:
		virtual void openFile(const std::string& fileName, std::ios_base::openmode = std::ios::in) =0;
		virtual void closeFile() =0;

		std::ifstream _fileReader;
		bool _isFileMishandled;
};

#endif