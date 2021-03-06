/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Mod.h    
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


#ifndef _INTERPRETER_MOD_H_
#define _INTERPRETER_MOD_H_
#include "Interpreter_base.h"


class Interpreter_Mod: public Interpreter_base
{
public:
	Interpreter_Mod(){};
	~Interpreter_Mod(){};

	Command*                  interpretModify             ( Command_Mod* commandType, std::string commandStr, Messenger &response, bool &flag);
};

#endif