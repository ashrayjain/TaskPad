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

#include "TaskFileHandler.h"
#include <Task.h>
#include <sstream>

using namespace std;
// @author A0100124J

const string TaskFileHandler::TASK_DIRECTORY				= "Tasks\\";
const string TaskFileHandler::TASK_FILE_EXTENSION			= ".task";
const string TaskFileHandler::TASK_DELETED_FILE_EXTENSION	= ".deltask";

const string TaskFileHandler::LABEL_START_OF_TASK			= "StartOfTask";
const string TaskFileHandler::LABEL_NAME					= "name:";
const string TaskFileHandler::LABEL_INDEX					= "index:";
const string TaskFileHandler::LABEL_DUE_DATE				= "due:";
const string TaskFileHandler::LABEL_FROM_DATE				= "from:";
const string TaskFileHandler::LABEL_TO_DATE					= "to:";
const string TaskFileHandler::LABEL_LOCATION				= "at:";
const string TaskFileHandler::LABEL_PARTICIPANT				= "ppl:";
const string TaskFileHandler::LABEL_NOTE					= "note:";
const string TaskFileHandler::LABEL_PRIORITY				= "impt:";
const string TaskFileHandler::LABEL_TAG						= "#:";
const string TaskFileHandler::LABEL_REMINDER_TIME			= "rt:";
const string TaskFileHandler::LABEL_STATE					= "state:";
const string TaskFileHandler::LABEL_END_OF_TASK				= "EndOfTask";

string TaskFileHandler::getTaskFilePath	(const Task& task) {
	string taskFileTitle	= getFileTitle(task);
	string taskFilePath		= TASK_DIRECTORY + taskFileTitle + TASK_FILE_EXTENSION;

	return taskFilePath;
}

string TaskFileHandler::getDeletedTaskFilePath	(const Task& task) {
	string taskFileTitle	= getFileTitle(task);
	string taskFilePath		= TASK_DIRECTORY + taskFileTitle + TASK_DELETED_FILE_EXTENSION;

	return taskFilePath;
}

string TaskFileHandler::getFileTitle (const Task& task) {
	stringstream tempStream;
	tempStream << (task.getIndex());

	return tempStream.str();
}
