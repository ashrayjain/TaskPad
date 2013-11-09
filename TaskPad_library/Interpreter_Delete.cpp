#include "Interpreter_Delete.h"

Command* Interpreter_Delete:: interpretDelete(Command_Del*commandType,std::string commandStr, Messenger &response, bool &flag){

	std::stringstream extractContent(commandStr);
	std::string content;
	getline(extractContent,content,NOTATION_ACCENT_GRAVE);
	content.clear();
	getline(extractContent,content,NOTATION_ACCENT_GRAVE);
	commandType->setName(content);


	response.setCommandType(DEL);
	response.setStatus(SUCCESS);
	return (Command*)commandType;

}