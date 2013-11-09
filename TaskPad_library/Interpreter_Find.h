#ifndef _INTERPRETER_FIND_H_
#define _INTERPRETER_FIND_H_

#include "Interpreter_base.h"
#include "Natty.h"

class Interpreter_Find: public Interpreter_base
{


	

public:
	Interpreter_Find(){};
	~Interpreter_Find(){};


	Command* interpretFind(Command_Find* commandType, std::string commandStr, Messenger &response, bool &flag);

	std::time_t       setTime(std::string commandStr, bool& flag);  
	void			  setHourMinuteForFromTo(time_t contentTime);		    
	bool		      getFromDateMessage(std::string command, bool&flag,std::time_t& content);      
	bool			  getToDateMessage(std::string command, bool&flag,std::time_t& content);  
	std::time_t		  pullDownFromDate(std::time_t givenTime);
	std::time_t		  pushUpToDate(std::time_t givenTime);
};
#endif




