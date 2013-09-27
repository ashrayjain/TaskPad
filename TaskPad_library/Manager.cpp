#include "Manager.h"
#include "Storage.h"
#include "Messenger.h"
#include "Executor.h"
#include "Command.h"
#include<ctime>
/*********
Assumptions made:
- Both interpreter and Executor do not change the status of the response object unless it is an error/intermediate
**********/

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
		if(this->hasNoInterpretationError())
		{
			this->_response.setStatus(DISPLAY);
			this->_executor->executeCommand(this->_cmd,this->_response);
		}
	}
	else if (isCommandWithIndexGiven(newCommand))
	{
		if(this->hasNoInterpretationError())
		{
			this->_response.setStatus(SUCCESS_INDEXED_COMMAND);
			this->_executor->executeCommand(this->_cmd,this->_response);
		}
	}
	else // a generic command and has already been interpreted by isCommandWithIndexGiven()
	{
		if(this->hasNoInterpretationError())
		{
			this->_executor->executeCommand(this->_cmd,this->_response);
		}
	}
	return;
}

void Manager::handleIntermediateScenarioCommands(string newCommand)
{

}

bool Manager::isIndexGiven(string newCommand)
{
	this->_cmd = this->_interpreter.interpretIndex(newCommand,this->_response);
	if(this->_response.getStatus() != ERROR)
	{
		return true;
	}
	return false;
}

bool Manager::isCommandWithIndexGiven(string newCommand)
{
	bool isModifyCommandWithIndex = false, isDeleteCommandWithIndex = false;
	this->_cmd = this->_interpreter.interpretCommand(newCommand,this->_response);

	switch (this->_cmd->getCommandType)
	{
		case this->_cmd->MOD:
			isModifyCommandWithIndex = this->hasIndexModifyCommand();
			break;
		case this->_cmd->DEL:
			isDeleteCommandWithIndex = this->hasIndexDeleteCommand();
			break;
		default:
			break;
	}
	if(isModifyCommandWithIndex || isDeleteCommandWithIndex)
	{
		this->_executor->executeCommand(this->_cmd,this->_response);
		return true;
	}
	//else
	return false;
}

bool Manager::hasIndexModifyCommand()
{
	Command_Mod tempCommand = *((Command_Mod *) this->_cmd);
	return tempCommand.getFlagIndex();
}

bool Manager::hasIndexDeleteCommand()
{
	Command_Del tempCommand = *((Command_Del *) this->_cmd);
	return tempCommand.getFlagIndex();
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

bool Manager::hasNoInterpretationError()
{
	return !this->hasInterpretationError();
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
	delete this->_cmd;
	this->_response.resetMessenger();
}