/*
 * =====================================================================================
 *
 *		Filename:  TaskFileHandler.cpp
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file implements all of constants defined in the TaskFileHandler.h file
  * 
 */

#ifndef _TASKFILEHANDLER_CPP_
#define _TASKFILEHANDLER_CPP_

#include "TaskFileHandler.h"
#include <Task.h>
#include <sstream>

const std::string TaskFileHandler::TASK_DIRECTORY				= "Tasks\\";

const std::string TaskFileHandler::LABEL_START_OF_TASK			= "StartOfTask";
const std::string TaskFileHandler::LABEL_NAME					= "name:";
const std::string TaskFileHandler::LABEL_INDEX					= "index:";
const std::string TaskFileHandler::LABEL_DUE_DATE				= "due:";
const std::string TaskFileHandler::LABEL_FROM_DATE				= "from:";
const std::string TaskFileHandler::LABEL_TO_DATE				= "to:";
const std::string TaskFileHandler::LABEL_LOCATION				= "at:";
const std::string TaskFileHandler::LABEL_PARTICIPANT			= "ppl:";
const std::string TaskFileHandler::LABEL_NOTE					= "note:";
const std::string TaskFileHandler::LABEL_PRIORITY				= "impt:";
const std::string TaskFileHandler::LABEL_TAG					= "#:";
const std::string TaskFileHandler::LABEL_REMINDER_TIME			= "rt:";
const std::string TaskFileHandler::LABEL_STATE					= "state:";
const std::string TaskFileHandler::LABEL_END_OF_TASK			= "EndOfTask";

std::string TaskFileHandler::getTaskFilePath	(const Task& task) {
	std::stringstream tempStream;
	tempStream << TASK_DIRECTORY;
	tempStream << (task.getIndex());
	tempStream << ".task";

	std::string taskFilePath = tempStream.str();

	return taskFilePath;
}

std::string TaskFileHandler::getDeletedTaskFilePath	(const Task& task)
{
	std::stringstream tempStream;
	tempStream << TASK_DIRECTORY;
	tempStream << (task.getIndex());
	tempStream << ".deltask";

	std::string taskFilePath = tempStream.str();

	return taskFilePath;
}

#endif
