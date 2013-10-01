/*
 * =====================================================================================
 *
 *       Filename:  Executor.h
 *
 *        Version:  1.0
 *        Created:  09/25/13 00:22:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include <list>
#include <string>
#include "Task.h"
#include "Messenger.h"
#include "Command.h"

using namespace std;

class Executor
{
public:
	Executor(list<Task>* data) { _data = data; }
	void executeCommand(Command* cmd, Messenger &response);

protected:
	list<Task>* _data;
};