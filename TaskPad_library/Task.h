/*
 * =====================================================================================
 *
 *       Filename:  Task.h
 *
 *        Version:  1.0
 *        Created:  09/23/13 17:32:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */


/*
 *		How to use these classes
 *
 *
 */

#include <string>
#include <ctime>

#ifndef TASKPAD_TASK_H
#define TASKPAD_TASK_H

/*
 *=======================
 
	Task Base class
 
 *=======================
 */

class Task
{
public:
	enum TASK_TYPE	{TIMED, DEADLINE, FLOATING};
	enum PRIORITY	{HIGH, MEDIUM, LOW};
	enum TASK_STATE {DONE, UNDONE};
	Task() { defaultTaskInit(); }
	TASK_TYPE getTaskType() { return _taskType; }

	
protected:
	// Default Task Initialization
	void defaultTaskInit();
	void setTaskType(TASK_TYPE type) { _taskType = type; }

	// Task Attributes
	TASK_TYPE	_taskType;
	std::string	_taskName;
	std::time_t	_taskDueDate;
	std::time_t	_taskFromDate;
	std::time_t	_taskToDate;
	std::string	_taskLocation;
	std::string	_taskParticipants;
	std::string	_taskNote;
	PRIORITY	_taskPriority;
	std::string	_taskTags;
	std::time_t	_taskRemindTime;
	TASK_STATE	_taskState;
	unsigned	_taskIndex;

	// Task Attribute Flags
	bool flagName;
	bool flagDueDate;
	bool flagFromDate;
	bool flagToDate;
	bool flagLocation;
	bool flagParticipant;
	bool flagNote;
	bool flagPriority;
	bool flagTags;
	bool flagRemindTime;
	bool flagState;
};

/*
 *=======================
 
 Timed Task
 
 *=======================
 */
class TimedTask: public Task
{
public:
	TimedTask()
	{
		defaultTaskInit();
		setTaskType(TIMED);
	}

	std::string	getName()			{ return _taskName;			}		
	std::time_t getFromDate()		{ return _taskFromDate;		}
	std::time_t getToDate()			{ return _taskToDate;		}
	std::string getLocation()		{ return _taskLocation;		}
	std::string getParticipants()	{ return _taskParticipants;	}
	std::string getNote()			{ return _taskNote;			}
	PRIORITY	getPriority()		{ return _taskPriority;		}
	std::string getTags()			{ return _taskTags;			}
	std::time_t getRemindTime()		{ return _taskRemindTime;	}	
	TASK_STATE	getState()			{ return _taskState;		}
	// - Possibly shift to Hash Table
	unsigned	getIndex()			{ return _taskIndex;		}

	// Setter Functions
	void setName(std::string newName)					{ _taskName			= newName;			flagName = true;			}
	void setFromDate(std::time_t newFromDate)			{ _taskFromDate		= newFromDate;		flagFromDate = true;		}
	void setToDate(std::time_t newToDate)				{ _taskToDate		= newToDate;		flagToDate = true;			}
	void setLocation(std::string newLocation)			{ _taskLocation		= newLocation;		flagLocation = true;		}
	void setParticipants(std::string newParticipants)	{ _taskParticipants = newParticipants;	flagParticipant = true;		}
	void setNote(std::string newNote)					{ _taskNote			= newNote;			flagNote = true;			}
	void setPriority(PRIORITY newPriority)				{ _taskPriority		= newPriority;		flagPriority = true;		}
	void setTags(std::string newTags)					{ _taskTags			= newTags;			flagTags = true;			}
	void setRemindTime(std::time_t newRemindTime)		{ _taskRemindTime	= newRemindTime;	flagRemindTime = true;		}
	void setState(TASK_STATE newState)					{ _taskState		= newState;			flagState = true;			}

	// Flag Getter Functions
	bool getFlagName()			{ return flagName; }
	bool getFlagFromDate()		{ return flagFromDate; }
	bool getFlagToDate()		{ return flagToDate; }
	bool getFlagLocation()		{ return flagLocation; }
	bool getFlagParticipant()	{ return flagParticipant; }
	bool getFlagNote()			{ return flagNote; }
	bool getFlagPriority()		{ return flagPriority; }
	bool getFlagTags()			{ return flagTags; }
	bool getFlagRemindTime()	{ return flagRemindTime; }
	bool getFlagState()			{ return flagState; }
};

/*
 *=======================
 
 Floating Task
 
 *=======================
 */
