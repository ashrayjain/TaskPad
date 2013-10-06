/*
 * =====================================================================================
 *
 *       Filename:  Command.h
 *
 *        Version:  1.0
 *        Created:  09/21/13 16:13:09
 *
 *         Author:  XIE KAI (A0102016E), gigikie@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */


/*
 *		How to use these classes
 *
 *		1. in class Interpreter
 *		once you know it's certain command, like command Add, you need to 'new Command_Add()', then
 *		set its properties/fields according to user input, e.g.:
 *		
 *		Command_Add* cmd_add = new Command_Add();
 *		cmd_add->setName(taskName);//name in command Add is compulsory
 *		if( user input contains dueDate )
 *			cmd_add->setDueDate(dueDate);
 *
 *		at the end, you will return this cmd_add pointer.
 *
 * = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
 *
 *		in class Executor:
 *		first you need to check command's commandType, then dynamically cast it down to certain command, e.g.:
 *
 *		Command_Add* cmd_add;
 *		//cmd's class is Command
 *		if( cmd->getCommandType() == Command::ADD )
 *			cmd_add = dynamic_cast<Command_Add*>(cmd);
 *		
 *		then you need to check its certain flag, and get certain field data, e.g.:
 *
 *		std::time_t dueDate;
 *		if( cmd_add->getFlagDue() )
 *			dueDate = cmd_add->getDueDate();
 *
 *		at the end, you must *DELETE* its pointer.
 *		delete cmd;
 *
 */

#include <list>
#include <string>
#include <ctime>
#include "Enum.h"

#ifndef TASKPAD_COMMAND_H
#define TASKPAD_COMMAND_H

/*
 *=======================
 
	Command Base class
 
 *=======================
 */
class Command
{
public:
	virtual					~Command() = 0			{};
	TP::COMMAND_TYPE		getCommandType()		{ return _type; }
	
protected:
	//init
	void init();
	void setCommandType(TP::COMMAND_TYPE type)		{ _type = type; }

	//getter for fields
	int						getIndex()				{ return _index;			}
	std::string				getName()				{ return _name;				}
	std::string				getOptName()			{ return _optName;			}
	std::time_t				getDueDate()			{ return _dueDate;			}
	std::time_t				getFromDate()			{ return _fromDate;			}
	std::time_t				getToDate()				{ return _toDate;			}
	std::string				getLocation()			{ return _location;			}
	std::list<std::string>	getParticipants()		{ return _participants;		}
	std::string				getNote()				{ return _note;				}
	TP::PRIORITY			getPriority()			{ return _priority;			}
	std::list<std::string>	getTags()				{ return _tags;				}
	std::list<std::time_t>	getRemindTime()			{ return _remindTime;		}
	bool					getTaskState()			{ return _taskState;		}
	TP::TASK_TYPE			getTaskType()			{ return _taskType;			}
	unsigned				getCreatedTime()		{ return _createdTime;		}
	std::string				getSyncProviderName()	{ return _syncProviderName; }
	
	//setter for fields
	void setIndex(int idx)							{ _index		= idx;		FLAG_index		= TP::EDITED; }
	void setName(std::string name)					{ _name			= name;		FLAG_name		= TP::EDITED; }
	void setOptName(std::string name)				{ _optName		= name;		FLAG_optName	= TP::EDITED; }
	void setDueDate(std::time_t dueDate)			{ _dueDate		= dueDate;	FLAG_due		= TP::EDITED; }
	void setFromDate(std::time_t fromDate)			{ _fromDate		= fromDate;	FLAG_from		= TP::EDITED; }
	void setToDate(std::time_t toDate)				{ _toDate		= toDate;	FLAG_to			= TP::EDITED; }
	void setLocation(std::string location)			{ _location		= location;	FLAG_location	= TP::EDITED; }
	void setParticipants(std::list<std::string> ppl){ _participants	= ppl;		FLAG_location	= TP::EDITED; }
	void setNote(std::string note)					{ _note			= note;		FLAG_note		= TP::EDITED; }
	void setPriority(TP::PRIORITY priority)			{ _priority		= priority;	FLAG_priority	= TP::EDITED; }
	void setTags(std::list<std::string> tags)		{ _tags			= tags;		FLAG_tags		= TP::EDITED; }
	void setRemindTime(std::list<std::time_t> time)	{ _remindTime	= time;		FLAG_remindTime	= TP::EDITED; }
	void setTaskState(bool state)					{ _taskState	= state;	FLAG_taskState	= TP::EDITED; }
	void setTaskType(TP::TASK_TYPE type)			{ _taskType		= type;		FLAG_taskType	= TP::EDITED; }
	void setCreatedTime(unsigned ctime)				{ _createdTime	= ctime;	FLAG_createdTime= TP::EDITED; }
	void setSyncProviderName(std::string syncName)	{ _syncProviderName	  = syncName; 
													  FLAG_syncProviderName = TP::EDITED; }

