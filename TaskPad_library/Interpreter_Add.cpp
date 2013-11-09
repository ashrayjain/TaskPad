#include"Interpreter_Add.h"


Command* Interpreter_Add::interpretAdd(Command_Add* commandType, std::string commandStr, Messenger &response, bool &flag){

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

	
	if(commandType->getFlagFrom()==true && commandType->getFlagTo()==true){
		if(commandType->getFromDate()>commandType->getToDate()){
			flag=false;
		}
	}

	Command*returnCommand=NULL;
	if(flag==true){
		response.setStatus(SUCCESS);
		response.setCommandType(ADD);
		returnCommand=(Command*)commandType;
	}
	else{ 
		response.setStatus(ERR);
		response.setErrorMsg("fk u");
	}
	return returnCommand;
}