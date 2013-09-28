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
	virtual			~Command() = 0  {};
	TP::COMMAND_TYPE	getCommandType(){ return _type; }
	
protected:
	void init();
	void setCommandType(TP::COMMAND_TYPE type){ _type = type; }

	//default values
	static const bool			DEFAULT_FLAG;
	static const TP::COMMAND_TYPE	DEFAULT_COMMAND_TYPE;
	static const TP::PRIORITY		DEFAULT_PRIORITY;
	static const bool			DEFAULT_TASK_STATE;
	static const TP::TASK_TYPE		DEFAULT_TASK_TYPE;
	static const std::string	DEFAULT_STRING;
	static const std::time_t	DEFAULT_TIME;
	static const int			DEFAULT_INDEX;
	static const unsigned		DEFAULT_CREATED_TIME;
	
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
	TP::COMMAND_TYPE	_type;//add | mod | del | find | undo | redo | sync | undefined
	int				_index;
	std::string		_name;
	std::string		_optName;
	std::time_t		_dueDate;
	std::time_t		_fromDate;
	std::time_t		_toDate;
	std::string		_location;
	std::string		_participants;
	std::string		_note;
	TP::PRIORITY		_priority;//high | medium | low
	std::string		_tags;
	std::time_t		_remindTime;
	std::string		_syncProviderName;
	bool			_taskState;//done | undone
	unsigned		_createdTime;
	TP::TASK_TYPE		_taskType;//timed | deadline | floating
};

/*
 *=======================
 
 Command Add
 
 *=======================
 */
class Command_Add: public Command
{
public:
	Command_Add() { init(); setCommandType(TP::ADD); }
	~Command_Add(){};
	
	//getter for fields
	std::string		getName()						{ return _name; }
	std::time_t		getDueDate()					{ return _dueDate; }
	std::time_t		getFromDate()					{ return _fromDate; }
	std::time_t		getToDate()						{ return _toDate; }
	std::string		getLocation()					{ return _location; }
	std::string		getParticipants()				{ return _participants; }
	std::string		getNote()						{ return _note; }
	TP::PRIORITY		getPriority()					{ return _priority; }
	std::string		getTags()						{ return _tags; }
	std::time_t		getRemindTime()					{ return _remindTime; }
	
	//setter for fields
	void setName(std::string name)					{ _name = name;			FLAG_name = TP::EDITED; }
	void setDueDate(std::time_t dueDate)			{ _dueDate = dueDate;	FLAG_due = TP::EDITED; }
	void setFromDate(std::time_t fromDate)			{ _fromDate = fromDate;	FLAG_from = TP::EDITED; }
	void setToDate(std::time_t toDate)				{ _toDate = toDate;		FLAG_to = TP::EDITED; }
	void setLocation(std::string location)			{ _location = location;	FLAG_location = TP::EDITED; }
	void setParticipants(std::string ppl)			{ _participants = ppl;	FLAG_location = TP::EDITED; }
	void setNote(std::string note)					{ _note = note;			FLAG_note = TP::EDITED; }
	void setPriority(TP::PRIORITY priority)				{ _priority = priority;	FLAG_priority = TP::EDITED; }
	void setTags(std::string tags)					{ _tags = tags;			FLAG_tags = TP::EDITED; }
	void setRemindTime(std::time_t time)			{ _remindTime = time;	FLAG_remindTime = TP::EDITED; }
	
	//getter for flags
	bool getFlagName()								{ return FLAG_name; }
	bool getFlagDue()								{ return FLAG_due; }
	bool getFlagFrom()								{ return FLAG_from; }
	bool getFLagTo()								{ return FLAG_to; }
	bool getFlagLocation()							{ return FLAG_location; }
	bool getFlagParticipants()						{ return FLAG_participants; }
	bool getFlagNote()								{ return FLAG_note; }
	bool getFLagPriority()							{ return FLAG_priority; }
	bool getFlagTags()								{ return FLAG_tags; }
	bool getFlagRemindTime()						{ return FLAG_remindTime; }
};

/*
 *=======================
 
 Command Mod
 
 *=======================
 */
class Command_Mod: public Command
{
public:
	Command_Mod(){ init(); setCommandType(TP::MOD); }
	~Command_Mod(){};
	
