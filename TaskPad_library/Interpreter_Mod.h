#ifndef _INTERPRETER_MOD_H_
#define _INTERPRETER_MOD_H_
#include "Interpreter_base.h"


class Interpreter_Mod: public Interpreter_base
{
public:
	Interpreter_Mod(){};
	~Interpreter_Mod(){};

	Command* interpretModify(Command_Mod* commandType, std::string commandStr, Messenger &response, bool &flag);
};

#endif