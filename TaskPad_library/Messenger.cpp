#include "Messenger.h"
#include "Task.h"

using namespace std;

// constructors
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
list<Task> Messenger::getIntermediateList()
{
	return this->_resultList;
}
list<Task> Messenger::getResultList()
{
	return this->getIntermediateList();
}
list<Task> Messenger::getList()
{
	return this->getIntermediateList();
}

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
}
bool Messenger::setList(list<Task> result)
{
	this->_resultList = result;
	return true;
}