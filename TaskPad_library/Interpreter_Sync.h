#include "Interpreter_base.h"



class Interpreter_Sync: public Interpreter_base
{

public:
	Interpreter_Sync(){};
	~Interpreter_Sync(){};

	Command* interpretSync(Command_Sync*commandType,std::string commandStr, Messenger &response, bool &flag);

};

Command* Interpreter_Sync::interpretSync(Command_Sync*commandType,std::string commandStr, Messenger &response, bool &flag){

	string content;
	if(getSyncProviderNameMessage(commandStr,flag,content)){
		commandType->setSyncProviderName(content);
	}
	response.setCommandType(SYNC);
	response.setStatus(SUCCESS);
	return (Command*)commandType;

}