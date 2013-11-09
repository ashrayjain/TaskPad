#ifndef _INTERPRETER_REDO_H
#define _INTERPRETER_REDO_H_
#include "Interpreter_base.h"


class Interpreter_Redo: public Interpreter_base
{

public:
	Interpreter_Redo(){};
	~Interpreter_Redo(){};

	Command* interpretRedo(Command_Redo*commandType,std::string commandStr, Messenger &response, bool &flag);


};


#endif