/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Redo.h    
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


#ifndef _INTERPRETER_REDO_H
#define _INTERPRETER_REDO_H_
#include "Interpreter_base.h"

class Interpreter_Redo: public Interpreter_base
{

public:
	Interpreter_Redo(){};
	~Interpreter_Redo(){};

	Command*                interpretRedo            ( Command_Redo*commandType,std::string commandStr, Messenger &response, bool &flag);

    
};


#endif