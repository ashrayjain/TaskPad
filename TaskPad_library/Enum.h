#ifndef _ENUM_H
#define _ENUM_H

#include <cstring>
namespace TP
{
	enum COMMAND_TYPE	{ ADD, MOD, DEL, FIND, UNDO, REDO, SYNC, UNDEFINED};
	enum PRIORITY		{ HIGH, MEDIUM, LOW };
	enum TASK_STATE		{ UNDONE = false, DONE = true };
	enum TASK_TYPE		{ TIMED, DEADLINE, FLOATING };
	enum FLAG			{ UNCHANGED = false, EDITED = true };
	enum STATUS			{
		ERROR, ERROR_INTERMEDIATE, SUCCESS, INTERMEDIATE, SUCCESS_INDEXED_COMMAND, DISPLAY
	};

	string COMMAND_TYPE_STRING[] = {"ADD","MOD","DEL","FIND","UNDO","REDO","SYNC","UNDEFINED"};
	string PRIORITY_STRING[] = {"HIGH","MEDIUM","LOW"};
	string TASK_STATE_STRING[] = {"UNDONE","DONE"};
	string TASK_TYPE_STRING[] = {"TIMED","DEADLINE","FLOATING"};
}
#endif