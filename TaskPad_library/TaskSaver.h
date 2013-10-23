/*
 * =====================================================================================
 *
 *		Filename:  TaskSaver.h
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file defines an abstract TaskSaver Class that inherits from the FileHandler Class
  * This class defines what it means to be a saver of tasks by specifying the necessary
  * functions that such a saver must implement.
  *
 */

#ifndef _TASKSAVER_H_
#define _TASKSAVER_H_

#include <list>
#include <fstream>
#include "FileHandler.h"
#include "Enum.h"

class Task;

class TaskSaver :public FileHandler
{
	public:
		virtual void save (const std::list<Task>& taskList, const std::string& fileName) =0;
		virtual void save (const Task& task, const TP::COMMAND_TYPE& cType) =0;
	protected:
		virtual void updateSaveRecord	(std::string entry) =0;
		virtual void removeSaveRecord	() =0;

		void openFile(const std::string& fileName, std::ios_base::openmode oMode = std::ios::trunc)	
							{ _fileWriter.open(fileName,oMode); }
		
		void closeFile()	{ _fileWriter.close(); }

		std::ofstream _fileWriter;
};

#endif