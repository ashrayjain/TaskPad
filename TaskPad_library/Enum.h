#ifndef _ENUM_H
#define _ENUM_H

#include <string>

namespace TP{

	static const int COMMAND_TYPE_COUNT = 8;
	static const int PRIORITY_COUNT = 3;
	static const int TASK_STATE_COUNT = 2;
	static const int TASK_TYPE_COUNT = 3;
	static const int  LOG_TYPE_COUNT = 5;
    enum COMMAND_TYPE	{ ADD, MOD, DEL, FIND, INDEX, UNDO, REDO, SYNC, UNDEFINED};
    enum PRIORITY		{ HIGH, MEDIUM, LOW };
    enum TASK_STATE		{ UNDONE = false, DONE = true };
    enum TASK_TYPE		{ TIMED, DEADLINE, FLOATING };
    enum CMD_PROPERTY	{ UNCHANGED = false, EDITED = true };
    enum STATUS			{
        ERROR, ERROR_INTERMEDIATE, SUCCESS, INTERMEDIATE, SUCCESS_INDEXED_COMMAND, DISPLAY
    };
    enum PERIOD_TYPE	{ DAY, WEEK, MONTH};
    enum LIST_OP		{ ADD_ELEMENT, REMOVE_ELEMENT };
	enum LOG_TYPE		{ INFOLOG, NOTICELOG, WARNINGLOG, ERRORLOG, FATALLOG };
	
	static const std::string ERROR_NAME = "Format Error: Name not within single quotation mark";
	static const std::string ERROR_DUPLICATE = "Duplicate key word";
	static const std::string ERROR_DATA = "Format Error: Data not within single quotation mark";
	static const std::string ERROR_INDEX = "Invalid index";

	//String representations of the Enums above
	static const std::string COMMAND_TYPE_STRING[COMMAND_TYPE_COUNT] =	
                    { "ADD","MOD","DEL","FIND","UNDO","REDO","SYNC","UNDEFINED"	};
	static const std::string PRIORITY_STRING[PRIORITY_COUNT]		=		{"HIGH","MEDIUM","LOW"};
	static const std::string TASK_STATE_STRING[TASK_STATE_COUNT]	=		{"UNDONE","DONE"};
	static const std::string TASK_TYPE_STRING[TASK_TYPE_COUNT]		=		{"TIMED","DEADLINE","FLOATING"};
	static const std::string LOG_TYPE_STRING[LOG_TYPE_COUNT]		=		{"INFOLOG","NOTICELOG","WARNINGLOG","ERRORLOG","FATALLOG"};
}
#endif
