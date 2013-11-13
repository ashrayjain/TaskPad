/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Undo.cpp 
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

#include "Interpreter_Undo.h"

using namespace std;
using namespace TP;

//AN JIANGZE (FUNCTIONALITY) LI ZIXUAN (REFACTORING)
// @author A0105729A 
Command* Interpreter_Undo::interpretUndo(Command_Undo*commandType,string commandStr, Messenger &response, bool &flag){
	response.setCommandType(UNDO);
	response.setStatus(SUCCESS);
	return (Command*)commandType;
}