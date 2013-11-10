#ifndef TASKPAD_COMMAND_FIND_H
#define TASKPAD_COMMAND_FIND_H

//@XIE KAI A0102016E
/*
 *=======================
 
 Command Find
 
 *=======================
 */

#include "Command.h"

class Command_Find: public Command
{
public:
	Command_Find()	{ init(); setCommandType(TP::FIND); }
	~Command_Find()	{};
	
	//getter for fields
	int							getIndex()			{ return Command::getIndex();		}
	std::string					getOptName()		{ return Command::getOptName();		}
	std::time_t					getFromDate()		{ return Command::getFromDate();	}
	std::time_t					getToDate()			{ return Command::getToDate();		}
	std::string					getLocation()		{ return Command::getLocation();	}
	std::list<std::string>		getParticipants()	{ return Command::getParticipants();}
	std::string					getNote()			{ return Command::getNote();		}
	TP::PRIORITY				getPriority()		{ return Command::getPriority();	}
	std::list<std::string>		getTags()			{ return Command::getTags();		}
	std::list<std::time_t>		getRemindTimes()	{ return Command::getRemindTimes();	}
	TP::TASK_STATE				getTaskState()		{ return Command::getTaskState();	}
	TP::TASK_TYPE				getTaskType()		{ return Command::getTaskType();	}
	
	//setter for fields
	void setIndex(int idx)							{ Command::setIndex(idx);			}
	void setOptName(std::string name)				{ Command::setOptName(name);		}
	void setFromDate(std::time_t fromDate)			{ Command::setFromDate(fromDate);	}
	void setToDate(std::time_t toDate)				{ Command::setToDate(toDate);		}
	void setLocation(std::string location)			{ Command::setLocation(location);	}
	void setParticipants(std::list<std::string> ppl){ Command::setParticipants(ppl);	}
	void setNote(std::string note)					{ Command::setNote(note);			}
	void setPriority(TP::PRIORITY priority)			{ Command::setPriority(priority);	}
	void setTags(std::list<std::string> tags)		{ Command::setTags(tags);			}
	void setRemindTimes(std::list<std::time_t> time){ Command::setRemindTimes(time);	}
	void setTaskState(TP::TASK_STATE state)			{ Command::setTaskState(state);		}
	void setTaskType(TP::TASK_TYPE type)			{ Command::setTaskType(type);		}

	//getter for flags
	bool getFlagIndex()								{ return Command::getFlagIndex();		}
	bool getFlagExact()								{ return Command::getFlagExact();		}
	bool getFlagName()								{ return Command::getFlagName();		}
	bool getFlagOptName()							{ return Command::getFlagOptName();		}
	bool getFlagFrom()								{ return Command::getFlagFrom();		}
	bool getFlagTo()								{ return Command::getFlagTo();			}
	bool getFlagLocation()							{ return Command::getFlagLocation();	}
	bool getFlagParticipants()						{ return Command::getFlagParticipants();}
	bool getFlagNote()								{ return Command::getFlagNote();		}
	bool getFlagPriority()							{ return Command::getFlagPriority();	}
	bool getFlagTags()								{ return Command::getFlagTags();		}
	bool getFlagRemindTimes()						{ return Command::getFlagRemindTimes();	}
	bool getFlagTaskState()							{ return Command::getFlagTaskState();	}
	bool getFlagTaskType()							{ return Command::getFlagTaskType();	}

	//setter for flags
	void setFlagExact()								{ Command::setFlagExact();				}
};

#endif
