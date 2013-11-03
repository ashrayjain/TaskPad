/*
 * =====================================================================================
 *
 *		Filename:  TaskLoaderText.h
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file defines the TaskLoaderText Class, its functions and attributes
  * This class inherits from the TaskLoader Class and implements the load function
  * defined in the TaskLoader Class. 
  * 
  * This class handles loading of tasks from a text file
  *
 */

#ifndef _TASKLOADERTEXT_H_
#define _TASKLOADERTEXT_H_

#include <list>
#include <set>
#include "TaskLoader.h"
#include "Enum.h"

class Logger;

class TaskLoaderText :public TaskLoader
{
	public:
		void load (std::list<Task>& taskList, const std::string& fileName);
		TaskLoaderText();
	private:

		Logger* _logger;

		//recovery system
		std::set<std::string> recoveredIndices;

		void recoverUnsavedChanges		(std::list<Task>& taskList);
		void loadDeletedIndices			();
		void loadModifiedTasks			(std::list<Task>& taskList);
		void validateAndAddTaskToList	(std::list<Task>& taskList, const Task& nextTask);

		//loader
		void loadTaskList				(std::list<Task>& taskList);

		//helper functions
		std::string			getNewLabel				(std::string);
		std::string			getNewData				(std::string);
		Task				getNextTask				();
		bool				validateAndCreateTask	(Task& newTask, const std::string& newData);
		void				skipThisTask			();
		unsigned long long	getTaskIndex			(std::string);
		bool				hasNextTask				();

		// task constructing functions
		Task createNewTask			(unsigned long long index);
		void setTaskName			(Task& task, std::string name);
		void setTaskDueDate			(Task& task, std::string dueDate);
		void setTaskFromDate		(Task& task, std::string fromDate);
		void setTaskToDate			(Task& task, std::string toDate);
		void setTaskLocation		(Task& task, std::string location);
		void setTaskParticipant		(Task& task, std::string participant);
		void setTaskNote			(Task& task, std::string note);
		void setTaskPriority		(Task& task, std::string taskPriority);
		void setTaskTag				(Task& task, std::string tag);
		void setTaskReminderTime	(Task& task, std::string reminderTime);
		void setTaskState			(Task& task, std::string taskState);
		//reader
		std::string getNextLineFromFile();

		// reverse string converters
		int					getIntFromString		(std::string attribute);
		unsigned long long	getIndexFromString		(std::string attribute);
		time_t				getTimeFromString		(std::string attribute);
		TP::PRIORITY		getPriorityFromString	(std::string attribute);
		TP::TASK_STATE		getTaskStateFromString	(std::string attribute);
};

#endif