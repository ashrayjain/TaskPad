#include "Interpreter_Redo.h"

using namespace TP;
using namespace std;

Command* Interpreter_Redo::interpretRedo(Command_Redo*commandType,string commandStr, Messenger &response, bool &flag){

	response.setCommandType(REDO);
	response.setStatus(SUCCESS);
	return (Command*)commandType;


}