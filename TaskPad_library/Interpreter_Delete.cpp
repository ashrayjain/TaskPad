
/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Delete.cpp    
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

#include "Interpreter_Delete.h"

using namespace TP;
using namespace std;

//AN JIANGZE (FUNCTIONALITY) LI ZIXUAN (REFACTORING)
//@AN JIANGZE A0105729A 
//@LI ZIXUAN  A0096582R
Command* Interpreter_Delete:: interpretDelete(Command_Del*commandType,string commandStr, Messenger &response, bool &flag){

	stringstream extractContent(commandStr);
	string content;
	getline(extractContent,content,NOTATION_ACCENT_GRAVE);
	content.clear();
	getline(extractContent,content,NOTATION_ACCENT_GRAVE);
	commandType->setName(content);


	response.setCommandType(DEL);
	response.setStatus(SUCCESS);
	return (Command*)commandType;

}