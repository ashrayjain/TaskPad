/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_base.h    
 *  
 *  Version V0.5
 *  Created 15/10/13 12:00
 *
 *  Author: An Jiangze(A0105729A),Li Zixuan(A0096582R)
 *  Organization: NUS,SOC
 *
==================================================================
==================================================================
 */


/*****************************************************************
*Summary of class:												 *
*Interpreter_Base is a virtual class that provides base functions*
*for its child classes to inherit								 *
*****************************************************************/
#ifndef _INTERPRETER_BASE_H_
#define _INTERPRETER_BASE_H_

#include "Command.h"
#include "Messenger.h"
#include "Enum.h"
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <ctime>


class Interpreter_base{


public:
	Interpreter_base() {};
	virtual ~Interpreter_base() = 0	{};

protected:
    enum ELEMENT_NUM{EMPTY_ITEM,ONE_ITEM,TWO_ITEMS,THREE_ITEMS};

	const static std::string FIELD_NAME;//=" name `[^`]*`";
	const static std::string FIELD_DUE;//=" (due|by) `[^`]*`";
	const static std::string FIELD_FROM;//=" from `[^`]*`";
	const static std::string FIELD_TO;//=" to `[^`]*`";
	const static std::string FIELD_AT;
	const static std::string FIELD_PPL;
	const static std::string FIELD_NOTE;
	const static std::string FIELD_PRIORITY;
	const static std::string FIELD_TAG;
	const static std::string FIELD_RT;
	const static std::string FIELD_TASK_STATE;
	const static std::string FIELD_TASK_TYPE;
	const static std::string FIELD_DUE_REMOVE;
	const static std::string FIELD_FROM_REMOVE;
	const static std::string FIELD_TO_REMOVE;
	const static std::string FIELD_RT_REMOVE_ALL;
	const static std::string FIELD_PPL_REMOVE_ALL;
	const static std::string FIELD_TAG_REMOVE_ALL;
	const static std::string FIELD_RT_REMOVE;
	const static std::string FIELD_PPL_REMOVE;
	const static std::string FIELD_TAG_REMOVE;
	const static std::string FIELD_RT_ADD;
	const static std::string FIELD_PPL_ADD;
	const static std::string FIELD_TAG_ADD;

	const static std::string PRIORITY_HIGH_ABBREV;
	const static std::string PRIORITY_HIGH_FULL;
	const static std::string PRIORITY_MEDIUM_ABBREV;
	const static std::string PRIORITY_MEDIUM_FULL;
	const static std::string PRIORITY_LOW_ABBREV;
	const static std::string PRIORITY_LOW_FULL;

	const static std::string STATUS_UNDONE;
	const static std::string STATUS_DONE;
	const static std::string STATUS_OVERDUE;

	const static std::string TYPE_TIMED;
	const static std::string TYPE_DEADLINE;

    const static std::string ERROR_TIME_FORMAT;
	const static std::string ERROR_DUPLICATE;

	const static char NOTATION_COMMA;
	const static char NOTATION_HASH;
	const static char NOTATION_SPACE;
	const static char NOTATION_SLASH;
	const static char NOTATION_COLON;
	const static char NOTATION_ACCENT_GRAVE;


	const static int INCREMENT_BY_ONE;
	const static int UNINITIALIZED_TIME;
	const static int DEFAULT_TIME;
	const static int DEFAULT_YEAR;
	const static int DEFAULT_MONTH;

	const static int UPPER_LIMIT_YEAR;
	const static int UPPER_LIMIT_MONTH;
	const static int UPPER_LIMIT_DAY;
	const static int UPPER_LIMIT_HOUR;
	const static int UPPER_LIMIT_MINUTE;
	const static int LOWER_LIMIT_YEAR;
	const static int CURRENT_CENTURY;
	const static int DEFAULT_CTIME_BASE_YEAR;

	const static int ELEMENT_ZERO;
	const static int ELEMENT_ONE;
	const static int ELEMENT_TWO;
	const static int ELEMENT_THREE;
	const static int START_LOOP_VALUE;
	const static int START_POSITION_VALUE;

	const static int EMPTY_STRING;
	const static int START_POSITION;
	const static int DUMMY_VALUE;
	const static int CHANGE_BY_ONE;
	const static int CHANGE_BY_TWO;
	 
	bool                             checkTimeValidity        ( int year, bool& isSuccessful, int month, int day, int hour, int min );
	bool                             integerConverter         ( std::string& required, int& number);	
	bool                             checkDuplicate           ( std::string command, std::string cmdTemplate,int startPosition, int matchLength);
	bool                             checkKeyWord             ( std::string command, int position);       
	void                             convertToTime            ( std::string timeStr, std::time_t &result,bool&isSuccessful);
	bool							 extractField             ( std::string command, std::smatch & match, std::regex extractTemplate, std::string& extracted);	
	bool                             setGeneralMessage        ( std::string command, bool&isSuccessful,std::string& content,std::string regexTemplete);
	bool                             setNoParameterMessage    ( std::string command, bool&isSuccessful, std::string regexTemplate);
	bool	                         setRemindTimesMessage    ( std::string command, bool&isSuccessful,std::list<std::time_t>& content,std::string regexTemplate);
	bool	                         setTagsMessage           ( std::string command, bool&isSuccessful,std::list<std::string>& content, std::string regexTemplate);	
	bool	                         setParticipantsMessage   ( std::string command, bool&isSuccessful,std::list<std::string>& content, std::string regexTemplate);
	bool			                 getTaskStateMessage      ( std::string command, bool&isSuccessful,TP::TASK_STATE& content);
	bool			                 getTaskTypeMessage       ( std::string command, bool&isSuccessful,TP::TASK_TYPE& content);
	bool				             getDueDateMessage        ( std::string command, bool&isSuccessful,std::time_t& content);
    bool				             getFromDateMessage       ( std::string command, bool&isSuccessful,std::time_t& content);	
	bool				             getToDateMessage         ( std::string command, bool&isSuccessful,std::time_t& content);
	bool			                 getPriorityMessage       ( std::string command, bool&isSuccessful,TP::PRIORITY& content);	
	bool                             isDueExistance           ( std::string &field,  std::string command, std::smatch match, bool& isSuccessful );
	bool                             isToExistance            ( std::string command, std::smatch match, std::string &field, bool& isSuccessful );
	bool                             isFromExistance          ( std::string command, std::smatch match, std::string &field, bool& isSuccessful );
	int                              extractTagMessage        ( std::string &field,  std::string &subStirng, std::smatch &match, std::list<std::string>&tagList, int count, std::regex extractTemplate );
	int	                             getIndexMessage          ( std::string command, bool&isSuccessful);
	std::time_t                      setTime                  ( std::string commandStr, bool&isSuccessful,bool& isDue);
	std::vector<std::string>         extractNoParameterMessage( std::string command, std::string regexTemplate,int &count); 
	std::string                      toUpper                  ( std::string str);
	std::string                      trim                     ( std::string content);
	void                             setUnitializeTime        ( int &year, struct tm &timeinfo, int &month, int &day, bool& isDue, int &hour, int &min );
	void                             extractTimeWithTwoSlash  ( std::string field, std::string &content, bool &isSuccessful, int& day, int& month, int &year, int& hour, int& min );
	void                             extractTimeWithOneSlash  ( std::string field, std::string &content, bool& isSuccessful, int& day, int& month, int& hour, int& min );
	void                             extractTimeWithEmptySlash( std::string &field, std::string &content, bool& isSuccessful, int& day, int& hour, int& min );
	void							 extractQuotedMessage     ( std::string commandStr, std::string& preContent);
};

#endif