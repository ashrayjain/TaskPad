/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Undo.h    
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

#ifndef _INTERPRETER_UNDO_H_
#define _INTERPRETER_UNDO_H_
#include "Interpreter_base.h"

class Interpreter_Undo: public Interpreter_base
{

public:
	Interpreter_Undo(){};
	~Interpreter_Undo(){};

	Command*            interpretUndo               ( Command_Undo*commandType,std::string commandStr, Messenger &response, bool &flag);


};



#endif