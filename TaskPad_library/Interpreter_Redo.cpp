/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Redo.cpp   
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

#include "Interpreter_Redo.h"

using namespace TP;
using namespace std;

//AN JIANGZE (FUNCTIONALITY) LI ZIXUAN (REFACTORING)
//@AN JIANGZE A0105729A 
//@LI ZIXUAN  A0096582R
Command* Interpreter_Redo::interpretRedo(Command_Redo*commandType,string commandStr, Messenger &response, bool &flag){

	response.setCommandType(REDO);
	response.setStatus(SUCCESS);
	return (Command*)commandType;

}