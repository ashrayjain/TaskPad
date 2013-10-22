/*
 * =====================================================================================
 *
 *		Filename:  TaskLoader.h
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file defines an abstract TaskLoader Class that inherits from the FileHandler Class
  * This class defines what it means to be a loader for tasks by specifying the necessary
  * functions that such a loader must implement.
  *
 */

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
		void openFile(const std::string& fileName, std::ios_base::openmode oMode = std::ios::in)
											{ this->_fileReader.open(fileName,oMode); }
		void closeFile() { _fileReader.close(); }

		std::ifstream _fileReader;
};

#endif