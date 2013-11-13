#ifndef TASKPAD_COMMAND_REDO_H
#define TASKPAD_COMMAND_REDO_H

// @author A0102016E
/*
 *=======================
 
 Command Redo
 
 *=======================
 */

#include "Command.h"

class Command_Redo: public Command
{
public:
	Command_Redo()	{ init(); setCommandType(TP::REDO); }
	~Command_Redo()	{};
};

#endif
