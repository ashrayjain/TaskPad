#include "Manager.h"
#include "Storage.h"
#include "Messenger.h"
#include "Executor.h"
#include<ctime>

Manager::Manager()
{
	this->_storage = new Storage(_tasks);
	this->_executor = new Executor(&_tasks);
	this->_interpreter = new Interpreter;
	this->_response = Messenger();
}

Messenger Manager::processCommand(string newCommand)
{
	switch(this->_response.getStatus())
	{
		case SUCCESS:
		case SUCCESS_INDEXED_COMMAND:
		case ERROR:
		case DISPLAY:
			handleNormalScenarioCommands(newCommand);
			break;
		case INTERMEDIATE:
		case ERROR_INTERMEDIATE:
			handleIntermediateScenarioCommands(newCommand);
			break;
	}
	return this->_response;
}

void Manager::handleNormalScenarioCommands(string newCommand)
{
	if(isIndexGiven(newCommand))
	{
		this->_response.setStatus(DISPLAY);
	}
	else if (isCommandWithIndexGiven(newCommand))
	{
		this->_response.setStatus(SUCCESS_INDEXED_COMMAND);
	}
	else // a generic command
	{
		this->handleGenericCommand(newCommand);
	}

	this->_cmd = this->_interpreter->interpretCommand(newCommand,this->_response);

	if(this->hasInterpretationError())
	{
		return;
	}
	//else
	this->_executor->executeCommand(this->_cmd, this->_response);
	
	return;
}

bool Manager::isIndexGiven(string newCommand)
{

}

bool Manager::hasInterpretationError()
{
	if(this->_response.getStatus() == ERROR)
	{
		return true;
	}
	//else
	return false;
}

Messenger Manager::getToday()
{
	time_t rawtime;
	struct tm * timeinfo;
	char todayCharArray [80];

	//get the time right now, convert it to a string in the format DD/MM/YYYY
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (todayCharArray,80,"%d/%m/%Y",timeinfo);

	//convert the char array to a string
	string today(todayCharArray);

	return this->processCommand("find due " + today + " from "+today + " to "+today);
}

void Manager::resetStatus()
{
}