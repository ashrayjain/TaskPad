#ifndef _STORAGE_H_
#define _STORAGE_H_

#include<fstream>
#include<list>
#include "Enum.h"

//forward definitions
class Task;
class Command;
class Logger;
class TaskLoader;
class TaskSaver;

class Storage
{
	private:
		Logger* _logger;
		TaskLoader* _loader;
		TaskSaver* _saver;

		static const std::string _fileName;

	public:
		Storage (std::list<Task>& taskList);

		bool save	(const std::list<Task>&);
		bool save	(const Task& task, const TP::COMMAND_TYPE& cType);

		void load	(std::list<Task>& taskList);

		const std::string FILENAME;

		~Storage();
};
#endif