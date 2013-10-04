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
#include <set>
#include "Enum.h"

#ifndef TASKPAD_TASK_H
#define TASKPAD_TASK_H

const std::string INDEX_INVALID_ERROR = "is not a valid index!";

/*
 *=======================
 
	Task Base class
 
 *=======================
 */

class Task
{
public:
	Task()						{ defaultTaskInit(); }
	Task(unsigned indexToPut)	{ defaultTaskInit(indexToPut); }
	TP::TASK_TYPE getTaskType() { return _taskType; }

	// Getter Functions common to all Tasks
	std::string		getName()			{ return _taskName;			}		
	std::string		getLocation()		{ return _taskLocation;		}
	std::string		getParticipants()	{ return _taskParticipants;	}
	std::string		getNote()			{ return _taskNote;			}
	TP::PRIORITY	getPriority()		{ return _taskPriority;		}
	std::string		getTags()			{ return _taskTags;			}
	std::time_t		getRemindTime()		{ return _taskRemindTime;	}	
	TP::TASK_STATE	getState()			{ return _taskState;		}
	unsigned		getIndex()			{ return _taskIndex;		}
	std::time_t		getFromDate()		{ return _taskFromDate;		}
	std::time_t		getToDate()			{ return _taskToDate;		}
	std::time_t		getDueDate()		{ return _taskDueDate;		}

	// Setter Functions common to all Tasks
	void setName(std::string newName)					{ _taskName			= newName;			flagName = true;			}	
	void setLocation(std::string newLocation)			{ _taskLocation		= newLocation;		flagLocation = true;		}
	void setParticipants(std::string newParticipants)	{ _taskParticipants = newParticipants;	flagParticipant = true;		}
	void setNote(std::string newNote)					{ _taskNote			= newNote;			flagNote = true;			}
	void setPriority(TP::PRIORITY newPriority)			{ _taskPriority		= newPriority;		flagPriority = true;		}
	void setTags(std::string newTags)					{ _taskTags			= newTags;			flagTags = true;			}
	void setRemindTime(std::time_t newRemindTime)		{ _taskRemindTime	= newRemindTime;	flagRemindTime = true;		}
	void setState(TP::TASK_STATE newState)				{ _taskState		= newState;			flagState = true;			}
	void setFromDate(std::time_t newFromDate)			{ _taskFromDate		= newFromDate;		flagFromDate = true;		}
	void setToDate(std::time_t newToDate)				{ _taskToDate		= newToDate;		flagToDate = true;			}
	void setDueDate(std::time_t newDueDate)				{ _taskDueDate		= newDueDate;		flagDueDate = true;			}

	// Flag Getter Functions common to all Tasks
	bool getFlagName()			{ return flagName; }
	bool getFlagLocation()		{ return flagLocation; }
	bool getFlagParticipant()	{ return flagParticipant; }
	bool getFlagNote()			{ return flagNote; }
	bool getFlagPriority()		{ return flagPriority; }
	bool getFlagTags()			{ return flagTags; }
	bool getFlagRemindTime()	{ return flagRemindTime; }
	bool getFlagState()			{ return flagState; }
	bool getFlagFromDate()		{ return flagFromDate; }
	bool getFlagToDate()		{ return flagToDate; }
	bool getFlagDueDate()		{ return flagDueDate; }

	
protected:
	// Set of All currently used indices
	static std::set<unsigned> listOfAllIndices;

	// Default Task Initialization
	void defaultTaskInit();
	void defaultTaskInit(unsigned indexToPut);
	void initFlags();
	void initTaskAttributes();
	bool isIndexInList(unsigned index) 
	{ 
		return listOfAllIndices.find(index) == listOfAllIndices.end();
	}
	void setTaskType(TP::TASK_TYPE type) { _taskType = type; }	

	// Task Attributes
	TP::TASK_TYPE	_taskType;
	std::string		_taskName;
	std::time_t		_taskDueDate;
	std::time_t		_taskFromDate;
	std::time_t		_taskToDate;
	std::string		_taskLocation;
	std::string		_taskParticipants;
	std::string		_taskNote;
	TP::PRIORITY	_taskPriority;
	std::string		_taskTags;
	std::time_t		_taskRemindTime;
	TP::TASK_STATE	_taskState;
	unsigned		_taskIndex;

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

#endif
