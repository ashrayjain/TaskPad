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
	enum PRIORITY	{HIGH, MEDIUM, LOW};
	enum TASK_STATE {DONE, UNDONE};
	Task()  {
		defaultTaskInit();
	}

	// Getter Functions
	std::string	taskName()			{ return _taskName;			}		
	std::time_t	taskDueDate()		{ return _taskDueDate;		}
	std::time_t	taskFromDate()		{ return _taskFromDate;		}
	std::time_t	taskToDate()		{ return _taskToDate;		}
	std::string	taskLocation()		{ return _taskLocation;		}
	std::string	taskParticipants()	{ return _taskParticipants;	}
	std::string	taskNote()			{ return _taskNote;			}
	PRIORITY	taskPriority()		{ return _taskPriority;		}
	std::string	taskTags()			{ return _taskTags;			}
	std::time_t	taskRemindTime()	{ return _taskRemindTime;	}	
	std::time_t taskCreatedTime()	{ return _taskCreatedTime;	}
	// - Possibly shift to Hash Table
	TASK_STATE	taskState()			{ return _taskState;		}
	// - Possibly shift to Hash Table
	unsigned	taskIndex()			{ return _taskIndex;		}

	// Setter Functions
	std::string	taskName(std::string newName)					{ _taskName			= newName;			}	
	std::time_t	taskDueDate(std::time_t newDueDate)				{ _taskDueDate		= newDueDate;		}
	std::time_t	taskFromDate(std::time_t newFromDate)			{ _taskFromDate		= newFromDate;		}
	std::time_t	taskToDate(std::time_t newToDate)				{ _taskToDate		= newToDate;		}
	std::string	taskLocation(std::string newLocation)			{ _taskLocation		= newLocation;		}
	std::string	taskParticipants(std::string newParticipants)	{ _taskParticipants = newParticipants;	}
	std::string	taskNote(std::string newNote)					{ _taskNote			= newNote;			}
	PRIORITY	taskPriority(PRIORITY newPriority)				{ _taskPriority		= newPriority;		}
	std::string	taskTags(std::string newTags)					{ _taskTags			= newTags;			}
	std::time_t	taskRemindTime(std::time_t newRemindTime)		{ _taskRemindTime	= newRemindTime;	}
	// - Possibly shift to Hash Table
	TASK_STATE	taskState(TASK_STATE newState)					{ _taskState		= newState;			}

protected:
	// Default Task Initialization
	void defaultTaskInit();

	// Task Attributes
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
	std::time_t	_taskCreatedTime;
};


#endif
