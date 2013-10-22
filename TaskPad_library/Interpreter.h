#pragma once
#include "Command.h"
#include "Messenger.h"
#include "Enum.h"
#include <string>
#include <sstream>
#include <vector>
#include<regex>
using namespace std;

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_



class Interpreter{

public:
	Interpreter(){}

	Command* interpretCommand(std::string commandStr, Messenger &response);


	void interpretCommand(unsigned ActualIndex, Command *prevCommand);

	int interpretIndex(std::string indexStr, Messenger &response); 

protected:

	Command* interpretAdd(Command_Add* commandType, std::string commandStr, Messenger &response, bool &flag);
	Command* interpretModify(Command_Mod* commandType, std::string commandStr, Messenger &response, bool &flag);
	Command* interpretFind(Command_Find* commandType, std::string commandStr, Messenger &response, bool &flag);
	Command* interpretDelete(Command_Del*commandType,std::string commandStr, Messenger &response, bool &flag);
	Command* interpretRedo(Command_Redo*commandType,std::string commandStr, Messenger &response, bool &flag);
	Command* interpretUndo(Command_Undo*commandType,std::string commandStr, Messenger &response, bool &flag);
	Command* interpretSync(Command_Sync*commandType,std::string commandStr, Messenger &response, bool &flag);

	time_t setTime(string commandStr, bool& _isSuccess); 


	bool integerConverter(string& requiredString, int& number);


	bool checkCommand(string command, int& commandType);


	int	                getIndexMessage(string command,bool&flag);			
	bool              getNameMessage(string command,bool&flag,string& content);
	bool              getOptNameMessage(string command, bool&flag, string& content);
	bool				getDueDateMessage(string command, bool&flag,time_t& content);
	bool				getFromDateMessage(string command, bool&flag,time_t& content);	
	bool				getToDateMessage(string command, bool&flag,time_t& content);
	bool				getLocationMessage(string command, bool&flag,string& content);
	bool	getParticipantsMessage(string command, bool&flag,list<std::string>& content);
	bool				getNoteMessage(string command, bool&flag,string& content);
	bool			getPriorityMessage(string command, bool&flag,PRIORITY& content);
	bool	getTagsMessage(string command, bool&flag,list<std::string>& content);
	bool	getRemindTimesMessage(string command, bool&flag,list<std::time_t>& content);
	bool			getTaskStateMessage(string command, bool&flag,TASK_STATE& content);
	bool			getTaskTypeMessage(string command, bool&flag,TASK_TYPE& content);
	bool				getSyncProviderNameMessage(string command, bool&flag,string& content);
	bool              checkDuplicate(string command, string cmdTemplate,int startPosition);
	bool             checkKeyWord(string command, int position);       
	void              convertToTime(string timeStr, time_t &result,bool&flag);

	unsigned long long		getCreatedTimeMessage(string command, bool&flag);

	string Interpreter::trim(string str);




};

#endif
