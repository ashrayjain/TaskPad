#include "Messenger.h"
#include "Task.h"
#include "Enum.h"

using namespace std;
using namespace TP;

// constructor

Messenger::Messenger(COMMAND_TYPE commandType,STATUS status, list<Task> resultList, Task resultTask, int index, string errorMsg)
{
	this->_status = status;
	this->_resultList = resultList;
	this->_resultTask = resultTask;
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

Task Messenger::getTask() const
{
	return this->_resultTask;
}

//setter methods

void Messenger::setErrorMsg(const string& errorMsg)
{
	this->_errorMsg = errorMsg;
}

void Messenger::setStatus(const STATUS& status)
{
	this->_status = status;
}

void Messenger::setList(const list<Task>& result)
{
	this->_resultList = result;
}

void Messenger::setInt(const int& index)
{
	this->_index = index;
}

void Messenger::setCommandType(const COMMAND_TYPE& commandType)
{
	this->_commandType = commandType;
}

void Messenger::setTask(const Task& task)
{
	this->_resultTask = task;
}

void Messenger::resetMessenger()
{
	this->setInt(-1);
	this->setStatus(SUCCESS);
	this->setErrorMsg("");
	this->setList(list<Task>());
	this->setTask(Task());
	this->setCommandType(UNDEFINED);
}