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

std::string Command::defaultSyncProvider = "GCal";

void Command::init()
{
	std::time_t currentTime = std::time(NULL);

	//flags init
	FLAG_index = false;
	FLAG_exact = false;
	FLAG_name = false;
	FLAG_due = false;
	FLAG_from = false;
	FLAG_to = false;
	FLAG_location = false;
	FLAG_participants = false;
	FLAG_note = false;
	FLAG_priority = false;
	FLAG_tags = false;
	FLAG_remindTime = false;
	FLAG_taskState = false;

	//fields init
	_type = UNDEFINED;
	_index = -1;
	_name = "";
	_optName = "";
	_dueDate = currentTime;
	_fromDate = currentTime;
	_toDate = currentTime;
	_location = "";
	_participants = "";
	_note = "";
	_priority = MEDIUM;
	_tags = "";
	_remindTime = currentTime;
	_syncProviderName = defaultSyncProvider;
	_taskState = false;//undone
}