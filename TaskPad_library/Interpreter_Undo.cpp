#include "Interpreter_Undo.h"

using namespace std;
using namespace TP;

Command* Interpreter_Undo::interpretUndo(Command_Undo*commandType,string commandStr, Messenger &response, bool &flag){

	response.setCommandType(UNDO);
	response.setStatus(SUCCESS);
	return (Command*)commandType;


}