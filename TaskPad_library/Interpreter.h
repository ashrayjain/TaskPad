//#pragma once
//#include "Command.h"
//#include "Messenger.h"
//#include "Enum.h"
//#include <string>
//#include <sstream>
//#include <vector>
//#include<regex>
//using namespace std;
//
//#ifndef _INTERPRETER_H_
//#define _INTERPRETER_H_
//
//
//
//class Interpreter{
//
//public:
//	Interpreter(){}
//
//	Command* interpretCommand(std::string commandStr, Messenger &response);
//
//
//	void interpretCommand(unsigned ActualIndex, Command *prevCommand);
//
//	int interpretIndex(std::string indexStr, Messenger &response); 
//
//protected:
//
//	Command* interpretAdd(Command_Add* commandType, std::string commandStr, Messenger &response, bool &flag);
//	Command* interpretModify(Command_Mod* commandType, std::string commandStr, Messenger &response, bool &flag);
//	Command* interpretFind(Command_Find* commandType, std::string commandStr, Messenger &response, bool &flag);
//	Command* interpretDelete(Command_Del*commandType,std::string commandStr, Messenger &response, bool &flag);
//	Command* interpretRedo(Command_Redo*commandType,std::string commandStr, Messenger &response, bool &flag);
//	Command* interpretUndo(Command_Undo*commandType,std::string commandStr, Messenger &response, bool &flag);
//	Command* interpretSync(Command_Sync*commandType,std::string commandStr, Messenger &response, bool &flag);
//
//	time_t setTime(string commandStr, bool& _isSuccess); 
//
//
//	bool integerConverter(string& requiredString, int& number);
//
//
//	bool checkCommand(string command, int& commandType);
//
//
//	int	                getIndexMessage(string command,bool&flag);			
//	bool              getNameMessage(string command,bool&flag,string& content);
//	bool              getOptNameMessage(string command, bool&flag, string& content);
//	bool				getDueDateMessage(string command, bool&flag,time_t& content);
//	bool				getFromDateMessage(string command, bool&flag,time_t& content);	
//	bool				getToDateMessage(string command, bool&flag,time_t& content);
//	bool				getLocationMessage(string command, bool&flag,string& content);
//	bool	getParticipantsMessage(string command, bool&flag,list<std::string>& content);
//	bool				getNoteMessage(string command, bool&flag,string& content);
//	bool			getPriorityMessage(string command, bool&flag,PRIORITY& content);
//	bool	getTagsMessage(string command, bool&flag,list<std::string>& content);
//	bool	getRemindTimesMessage(string command, bool&flag,list<std::time_t>& content);
//	bool			getTaskStateMessage(string command, bool&flag,TASK_STATE& content);
//	bool			getTaskTypeMessage(string command, bool&flag,TASK_TYPE& content);
//	bool				getSyncProviderNameMessage(string command, bool&flag,string& content);
//	bool              checkDuplicate(string command, string cmdTemplate,int startPosition);
//	bool             checkKeyWord(string command, int position);       
//	void              convertToTime(string timeStr, time_t &result,bool&flag);
//
//	unsigned long long		getCreatedTimeMessage(string command, bool&flag);
//
//	string Interpreter::trim(string str);
//
//
//
//
//};
//
//#endif
//
//

