#pragma once
//#include "Header.h"
#include "Command.h"
#include "Messenger.h"
#include "Enum.h"
#include <string>
#include <sstream>
#include <vector>
#include<regex>
#include<ctime>

using namespace std;

#ifndef _INTERPRETER_CASE_H_
#define _INTERPRETER_CASE_H_


class Interpreter_base{


public:
	Interpreter_base() {};
	virtual ~Interpreter_base() = 0	{};

protected:
	time_t                           setTime(string commandStr, bool&flag,bool& isDue); 
	bool                             integerConverter(string& requiredString, int& number);
    string                           toUpper(string str);
	int	                             getIndexMessage(string command,bool&flag);			
	
/*

	bool                             getNameMessage(string command,bool&flag,string& content);
	bool                             getOptNameMessage(string command, bool&flag, string& content);
	bool				             getLocationMessage(string command, bool&flag,string& content);
	bool				             getNoteMessage(string command, bool&flag,string& content);
	
*/	
	
	bool                             setGeneralMessage(string command, bool&flag,string& content,string regexTemplete);
	bool                             setSingleRemoveMessage(string command, bool&flag, string regexTemplate);
	


	bool			                 getTaskStateMessage(string command, bool&flag,TASK_STATE& content);
	bool			                 getTaskTypeMessage(string command, bool&flag,TASK_TYPE& content);


	bool				             getDueDateMessage(string command, bool&flag,time_t& content);
	bool				             getFromDateMessage(string command, bool&flag,time_t& content);	
	bool				             getToDateMessage(string command, bool&flag,time_t& content);
	
	
	
	
	bool			                 getPriorityMessage(string command, bool&flag,PRIORITY& content);
	
	
	
	
	bool				             getSyncProviderNameMessage(string command, bool&flag,string& content);
	
	


	
	
	bool	                         getRemindTimesMessage(string command, bool&flag,list<std::time_t>& content);
	bool							 getAddRemindTimesMessage(string command, bool&flag,list<std::time_t>& content);
	bool							 getRemoveRemindTimesMessage(string command, bool&flag,list<std::time_t>& content);

	
	bool	                         getTagsMessage(string command, bool&flag,list<std::string>& content);	
	bool							 getAddTagsMessage(string command, bool&flag,list<std::string>& content);
	bool							 getRemoveTagsMessage(string command, bool&flag,list<std::string>& content);
	
	bool	                         getParticipantsMessage(string command, bool&flag,list<std::string>& content);
	bool							 getAddParticipantsMessage(string command, bool&flag,list<std::string>& content);
	bool							 getRemoveParticipantsMessage(string command, bool&flag,list<std::string>& content);
	
	

    bool                             checkDuplicate(string command, string cmdTemplate,int startPosition);
	bool                             checkKeyWord(string command, int position);       
	void                             convertToTime(string timeStr, time_t &result,bool&flag);

};

#endif;