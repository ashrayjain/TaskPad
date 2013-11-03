/*
 * =====================================================================================
 *
 *       Filename:  Executor_Base.h
 *
 *        Version:  1.0
 *        Created:  11/02/13 13:47:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#pragma once

#include <string>
#include <cstring>
#include <list>
#include <unordered_set>
#include <algorithm>
#include "Task.h"
#include "Messenger.h"
#include "Command.h"
#include "Datastore.h"

class Executor_Base {
protected:

	// Class constants
	static const unsigned		EMPTY_LIST_SIZE;
	static const unsigned		SINGLE_RESULT_LIST_SIZE;
	static const std::string	NAME_NOT_FOUND_ERROR;
	static const std::string	INVALID_INDEX_ERROR;
	static const std::string	UNDOSTACK_EMPTY_MSG;
	static const std::string	REDOSTACK_EMPTY_MSG;
	static const std::string	MODIFY_SAME_NAME_ERROR;
	static const std::string	INVALID_FROMDATE_ERROR;
	static const std::string	INVALID_TODATE_ERROR;
	static const std::string	INVALID_FROMDATE_TODATE_ERROR;
	static const std::string	NAME_NOT_SPECIFIED_ERROR;

	// Utility functions
	std::string				getLowerStr		(std::string str);
	std::list<std::string>	getLowerStrList	(std::list<std::string> strList);

	// Setters for Messenger to return
	void setOpSuccessTask			(const Task &retTask, Messenger &response);
	void setOpSuccessTaskList		(const std::list<Task>& results, Messenger &response);
	void setOpIntermediateTaskList	(const std::list<Task> &results, Messenger &response);
	void setIndexNotFound			(const unsigned long long &index, Messenger &response);
	void setNameNotFound			(const std::string &name, Messenger &response);
	void setErrorWithErrMsg			(Messenger &response, const std::string errMsg);

public:
	virtual ~Executor_Base()=0;
	virtual void executeCommand(Command* cmd, Messenger &response, Datastore &ds)=0;
};
