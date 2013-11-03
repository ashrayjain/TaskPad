/*
 * =====================================================================================
 *
 *       Filename:  Executor_Add.h
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

class Executor_Add: public Executor_Base {
public:
	void			executeCommand					(Command_Add* cmd, Messenger &response, Datastore &ds);
private:
	static const unsigned RT_MIN_H_ARR[];
	static const unsigned RT_MIN_M_ARR[];
	static const unsigned RT_MIN_L_ARR[];

	bool			validAddCmd						(Command_Add* cmd, Messenger &response);
	Task			formTaskFromAddCmd				(Command_Add* cmd);
	void			setDefaultRemindTimes			(Task &task);
	void			setDefaultRemindTimesPriorityH	(Task &task);
	void			setDefaultRemindTimesPriorityM	(Task &task);
	void			setDefaultRemindTimesPriorityL	(Task &task);
	list<time_t>	getRemindTimesFromMinutesBefore	(const unsigned minutesBeforeList[], const int N, const time_t &deadline) const;
};
