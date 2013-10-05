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

#include <list>
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
    // Default Constructor
    Task()						{ defaultTaskInit(); }
    // Constructor that takes in the index for the task
    // Note: Throws Exception on invalid index
    Task(unsigned indexToPut)	{ defaultTaskInit(indexToPut); }

    // Getter Functions common to all Tasks
    TP::TASK_TYPE			getTaskType()		{ return _taskType;			}
    std::string				getName()			{ return _taskName;			}		
    std::string				getLocation()		{ return _taskLocation;		}
    std::string				getNote()			{ return _taskNote;			}
    std::list<std::string>	getParticipants()	{ return _taskParticipants;	}
    std::list<std::string>	getTags()			{ return _taskTags;			}
    std::list<std::time_t>	getRemindTimes()	{ return _taskRemindTimes;	}
    TP::TASK_STATE			getState()			{ return _taskState;		}
    TP::PRIORITY			getPriority()		{ return _taskPriority;		}
    unsigned				getIndex()			{ return _taskIndex;		}
    std::time_t				getFromDate()		{ return _taskFromDate;		}
    std::time_t				getToDate()			{ return _taskToDate;		}
    std::time_t				getDueDate()		{ return _taskDueDate;		}	

    // Setter Functions common to all Tasks
    void setName(std::string newName)								{ _taskName			= newName;			flagName = true;			}		
    void setLocation(std::string newLocation)						{ _taskLocation		= newLocation;		flagLocation = true;		}
    void setNote(std::string newNote)								{ _taskNote			= newNote;			flagNote = true;			}
    void setRemindTimes(std::list<std::time_t> newRemindTimes)		{ _taskRemindTimes	= newRemindTimes;	flagRemindTimes = true;		}
    void setParticipants(std::list<std::string> newParticipants)	{ _taskParticipants = newParticipants;	flagParticipants = true;	}
    void setTags(std::list<std::string> newTags)					{ _taskTags			= newTags;			flagTags = true;			}
    void setPriority(TP::PRIORITY newPriority)						{ _taskPriority		= newPriority;									}
    void setState(TP::TASK_STATE newState)							{ _taskState		= newState;										}	
        
    void setRemindTimes(std::time_t newRemindTime, TP::LIST_OP op);
    void setParticipants(std::string newParticipant, TP::LIST_OP op);
    void setTags(std::string newTag, TP::LIST_OP op);
    
    void setFromDate(std::time_t newFromDate);
    void setToDate(std::time_t newToDate);
    void setDueDate(std::time_t newDueDate);
    

    // Flag Getter Functions common to all Tasks
    bool getFlagName()			{ return flagName; }
    bool getFlagLocation()		{ return flagLocation; }
    bool getFlagParticipants()	{ return flagParticipants; }
    bool getFlagNote()			{ return flagNote; }
    bool getFlagTags()			{ return flagTags; }
    bool getFlagRemindTimes()	{ return flagRemindTimes; }
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
    
    // General helper functions
    bool isIndexInList(unsigned index)			{ return listOfAllIndices.find(index) == listOfAllIndices.end();	}
    void setTaskType(TP::TASK_TYPE taskType)	{ _taskType = taskType;	}
    void handleDatesChange();

    // Task Attributes
    TP::TASK_TYPE			_taskType;
    std::string				_taskName;
    std::string				_taskLocation;
    std::string				_taskNote;
    TP::PRIORITY			_taskPriority;
    std::list<std::string>	_taskParticipants;
    std::list<std::string>	_taskTags;
    std::list<std::time_t>	_taskRemindTimes;
    std::time_t				_taskDueDate;
    std::time_t				_taskFromDate;
    std::time_t				_taskToDate;
    TP::TASK_STATE			_taskState;
    unsigned				_taskIndex;

    // Task Attribute Flags
    bool flagName;
    bool flagDueDate;
    bool flagFromDate;
    bool flagToDate;
    bool flagLocation;
    bool flagParticipants;
    bool flagNote;
    bool flagPriority;
    bool flagTags;
    bool flagRemindTimes;
    bool flagState;
};

#endif
