/*
 * =====================================================================================
 *
 *       Filename:  Executor_Redo.h
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

class Executor_Redo: public Executor_Base {
public:
	void executeCommand(Command_Redo* cmd, Messenger &response, Datastore &ds);
private:
};