	//getter for flags
	bool getFlagIndex()								{ return FLAG_index;			}
	bool getFlagExact()								{ return FLAG_exact;			}
	bool getFlagName()								{ return FLAG_name;				}
	bool getFlagDue()								{ return FLAG_due;				}
	bool getFlagFrom()								{ return FLAG_from;				}
	bool getFlagTo()								{ return FLAG_to;				}
	bool getFlagLocation()							{ return FLAG_location;			}
	bool getFlagParticipants()						{ return FLAG_participants;		}
	bool getFlagNote()								{ return FLAG_note;				}
	bool getFlagPriority()							{ return FLAG_priority;			}
	bool getFlagTags()								{ return FLAG_tags;				}
	bool getFlagRemindTime()						{ return FLAG_remindTime;		}
	bool getFlagTaskState()							{ return FLAG_taskState;		}
	bool getFlagTaskType()							{ return FLAG_taskType;			}
	bool getFlagCreatedTime()						{ return FLAG_createdTime;		}
	bool getFlagSyncProviderName()					{ return FLAG_syncProviderName; }

	//setter for flags
	void setFlagExact()								{ FLAG_exact = TP::EDITED;		}

private:
	//default values
	static const bool								DEFAULT_FLAG;
	static const TP::COMMAND_TYPE					DEFAULT_COMMAND_TYPE;
	static const TP::PRIORITY						DEFAULT_PRIORITY;
	static const bool								DEFAULT_TASK_STATE;
	static const TP::TASK_TYPE						DEFAULT_TASK_TYPE;
	static const std::string						DEFAULT_STRING;
	static const std::list<std::string>				DEFAULT_LIST_OF_STRING;
	static const std::time_t						DEFAULT_TIME;
	static const std::list<std::time_t>				DEFAULT_REMIND_TIME;
	static const int								DEFAULT_INDEX;
	static const unsigned							DEFAULT_CREATED_TIME;
	
	//flags
	bool FLAG_index;
	bool FLAG_exact;
	bool FLAG_name;
	bool FLAG_optName;
	bool FLAG_due;
	bool FLAG_from;
	bool FLAG_to;
	bool FLAG_location;
	bool FLAG_participants;
	bool FLAG_note;
	bool FLAG_priority;
	bool FLAG_tags;
	bool FLAG_remindTime;
	bool FLAG_taskState;
	bool FLAG_createdTime;
	bool FLAG_taskType;
	bool FLAG_syncProviderName;

	//fields
	TP::COMMAND_TYPE								_type;
	int												_index;
	std::string										_name;
	std::string										_optName;
	std::time_t										_dueDate;
	std::time_t										_fromDate;
	std::time_t										_toDate;
	std::string										_location;
	std::list<std::string>							_participants;
	std::string										_note;
	TP::PRIORITY									_priority;//high | medium | low
	std::list<std::string>							_tags;
	std::list<std::time_t>							_remindTime;
	std::string										_syncProviderName;
	bool											_taskState;//done | undone
	unsigned										_createdTime;
	TP::TASK_TYPE									_taskType;//timed | deadline | floating
};

/*
 *=======================
 
 Command Add
 
 *=======================
 */
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
	std::list<std::time_t>		getRemindTime()		{ return Command::getRemindTime();	}
	
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
	void setRemindTime(std::list<std::time_t> time)	{ Command::setRemindTime(time);		}
	
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
	bool getFlagRemindTime()						{ return Command::getFlagRemindTime();	}
};

/*
 *=======================
 
 Command Mod
 
 *=======================
 */
