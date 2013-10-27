#include "Interpreter_base.h"


class Interpreter_Redo: public Interpreter_base
{

public:
	Interpreter_Redo(){};
	~Interpreter_Redo(){};

	Command* interpretRedo(Command_Redo*commandType,std::string commandStr, Messenger &response, bool &flag);


};

Command* Interpreter_Redo::interpretRedo(Command_Redo*commandType,std::string commandStr, Messenger &response, bool &flag){

	response.setCommandType(REDO);
	response.setStatus(SUCCESS);
	return (Command*)commandType;


}