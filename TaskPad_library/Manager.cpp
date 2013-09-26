#include "Manager.h"
#include "Storage.h"


Manager::Manager()
{
	this->_executor = new Executor;
	this->_interpreter = new Interpreter;
	this->_storage = new Storage;
}

Messenger Manager::processCommand(string newCommand)
{
	
}

void Manager::resetStatus()
{
}