class Command_Mod: public Command
{
public:
	Command_Mod()	{ init(); setCommandType(TP::MOD); }
	~Command_Mod()	{};
	
	//getter for fields
	int							getIndex()			{ return Command::getIndex();		}
	std::string					getName()			{ return Command::getName();		}
	std::string					getOptName()		{ return Command::getOptName();		}
	std::time_t					getDueDate()		{ return Command::getDueDate();		}
	std::time_t					getFromDate()		{ return Command::getFromDate();	}
	std::time_t					getToDate()			{ return Command::getToDate();		}
	std::string					getLocation()		{ return Command::getLocation();	}
	std::list<std::string>		getParticipants()	{ return Command::getParticipants();}
	std::string					getNote()			{ return Command::getNote();		}
	TP::PRIORITY				getPriority()		{ return Command::getPriority();	}
	std::list<std::string>		getTags()			{ return Command::getTags();		}
	std::list<std::time_t>		getRemindTime()		{ return Command::getRemindTime();	}
	bool						getTaskState()		{ return Command::getTaskState();	}
	unsigned					getCreatedTime()	{ return Command::getCreatedTime();	}
	
	//setter for fields
	void setIndex(int idx)							{ Command::setIndex(idx);			}
	void setName(std::string name)					{ Command::setName(name);			}
	void setOptName(std::string name)				{ Command::setOptName(name);		}
	void setDueDate(std::time_t dueDate)			{ Command::setDueDate(dueDate);		}
	void setFromDate(std::time_t fromDate)			{ Command::setFromDate(fromDate);	}
	void setToDate(std::time_t toDate)				{ Command::setToDate(toDate);		}
	void setLocation(std::string location)			{ Command::setLocation(location);	}
	void setParticipants(std::list<std::string> ppl){ Command::setParticipants(ppl);	}
	void setNote(std::string note)					{ Command::setNote(note);			}
	void setPriority(TP::PRIORITY priority)			{ Command::setPriority(priority);	}
	void setTags(std::list<std::string> tags)		{ Command::setTags(tags);			}
	void setRemindTime(std::list<std::time_t> time)	{ Command::setRemindTime(time);		}
	void setTaskState(bool state)					{ Command::setTaskState(state);		}
	void setCreatedTime(unsigned ctime)				{ Command::setCreatedTime(ctime);	}
	
	//getter for flags
	bool getFlagIndex()								{ return Command::getFlagIndex();		}
	bool getFlagExact()								{ return Command::getFlagExact();		}
	bool getFlagName()								{ return Command::getFlagName();		}
	bool getFlagDue()								{ return Command::getFlagDue();			}
	bool getFlagFrom()								{ return Command::getFlagFrom();		}
	bool getFlagTo()								{ return Command::getFlagTo();			}
	bool getFlagLocation()							{ return Command::getFlagLocation();	}
	bool getFlagParticipants()						{ return Command::getFlagParticipants();}
	bool getFlagNote()								{ return Command::getFlagNote();		}
	bool getFlagPriority()							{ return Command::getFlagPriority();	}
	bool getFlagTags()								{ return Command::getFlagTags();		}
	bool getFlagRemindTime()						{ return Command::getFlagRemindTime();	}
	bool getFlagTaskState()							{ return Command::getFlagTaskState();	}
	bool getFlagCreatedTime()						{ return Command::getFlagCreatedTime(); }
	
	//setter for flags
	void setFlagExact()								{ Command::setFlagExact();				}
};

/*
 *=======================
 
 Command Del
 
 *=======================
 */
class Command_Del: public Command
{
public:
	Command_Del()	{ init(); setCommandType(TP::DEL); }
	~Command_Del()	{};
	
	//getter for fields
	int							getIndex()			{ return Command::getIndex();		}
	std::string					getName()			{ return Command::getName();		}
	unsigned					getCreatedTime()	{ return Command::getCreatedTime();	}
	
	//setter for fields
	void setIndex(int idx)							{ Command::setIndex(idx);			}
	void setName(std::string name)					{ Command::setName(name);			}
	void setCreatedTime(unsigned ctime)				{ Command::setCreatedTime(ctime);	}
	
