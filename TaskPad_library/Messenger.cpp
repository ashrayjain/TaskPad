#include "Messenger.h"
#include "Task.h"

using namespace std;

// constructor
Messenger::Messenger()
{
	list<Task> tempList;
	this->_resultList = tempList;
	this->_errorMsg = "";
	this->_index = -1;
	this->_status = SUCCESS;
}

Messenger::Messenger(string errorMsg, STATUS status, list<Task> result)
{
	this->_errorMsg = errorMsg;
	this->_status = status;
	this->_resultList = result;
}

//getter methods
		
string Messenger::getErrorMsg()
{
	return this->_errorMsg;
}

STATUS Messenger::getStatus()
{
	return this->_status;
}

list<Task> Messenger::getList()
{
	return this->_resultList;
}

int Messenger::getIndex()
{
	return this->_index;
}

//setter methods

bool Messenger::setErrorMsg(string errorMsg)
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

bool Messenger::setStatus(STATUS status)
{
	this->_status = status;
	return true;
}

bool Messenger::setList(list<Task> result)
{
	this->_resultList = result;
	return true;
}

bool Messenger::setInt(int index)
{
	if (isValidIndex(index))
	{
		this->_index = index;
		return true;
	}
	return false;
}

bool Messenger::isValidIndex (int index)
{
	if(index > 0)
		return true;

	return false;
}