class FloatingTask: public Task
{
public:
	FloatingTask()
	{
		defaultTaskInit();
		setTaskType(FLOATING);
	}

	std::string	getName()			{ return _taskName;			}		
	std::string getLocation()		{ return _taskLocation;		}
	std::string getParticipants()	{ return _taskParticipants;	}
	std::string getNote()			{ return _taskNote;			}
	PRIORITY	getPriority()		{ return _taskPriority;		}
	std::string getTags()			{ return _taskTags;			}
	std::time_t getRemindTime()		{ return _taskRemindTime;	}	
	TASK_STATE	getState()			{ return _taskState;		}
	// - Possibly shift to Hash Table
	unsigned	getIndex()			{ return _taskIndex;		}

	// Setter Functions
	void setName(std::string newName)					{ _taskName			= newName;			flagName = true;			}	
	void setLocation(std::string newLocation)			{ _taskLocation		= newLocation;		flagLocation = true;		}
	void setParticipants(std::string newParticipants)	{ _taskParticipants = newParticipants;	flagParticipant = true;		}
	void setNote(std::string newNote)					{ _taskNote			= newNote;			flagNote = true;			}
	void setPriority(PRIORITY newPriority)				{ _taskPriority		= newPriority;		flagPriority = true;		}
	void setTags(std::string newTags)					{ _taskTags			= newTags;			flagTags = true;			}
	void setRemindTime(std::time_t newRemindTime)		{ _taskRemindTime	= newRemindTime;	flagRemindTime = true;		}
	void setState(TASK_STATE newState)					{ _taskState		= newState;			flagState = true;			}

	// Flag Getter Functions
	bool getFlagName()			{ return flagName; }
	bool getFlagLocation()		{ return flagLocation; }
	bool getFlagParticipant()	{ return flagParticipant; }
	bool getFlagNote()			{ return flagNote; }
	bool getFlagPriority()		{ return flagPriority; }
	bool getFlagTags()			{ return flagTags; }
	bool getFlagRemindTime()	{ return flagRemindTime; }
	bool getFlagState()			{ return flagState; }

};

/*
 *=======================
 
 Deadline Task
 
 *=======================
 */
class DeadlineTask: public Task
{
public:
	DeadlineTask()
	{
		defaultTaskInit();
		setTaskType(DEADLINE);
	}

	std::string	getName()			{ return _taskName;			}		
	std::time_t	getDueDate()		{ return _taskDueDate;		}
	std::string getLocation()		{ return _taskLocation;		}
	std::string getParticipants()	{ return _taskParticipants;	}
	std::string getNote()			{ return _taskNote;			}
	PRIORITY	getPriority()		{ return _taskPriority;		}
	std::string getTags()			{ return _taskTags;			}
	std::time_t getRemindTime()		{ return _taskRemindTime;	}	
	TASK_STATE	getState()			{ return _taskState;		}
	// - Possibly shift to Hash Table
	unsigned	getIndex()			{ return _taskIndex;		}

	// Setter Functions
	void setName(std::string newName)					{ _taskName			= newName;			flagName = true;			}	
	void setDueDate(std::time_t newDueDate)				{ _taskDueDate		= newDueDate;		flagDueDate = true;			}
	void setLocation(std::string newLocation)			{ _taskLocation		= newLocation;		flagLocation = true;		}
	void setParticipants(std::string newParticipants)	{ _taskParticipants = newParticipants;	flagParticipant = true;		}
	void setNote(std::string newNote)					{ _taskNote			= newNote;			flagNote = true;			}
	void setPriority(PRIORITY newPriority)				{ _taskPriority		= newPriority;		flagPriority = true;		}
	void setTags(std::string newTags)					{ _taskTags			= newTags;			flagTags = true;			}
	void setRemindTime(std::time_t newRemindTime)		{ _taskRemindTime	= newRemindTime;	flagRemindTime = true;		}
	void setState(TASK_STATE newState)					{ _taskState		= newState;			flagState = true;			}

	// Flag Getter Functions
	bool getFlagName()			{ return flagName; }
	bool getFlagDueDate()		{ return flagDueDate; }
	bool getFlagLocation()		{ return flagLocation; }
	bool getFlagParticipant()	{ return flagParticipant; }
	bool getFlagNote()			{ return flagNote; }
	bool getFlagPriority()		{ return flagPriority; }
	bool getFlagTags()			{ return flagTags; }
	bool getFlagRemindTime()	{ return flagRemindTime; }
	bool getFlagState()			{ return flagState; }
};

#endif
