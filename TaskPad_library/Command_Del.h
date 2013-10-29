/*
 *=======================
 
 Command Del
 
 *=======================
 */

#include "Command.h"

#ifndef TASKPAD_COMMAND_DEL_H
#define TASKPAD_COMMAND_DEL_H

class Command_Del: public Command
{
public:
	Command_Del()	{ init(); setCommandType(TP::DEL); }
	~Command_Del()	{};
	
	//getter for fields
	int							getIndex()			{ return Command::getIndex();		}
	std::string					getName()			{ return Command::getName();		}
	unsigned long long			getCreatedTime()	{ return Command::getCreatedTime();	}
	
	//setter for fields
	void setIndex(int idx)							{ Command::setIndex(idx);			}
	void setName(std::string name)					{ Command::setName(name);			}
	void setCreatedTime(unsigned long long ctime)	{ Command::setCreatedTime(ctime);	}
	
	//getter for flags
	bool getFlagIndex()								{ return Command::getFlagIndex();		}
	bool getFlagExact()								{ return Command::getFlagExact();		}
	bool getFlagCreatedTime()						{ return Command::getFlagCreatedTime(); }
	
	//setter for flags
	void setFlagExact()								{ Command::setFlagExact();				}
};

#endif
