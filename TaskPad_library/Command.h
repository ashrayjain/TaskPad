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
#include "Command_Add.h"
#include "Command_Mod.h"
#include "Command_Del.h"
#include "Command_Find.h"
#include "Command_Undo.h"
#include "Command_Redo.h"
#include "Command_Sync.h"

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
	int						getIndex()				{ return _index;			 }
	std::string				getName()				{ return _name;				 }
	std::string				getOptName()			{ return _optName;			 }
	std::time_t				getDueDate()			{ return _dueDate;			 }
	std::time_t				getFromDate()			{ return _fromDate;			 }
	std::time_t				getToDate()				{ return _toDate;			 }
	std::string				getLocation()			{ return _location;			 }
	std::list<std::string>	getParticipants()		{ return _participants;		 }
	std::list<std::string>	getRemoveParticipants()	{ return _removeParticipants;}
	std::list<std::string>	getAddParticipants()	{ return _addParticipants;	 }
	std::string				getNote()				{ return _note;				 }
	TP::PRIORITY			getPriority()			{ return _priority;			 }
	std::list<std::string>	getTags()				{ return _tags;				 }
	std::list<std::string>	getRemoveTags()			{ return _removeTags;		 }
	std::list<std::string>	getAddTags()			{ return _addTags;			 }
	std::list<std::time_t>	getRemindTimes()		{ return _remindTime;		 }
	std::list<std::time_t>	getRemoveRemindTimes()	{ return _removeRemindTime;	 }
	std::list<std::time_t>	getAddRemindTimes()		{ return _addRemindTime;	 }
	TP::TASK_STATE			getTaskState()			{ return _taskState;		 }
	TP::TASK_TYPE			getTaskType()			{ return _taskType;			 }
	unsigned long long		getCreatedTime()		{ return _createdTime;		 }
	std::string				getSyncProviderName()	{ return _syncProviderName;  }
	
	//setter for fields
	void setIndex(int idx)							     { _index		      = idx;		FLAG_index		= TP::EDITED;       }
	void setName(std::string name)					     { _name			  = name;		FLAG_name		= TP::EDITED;       }
	void setOptName(std::string name)				     { _optName		      = name;		FLAG_optName	= TP::EDITED;       }
	void setDueDate(std::time_t dueDate)			     { _dueDate		      = dueDate;	FLAG_due		= TP::EDITED;       }
	void setFromDate(std::time_t fromDate)			     { _fromDate		  = fromDate;	FLAG_from		= TP::EDITED;       }
	void setToDate(std::time_t toDate)				     { _toDate		      = toDate;	    FLAG_to			= TP::EDITED;       }
	void setLocation(std::string location)			     { _location		  = location;	FLAG_location	= TP::EDITED;       }
	void setParticipants(std::list<std::string> ppl)     { _participants	  = ppl;		FLAG_participants= TP::EDITED;      }
	void setRemoveParticipants(std::list<std::string>ppl){_removeParticipants = ppl;        FLAG_removeParticipants= TP::EDITED;}
	void setAddParticipants(std::list<std::string>ppl)   {_addParticipants    = ppl;	    FLAG_addParticipants = TP::EDITED;  }
	void setNote(std::string note)					     { _note			  = note;		FLAG_note		= TP::EDITED;       }
	void setPriority(TP::PRIORITY priority)			     { _priority		  = priority;	FLAG_priority	= TP::EDITED;       }
	void setTags(std::list<std::string> tags)		     { _tags			  = tags;		FLAG_tags		= TP::EDITED;       }
	void setRemoveTags(std::list<std::string> tags)	     { _removeTags	      = tags;		FLAG_removeTags	= TP::EDITED;       }
	void setAddTags(std::list<std::string> tags)         { _addTags	          = tags;		FLAG_addTags	= TP::EDITED;		}
	void setRemindTimes(std::list<std::time_t> time)     { _remindTime	      = time;		FLAG_remindTime	= TP::EDITED;       }
	void setRemoveRemindTimes(std::list<std::time_t>time){_removeRemindTime   = time;       FLAG_removeRemindTime = TP::EDITED; }
	void setAddRemindTimes(std::list<std::time_t>time)   {_addRemindTime      = time;	    FLAG_addRemindTime = TP::EDITED;    }
	void setTaskState(TP::TASK_STATE state)			     { _taskState	      = state;	    FLAG_taskState	= TP::EDITED;       }
	void setTaskType(TP::TASK_TYPE type)			     { _taskType		  = type;		FLAG_taskType	= TP::EDITED;       }
	void setCreatedTime(unsigned long long ctime)	     { _createdTime	      = ctime;	    FLAG_createdTime= TP::EDITED;       }
	void setSyncProviderName(std::string syncName)		 { _syncProviderName  = syncName;   FLAG_syncProviderName = TP::EDITED;	}

	//getter for flags
	bool getFlagIndex()								{ return FLAG_index;			    }
	bool getFlagExact()								{ return FLAG_exact;			    }
	bool getFlagName()								{ return FLAG_name;				    }
	bool getFlagOptName()							{ return FLAG_optName;			    }
	bool getFlagDue()								{ return FLAG_due;				    }
	bool getFlagRemoveDue()							{ return FLAG_removeDue;		    }
	bool getFlagFrom()								{ return FLAG_from;				    }
	bool getFlagRemoveFrom()						{ return FLAG_removeFrom;		    }
	bool getFlagTo()								{ return FLAG_to;				    }
	bool getFlagRemoveTo()							{ return FLAG_removeTo;			    }
	bool getFlagLocation()							{ return FLAG_location;			    }
	bool getFlagParticipants()						{ return FLAG_participants;		    }
	bool getFlagRemoveParticipants()				{ return FLAG_removeParticipants;   }
	bool getFlagAddParticipants()					{ return FLAG_addParticipants;	    }
	bool getFlagRemoveAllParticipants()				{ return FLAG_removeAllParticipants;}
	bool getFlagNote()								{ return FLAG_note;				    }
	bool getFlagPriority()							{ return FLAG_priority;			    }
	bool getFlagTags()								{ return FLAG_tags;				    }
	bool getFlagRemoveTags()						{ return FLAG_removeTags;		    }
	bool getFlagAddTags()							{ return FLAG_addTags;				}
	bool getFlagRemoveAllTags()						{ return FLAG_removeAllTags;	    }
	bool getFlagRemindTimes()						{ return FLAG_remindTime;		    }
	bool getFlagRemoveRemindTimes()					{ return FLAG_removeRemindTime;	    }
	bool getFlagAddRemindTimes()					{ return FLAG_addRemindTime;	    }
	bool getFlagRemoveAllRemindTimes()				{ return FLAG_removeAllRemindTimes; }
	bool getFlagTaskState()							{ return FLAG_taskState;		    }
	bool getFlagTaskType()							{ return FLAG_taskType;			    }
	bool getFlagCreatedTime()						{ return FLAG_createdTime;		    }
	bool getFlagSyncProviderName()					{ return FLAG_syncProviderName;     }

	//setter for flags
	void setFlagExact()								{ FLAG_exact = TP::EDITED;		          }
	void setFlagRemoveDue()							{ FLAG_removeDue = TP::EDITED;	          }
	void setFlagRemoveFrom()						{ FLAG_removeFrom = TP::EDITED;	          }
	void setFlagRemoveTo()							{ FLAG_removeTo = TP::EDITED;	          }
	void setFlagRemoveAllTags()						{ FLAG_removeAllTags = TP::EDITED;        }
	void setFlagRemoveAllParticipants()				{ FLAG_removeAllParticipants = TP::EDITED;}
	void setFlagRemoveAllRemindTimes()				{ FLAG_removeAllRemindTimes = TP::EDITED; }

