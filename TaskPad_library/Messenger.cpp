/*
 * =====================================================================================
 *
 *		Filename:  Messenger.cpp
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file implements all of the functions defined in Messenger.h file
  * 
 */

#include "Messenger.h"
#include "Task.h"
#include "Enum.h"

using namespace std;
using namespace TP;

const int				Messenger::DEFAULT_INDEX_VALUE = -1;
const TP::STATUS		Messenger::DEFAULT_STATUS_VALUE = SUCCESS;
const TP::COMMAND_TYPE	Messenger::DEFAULT_COMMAND_TYPE_VALUE = UNDEFINED;
const Task				Messenger::DEFAULT_TASK_VALUE = Task();
const list<Task>		Messenger::DEFAULT_TASK_LIST_VALUE = list<Task>();
const std::string		Messenger::DEFAULT_ERROR_MESSAGE_VALUE = "";

// constructor
Messenger::Messenger(COMMAND_TYPE commandType,STATUS status, list<Task> resultList, Task resultTask, int index, string errorMsg)
{
	this->jvmCreated = status;
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
	return this->jvmCreated;
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
	this->jvmCreated = status;
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
	this->setInt			(DEFAULT_INDEX_VALUE);
	this->setStatus			(DEFAULT_STATUS_VALUE);
	this->setErrorMsg		(DEFAULT_ERROR_MESSAGE_VALUE);
	this->setList			(DEFAULT_TASK_LIST_VALUE);
	this->setTask			(DEFAULT_TASK_VALUE);
	this->setCommandType	(DEFAULT_COMMAND_TYPE_VALUE);
}