#include"Interpreter_Add.h"


Command* Interpreter_Add::interpretAdd(Command_Add* commandType, std::string commandStr, Messenger &response, bool &flag){

	if(flag && commandType->getFlagDue()==false){		

		std::time_t content;
		if(getDueDateMessage(commandStr,flag,content)){		
			commandType->setDueDate(content);		
		}		
	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagFrom() ==false){

		std::time_t content;
		if(getFromDateMessage(commandStr,flag,content)){
			commandType->setFromDate(content);
		}
	}
	else {
		flag=false;

	}

	if(flag && commandType->getFlagTo() ==false){
		std::time_t content;

		if(getToDateMessage(commandStr,flag,content)){
			commandType->setToDate(content);
		}
	}
	else{

		flag=false;
	}

	if(flag && commandType->getFlagParticipants()==false){

		std::list<std::string>content;
		if(setParticipantsMessage(commandStr,flag,content,FIELD_PPL)){
			commandType->setParticipants(content);
		}

	}
	else{

		flag=false;
	}

	if(flag && commandType->getFlagNote()==false){

		std::string content;
		if(setGeneralMessage(commandStr,flag,content,FIELD_NOTE)){
			commandType->setNote(content);
		}
	}
	else{

		flag=false;

	}

	if(flag && commandType->getFlagLocation()==false){
		std::string content;
		if(setGeneralMessage(commandStr,flag,content,FIELD_AT)){
			commandType->setLocation(content);
		}
	}
	else{

		flag=false;
	}

	if(flag && commandType->getFlagRemindTimes()==false){
		std::list<std::time_t>content;
		if(setRemindTimesMessage(commandStr,flag,content,FIELD_RT)){
			commandType->setRemindTimes(content);
		}
	}
	else{

		flag=false;

	}

	if(flag && commandType->getFlagPriority()==false){
		PRIORITY content;
		if(getPriorityMessage(commandStr,flag,content)){
			commandType->setPriority(content);
		}
	}
	else {
		flag=false;

	}

	if(flag && commandType->getFlagTags()==false){
		std::list<std::string>content;

		if(setTagsMessage(commandStr,flag,content,FIELD_TAG)){
			commandType->setTags(content);
		}
	}
	else {

		flag=false;
	}

	
	if(commandType->getFlagFrom()==true && commandType->getFlagTo()==true){
	
		if(commandType->getFromDate()>commandType->getToDate())flag=false;
	}
	if(flag==true){

		response.setStatus(SUCCESS);
		response.setCommandType(ADD);
		return (Command*)commandType;
	}

	else return NULL;
	}