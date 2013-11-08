#include "Interpreter_base.h"


class Interpreter_Undo: public Interpreter_base
{

public:
	Interpreter_Undo(){};
	~Interpreter_Undo(){};

	Command* interpretUndo(Command_Undo*commandType,std::string commandStr, Messenger &response, bool &flag);


};

Command* Interpreter_Undo::interpretUndo(Command_Undo*commandType,std::string commandStr, Messenger &response, bool &flag){

	response.setCommandType(UNDO);
	response.setStatus(SUCCESS);
	return (Command*)commandType;


}