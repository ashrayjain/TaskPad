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

#ifndef TASKPAD_EXECUTOR_FIND_H
#define TASKPAD_EXECUTOR_FIND_H

#include "Executor_Base.h"

//@ASHRAY JAIN A0105199B

class Executor_Find: public Executor_Base {
public:
	void executeCommand(Command* cmd, Messenger &response, Datastore &ds);
private:
	void			formTaskFromFindCmd			(Command_Find* cmd, Task &newTask);
	void			findByIndex					(const unsigned long long index, Messenger &response, Datastore &ds);
	void			findGeneral					(Command_Find* cmd, Messenger &response, Datastore &ds);
	void			runSearch					(const Task &taskToCompare, std::list<Task> &results, std::string substringName, 
												 std::set<Task> &customData, bool customDataSet, Datastore &ds);
	void			findByTags					(Command_Find* cmd, Messenger &response);
	void			getCustomDataRangeByTags	(std::set<Task> &customDataRange, std::list<std::string> &tags, Datastore &ds);
	void			getCustomDataRangeByRT		(std::set<Task> &customDataRange, std::list<time_t> &remindTimes, Datastore &ds);
	void			filterResponseListByType	(Messenger &response, std::list<TP::TASK_TYPE> &types);
	void			runSearchWithTask			(const Task &taskToCompare,	std::list<Task> &results, Datastore &ds);
	void			runSearchWithTask			(const Task &taskToCompare, std::list<Task> &results, 
												 std::string substringName, Datastore &ds);
	void			runSearchWithTask			(const Task &taskToCompare,	std::list<Task> &results,
												 std::set<Task> &customData);
	void			runSearchWithTask			(const Task &taskToCompare, std::list<Task> &results, 
												 std::string substringName, std::set<Task> &customData);
	bool			taskMatch					(const Task& lhs, const Task& rhs);
	bool			taskStatesNotEqual			(const TP::TASK_STATE &lhs, const TP::TASK_STATE &rhs) const;
	bool			participantsMatchFound		(const std::list<std::string> &rhsParticipants, 
												 const std::list<std::string> &lhsParticipants) const;
	bool			invalidDateChk				(const Task &lhs, const Task &rhs) const;
	bool			validDateBound				(const time_t &fromTime, const time_t &toTime, const Task &lhs) const;
	bool			chkFromDateBound			(const time_t &fromTime, const Task &lhs) const;
	bool			chkToDateBound				(const time_t &toTime, const Task &lhs) const;
	std::list<Task> getSortListByPriority		(std::list<Task> &taskList);
	std::list<Task> getSortListByFromTime		(std::list<Task> &taskList);

	// Custom Comparator for sorting Tasks by Priority
	static bool sortTaskByPriorityComparator	(const Task &first, const Task &second);
	static bool sortTaskByFromTimeComparator	(const Task &first, const Task &second); 
};
#endif