	//getter for fields
	int				getIndex()						{ return _index; }
	std::string		getName()						{ return _name; }
	std::string		getOptName()					{ return _optName; }
	std::time_t		getDueDate()					{ return _dueDate; }
	std::time_t		getFromDate()					{ return _fromDate; }
	std::time_t		getToDate()						{ return _toDate; }
	std::string		getLocation()					{ return _location; }
	std::string		getParticipants()				{ return _participants; }
	std::string		getNote()						{ return _note; }
	TP::PRIORITY		getPriority()					{ return _priority; }
	std::string		getTags()						{ return _tags; }
	std::time_t		getRemindTime()					{ return _remindTime; }
	bool			getTaskState()					{ return _taskState; }
	unsigned		getCreatedTime()				{ return _createdTime; }
	
	//setter for fields
	void setIndex(int idx)							{ _index = idx;			FLAG_index = TP::EDITED; }
	void setName(std::string name)					{ _name = name;			FLAG_name = TP::EDITED; }
	void setOptName(std::string name)				{ _optName = name;		FLAG_optName = TP::EDITED; }
	void setDueDate(std::time_t dueDate)			{ _dueDate = dueDate;	FLAG_due = TP::EDITED; }
	void setFromDate(std::time_t fromDate)			{ _fromDate = fromDate;	FLAG_from = TP::EDITED; }
	void setToDate(std::time_t toDate)				{ _toDate = toDate;		FLAG_to = TP::EDITED; }
	void setLocation(std::string location)			{ _location = location;	FLAG_location = TP::EDITED; }
	void setParticipants(std::string ppl)			{ _participants = ppl;	FLAG_location = TP::EDITED; }
	void setNote(std::string note)					{ _note = note;			FLAG_note = TP::EDITED; }
	void setPriority(TP::PRIORITY priority)				{ _priority = priority;	FLAG_priority = TP::EDITED; }
	void setTags(std::string tags)					{ _tags = tags;			FLAG_tags = TP::EDITED; }
	void setRemindTime(std::time_t time)			{ _remindTime = time;	FLAG_remindTime = TP::EDITED; }
	void setTaskState(bool state)					{ _taskState = state;	FLAG_taskState = TP::EDITED; }
	void setCreatedTime(unsigned ctime)				{ _createdTime = ctime;	FLAG_createdTime = TP::EDITED; }
	
	//getter for flags
	bool getFlagIndex()								{ return FLAG_index; }
	bool getFlagExact()								{ return FLAG_exact; }
	bool getFlagName()								{ return FLAG_name; }
	bool getFlagDue()								{ return FLAG_due; }
	bool getFlagFrom()								{ return FLAG_from; }
	bool getFLagTo()								{ return FLAG_to; }
	bool getFlagLocation()							{ return FLAG_location; }
	bool getFlagParticipants()						{ return FLAG_participants; }
	bool getFlagNote()								{ return FLAG_note; }
	bool getFLagPriority()							{ return FLAG_priority; }
	bool getFlagTags()								{ return FLAG_tags; }
	bool getFlagRemindTime()						{ return FLAG_remindTime; }
	bool getFlagTaskState()							{ return FLAG_taskState; }
	bool getFlagCreatedTime()						{ return FLAG_createdTime; }
	
	//setter for flags
	void setFlagExact()								{ FLAG_exact = TP::EDITED; }
};

/*
 *=======================
 
 Command Del
 
 *=======================
 */
class Command_Del: public Command
{
public:
	Command_Del(){ init(); setCommandType(TP::DEL); }
	~Command_Del(){};
	
	//getter for fields
	int				getIndex()						{ return _index; }
	std::string		getName()						{ return _name; }
	unsigned		getCreatedTime()				{ return _createdTime; }
	
	//setter for fields
	void setIndex(int idx)							{ _index = idx;			FLAG_index = TP::EDITED; }
	void setName(std::string name)					{ _name = name;			FLAG_name = TP::EDITED; }
	void setCreatedTime(unsigned ctime)				{ _createdTime = ctime;	FLAG_createdTime = TP::EDITED; }
	
	//getter for flags
	bool getFlagIndex()								{ return FLAG_index; }
	bool getFlagExact()								{ return FLAG_exact; }
	bool getFlagCreatedTime()						{ return FLAG_createdTime; }
	
	//setter for flags
	void setFlagExact()								{ FLAG_exact = TP::EDITED; }
};

/*
 *=======================
 
 Command Find
 
 *=======================
 */
