#ifndef _ENUM_H
#define _ENUM_H

#include <string>

namespace TP
{
    enum COMMAND_TYPE	{ ADD, MOD, DEL, FIND, UNDO, REDO, SYNC, UNDEFINED};
    enum PRIORITY		{ HIGH, MEDIUM, LOW };
    enum TASK_STATE		{ UNDONE = false, DONE = true };
    enum TASK_TYPE		{ TIMED, DEADLINE, FLOATING };
    enum CMD_PROPERTY	{ UNCHANGED = false, EDITED = true };
    enum STATUS			{
        ERROR, ERROR_INTERMEDIATE, SUCCESS, INTERMEDIATE, SUCCESS_INDEXED_COMMAND, DISPLAY
    };
    enum PERIOD_TYPE	{ DAY, WEEK, MONTH};
    enum LIST_OP		{ ADD_ELEMENT, REMOVE_ELEMENT };

    //String representations of the Enums above

    static const std::string COMMAND_TYPE_STRING[8] =	
                    { "ADD","MOD","DEL","FIND","UNDO","REDO","SYNC","UNDEFINED"	};
    static const std::string PRIORITY_STRING[3] =		{"HIGH","MEDIUM","LOW"};
    static const std::string TASK_STATE_STRING[2] =		{"UNDONE","DONE"};
    static const std::string TASK_TYPE_STRING[3] =		{"TIMED","DEADLINE","FLOATING"};
}
#endif