/*
 * =====================================================================================
 *
 *       Filename:  Interpreter.h
 *
 *        Version:  1.0
 *        Created:  16/10/13 18:13:09
 *
 *         Author:  XIE KAI (A0102016E), gigikie@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Command.h"
#include "Messenger.h"
#include <regex>
#include <string>
#include <vector>

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

class Interpreter{

public:
	Interpreter():garbagePtr(NULL){}
	Command* interpretCommand(std::string commandStr, Messenger &response);
	int interpretIndex(std::string indexStr, Messenger &response); 

protected:
	/*****************
	* Format Handler *
	******************/
	void buildDelimiterPairs() throw(string);
	bool getCommand(std::string &result) throw(string);
	void validateFormat(TP::COMMAND_TYPE) throw(string);
	/******************
	* Command Handler *
	*******************/
	//Common commands are: Add, Mod, Del, Find, Sync
	template<typename T>
	Command* handleCmdCommon(Messenger &response) throw(std::string);
	//Common commands are: Undo, Redo
	template<typename T>
	Command* handleCmdSimple(Messenger &response) throw(std::string);
	Command* handleCmdAdd(Messenger &response) throw(std::string);
	Command* handleCmdMod(Messenger &response) throw(std::string);
	Command* handleCmdDel(Messenger &response) throw(std::string);
	Command* handleCmdFind( Messenger &response) throw(std::string);
	Command* handleCmdUndo(Messenger &response) throw(std::string);
	Command* handleCmdRedo(Messenger &response) throw(std::string);
	Command* handleCmdSync(Messenger &response) throw(std::string);
	void setSuccess(Messenger &response, TP::COMMAND_TYPE tp);
	void setError(Messenger &response, std::string errorMsg);
	/************
	* Extractor *
	*************/
	bool extractKeyValuePair(std::regex pattern, std::string &result) throw(std::string);
	bool extractValue(std::regex pattern, std::string &result, bool needAppend = false) throw(std::string);
	void checkDuplicant(string searchStr, regex pattern) throw(std::string);
	void checkDuplicant(int lastTimeEndPos, regex pattern) throw(std::string);
	bool isWithinDelimiterPair(int index);
	/************
	* Converter *
	*************/
	bool integerConverter(string& requiredString, int& number);
	void convertToTime(std::string timeStr, std::time_t &result) throw(std::string);
	std::list<std::string> convertToList(std::string listStr) throw(std::string);
	std::list<std::time_t> convertToTimeList(std::string listStr) throw(std::string);
	int convertToEnum(std::string enumStr) throw(std::string);
	/*****************
	* Format Handler *
	******************/
	std::string trim(std::string str);
	std::string toUpper(std::string str) throw(string);

private:
	static const std::string EMPTY;
	static const std::string SHARP;
	static const std::string SPACE;
	static const std::time_t DEFAULT_TIME;
	static const std::string STR_DONE;
	static const std::string STR_UNDONE;
	static const std::string STR_TIMED;
	static const std::string STR_DEADLINE;
	static const std::string STR_FLOATING;
	static const std::string STR_H;
	static const std::string STR_M;
	static const std::string STR_L;
	static const std::string STR_HIGH;
	static const std::string STR_MEDIUM;
	static const std::string STR_LOW;
	static const std::string ERROR_WRONG_COMMAND;
	static const std::string ERROR_WRONG_FORMAT;
	static const std::string ERROR_WRONG_TIME_FORMAT;
	static const std::string CMD_ADD;
	static const std::string CMD_MOD;
	static const std::string CMD_DEL;
	static const std::string CMD_FIND;
	static const std::string CMD_UNDO;
	static const std::string CMD_REDO;
	static const std::string CMD_SYNC;
	static const std::string DELIMITER;
	static const char DELIMITER_COMMA;
	static const std::string DELIMITER_PAIR;
	static const std::string FORMAT_INTEGER;
	static const char* FORMAT_DD_MM;
	static const char* FORMAT_DD_MM_HOUR_MIN;
	static const char* FORMAT_DD_MM_YY;
	static const char* FORMAT_DD_MM_YY_HOUR_MIN;
	static const std::string REGEXP_DATE_DD_MM;
	static const std::string REGEXP_DATE_DD_MM_HOUR_MIN;
	static const std::string REGEXP_DATE_DD_MM_YY;
	static const std::string REGEXP_DATE_DD_MM_YY_HOUR_MIN;
	static const std::string REGEXP_VALUE_INDEX;
	static const std::string REGEXP_VALUE_EXACT;
	static const std::string REGEXP_VALUE_NAME;
	static const std::string REGEXP_VALUE_OPT_NAME;
	static const std::string REGEXP_VALUE_DUE_DATE;
	static const std::string REGEXP_VALUE_FROM_DATE;
	static const std::string REGEXP_VALUE_TO_DATE;
	static const std::string REGEXP_VALUE_LOCATION;
	static const std::string REGEXP_VALUE_PARTICIPANTS;
	static const std::string REGEXP_VALUE_NOTE;
	static const std::string REGEXP_VALUE_PRIORITY;
	static const std::string REGEXP_VALUE_TAGS;
	static const std::string REGEXP_VALUE_REMIND_TIME;
	static const std::string REGEXP_VALUE_SYNC;
	static const std::string REGEXP_VALUE_TASK_STATE;
	static const std::string REGEXP_VALUE_TASK_TYPE;
	static const std::string REGEXP_CMD;
	static const std::string REGEXP_CMD_ADD;
	static const std::string REGEXP_CMD_MOD;
	static const std::string REGEXP_CMD_DEL;
	static const std::string REGEXP_CMD_FIND;
	static const std::string REGEXP_CMD_UNDO;
	static const std::string REGEXP_CMD_REDO;
	static const std::string REGEXP_CMD_SYNC;

	std::vector<std::pair<int, int> > _delimiterPairs;
	std::string _commandStr;
	Command* garbagePtr;
};

