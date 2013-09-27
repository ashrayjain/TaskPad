#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "Messenger.h"

// forward declarations
class Interpreter;
class Executor;
class Storage;
class Command;

class Manager
{
	private:
		//other components
		Interpreter* _interpreter;
		Executor* _executor;
		Storage* _storage;

		//state determinants
		Command* _cmd;
		Messenger _response;
		int _index;
		//the list of tasks
		list<Task> _tasks;

		//helper functions
		bool hasInterpretationError();
		bool hasNoInterpretationError();
		bool isIndexGiven(string newCommand);
		bool isCommandWithIndexGiven(string newCommand);
		bool isIndexedModifyCommand();
		bool isIndexedDeleteCommand();
		bool isIndexWithinRange();

		void handleGenericCommand(string newCommand);
		void handleNormalScenarioCommands(string newCommand);
		void handleIntermediateScenarioCommands(string newCommand);
		void insertCreatedTimeIntoModifyCommand();
		void insertCreatedTimeIntoDeleteCommand();

		Task* getPointerToChosenTask();
		unsigned getCreatedTimeOfTask(Task* baseTask);
		unsigned getCreatedTimeOfDeadlineTask(Task* baseTask);
		unsigned getCreatedTimeOfTimedTask(Task* baseTask);
		unsigned getCreatedTimeOfFloatingTask(Task* baseTask);


	public:
		//constructor
		Manager();

		Messenger processCommand(string);
		Messenger getToday();
		void resetStatus();

		~Manager();
};

#endif