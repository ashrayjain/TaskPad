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

	public:
		//constructor
		Manager();

		Messenger processCommand(string);
		Messenger getToday();
		void resetStatus();

		~Manager();
};

#endif