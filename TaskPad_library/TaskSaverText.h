/*
 * =====================================================================================
 *
 *		Filename:  TaskSaverText.h
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file defines the TaskSaverText Class, its functions and attributes
  * This class inherits from the TaskSaver Class and implements the save function
  * defined in the TaskSaver Class. 
  * 
  * This class handles saving of tasks to a text file
  *
 */

#pragma once

#include <list>
#include "TaskSaver.h"
#include "Enum.h"

class Logger;

class TaskSaverText: public TaskSaver
{
	public:
		void save						(StorableTaskDatastore* taskDS, const std::string& fileName);
		void save						(const Task& task, const TP::COMMAND_TYPE& cType);

		TaskSaverText();
	private:

		Logger* _logger;

		//save records
		void saveDeleteCommand			(const Task& task);
		void saveNonDeleteCommands		(const Task& task);
		void removeTaskFile				(const Task& task);
		void removeDeletedTaskFile		(const Task& task);
		void updateSaveRecord			(const std::string& entry);
		void removeSaveRecord			();
		void updateDeleteRecord			(const unsigned long long& entry);
		void removeDeleteRecord			();

		//savers
		void saveTaskList				(const std::list<Task>& taskList);
		void saveTaskDS					(StorableTaskDatastore* taskDS);
		void saveTask					(const			 Task &	task);
		void saveTaskAttributes			(const			 Task &	tempTaskTask);

		//file level attributes
		void saveTaskLevelLabel			(std::string LabelStr);
		void saveAttributeLevelLabel	(std::string LabelStr);

		//saving generic attributes of all tasks
		void saveIndex					(const Task& tempTask);
		void saveName					(const Task& tempTask);
		void saveLocation				(const Task& tempTask);
		void saveParticipants			(const Task& tempTask);
		void saveNote					(const Task& tempTask);
		void savePriority				(const Task& tempTask);
		void saveTags					(const Task& tempTask);
		void saveReminderTimes			(const Task& tempTask);
		void saveState					(const Task& tempTask);
		void saveDueDate				(const Task& tempTask);
		void saveFromDate				(const Task& tempTask);
		void saveToDate					(const Task& tempTask);

		// std::string converters
		std::string convertToString		(int num);
		std::string convertToString		(unsigned long long index);
		std::string convertToString		(time_t time);
		std::string convertToString		(TP::PRIORITY priority);
		std::string convertToString		(TP::TASK_STATE state);

		//writers
		void writeLineToFile			(std::string line, bool newLine = true);
				
		//removes Task files
		void removeTaskFiles();
};
