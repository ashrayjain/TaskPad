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

const string FIELD_NAME=" name `[^`]*`";
const string FIELD_DUE=" due `[^`]*`";
const string FIELD_FROM=" from `[^`]*`";
const string FIELD_TO=" to `[^`]*`";
const string FIELD_AT=" at `[^`]*`";
const string FIELD_PPL=" ppl `[^`]*`";
const string FIELD_NOTE=" note `[^`]*`";
const string FIELD_PRIORITY=" impt `[^`]*`";
const string FIELD_TAG="\\s(#[^( |`)]*)(\\s|$)";
const string FIELD_RT=" rt `[^`]*`";
const string FIELD_TASK_STATE="\\s(done|undone|overdue)(\\s|$)";
const string FIELD_TASK_TYPE="\\s(timed|deadline|floating)(\\s|$)";
const string FIELD_DUE_REMOVE="\\s(-due)(\\s|$)";
const string FIELD_FROM_REMOVE="\\s(-from)(\\s|$)";
const string FIELD_TO_REMOVE="\\s(-to)(\\s|$)";
const string FIELD_RT_REMOVE_ALL="\\s(-rtall)(\\s|$)";
const string FIELD_PPL_REMOVE_ALL="\\s(-pplall)(\\s|$)";
const string FIELD_TAG_REMOVE_ALL="\\s(-#)(\\s|$)";
const string FIELD_RT_REMOVE=" -rt `[^`]*`";
const string FIELD_PPL_REMOVE=" -ppl `[^`]*`";
const string FIELD_TAG_REMOVE="\\s(-#[^( |`)]*)(\\s|$)";
const string FIELD_RT_ADD=" \\+rt `[^`]*`";
const string FIELD_PPL_ADD=" \\+ppl `[^`]*`";
const string FIELD_TAG_ADD="\\s(\\+#[^( |`)]*)(\\s|$)";

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
	bool                             setNoParameterMessage(string command, bool&flag, string regexTemplate);
	


	bool			                 getTaskStateMessage(string command, bool&flag,TASK_STATE& content);
	bool			                 getTaskTypeMessage(string command, bool&flag,TASK_TYPE& content);


	bool				             getDueDateMessage(string command, bool&flag,time_t& content);
	bool				             getFromDateMessage(string command, bool&flag,time_t& content);	
	bool				             getToDateMessage(string command, bool&flag,time_t& content);
	
	
	
	
	bool			                 getPriorityMessage(string command, bool&flag,PRIORITY& content);
	
	
	
	
	bool				             getSyncProviderNameMessage(string command, bool&flag,string& content);
	
	
	bool	                         setRemindTimesMessage(string command, bool&flag,list<std::time_t>& content,string regexTemplate);
	bool	                         setTagsMessage(string command, bool&flag,list<std::string>& content, string regexTemplate);
	bool	                         setParticipantsMessage(string command, bool&flag,list<std::string>& content, string regexTemplate);

	
	/*
	bool	                         getRemindTimesMessage(string command, bool&flag,list<std::time_t>& content);
	bool							 getAddRemindTimesMessage(string command, bool&flag,list<std::time_t>& content);
	bool							 getRemoveRemindTimesMessage(string command, bool&flag,list<std::time_t>& content);

	bool	                         getTagsMessage(string command, bool&flag,list<std::string>& content);	
	bool							 getAddTagsMessage(string command, bool&flag,list<std::string>& content);
	bool							 getRemoveTagsMessage(string command, bool&flag,list<std::string>& content);
	
	bool	                         getParticipantsMessage(string command, bool&flag,list<std::string>& content);
	bool							 getAddParticipantsMessage(string command, bool&flag,list<std::string>& content);
	bool							 getRemoveParticipantsMessage(string command, bool&flag,list<std::string>& content);
	*/
	
	//bool							 getOverdueMsg(string command, bool&flag);
	

    bool                             checkDuplicate(string command, string cmdTemplate,int startPosition);
	bool                             checkKeyWord(string command, int position);       
	void                             convertToTime(string timeStr, time_t &result,bool&flag);

};

#endif;