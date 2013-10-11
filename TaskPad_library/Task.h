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
#include <chrono>
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
    Task()								{ defaultTaskInit(); }
    // Constructor that takes in the index for the task
    // Note: Throws Exception on invalid index
    Task(unsigned long long indexToPut)	{ defaultTaskInit(indexToPut); }
	~Task()								{ listOfAllIndices.erase(_taskIndex); }

    // Getter Functions common to all Tasks
    TP::TASK_TYPE			getTaskType()		const { return _taskType;			}
    std::string				getName()			const { return _taskName;			}		
    std::string				getLocation()		const { return _taskLocation;		}
    std::string				getNote()			const { return _taskNote;			}
    std::list<std::string>	getParticipants()	const { return _taskParticipants;	}
    std::list<std::string>	getTags()			const { return _taskTags;			}
    std::list<std::time_t>	getRemindTimes()	const { return _taskRemindTimes;	}
    TP::TASK_STATE			getState()			const { return _taskState;			}
    TP::PRIORITY			getPriority()		const { return _taskPriority;		}
    unsigned long long		getIndex()			const { return _taskIndex;			}
    std::time_t				getFromDate()		const { return _taskFromDate;		}
    std::time_t				getToDate()			const { return _taskToDate;			}
    std::time_t				getDueDate()		const { return _taskDueDate;		}
	
	std::list<std::list<Task*>::iterator>	getHashTagPtrs()	  { return _hashTagPtrs;		}

    // Setter Functions common to all Tasks
    void setName(std::string newName)								{ _taskName			= newName;			flagName = true;			}		
    void setLocation(std::string newLocation)						{ _taskLocation		= newLocation;		flagLocation = true;		}
    void setNote(std::string newNote)								{ _taskNote			= newNote;			flagNote = true;			}
    void setRemindTimes(std::list<std::time_t> newRemindTimes)		{ _taskRemindTimes	= newRemindTimes;	flagRemindTimes = true;		}
    void setParticipants(std::list<std::string> newParticipants)	{ _taskParticipants = newParticipants;	flagParticipants = true;	}
    void setTags(std::list<std::string> newTags)					{ _taskTags			= newTags;			flagTags = true;			}
	void setPriority(TP::PRIORITY newPriority)						{ _taskPriority		= newPriority;		flagPriority = true;		}
	void setState(TP::TASK_STATE newState)							{ _taskState		= newState;			flagState = true;			}	
        
    void setRemindTimes(std::time_t newRemindTime, TP::LIST_OP op);
    void setParticipants(std::string newParticipant, TP::LIST_OP op);
    void setTags(std::string newTag, TP::LIST_OP op);
    
    void setFromDate(std::time_t newFromDate);
    void setToDate(std::time_t newToDate);
    void setDueDate(std::time_t newDueDate);
    
	void setHashTagPtrs(std::list<std::list<Task*>::iterator> newHashTagPtrs)	{ _hashTagPtrs		= newHashTagPtrs;								}

    // Flag Getter Functions common to all Tasks
    bool getFlagName()			const { return flagName;			}
    bool getFlagLocation()		const { return flagLocation;		}
    bool getFlagParticipants()	const { return flagParticipants;	}
    bool getFlagNote()			const { return flagNote;			}
    bool getFlagTags()			const { return flagTags;			}
    bool getFlagRemindTimes()	const { return flagRemindTimes;		}
    bool getFlagFromDate()		const { return flagFromDate;		}
    bool getFlagToDate()		const { return flagToDate;			}
    bool getFlagDueDate()		const { return flagDueDate;			}
	bool getFlagState()			const { return flagState;			}
	bool getFlagPriority()		const { return flagPriority;		}

protected:
    // Set of All currently used indices
    static std::set<unsigned long long> listOfAllIndices;

    // Default Task Initialization
    void defaultTaskInit();
    void defaultTaskInit(unsigned long long indexToPut);
    void initFlags();
    void initTaskAttributes();
    
    // General helper functions
    bool isIndexInList(unsigned long long index)	{ return listOfAllIndices.find(index) == listOfAllIndices.end();	}
    void setTaskType(TP::TASK_TYPE taskType)		{ _taskType = taskType;	}
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
    unsigned long long		_taskIndex;
	
	// Ptrs to hash table for hashtags
	std::list<std::list<Task*>::iterator>	_hashTagPtrs;

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
