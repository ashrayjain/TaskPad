/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Add.cpp   
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
#include"Interpreter_Add.h"

using namespace std;
using namespace TP;

//AN JIANGZE (FUNCTIONALITY) LI ZIXUAN (REFACTORING)
// @author A0105729A 
// @author  A0096582R
Command* Interpreter_Add::interpretAdd(Command_Add* commandType, string commandStr, Messenger &response, bool &flag){

	PRIORITY		contentPriority;
	string			contentString;
	list<string>	contentStringList;
	TASK_STATE		contentTaskState;
	TASK_TYPE		contentTaskType;
	time_t			contentTime;
	list<time_t>	contentTimeList;

	if(setDueDateMessage(commandStr,flag,contentTime)){		
		commandType->setDueDate(contentTime);		
	}		
	if(setFromDateMessage(commandStr,flag,contentTime)){
		commandType->setFromDate(contentTime);
	}
	if(setToDateMessage(commandStr,flag,contentTime)){
		commandType->setToDate(contentTime);
	}
	if(setParticipantsMessage(commandStr,flag,contentStringList,FIELD_PPL)){
		commandType->setParticipants(contentStringList);
	}
	if(setGeneralMessage(commandStr,flag,contentString,FIELD_NOTE)){
		commandType->setNote(contentString);
	}
	if(setGeneralMessage(commandStr,flag,contentString,FIELD_AT)){
		commandType->setLocation(contentString);
	}
	if(setRemindTimesMessage(commandStr,flag,contentTimeList,FIELD_RT)){
		commandType->setRemindTimes(contentTimeList);
	}
	if(setPriorityMessage(commandStr,flag,contentPriority)){
		commandType->setPriority(contentPriority);
	}
	if(setTagsMessage(commandStr,flag,contentStringList,FIELD_TAG)){
		commandType->setTags(contentStringList);
	}
	if(commandType->getFlagFrom()==true && commandType->getFlagTo()==true){
		if(commandType->getFromDate()>commandType->getToDate()){
			flag=false;
		}
	}

	if(flag==true){
		response.setStatus(SUCCESS);
		response.setCommandType(ADD);
	}
	else{
		delete commandType;
		commandType=NULL;
	}
	return (Command*)commandType;
}