template<typename T>
Command* Interpreter::handleCmdCommon(Messenger &response) throw(std::string){
	T* cmdT_ptr = new T();
	garbagePtr = cmdT_ptr;
	validateFormat(cmdT_ptr->getCommandType());

	Command_Mod* cmdMod_ptr = (Command_Mod*) cmdT_ptr;
	Command_Find* cmdFind_ptr = (Command_Find*) cmdMod_ptr;
	Command_Sync* cmdSync_ptr = (Command_Sync*) cmdMod_ptr;

	string index;      string name;
	string optName;    string dueDate;
	string fromDate;   string toDate;
	string location;   string participants;
	string note;       string priority;
	string tags;       string remindTime;
	string taskState;  string taskType;
	string syncName;
	
	time_t time;       list<string> resultList;
	int enumResult;    int indexNum;

	if(extractValue(regex(REGEXP_VALUE_INDEX), index)){
		if(integerConverter(index, indexNum))
			cmdMod_ptr->setIndex(indexNum);
		else
			throw ERROR_WRONG_FORMAT;
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_NAME), name)){
		cmdMod_ptr->setName(name);
	}
	if(extractValue(regex(REGEXP_VALUE_EXACT), string())){
		cmdMod_ptr->setFlagExact();
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_OPT_NAME), optName)){
		if(optName != EMPTY)
			cmdMod_ptr->setOptName(optName);
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_DUE_DATE), dueDate)){
		if(dueDate != EMPTY){
			convertToTime(dueDate, time);
			cmdMod_ptr->setDueDate(time);
		}
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_FROM_DATE), fromDate)){
		if(fromDate != EMPTY){
			convertToTime(fromDate, time);
			cmdMod_ptr->setFromDate(time);
		}
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_TO_DATE), toDate)){
		if(toDate != EMPTY){
			convertToTime(toDate, time);
			cmdMod_ptr->setToDate(time);
		}
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_LOCATION), location)){
		if(location != EMPTY);
			cmdMod_ptr->setLocation(location);
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_PARTICIPANTS), participants)){
		if(participants != EMPTY){
			resultList = convertToList(participants);
			cmdMod_ptr->setParticipants(resultList);
		}
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_NOTE), note)){
		if(note != EMPTY)
			cmdMod_ptr->setNote(note);
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_PRIORITY), priority)){
		if(priority != EMPTY){
			enumResult = convertToEnum(priority);
			if(enumResult != -1)
				cmdMod_ptr->setPriority(static_cast<TP::PRIORITY>(enumResult));
		}
	}
	if(extractValue(regex(REGEXP_VALUE_TAGS), tags, true)){
		if(trim(tags) != ", #"){
			resultList = convertToList(tags);
			cmdMod_ptr->setTags(resultList);
		}
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_REMIND_TIME), remindTime)){
		if(remindTime != EMPTY){
			list<time_t> rt_list = convertToTimeList(remindTime);
			cmdMod_ptr->setRemindTimes(rt_list);
		}
	}
	if(extractValue(regex(REGEXP_VALUE_TASK_STATE), taskState)){
		if(taskState != EMPTY){
			enumResult = convertToEnum(taskState);
			if(enumResult != -1)
				cmdMod_ptr->setTaskState(static_cast<TP::TASK_STATE>(enumResult));
		}
	}
	if(extractValue(regex(REGEXP_VALUE_TASK_TYPE), taskType)){
		if(taskType != EMPTY){
			enumResult = convertToEnum(taskType);
			if(enumResult != -1)
				cmdFind_ptr->setTaskType(static_cast<TP::TASK_TYPE>(enumResult));
		}
	}
	if(extractKeyValuePair(regex(REGEXP_VALUE_SYNC), syncName)){
		if(syncName != EMPTY)
			cmdSync_ptr->setSyncProviderName(syncName);
	}
	setSuccess(response, cmdT_ptr->getCommandType());
	return cmdT_ptr;
}

template<typename T>
Command* Interpreter::handleCmdSimple(Messenger &response) throw(std::string){
	T* cmdT_ptr = new T();
	garbagePtr = cmdT_ptr;
	validateFormat(cmdT_ptr->getCommandType());
	setSuccess(response, cmdT_ptr->getCommandType());
	return cmdT_ptr;
}

#endif
