#ifndef TASKPAD_COMMAND_ADD_H
#define TASKPAD_COMMAND_ADD_H

//@XIE KAI A0102016E
/*
 *=======================
 
 Command Add
 
 *=======================
 */

#include "Command.h"

class Command_Add: public Command
{
public:
	Command_Add()	{ init(); setCommandType(TP::ADD); }
	~Command_Add()	{};
	
	//getter for fields
	std::string					getName()			{ return Command::getName();		}
	std::time_t					getDueDate()		{ return Command::getDueDate();		}
	std::time_t					getFromDate()		{ return Command::getFromDate();	}
	std::time_t					getToDate()			{ return Command::getToDate();		}
	std::string					getLocation()		{ return Command::getLocation();	}
	std::list<std::string>		getParticipants()	{ return Command::getParticipants();}
	std::string					getNote()			{ return Command::getNote();		}
	TP::PRIORITY				getPriority()		{ return Command::getPriority();	}
	std::list<std::string>		getTags()			{ return Command::getTags();		}
	std::list<std::time_t>		getRemindTimes()	{ return Command::getRemindTimes();	}
	unsigned long long			getCreatedTime()	{ return Command::getCreatedTime(); }
	
	//setter for fields
	void setName(std::string name)					{ Command::setName(name);			}
	void setDueDate(std::time_t dueDate)			{ Command::setDueDate(dueDate);		}
	void setFromDate(std::time_t fromDate)			{ Command::setFromDate(fromDate);	}
	void setToDate(std::time_t toDate)				{ Command::setToDate(toDate);		}
	void setLocation(std::string location)			{ Command::setLocation(location);	}
	void setParticipants(std::list<std::string> ppl){ Command::setParticipants(ppl);	}
	void setNote(std::string note)					{ Command::setNote(note);			}
	void setPriority(TP::PRIORITY priority)			{ Command::setPriority(priority);	}
	void setTags(std::list<std::string> tags)		{ Command::setTags(tags);			}
	void setRemindTimes(std::list<std::time_t> time){ Command::setRemindTimes(time);	}
	void setCreatedTime(unsigned long long ctime)	{ Command::setCreatedTime(ctime);	}
	
	//getter for flags
	bool getFlagName()								{ return Command::getFlagName();		}
	bool getFlagDue()								{ return Command::getFlagDue();			}
	bool getFlagFrom()								{ return Command::getFlagFrom();		}
	bool getFlagTo()								{ return Command::getFlagTo();			}
	bool getFlagLocation()							{ return Command::getFlagLocation();	}
	bool getFlagParticipants()						{ return Command::getFlagParticipants();}
	bool getFlagNote()								{ return Command::getFlagNote();		}
	bool getFlagPriority()							{ return Command::getFlagPriority();	}
	bool getFlagTags()								{ return Command::getFlagTags();		}
	bool getFlagRemindTimes()						{ return Command::getFlagRemindTimes();	}
	bool getFlagCreatedTime()						{ return Command::getFlagCreatedTime();	}
};

#endif