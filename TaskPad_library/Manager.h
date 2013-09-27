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
		void insertCreatedTimeIntoCommand();
		void insertCreatedTimeIntoModifyCommand();
		void insertCreatedTimeIntoDeleteCommand();

		Task* getPointerToChosenTask() const;
		unsigned getCreatedTimeOfTask(Task* baseTask) const;
		unsigned getCreatedTimeOfDeadlineTask(Task* baseTask) const;
		unsigned getCreatedTimeOfTimedTask(Task* baseTask) const;
		unsigned getCreatedTimeOfFloatingTask(Task* baseTask) const;


	public:
		//constructor
		Manager();

		Messenger processCommand(const string&);
		Messenger getToday();
		void resetStatus();

		~Manager();
};

#endif