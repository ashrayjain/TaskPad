/*
 * =====================================================================================
 *
 *		Filename:  Manager.h
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file defines all the functions and attributes of the Manager Class
  * This class serves as a facade for the rest of the components that collectively
  * interpret, execute and save the changes, thus processing the command the user gives
 */

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "Messenger.h"

// forward declarations
class Interpreter;
class Executor;
class Storage;
class Command;
class Logger;
class Datastore;

class Manager {
	private:
		//other components
		Interpreter*	_interpreter;
		Executor*		_executor;
		Storage*		_storage;
		Logger*			_logger;

		//state determinants
		Command*	_cmd;
		Command*	_lastSuccessfulFindCmd;
		Messenger	_response;
		int			_index;
		std::pair<tm,tm> _currentPeriod;

		//the list of tasks
		Datastore*	_taskDS;

		//saver
		void saveChanges							();

		//helper functions
		void setResponseToError						(const std::string& newCommand);
		bool hasNoError								();
		bool isSuccessfulCommand					();
		bool hasInterpretationError					();
		bool hasNoInterpretationError				();
		bool isIndexGiven							(std::string newCommand);
		bool isCommandWithIndexGiven				(std::string newCommand);
		bool isIndexedModifyCommand					();
		bool isIndexedDeleteCommand					();
		bool isIndexWithinRange						();

		bool isDeleteCommand						();
		bool isModifyCommand						();

		void handleIndexCommand						();
		void handleCommandWithIndex					();
		void handleGenericCommand					();

		void handleNormalScenarioCommands			(std::string newCommand);
		void handleIntermediateScenarioCommands		(std::string newCommand);
		void insertActualIndexIntoCommand			();
		void insertActualIndexIntoModifyCommand		();
		void insertActualIndexIntoDeleteCommand		();
		void removePreviousCommand					();
		void removeLastSuccessfulFindCommand		();
		void updateLastSuccessfulFindCommand		();
		void storeIndexFromCommandToClassAttribute	();

		void editTaskListInResponse					();

		//state determinant functions
		void setCurrPeriod	(std::tm, std::tm);

		// helpers for getting list of tasks in a specified period
		Task				getPointerToChosenTask		()							const;
		unsigned long long	getCreatedTimeOfTask		(Task baseTask)				const;
		std::tm				getTodayTm					();
		std::tm				getNextDayTm				(std::tm currTm);
		std::tm				getNextWeekTm				(std::tm currTm);
		std::tm				getNextMonthTm				(std::tm currTm);
		std::tm				getPrevDayTm				(std::tm currTm);
		std::tm				getPrevWeekTm				(std::tm currTm);
		std::tm				getPrevMonthTm				(std::tm currTm);
		std::string			getStrFromTm				(std::tm timeInfo);
		std::string			createFindCommand			(std::tm startTm, std::tm endTm);

	public:
		//constructor
		Manager();

		Messenger			processCommand		(const std::string&);
		Messenger			getTodayTasks		();
		Messenger			getNextPeriodTasks	(TP::PERIOD_TYPE);
		Messenger			getPrevPeriodTasks	(TP::PERIOD_TYPE);
		void				resetStatus			();
		Messenger			refreshList			();
		std::list<Task>		getCurrentReminders	();
		std::pair<tm,tm>	getCurrentPeriod	();
		void				syncTaskList		(const std::list<Task>&);
		void				syncTask			(const Task&);

		//destructor
		~Manager();

		// response messages
		static const std::string MESSAGE_INDEX_OUT_OF_RANGE;
		static const std::string MESSAGE_ERROR_UNEXPECTED_COMMAND_TYPE_WITH_INDEX;
		static const std::string MESSAGE_DATE_LIMIT_REACHED;
};

#endif