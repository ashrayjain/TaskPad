#include "Interpreter_base.h"


class Interpreter_Delete: public Interpreter_base
{


public:
	Interpreter_Delete(){};
	~Interpreter_Delete(){};

	Command*          interpretDelete(Command_Del*commandType,std::string commandStr, Messenger &response, bool &flag);
	int	              getIndexMessage(string command,bool&flag)  { return Interpreter_base::getIndexMessage(command,flag); }
};

Command* Interpreter_Delete:: interpretDelete(Command_Del*commandType,std::string commandStr, Messenger &response, bool &flag){

	stringstream extractContent(commandStr);
	string content;
	getline(extractContent,content,'`');
	content.clear();
	getline(extractContent,content,'`');
	commandType->setName(content);


	response.setCommandType(DEL);
	response.setStatus(SUCCESS);
	return (Command*)commandType;

}