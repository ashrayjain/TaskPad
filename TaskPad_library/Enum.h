#ifndef _ENUM_H
#define _ENUM_H

#include <string>
using namespace std;

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
	enum PERIOD_TYPE	{ DAY, WEEK, MONTH};

	//String representations of the Enums above

	static const string COMMAND_TYPE_STRING[8] =	
					{ "ADD","MOD","DEL","FIND","UNDO","REDO","SYNC","UNDEFINED"	};
	static const string PRIORITY_STRING[3] =		{"HIGH","MEDIUM","LOW"};
	static const string TASK_STATE_STRING[2] =		{"UNDONE","DONE"};
	static const string TASK_TYPE_STRING[3] =		{"TIMED","DEADLINE","FLOATING"};
}
#endif