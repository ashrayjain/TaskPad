#include "Interpreter_Undo.h"


Command* Interpreter_Undo::interpretUndo(Command_Undo*commandType,std::string commandStr, Messenger &response, bool &flag){

	response.setCommandType(UNDO);
	response.setStatus(SUCCESS);
	return (Command*)commandType;


}