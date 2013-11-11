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
  * This file defines an abstract TaskLoader Class that inherits from the TaskFileHandler Class
  * This class defines what it means to be a loader for tasks by specifying the necessary
  * functions that such a loader must implement.
  *
 */

#ifndef _TASKLOADER_H_
#define _TASKLOADER_H_

#include <list>
#include <fstream>
#include "TaskFileHandler.h"

class Task;
class StorableTaskDatastore;

class TaskLoader: public TaskFileHandler {
	public:
		virtual void load (const std::string& fileName) =0;
		virtual ~TaskLoader() {}
	protected:

		StorableTaskDatastore* _taskDS;
		void openFile(const std::string& fileName, std::ios_base::openmode oMode = std::ios::in);
		void closeFile();

		static const std::string MESSAGE_FILE_OPENING_ERROR;
		static const std::string MESSAGE_FILE_CLOSING_ERROR;
		static const std::string MESSAGE_RESTART_ADVICE;

		std::ifstream _fileReader;
};

#endif