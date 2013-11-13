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
  * This file defines an abstract TaskSaver Class that inherits from the TaskFileHandler Class
  * This class defines what it means to be a saver of tasks by specifying the necessary
  * functions that such a saver must implement.
  *
 */

#ifndef _TASKSAVER_H_
#define _TASKSAVER_H_

#include <list>
#include <fstream>
#include "TaskFileHandler.h"
#include "Enum.h"

class Task;
class StorableTaskDatastore;

class TaskSaver :public TaskFileHandler {
	public:
		virtual void save (StorableTaskDatastore* taskDB, const std::string& fileName) =0;
		virtual void save (const Task& task, const TP::COMMAND_TYPE& cType) =0;
	protected:

		void openFile(const std::string& fileName, std::ios_base::openmode oMode = std::ios::trunc);
		void closeFile();

		static const std::string MESSAGE_FILE_OPENING_ERROR;
		static const std::string MESSAGE_FILE_CLOSING_ERROR;
		static const std::string MESSAGE_RESTART_ADVICE;

		std::ofstream _fileWriter;
};

#endif