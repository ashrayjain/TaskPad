/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Mod.cpp 
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

#include "Interpreter_Mod.h"

using namespace std;
using namespace TP;

Command* Interpreter_Mod ::interpretModify(Command_Mod* commandType, string commandStr, Messenger &response, bool &flag){

	PRIORITY		contentPriority;
	string			contentString;
	list<string>	contentStringList;
	TASK_STATE		contentTaskState;
	TASK_TYPE		contentTaskType;
	time_t			contentTime;
	list<time_t>	contentTimeList;

	if(getDueDateMessage(commandStr,flag,contentTime)){		
		commandType->setDueDate(contentTime);		
		
	}		
	if(getFromDateMessage(commandStr,flag,contentTime)){
		commandType->setFromDate(contentTime);
		
	}
	if(getToDateMessage(commandStr,flag,contentTime)){
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
	if(getPriorityMessage(commandStr,flag,contentPriority)){
		commandType->setPriority(contentPriority);
	}
	if(setTagsMessage(commandStr,flag,contentStringList,FIELD_TAG)){
		commandType->setTags(contentStringList);
	}
	if(setGeneralMessage(commandStr,flag,contentString,FIELD_NAME)){
		commandType->setOptName(contentString);
	}
	if(getTaskStateMessage(commandStr,flag,contentTaskState)){
		commandType->setTaskState(contentTaskState);
	}
	if(setNoParameterMessage(commandStr,flag,FIELD_DUE_REMOVE)){
		commandType->setFlagRemoveDue();
	}
	if(setNoParameterMessage(commandStr,flag,FIELD_FROM_REMOVE)){
		commandType->setFlagRemoveFrom();
	}
	if(setNoParameterMessage(commandStr,flag,FIELD_TO_REMOVE)){
		commandType->setFlagRemoveTo();
	}
	if(setNoParameterMessage(commandStr,flag,FIELD_RT_REMOVE_ALL)){
		commandType->setFlagRemoveAllRemindTimes();
	}
	if(setNoParameterMessage(commandStr,flag,FIELD_PPL_REMOVE_ALL)){
		commandType->setFlagRemoveAllParticipants();
	}
	if(setNoParameterMessage(commandStr,flag,FIELD_TAG_REMOVE_ALL)){
		commandType->setFlagRemoveAllTags();
	}
	if(setRemindTimesMessage(commandStr,flag, contentTimeList,FIELD_RT_REMOVE)){
		commandType->setRemoveRemindTimes(contentTimeList);
	}
	if(setParticipantsMessage(commandStr,flag, contentStringList,FIELD_PPL_REMOVE)){
		commandType->setRemoveParticipants(contentStringList);
	}
	else if(setTagsMessage(commandStr,flag, contentStringList,FIELD_TAG_REMOVE)){
		commandType->setRemoveTags(contentStringList);
	}
	if(setRemindTimesMessage(commandStr,flag, contentTimeList,FIELD_RT_ADD)){
		commandType->setAddRemindTimes(contentTimeList);
	}
	if(setParticipantsMessage(commandStr,flag, contentStringList,FIELD_PPL_ADD)){
		commandType->setAddParticipants(contentStringList);
	}
	if(setTagsMessage(commandStr,flag, contentStringList,FIELD_TAG_ADD)){
		commandType->setAddTags(contentStringList);
	}

	if(commandType->getFlagFrom()==true && commandType->getFlagTo()==true){
		if(commandType->getFromDate()>commandType->getToDate()){
			flag=false;
		}
	}

	if(flag==true){
		response.setStatus(SUCCESS);
		response.setCommandType(MOD);
	}
	else{
		delete commandType;
		commandType=NULL;
	}
	return (Command*)commandType;
}
