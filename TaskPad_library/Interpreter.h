/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter.h    
 *  
 *  Version V0.5
 *  Created 15/10/13 12:00
 *
 *  Author: An Jiangze(A0105729A),Li Zixuan(A0096582R)
 *  Organization: NUS,SOC
 *
==================================================================
==================================================================
 */

/*****************************************************************
*Summary of class:												 *
*Interpreter is a facade for all interpreter classes			 *
*****************************************************************/


#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_
#include<string>


class Messenger;
class Command;


class Interpreter{

private:
	//local functions
	bool                 checkCommand    (std::string command, int& commandType);
	int                  getIndexMessage (std::string command,bool&isSuccessful);
    bool                 integerConvert  (std::string& requiredString, int& number);
	void                 extractQuotedMessage(std::string field, std::string& quotedMessage);
public:
	Interpreter          () {};
	//APIs
	Command*             interpretCommand(std::string commandStr, Messenger &response);	
	void                 interpretCommand(unsigned ActualIndex, Command *prevCommand);	
};
#endif
