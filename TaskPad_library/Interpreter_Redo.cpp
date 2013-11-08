#include "Interpreter_Redo.h"


Command* Interpreter_Redo::interpretRedo(Command_Redo*commandType,std::string commandStr, Messenger &response, bool &flag){

	response.setCommandType(REDO);
	response.setStatus(SUCCESS);
	return (Command*)commandType;


}