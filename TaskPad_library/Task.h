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

#ifndef TASKPAD_TASK_H
#define TASKPAD_TASK_H

#include <list>
#include <string>
#include <ctime>
#include <chrono>
#include <set>
#include "Enum.h"

//@ASHRAY JAIN A0105199B


/*
 *=======================
 
    Task Base class
 
 *=======================
 */

class Task
{
public:
    // Default Constructor
    Task()								{ defaultTaskInit(false); }
	// Constructor that takes in the task name
	Task(std::string nameTo)			{ defaultTaskInit(); setName(nameTo); }
    // Constructor that takes in the index for the task
    // Note: Throws Exception on invalid index
    Task(unsigned long long indexToPut)	{ defaultTaskInit(indexToPut); }
	
	// Overloaded Operators
	bool operator<	(const Task& a) const { return this->getIndex() < a.getIndex();  }
	bool operator==	(const Task& a) const { return this->getIndex() == a.getIndex(); }
	bool operator<=	(const Task& a) const { return !operator>(a);					 }
	bool operator>=	(const Task& a) const { return !operator<(a);					 }
	bool operator!=	(const Task& a) const { return !operator==(a);					 }
	bool operator>	(const Task& a) const { return this->getIndex() > a.getIndex();  }

	// Default Attributes Values
	static const unsigned long long						DEFAULT_INDEX;
	static const std::string							DEFAULT_NAME;
	static const std::string							DEFAULT_LOCATION;
	static const std::string							DEFAULT_NOTE;
	static const TP::PRIORITY							DEFAULT_PRIORITY;
	static const std::list<std::string>					DEFAULT_PARTICIPANTS;
	static const std::list<std::string>					DEFAULT_TAGS;
	static const std::list<std::list<Task*>::iterator>	DEFAULT_HASHTAG_PTRS;
	static const std::list<std::time_t>					DEFAULT_REMINDTIMES;
	static const std::list<std::list<Task*>::iterator>	DEFAULT_REMINDTIMES_PTRS;
	static const std::time_t							DEFAULT_FROMDATE;
	static const std::time_t							DEFAULT_TODATE;
	static const TP::TASK_STATE							DEFAULT_STATE;	
	static const TP::TASK_TYPE							DEFAULT_TYPE;

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
    std::time_t				getDueDate()		const { return _taskFromDate;		}
	
	std::list<std::list<Task*>::iterator>	getHashTagPtrs()		{ return _hashTagPtrs;			}
	std::list<std::list<Task*>::iterator>	getRemindTimesPtrs()	{ return _remindTimesPtrs;		}
	
	void freeIndex()												{ listOfAllIndices.erase(_taskIndex);	}	
	static void flushAllIndices()									{ listOfAllIndices.clear();				}
    
	// Setter Functions common to all Tasks
	void setName(std::string newName)								{ _taskName			= newName;			flagName			= _taskName	!= DEFAULT_NAME;				}		
    void setLocation(std::string newLocation)						{ _taskLocation		= newLocation;		flagLocation		= _taskLocation	!= DEFAULT_LOCATION;		}
	void setNote(std::string newNote)								{ _taskNote			= newNote;			flagNote			= _taskNote	!= DEFAULT_NOTE;				}
    void setRemindTimes(std::list<std::time_t> newRemindTimes)		{ _taskRemindTimes	= newRemindTimes;	flagRemindTimes		= _taskRemindTimes != DEFAULT_REMINDTIMES;	}
    void setParticipants(std::list<std::string> newParticipants)	{ _taskParticipants = newParticipants;	flagParticipants	= _taskParticipants != DEFAULT_PARTICIPANTS;}
    void setTags(std::list<std::string> newTags)					{ _taskTags			= newTags;			flagTags			= _taskTags	!= DEFAULT_TAGS;				}
	void setPriority(TP::PRIORITY newPriority)						{ _taskPriority		= newPriority;		flagPriority		= true;										}
	void setState(TP::TASK_STATE newState)							{ _taskState		= newState;			flagState			= true;										}	
        
    void setRemindTimes(std::time_t newRemindTime, TP::LIST_OP op);
    void setParticipants(std::string newParticipant, TP::LIST_OP op);
    void setTags(std::string newTag, TP::LIST_OP op);
    
    void setFromDate(std::time_t newFromDate);
    void setToDate(std::time_t newToDate);
    void setDueDate(std::time_t newDueDate);
    
	void setHashTagPtrs(std::list<std::list<Task*>::iterator> newHashTagPtrs)			{ _hashTagPtrs = newHashTagPtrs; }
	void setRemindTimesPtrs(std::list<std::list<Task*>::iterator> newRemindTimesPtrs)	{ _remindTimesPtrs = newRemindTimesPtrs; }

    // Flag Getter Functions common to all Tasks
    bool getFlagName()			const { return flagName;			}
    bool getFlagLocation()		const { return flagLocation;		}
    bool getFlagParticipants()	const { return flagParticipants;	}
    bool getFlagNote()			const { return flagNote;			}
    bool getFlagTags()			const { return flagTags;			}
    bool getFlagRemindTimes()	const { return flagRemindTimes;		}
    bool getFlagFromDate()		const { return flagFromDate;		}
    bool getFlagToDate()		const { return flagToDate;			}
	bool getFlagState()			const { return flagState;			}
	bool getFlagPriority()		const { return flagPriority;		}
	bool getFlagDueDate()		const { return flagFromDate && flagToDate && _taskFromDate == _taskToDate;	}
	bool getFlagIndex()			const { return _taskIndex!=DEFAULT_INDEX;       }

protected:
	// Task Class Constants
	static const std::string							INDEX_INVALID_ERROR;
	
    // Set of All currently used indices
    static std::set<unsigned long long> listOfAllIndices;

    // Default Task Initialization
	void defaultTaskInit(bool createIndex = true);
    void defaultTaskInit(unsigned long long indexToPut);
    void initFlags();
    void initTaskAttributes();
    
    // General helper functions
    bool isIndexInList(unsigned long long index)	{ return listOfAllIndices.find(index) != listOfAllIndices.end();	}
    void setTaskType(TP::TASK_TYPE taskType)		{ _taskType = taskType;	}
    void handleDatesChange();
	void removeSingleRTOccurrence(std::time_t &toRemove);
	void removeSingleParticipansOccurrence(std::string &toRemove);
	void removeSingleTagOccurrence(std::string &toRemove);

    // Task Attributes
    TP::TASK_TYPE			_taskType;
    std::string				_taskName;
    std::string				_taskLocation;
    std::string				_taskNote;
    TP::PRIORITY			_taskPriority;
    std::list<std::string>	_taskParticipants;
    std::list<std::string>	_taskTags;
    std::list<std::time_t>	_taskRemindTimes;
    std::time_t				_taskFromDate;
    std::time_t				_taskToDate;
    TP::TASK_STATE			_taskState;
    unsigned long long		_taskIndex;
	
	// Ptrs to hash table for hashtags
	std::list<std::list<Task*>::iterator>	_hashTagPtrs;
	std::list<std::list<Task*>::iterator>	_remindTimesPtrs;

    // Task Attribute Flags
    bool flagName;
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
