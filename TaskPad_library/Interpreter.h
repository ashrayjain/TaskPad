#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_
#include<string>


class Messenger;
class Command;


class Interpreter{


public:
	Interpreter() {};

	Command* interpretCommand(std::string commandStr, Messenger &response);
	
	void interpretCommand(unsigned ActualIndex, Command *prevCommand);
	
	int interpretIndex(std::string indexStr, Messenger &response); 

protected:
	bool checkCommand(std::string command, int& commandType);
    bool integerConvert(std::string& requiredString, int& number);
	bool integerConverter(std::string& required, int& number);
    
	int getIndexMessage(std::string command,bool&flag);

};
#endif
