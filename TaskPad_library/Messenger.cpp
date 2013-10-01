#include "Messenger.h"
#include "Task.h"
#include "Enum.h"

using namespace std;
using namespace TP;

// constructor

Messenger::Messenger(COMMAND_TYPE commandType,STATUS status, list<Task> result, int index, string errorMsg)
{
	this->_status = status;
	this->_resultList = result;
	this->_errorMsg = errorMsg;
	this->_index = index;
	this->_commandType = commandType;
}

//getter methods

string Messenger::getErrorMsg()  const
{
	return this->_errorMsg;
}

STATUS Messenger::getStatus() const
{
	return this->_status;
}

list<Task> Messenger::getList() const
{
	return this->_resultList;
}

int Messenger::getIndex()  const
{
	return this->_index;
}

COMMAND_TYPE Messenger::getCommandType() const
{
	return this->_commandType;
}

//setter methods

bool Messenger::setErrorMsg(const string& errorMsg)
{
	if(errorMsg != "")
	{
		this->_errorMsg = errorMsg;
		return true;
	}
	else
	{
		return false;
	}
}

bool Messenger::setStatus(const STATUS& status)
{
	this->_status = status;
	return true;
}

bool Messenger::setList(const list<Task>& result)
{
	this->_resultList = result;
	return true;
}

bool Messenger::setInt(const int& index)
{
	if (isValidIndex(index))
	{
		this->_index = index;
		return true;
	}
	return false;
}

//helper for setInt
bool Messenger::isValidIndex (const int& index) const
{
	if(index > 0)
		return true;

	return false;
}

bool Messenger::setCommandType(const COMMAND_TYPE& commandType)
{
	this->_commandType = commandType;
	return true;
}

void Messenger::resetMessenger()
{
	this->setInt(-1);
	this->setStatus(SUCCESS);
	this->setErrorMsg("");
	this->setList(list<Task>());
}