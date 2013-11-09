#ifndef _INTERPRETER_UNDO_H_
#define _INTERPRETER_UNDO_H_
#include "Interpreter_base.h"


class Interpreter_Undo: public Interpreter_base
{

public:
	Interpreter_Undo(){};
	~Interpreter_Undo(){};

	Command* interpretUndo(Command_Undo*commandType,std::string commandStr, Messenger &response, bool &flag);


};



#endif