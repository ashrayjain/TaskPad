#include "Messenger.h"
#include "Task.h"

using namespace std;

// constructor

Messenger::Messenger(STATUS status=SUCCESS, list<Task> result= list<Task>(), int index =-1, string errorMsg="")
{
	this->_status = status;
	this->_resultList = result;
	this->_errorMsg = errorMsg;
	this->_index = index;
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