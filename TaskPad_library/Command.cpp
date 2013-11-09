/*
*=====================================================================================
*
*       Filename:  Command.cpp
*
*        Version:  1.0
*        Created:  09/21/13 16:13:09
*
*         Author:  XIE KAI (A0102016E), gigikie@gmail.com
*   Organization:  NUS, SoC
*
*=====================================================================================
*/

#include "Command.h"

using namespace std;
using namespace TP;

const bool				Command::DEFAULT_FLAG					= UNCHANGED;
const COMMAND_TYPE		Command::DEFAULT_COMMAND_TYPE			= UNDEFINED;
const PRIORITY			Command::DEFAULT_PRIORITY				= MEDIUM;
const TASK_STATE		Command::DEFAULT_TASK_STATE				= UNDONE;//undone
const TASK_TYPE			Command::DEFAULT_TASK_TYPE				= FLOATING;
const string			Command::DEFAULT_STRING					= "";
const list<string>		Command::DEFAULT_LIST_OF_STRING			= list<string>();
const time_t			Command::DEFAULT_TIME					= -1;
const list<time_t>		Command::DEFAULT_REMIND_TIME			= list<time_t>();
const int				Command::DEFAULT_INDEX					= -1;
const unsigned long long Command::DEFAULT_CREATED_TIME			= 0;

void Command::init()
{
	//flags init
	FLAG_index			                            = DEFAULT_FLAG;
	FLAG_exact			                            = DEFAULT_FLAG;
	FLAG_name			                            = DEFAULT_FLAG;
	FLAG_optName		                            = DEFAULT_FLAG;
	FLAG_due			                            = DEFAULT_FLAG;
	FLAG_removeDue									= DEFAULT_FLAG;
	FLAG_from			                            = DEFAULT_FLAG;
	FLAG_removeFrom									= DEFAULT_FLAG;
	FLAG_to				                            = DEFAULT_FLAG;
	FLAG_removeTo									= DEFAULT_FLAG;
	FLAG_location		                            = DEFAULT_FLAG;
	FLAG_participants	                            = DEFAULT_FLAG;
	FLAG_removeParticipants							= DEFAULT_FLAG;
	FLAG_addParticipants							= DEFAULT_FLAG;
	FLAG_removeAllParticipants						= DEFAULT_FLAG;
	FLAG_note			                            = DEFAULT_FLAG;
	FLAG_priority		                            = DEFAULT_FLAG;
	FLAG_tags			                            = DEFAULT_FLAG;
	FLAG_removeTags									= DEFAULT_FLAG;
	FLAG_addTags									= DEFAULT_FLAG;
	FLAG_removeAllTags								= DEFAULT_FLAG;
	FLAG_remindTime		                            = DEFAULT_FLAG;
	FLAG_removeRemindTime							= DEFAULT_FLAG;
	FLAG_addRemindTime								= DEFAULT_FLAG;
	FLAG_removeAllRemindTimes						= DEFAULT_FLAG;
	FLAG_taskState		                            = DEFAULT_FLAG;
	FLAG_createdTime	                            = DEFAULT_FLAG;
	FLAG_taskType		                            = DEFAULT_FLAG;
	FLAG_syncProviderName                           = DEFAULT_FLAG;

	//fields init
	_type				                            = DEFAULT_COMMAND_TYPE;
	_index				                            = DEFAULT_INDEX;
	_name				                            = DEFAULT_STRING;
	_optName			                            = DEFAULT_STRING;
	_dueDate			                            = DEFAULT_TIME;
	_fromDate			                            = DEFAULT_TIME;
	_toDate				                            = DEFAULT_TIME;
	_location			                            = DEFAULT_STRING;
	_participants		                            = DEFAULT_LIST_OF_STRING;
	_removeParticipants								= DEFAULT_LIST_OF_STRING;
	_addParticipants								= DEFAULT_LIST_OF_STRING;
	_note				                            = DEFAULT_STRING;
	_priority			                            = DEFAULT_PRIORITY;
	_tags				                            = DEFAULT_LIST_OF_STRING;
	_removeTags										= DEFAULT_LIST_OF_STRING;
	_addTags										= DEFAULT_LIST_OF_STRING;
	_remindTime			                            = DEFAULT_REMIND_TIME;
	_removeRemindTime								= DEFAULT_REMIND_TIME;
	_addRemindTime									= DEFAULT_REMIND_TIME;
	_syncProviderName	                            = DEFAULT_STRING;
	_taskState			                            = DEFAULT_TASK_STATE;
	_createdTime		                            = DEFAULT_CREATED_TIME;
	_taskType			                            = DEFAULT_TASK_TYPE;
}