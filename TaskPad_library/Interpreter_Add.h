#ifndef _INTERPRETER_ADD_H_
#define _INTERPRETER_ADD_H_

#include "Interpreter_base.h"


class Interpreter_Add: public Interpreter_base
{

public:
	Interpreter_Add(){};
	~Interpreter_Add(){};

	Command* interpretAdd(Command_Add* commandType, std::string commandStr, Messenger &response, bool &flag);

	std::time_t            setTime(std::string commandStr, bool& _isSuccess,bool& isDue)                        { return Interpreter_base::setTime(commandStr, _isSuccess,isDue); }
	bool              integerConverter(std::string& requiredString, int& number)               { return Interpreter_base::integerConverter(requiredString,number);}
	std::string            toUpper(std::string str)                                                 { return Interpreter_base::toUpper(str);}
	int	              getIndexMessage(std::string command,bool&flag)                           { return Interpreter_base::getIndexMessage(command,flag); }			
	
	

	
	bool             setGeneralMessage(std::string command, bool&flag,std::string& content,std::string regexTemplete) { return Interpreter_base::setGeneralMessage(command,flag,content,regexTemplete);}
	
	bool			  getDueDateMessage(std::string command, bool&flag,std::time_t& content)        { return Interpreter_base::getDueDateMessage(command,flag,content); }
	bool		      getFromDateMessage(std::string command, bool&flag,std::time_t& content)       { return Interpreter_base:: getFromDateMessage(command,flag,content); }
	bool			  getToDateMessage(std::string command, bool&flag,std::time_t& content)         { return Interpreter_base::getToDateMessage(command,flag,content); }

	bool			  getPriorityMessage(std::string command, bool&flag,PRIORITY& content)     { return Interpreter_base::getPriorityMessage(command, flag,content); }
	bool			  getTaskStateMessage(std::string command, bool&flag,TASK_STATE& content)  { return Interpreter_base::getTaskStateMessage(command,flag,content); }
	bool			  getTaskTypeMessage(std::string command, bool&flag,TASK_TYPE& content)    { return Interpreter_base::getTaskTypeMessage(command,flag,content);}
	bool              checkDuplicate(std::string command, std::string cmdTemplate,int startPosition){ return Interpreter_base::checkDuplicate(command, cmdTemplate,startPosition);}
	bool              checkKeyWord(std::string command, int position)                          { return Interpreter_base::checkKeyWord(command,position); }      
	void              convertToTime(std::string timeStr, std::time_t &result,bool&flag)             { Interpreter_base::convertToTime(timeStr,result,flag);}


};

	#endif