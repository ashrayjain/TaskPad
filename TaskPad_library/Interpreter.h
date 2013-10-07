#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_


#include "Command.h"
#include "Messenger.h"

#include <sstream>
#include<string>


class Interpreter{
	//dsadsada
public:
	Interpreter(){}

	Command* interpretCommand(std::string commandStr, Messenger &response);
	//void interpretCommand(std::time_t ActualIndex, Command *prevCommand);

	void interpretCommand(unsigned ActualIndex, Command *prevCommand);

	int interpretIndex(std::string indexStr, Messenger &response); 

protected:

	Command* interpretAdd(Command_Add* commandType, std::string commandStr, Messenger &response, bool &flag);
	Command* interpretModify(Command_Mod* commandType, std::string commandStr, Messenger &response, bool &flag);

	Command* interpretFind(Command_Find* commandType, std::string commandStr, Messenger &response, bool &flag);


	time_t setTime(string commandStr, bool&); 


	bool integerConverter(string& requiredString, int& number);
	bool checkPun(string requiredString);



};

#endif
