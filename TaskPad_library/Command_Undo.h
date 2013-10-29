#ifndef TASKPAD_COMMAND_UNDO_H
#define TASKPAD_COMMAND_UNDO_H

/*
 *=======================
 
 Command Undo
 
 *=======================
 */

#include "Command.h"

class Command_Undo: public Command
{
public:
	Command_Undo()	{ init(); setCommandType(TP::UNDO); }
	~Command_Undo()	{};
};

#endif
