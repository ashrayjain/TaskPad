/*
 * =====================================================================================
 *
 *       Filename:  Executor_Find.h
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

#include "Executor_Base.h"

class Executor_Find: public Executor_Base {
public:
	void executeCommand(Command* cmd, Messenger &response, Datastore &ds);
private:
	void			formTaskFromFindCmd			(Command_Find* cmd, Task &newTask);
	void			findByIndex					(const unsigned long long index, Messenger &response, Datastore &ds);
	void			findGeneral					(Command_Find* cmd, Messenger &response, Datastore &ds);
	void			runSearch					(const Task &taskToCompare, list<Task> &results, string substringName, 
												 set<Task> &customData, bool customDataSet, Datastore &ds);
	void			findByTags					(Command_Find* cmd, Messenger &response);
	void			getCustomDataRangeByTags	(set<Task> &customDataRange, list<string> &tags, Datastore &ds);
	void			getCustomDataRangeByRT		(set<Task> &customDataRange, list<time_t> &remindTimes, Datastore &ds);
	void			filterResponseListByType	(Messenger &response, list<TP::TASK_TYPE> &types);
	void			runSearchWithTask			(const Task &taskToCompare,	list<Task> &results, Datastore &ds);
	void			runSearchWithTask			(const Task &taskToCompare, list<Task> &results, string substringName, Datastore &ds);
	void			runSearchWithTask			(const Task &taskToCompare,	list<Task> &results, set<Task> &customData);
	void			runSearchWithTask			(const Task &taskToCompare, list<Task> &results, string substringName, 
												 set<Task> &customData);
	bool			taskMatch					(const Task& lhs, const Task& rhs);
	bool			taskStatesNotEqual			(const TP::TASK_STATE &lhs, const TP::TASK_STATE &rhs) const;
	bool			participantsMatchFound		(const list<string> &rhsParticipants, const list<string> &lhsParticipants) const;
	bool			invalidDateChk				(const Task &lhs, const Task &rhs) const;
	bool			chkDateBound				(const time_t &fromTime, const time_t &toTime, const Task &lhs) const;
	bool			chkFromDateBound			(const time_t &fromTime, const Task &lhs) const;
	bool			chkToDateBound				(const time_t &toTime, const Task &lhs) const;
	std::list<Task> getSortListByPriority		(std::list<Task> &taskList);
	std::list<Task> getSortListByFromTime		(std::list<Task> &taskList);

	// Custom Comparator for sorting Tasks by Priority
	static bool sortTaskByPriorityComparator	(const Task &first, const Task &second);
	static bool sortTaskByFromTimeComparator	(const Task &first, const Task &second); 
};
