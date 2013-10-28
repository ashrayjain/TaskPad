/*
 * =====================================================================================
 *
 *		Filename:  Storage.h
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file defines the Storage Class, its functions and attributes
  * The Storage Class is responsible for saving the current program state to file and loading 
  * tasks from file. Uses a TaskLoader and a TaskSaver object to achieve this
  * 
 */

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
class StorableTaskDatabase;

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
		bool save	(StorableTaskDatabase* taskDB) {};
		bool save	(const Task& task, const TP::COMMAND_TYPE& cType);

		void load	(std::list<Task>& taskList);
		void load	(StorableTaskDatabase* taskDB) {};

		const std::string FILENAME;

		~Storage();
};
#endif