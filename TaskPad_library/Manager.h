#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "Messenger.h"

// forward declarations
class Interpreter;
class Executor;
class Storage;
class Command;

class Manager {
	private:
		//other components
		Interpreter*	_interpreter;
		Executor*		_executor;
		Storage*		_storage;

		//state determinants
		Command*	_cmd;
		Messenger	_response;
		int			_index;
		std::tm		_currTm;

		//the list of tasks
		list<Task>	_tasks;

		//saver
		void saveChanges	();

		//helper functions
		bool hasNoError					();
		bool isSuccessfulCommand		();
		bool hasInterpretationError		();
		bool hasNoInterpretationError	();
		bool isIndexGiven				(string newCommand);
		bool isCommandWithIndexGiven	(string newCommand);
		bool isIndexedModifyCommand		();
		bool isIndexedDeleteCommand		();
		bool isIndexWithinRange			();

		bool isDeleteCommand			();
		bool isModifyCommand			();

		void handleIndexCommand			();
		void handleCommandWithIndex		();
		void handleGenericCommand		();

		void handleNormalScenarioCommands			(string newCommand);
		void handleIntermediateScenarioCommands		(string newCommand);
		void insertCreatedTimeIntoCommand			();
		void insertCreatedTimeIntoModifyCommand		();
		void insertCreatedTimeIntoDeleteCommand		();
		void removePreviousCommand					();
		void storeIndexFromCommandToClassAttribute	();

		void editTaskListInResponse					();

		//state determinant functions
		void setCurrTm	(std::tm);

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

		Messenger	processCommand		(const string&);
		Messenger	getTodayTasks		();
		Messenger	getNextPeriodTasks	(PERIOD_TYPE);
		Messenger	getPrevPeriodTasks	(PERIOD_TYPE);
		void		resetStatus			();

		~Manager();

		// response messages
		static const string MESSAGE_INDEX_OUT_OF_RANGE;
		static const string MESSAGE_ERROR_UNEXPECTED_COMMAND_TYPE_WITH_INDEX;
};

#endif