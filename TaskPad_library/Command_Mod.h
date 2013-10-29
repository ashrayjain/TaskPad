/*
 *=======================
 
 Command Mod
 
 *=======================
 */

#include "Command.h"

#ifndef TASKPAD_COMMAND_MOD_H
#define TASKPAD_COMMAND_MOD_H

class Command_Mod: public Command
{
public:
	Command_Mod()	{ init(); setCommandType(TP::MOD); }
	~Command_Mod()	{};
	
	//getter for fields
	int							getIndex()			    { return Command::getIndex();		      }
	std::string					getName()			    { return Command::getName();		      }
	std::string					getOptName()		    { return Command::getOptName();		      }
	std::time_t					getDueDate()		    { return Command::getDueDate();		      }
	std::time_t					getFromDate()		    { return Command::getFromDate();	      }
	std::time_t					getToDate()			    { return Command::getToDate();		      }
	std::string					getLocation()		    { return Command::getLocation();	      }
	std::list<std::string>		getParticipants()	    { return Command::getParticipants();      }
	std::list<std::string>		getRemoveParticipants()	{ return Command::getRemoveParticipants();}
	std::list<std::string>		getAddParticipants()	{ return Command::getAddParticipants();	  }
	std::string					getNote()			    { return Command::getNote();		      }
	TP::PRIORITY				getPriority()		    { return Command::getPriority();	      }
	std::list<std::string>		getTags()			    { return Command::getTags();		      }
	std::list<std::string>		getRemoveTags()		    { return Command::getRemoveTags();	      }
	std::list<std::time_t>		getRemindTimes()	    { return Command::getRemindTimes();	      }
	std::list<std::time_t>		getRemoveRemindTimes()  { return Command::getRemoveRemindTimes(); }
	std::list<std::time_t>		getAddRemindTimes()	    { return Command::getAddRemindTimes();    }
	TP::TASK_STATE				getTaskState()		    { return Command::getTaskState();	      }
	unsigned long long			getCreatedTime()	    { return Command::getCreatedTime();	      }
	
	//setter for fields
	void setIndex(int idx)							     { Command::setIndex(idx);			    }
	void setName(std::string name)					     { Command::setName(name);			    }
	void setOptName(std::string name)				     { Command::setOptName(name);		    }
	void setDueDate(std::time_t dueDate)			     { Command::setDueDate(dueDate);		}
	void setFromDate(std::time_t fromDate)			     { Command::setFromDate(fromDate);	    }
	void setToDate(std::time_t toDate)				     { Command::setToDate(toDate);		    }
	void setLocation(std::string location)			     { Command::setLocation(location);	    }
	void setParticipants(std::list<std::string> ppl)     { Command::setParticipants(ppl);	    }
	void setRemoveParticipants(std::list<std::string>ppl){ Command::setRemoveParticipants(ppl); }
	void setAddParticipants(std::list<std::string>ppl)   { Command::setAddParticipants(ppl);    }
	void setNote(std::string note)					     { Command::setNote(note);			    }
	void setPriority(TP::PRIORITY priority)			     { Command::setPriority(priority);	    }
	void setTags(std::list<std::string> tags)		     { Command::setTags(tags);			    }
	void setRemoveTags(std::list<std::string> tags)	     { Command::setRemoveTags(tags);		}
	void setRemindTimes(std::list<std::time_t> time)     { Command::setRemindTimes(time);	    }
	void setRemoveRemindTimes(std::list<std::time_t>time){ Command::setRemoveRemindTimes(time); }
	void setAddRemindTimes(std::list<std::time_t>time)   { Command::setAddRemindTimes(time);    }
	void setTaskState(TP::TASK_STATE state)			     { Command::setTaskState(state);		}
	void setCreatedTime(unsigned long long ctime)	     { Command::setCreatedTime(ctime);	    }
	
	//getter for flags
	bool getFlagIndex()								{ return Command::getFlagIndex();		         }
	bool getFlagExact()								{ return Command::getFlagExact();		         }
	bool getFlagName()								{ return Command::getFlagName();		         }
	bool getFlagOptName()							{ return Command::getFlagOptName();		         }
	bool getFlagDue()								{ return Command::getFlagDue();			         }
	bool getFlagRemoveDue()							{ return Command::getFlagRemoveDue();	         }
	bool getFlagFrom()								{ return Command::getFlagFrom();		         }
	bool getFlagRemoveFrom()						{ return Command::getFlagRemoveFrom();	         }
	bool getFlagTo()								{ return Command::getFlagTo();			         }
	bool getFlagRemoveTo()							{ return Command::getFlagRemoveTo();	         }
	bool getFlagLocation()							{ return Command::getFlagLocation();	         }
	bool getFlagParticipants()						{ return Command::getFlagParticipants();         }
	bool getFlagRemoveParticipants()				{ return Command::getFlagRemoveParticipants();   }
	bool getFlagAddParticipants()					{ return Command::getFlagAddParticipants();	     }
	bool getFlagRemoveAllParticipants()				{ return Command::getFlagRemoveAllParticipants();}
	bool getFlagNote()								{ return Command::getFlagNote();		         }
	bool getFlagPriority()							{ return Command::getFlagPriority();	         }
	bool getFlagTags()								{ return Command::getFlagTags();		         }
	bool getFlagRemoveTags()						{ return Command::getFlagRemoveTags();	         }
	bool getFlagRemoveAllTags()						{ return Command::getFlagRemoveAllTags();        }
	bool getFlagRemindTimes()						{ return Command::getFlagRemindTimes();	         }
	bool getFlagRemoveRemindTimes()					{ return Command::getFlagRemoveRemindTimes();	 }
	bool getFlagAddRemindTimes()					{ return Command::getFlagAddRemindTimes();	     }
	bool getFlagRemoveAllRemindTimes()				{ return Command::getFlagRemoveAllRemindTimes(); }
	bool getFlagTaskState()							{ return Command::getFlagTaskState();	         }
	bool getFlagCreatedTime()						{ return Command::getFlagCreatedTime();          }
	
	//setter for flags
	void setFlagExact()								{ Command::setFlagExact();				  }
	void setFlagRemoveDue()							{ Command::setFlagRemoveDue();	          }
	void setFlagRemoveFrom()						{ Command::setFlagRemoveFrom();	          }
	void setFlagRemoveTo()							{ Command::setFlagRemoveTo();	          }
	void setFlagRemoveAllTags()						{ Command::setFlagRemoveAllTags();        }
	void setFlagRemoveAllParticipants()				{ Command::setFlagRemoveAllParticipants();}
	void setFlagRemoveAllRemindTimes()				{ Command::setFlagRemoveAllRemindTimes(); }
};

#endif
