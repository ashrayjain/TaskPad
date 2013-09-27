/*
* =====================================================================================
*
*       Filename:  Command.cpp
*
*        Version:  1.0
*        Created:  09/21/13 16:13:09
*
*         Author:  XIE KAI (A0102016E), gigikie@gmail.com
*   Organization:  NUS, SoC
*
* =====================================================================================
*/

#include "Command.h"

const bool					Command::DEFAULT_FLAG			= Command::UNCHANGED;
const Command::COMMAND_TYPE Command::DEFAULT_COMMAND_TYPE	= Command::UNDEFINED;
const Command::PRIORITY		Command::DEFAULT_PRIORITY		= Command::MEDIUM;
const bool					Command::DEFAULT_TASK_STATE		= Command::UNDONE;//undone
const Command::TASK_TYPE	Command::DEFAULT_TASK_TYPE		= Command::FLOATING;
const std::string			Command::DEFAULT_STRING			= "";
const std::time_t			Command::DEFAULT_TIME			= -1;
const int					Command::DEFAULT_INDEX			= -1;
const unsigned				Command::DEFAULT_CREATED_TIME	= 0;

void Command::init()
{
	//flags init
	FLAG_index			= DEFAULT_FLAG;
	FLAG_exact			= DEFAULT_FLAG;
	FLAG_name			= DEFAULT_FLAG;
	FLAG_optName		= DEFAULT_FLAG;
	FLAG_due			= DEFAULT_FLAG;
	FLAG_from			= DEFAULT_FLAG;
	FLAG_to				= DEFAULT_FLAG;
	FLAG_location		= DEFAULT_FLAG;
	FLAG_participants	= DEFAULT_FLAG;
	FLAG_note			= DEFAULT_FLAG;
	FLAG_priority		= DEFAULT_FLAG;
	FLAG_tags			= DEFAULT_FLAG;
	FLAG_remindTime		= DEFAULT_FLAG;
	FLAG_taskState		= DEFAULT_FLAG;
	FLAG_createdTime	= DEFAULT_FLAG;
	FLAG_taskType		= DEFAULT_FLAG;
	FLAG_syncProviderName = DEFAULT_FLAG;

	//fields init
	_type				= DEFAULT_COMMAND_TYPE;
	_index				= DEFAULT_INDEX;
	_name				= DEFAULT_STRING;
	_optName			= DEFAULT_STRING;
	_dueDate			= DEFAULT_TIME;
	_fromDate			= DEFAULT_TIME;
	_toDate				= DEFAULT_TIME;
	_location			= DEFAULT_STRING;
	_participants		= DEFAULT_STRING;
	_note				= DEFAULT_STRING;
	_priority			= DEFAULT_PRIORITY;
	_tags				= DEFAULT_STRING;
	_remindTime			= DEFAULT_TIME;
	_syncProviderName	= DEFAULT_STRING;
	_taskState			= DEFAULT_TASK_STATE;
	_createdTime		= DEFAULT_CREATED_TIME;
	_taskType			= DEFAULT_TASK_TYPE;
}