	//getter for flags
	bool getFlagIndex()								{ return Command::getFlagIndex();		}
	bool getFlagExact()								{ return Command::getFlagExact();		}
	bool getFlagCreatedTime()						{ return Command::getFlagCreatedTime(); }
	
	//setter for flags
	void setFlagExact()								{ Command::setFlagExact();				}
};

/*
 *=======================
 
 Command Find
 
 *=======================
 */
class Command_Find: public Command
{
public:
	Command_Find()	{ init(); setCommandType(TP::FIND); }
	~Command_Find()	{};
	
	//getter for fields
	int							getIndex()			{ return Command::getIndex();		}
	std::string					getOptName()		{ return Command::getOptName();		}
	std::time_t					getDueDate()		{ return Command::getDueDate();		}
	std::time_t					getFromDate()		{ return Command::getFromDate();	}
	std::time_t					getToDate()			{ return Command::getToDate();		}
	std::string					getLocation()		{ return Command::getLocation();	}
	std::list<std::string>		getParticipants()	{ return Command::getParticipants();}
	std::string					getNote()			{ return Command::getNote();		}
	TP::PRIORITY				getPriority()		{ return Command::getPriority();	}
	std::list<std::string>		getTags()			{ return Command::getTags();		}
	std::list<std::time_t>		getRemindTime()		{ return Command::getRemindTime();	}
	bool						getTaskState()		{ return Command::getTaskState();	}
	TP::TASK_TYPE				getTaskType()		{ return Command::getTaskType();	}
	
	//setter for fields
	void setIndex(int idx)							{ Command::setIndex(idx);			}
	void setOptName(std::string name)				{ Command::setOptName(name);		}
	void setDueDate(std::time_t dueDate)			{ Command::setDueDate(dueDate);		}
	void setFromDate(std::time_t fromDate)			{ Command::setFromDate(fromDate);	}
	void setToDate(std::time_t toDate)				{ Command::setToDate(toDate);		}
	void setLocation(std::string location)			{ Command::setLocation(location);	}
	void setParticipants(std::list<std::string> ppl){ Command::setParticipants(ppl);	}
	void setNote(std::string note)					{ Command::setNote(note);			}
	void setPriority(TP::PRIORITY priority)			{ Command::setPriority(priority);	}
	void setTags(std::list<std::string> tags)		{ Command::setTags(tags);			}
	void setRemindTime(std::list<std::time_t> time)	{ Command::setRemindTime(time);		}
	void setTaskState(bool state)					{ Command::setTaskState(state);		}
	void setTaskType(TP::TASK_TYPE type)			{ Command::setTaskType(type);		}

	//getter for flags
	bool getFlagIndex()								{ return Command::getFlagIndex();		}
	bool getFlagExact()								{ return Command::getFlagExact();		}
	bool getFlagName()								{ return Command::getFlagName();		}
	bool getFlagDue()								{ return Command::getFlagDue();			}
	bool getFlagFrom()								{ return Command::getFlagFrom();		}
	bool getFlagTo()								{ return Command::getFlagTo();			}
	bool getFlagLocation()							{ return Command::getFlagLocation();	}
	bool getFlagParticipants()						{ return Command::getFlagParticipants();}
	bool getFlagNote()								{ return Command::getFlagNote();		}
	bool getFlagPriority()							{ return Command::getFlagPriority();	}
	bool getFlagTags()								{ return Command::getFlagTags();		}
	bool getFlagRemindTime()						{ return Command::getFlagRemindTime();	}
	bool getFlagTaskState()							{ return Command::getFlagTaskState();	}
	bool getFlagTaskType()							{ return Command::getFlagTaskType();	}

	//setter for flags
	void setFlagExact()								{ Command::setFlagExact();				}
};

/*
 *=======================
 
 Command Undo
 
 *=======================
 */
class Command_Undo: public Command
{
public:
	Command_Undo()	{ init(); setCommandType(TP::UNDO); }
	~Command_Undo()	{};
};

/*
 *=======================
 
 Command Redo
 
 *=======================
 */
class Command_Redo: public Command
{
public:
	Command_Redo()	{ init(); setCommandType(TP::REDO); }
	~Command_Redo()	{};
};

/*
 *=======================
 
 Command Sync
 
 *=======================
 */
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