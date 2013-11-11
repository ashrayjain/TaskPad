/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_Find.cpp  
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


#include "Interpreter_Find.h"
#include "Natty.h"

using namespace std;
using namespace TP;

const int PULLED_DOWN_HOUR=0;
const int PULLED_DOWN_MIN=0;
const int PUSHED_UP_HOUR=23;
const int PUSHED_UP_MIN=59;

//AN JIANGZE (FUNCTIONALITY) LI ZIXUAN (REFACTORING)
//@AN JIANGZE A0105729A 
//@LI ZIXUAN  A0096582R
Command* Interpreter_Find::interpretFind(Command_Find* commandType, string commandStr, Messenger &response, bool &flag){
	
	PRIORITY		contentPriority;
	string			contentString;
	list<string>	contentStringList;
	TASK_STATE		contentTaskState;
	TASK_TYPE		contentTaskType;
	time_t			contentTime;
	list<time_t>	contentTimeList;

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
	if(setTagsMessage(commandStr,flag,contentStringList,FIELD_TAG)){
		commandType->setTags(contentStringList);
	}
	if(setGeneralMessage(commandStr,flag,contentString,FIELD_NAME)){
		commandType->setOptName(contentString);
	}
	if(setTaskStateMessage(commandStr,flag,contentTaskState)){
		commandType->setTaskState(contentTaskState);
	}
	if(setTaskTypeMessage(commandStr,flag,contentTaskType)){
		commandType->setTaskType(contentTaskType);
	}	
	if(setPriorityMessage(commandStr,flag,contentPriority)){
		commandType->setPriority(contentPriority);
	}
	
	if(commandType->getFlagFrom()==true && commandType->getFlagTo()==true){
		if(commandType->getFromDate()>commandType->getToDate()){
			flag=false;
		}
		//pull up and pull down if FromDate=DueDate
		if(commandType->getFromDate()==commandType->getToDate()){
			commandType->setFromDate(pullDownFromDate(commandType->getFromDate()));
			commandType->setToDate(pushUpToDate(commandType->getToDate()));
		}
	}

	if(flag==true){
		response.setStatus(SUCCESS);
		response.setCommandType(FIND);
	}
	else{ 
		delete commandType;
		commandType=NULL;
	}
	return (Command*)commandType;
}

time_t Interpreter_Find::pullDownFromDate(time_t givenTime){
	struct tm pulledDownTime;
	localtime_s(&pulledDownTime,&givenTime);
	pulledDownTime.tm_hour=PULLED_DOWN_HOUR;
	pulledDownTime.tm_min=PULLED_DOWN_MIN;

	return mktime(&pulledDownTime);
}
time_t Interpreter_Find::pushUpToDate(time_t givenTime){
	struct tm pushedUpTime;
	localtime_s(&pushedUpTime,&givenTime);
	pushedUpTime.tm_hour=PUSHED_UP_HOUR;
	pushedUpTime.tm_min=PUSHED_UP_MIN;

	return mktime(&pushedUpTime);
}


