
/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Delete.h    
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


#ifndef _INTERPRETER_DELETE_H_
#define _INTERPRETER_DELETE_H_
#include "Interpreter_base.h"

class Interpreter_Delete: public Interpreter_base
{


public:
	Interpreter_Delete(){};
	~Interpreter_Delete(){};

	Command*          interpretDelete             ( Command_Del*commandType,std::string commandStr, Messenger &response, bool &flag);
	int	              getIndexMessage             ( std::string command,bool&flag)  { return Interpreter_base::getIndexMessage(command,flag); }
};

#endif