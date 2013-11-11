/*
 * =====================================================================================
 *
 *       Filename:  Executor_Base.cpp
 *
 *        Version:  1.0
 *        Created:  11/02/13 13:47:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Executor_Base.h"

using namespace std;
using namespace TP;

const unsigned			Executor_Base::EMPTY_LIST_SIZE				= 0;
const unsigned			Executor_Base::SINGLE_RESULT_LIST_SIZE		= 1;
const std::string		Executor_Base::NAME_NOT_FOUND_ERROR			= "No results for name: ";
const std::string		Executor_Base::INVALID_INDEX_ERROR			= " is not a valid index!";
const std::string		Executor_Base::UNDOSTACK_EMPTY_MSG			= "Nothing to Undo!";
const std::string		Executor_Base::REDOSTACK_EMPTY_MSG			= "Nothing to Redo!";
const std::string		Executor_Base::MODIFY_SAME_NAME_ERROR		= "New name is the same as the Existing name!";
const std::string		Executor_Base::INVALID_FROMDATE_ERROR		= "Invalid 'From' Attribute!";
const std::string		Executor_Base::INVALID_TODATE_ERROR			= "Invalid 'To' Attribute!";
const std::string		Executor_Base::INVALID_FROMDATE_TODATE_ERROR= "Invalid 'From' and 'To' Attributes!";
const std::string		Executor_Base::NAME_NOT_SPECIFIED_ERROR		= "No Name specified!";

// Status setting functions

void Executor_Base::setOpSuccessTask(const Task &retTask, Messenger &response) {
	response.setStatus(TP::STATUS::SUCCESS);
	response.setTask(Task(retTask));
}

void Executor_Base::setOpSuccessTaskList(const list<Task>& results, Messenger &response) {
	response.setStatus(TP::STATUS::SUCCESS);
	response.setList(results);
}

void Executor_Base::setOpIntermediateTaskList(const list<Task>& results, Messenger &response) {
	response.setStatus(TP::STATUS::INTERMEDIATE);
	response.setList(results);
}

void Executor_Base::setIndexNotFound(const unsigned long long &index, Messenger &response) {
	response.setStatus(TP::STATUS::ERR);
	response.setErrorMsg(std::to_string(index) + INVALID_INDEX_ERROR);
}

void Executor_Base::setNameNotFound(const string &name, Messenger &response) {
	response.setStatus(TP::STATUS::ERR);
	response.setErrorMsg(NAME_NOT_FOUND_ERROR + name);
}

void Executor_Base::setErrorWithErrMsg(Messenger &response, const string errMsg) {
	response.setStatus(TP::STATUS::ERR);
	response.setErrorMsg(errMsg);
}