class Command_Find: public Command
{
public:
	Command_Find(){ init(); setCommandType(TP::FIND); }
	~Command_Find(){};
	
	//getter for fields
	int				getIndex()						{ return _index; }
	std::string		getOptName()					{ return _optName; }
	std::time_t		getDueDate()					{ return _dueDate; }
	std::time_t		getFromDate()					{ return _fromDate; }
	std::time_t		getToDate()						{ return _toDate; }
	std::string		getLocation()					{ return _location; }
	std::string		getParticipants()				{ return _participants; }
	std::string		getNote()						{ return _note; }
	TP::PRIORITY		getPriority()					{ return _priority; }
	std::string		getTags()						{ return _tags; }
	std::time_t		getRemindTime()					{ return _remindTime; }
	bool			getTaskState()					{ return _taskState; }
	TP::TASK_TYPE		getTaskType()					{ return _taskType; }
	
	//setter for fields
	void setIndex(int idx)							{ _index = idx;			FLAG_index = TP::EDITED; }
	void setOptName(std::string name)				{ _optName = name;		FLAG_optName = TP::EDITED; }
	void setDueDate(std::time_t dueDate)			{ _dueDate = dueDate;	FLAG_due = TP::EDITED; }
	void setFromDate(std::time_t fromDate)			{ _fromDate = fromDate;	FLAG_from = TP::EDITED; }
	void setToDate(std::time_t toDate)				{ _toDate = toDate;		FLAG_to = TP::EDITED; }
	void setLocation(std::string location)			{ _location = location;	FLAG_location = TP::EDITED; }
	void setParticipants(std::string ppl)			{ _participants = ppl;	FLAG_location = TP::EDITED; }
	void setNote(std::string note)					{ _note = note;			FLAG_note = TP::EDITED; }
	void setPriority(TP::PRIORITY priority)				{ _priority = priority;	FLAG_priority = TP::EDITED; }
	void setTags(std::string tags)					{ _tags = tags;			FLAG_tags = TP::EDITED; }
	void setRemindTime(std::time_t time)			{ _remindTime = time;	FLAG_remindTime = TP::EDITED; }
	void setTaskState(bool state)					{ _taskState = state;	FLAG_taskState = TP::EDITED; }
	void setTaskType(TP::TASK_TYPE type)				{ _taskType = type;		FLAG_taskType = TP::EDITED; }
	
	//getter for flags
	bool getFlagIndex()								{ return FLAG_index; }
	bool getFlagExact()								{ return FLAG_exact; }
	bool getFlagName()								{ return FLAG_name; }
	bool getFlagDue()								{ return FLAG_due; }
	bool getFlagFrom()								{ return FLAG_from; }
	bool getFLagTo()								{ return FLAG_to; }
	bool getFlagLocation()							{ return FLAG_location; }
	bool getFlagParticipants()						{ return FLAG_participants; }
	bool getFlagNote()								{ return FLAG_note; }
	bool getFLagPriority()							{ return FLAG_priority; }
	bool getFlagTags()								{ return FLAG_tags; }
	bool getFlagRemindTime()						{ return FLAG_remindTime; }
	bool getFlagTaskState()							{ return FLAG_taskState; }
	bool getFlagTaskType()							{ return FLAG_taskType; }
	
	//setter for flags
	void setFlagExact()								{ FLAG_exact = TP::EDITED; }
};

/*
 *=======================
 
 Command Undo
 
 *=======================
 */
class Command_Undo: public Command
{
public:
	Command_Undo(){ init(); setCommandType(TP::UNDO); }
	~Command_Undo(){};
};

/*
 *=======================
 
 Command Redo
 
 *=======================
 */
class Command_Redo: public Command
{
public:
	Command_Redo(){ init(); setCommandType(TP::REDO); }
	~Command_Redo(){};
};

/*
 *=======================
 
 Command Sync
 
 *=======================
 */
class Command_Sync: public Command
{
public:
	Command_Sync(){ init(); setCommandType(TP::SYNC); }
	~Command_Sync(){};
	
	//getter for fields
	std::string		getSyncProviderName()			{ return _syncProviderName; }
	
	//setter for fields
	void setSyncProviderName(std::string syncName)	{ _syncProviderName = syncName; FLAG_syncProviderName = TP::EDITED; }

	//getter for flags
	bool getFlagSyncProviderName()					{ return FLAG_syncProviderName; }
};

#endif