private:
	//default values
	static const bool								DEFAULT_FLAG;
	static const TP::COMMAND_TYPE					DEFAULT_COMMAND_TYPE;
	static const TP::PRIORITY						DEFAULT_PRIORITY;
	static const TP::TASK_STATE						DEFAULT_TASK_STATE;
	static const TP::TASK_TYPE						DEFAULT_TASK_TYPE;
	static const std::string						DEFAULT_STRING;
	static const std::list<std::string>				DEFAULT_LIST_OF_STRING;
	static const std::time_t						DEFAULT_TIME;
	static const std::list<std::time_t>				DEFAULT_REMIND_TIME;
	static const int								DEFAULT_INDEX;
	static const unsigned long long					DEFAULT_CREATED_TIME;
	
	//flags
	bool FLAG_index;
	bool FLAG_exact;
	bool FLAG_name;
	bool FLAG_optName;
	bool FLAG_due;
	bool FLAG_removeDue;
	bool FLAG_from;
	bool FLAG_removeFrom;
	bool FLAG_to;
	bool FLAG_removeTo;
	bool FLAG_location;
	bool FLAG_participants;
	bool FLAG_removeParticipants;
	bool FLAG_addParticipants;
	bool FLAG_removeAllParticipants;
	bool FLAG_note;
	bool FLAG_priority;
	bool FLAG_tags;
	bool FLAG_removeTags;
	bool FLAG_addTags;
	bool FLAG_removeAllTags;
	bool FLAG_remindTime;
	bool FLAG_removeRemindTime;
	bool FLAG_addRemindTime;
	bool FLAG_removeAllRemindTimes;
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
	std::list<std::string>							_removeParticipants;
	std::list<std::string>							_addParticipants;
	std::string										_note;
	TP::PRIORITY									_priority;//high | medium | low
	std::list<std::string>							_tags;
	std::list<std::string>							_removeTags;
	std::list<std::string>							_addTags;
	std::list<std::time_t>							_remindTime;
	std::list<std::time_t>							_removeRemindTime;
	std::list<std::time_t>							_addRemindTime;
	std::string										_syncProviderName;
	TP::TASK_STATE									_taskState;//done | undone
	unsigned long long								_createdTime;
	TP::TASK_TYPE									_taskType;//timed | deadline | floating
};

#endif