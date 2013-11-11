
/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Find.h    
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

#ifndef _INTERPRETER_FIND_H_
#define _INTERPRETER_FIND_H_

#include "Interpreter_base.h"
#include "Natty.h"

class Interpreter_Find: public Interpreter_base{
private:
	
	std::time_t        setTime                            ( std::string commandStr, bool& flag);  
	bool               checkTimeValidityForFind           ( int year, bool& flag, int month, int day, int hour, int min );
	void               setUnitializeTimeForFind           ( int &year, struct tm &timeinfo, int &month, int &day, int &hour, int &min, int &second );
	void			   setHourMinuteForFromTo             ( time_t contentTime);
	void               extractTimeWithTwoSlashForFind     ( std::string commandStr, std::string &content, bool &flag, int& day, int& month, int &year, int& hour, int& min );
	bool               extractTimeWithOneSlashForFind     ( std::string commandStr, std::string &content, bool& flag, int& day, int& month, int& hour, int& min );
	bool               extractTimeWithNoSlashForFind      ( std::string &commandStr, std::string &content, bool& flag, int& day, int& hour, int& min );
	bool		       getFromDateMessage                 ( std::string command, bool&flag,std::time_t& content);      
	bool			   getToDateMessage                   ( std::string command, bool&flag,std::time_t& content);  
	std::time_t		   pullDownFromDate                   ( std::time_t givenTime);
	std::time_t		   pushUpToDate                       ( std::time_t givenTime);
public:
	Interpreter_Find                                      (){};
	~Interpreter_Find                                     (){};
    Command*           interpretFind                      ( Command_Find* commandType, std::string commandStr, Messenger &response, bool &flag);
};
#endif




