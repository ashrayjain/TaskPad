#ifndef _INTERPRETER_ADD_H_
#define _INTERPRETER_ADD_H_

#include "Interpreter_base.h"

class Interpreter_Add: public Interpreter_base
{

public:
	Interpreter_Add(){};
	~Interpreter_Add(){};

	Command* interpretAdd(Command_Add* commandType, std::string commandStr, Messenger &response, bool &flag);
};

#endif