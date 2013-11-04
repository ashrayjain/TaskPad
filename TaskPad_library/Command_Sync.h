#ifndef TASKPAD_COMMAND_SYNC_H
#define TASKPAD_COMMAND_SYNC_H

/*
 *=======================
 
 Command Sync
 
 *=======================
 */

#include "Command.h"

class Command_Sync: public Command
{
public:
	Command_Sync()	{ init(); setCommandType(TP::SYNC); }
	~Command_Sync()	{};
	
	//getter for fields
	std::string		getSyncProviderName()			{ return Command::getSyncProviderName();	}
	
	//setter for fields
	void setSyncProviderName(std::string syncName)	{ Command::setSyncProviderName(syncName);	}

	//getter for flags
	bool getFlagSyncProviderName()					{ return Command::getFlagSyncProviderName();}
};

#endif
