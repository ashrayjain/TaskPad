#ifndef _INTERPRETER_DELETE_H_
#define _INTERPRETER_DELETE_H_
#include "Interpreter_base.h"


class Interpreter_Delete: public Interpreter_base
{


public:
	Interpreter_Delete(){};
	~Interpreter_Delete(){};

	Command*          interpretDelete(Command_Del*commandType,std::string commandStr, Messenger &response, bool &flag);
	int	              getIndexMessage(std::string command,bool&flag)  { return Interpreter_base::getIndexMessage(command,flag); }
};

#endif