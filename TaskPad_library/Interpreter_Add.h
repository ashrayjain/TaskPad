/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Add.h    
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

#ifndef _INTERPRETER_ADD_H_
#define _INTERPRETER_ADD_H_

#include "Interpreter_base.h"

class Interpreter_Add: public Interpreter_base
{

public:
	Interpreter_Add(){};
	~Interpreter_Add(){};

	Command* interpretAdd(Command_Add* commandType, std::string commandStr, Messenger &response, bool &flag);
};

#endif