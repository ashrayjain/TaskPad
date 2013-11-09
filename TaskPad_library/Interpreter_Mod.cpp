#include "Interpreter_Mod.h"


Command* Interpreter_Mod ::interpretModify(Command_Mod* commandType, std::string commandStr, Messenger &response, bool &flag){

	if(flag && commandType->getFlagDue()==false){		

		time_t content;
		if(getDueDateMessage(commandStr,flag,content)){		
			commandType->setDueDate(content);		
		}		

	}
	else {

		flag=false;
	}


	if(flag && commandType->getFlagFrom() ==false){

		time_t content;
		if(getFromDateMessage(commandStr,flag,content)){
			commandType->setFromDate(content);
		}
	}
	else {
		flag=false;

	}


	if(flag && commandType->getFlagTo() ==false){
		time_t content;

		if(getToDateMessage(commandStr,flag,content)){
			commandType->setToDate(content);
		}
	}
	else{

		flag=false;
	}


	if(flag && commandType->getFlagParticipants()==false){

		list<string>content;
		if(setParticipantsMessage(commandStr,flag,content,FIELD_PPL)){
			commandType->setParticipants(content);
		}

	}
	else{

		flag=false;
	}

	if(flag && commandType->getFlagNote()==false){

		string content;
		if(setGeneralMessage(commandStr,flag,content,FIELD_NOTE)){
			commandType->setNote(content);
		}
	}
	else{

		flag=false;

	}

	if(flag && commandType->getFlagLocation()==false){
		string content;
		if(setGeneralMessage(commandStr,flag,content,FIELD_AT)){
			commandType->setLocation(content);
		}
	}
	else{

		flag=false;
	}

	if(flag && commandType->getFlagRemindTimes()==false){
		list<time_t>content;
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
		list<string>content;

		if(setTagsMessage(commandStr,flag,content,FIELD_TAG)){
			commandType->setTags(content);
		}
	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagOptName()==false){

		string content;
		if(setGeneralMessage(commandStr,flag,content,FIELD_NAME)){
			commandType->setOptName(content);
		}
	}
	else {
		flag=false;
	}

	if(flag && commandType->getFlagTaskState()==false){

		TASK_STATE content;
		if(getTaskStateMessage(commandStr,flag,content)){
			commandType->setTaskState(content);
		}
	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveDue()==false){

		if(setNoParameterMessage(commandStr,flag,FIELD_DUE_REMOVE)){
			commandType->setFlagRemoveDue();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveFrom()==false){

		if(setNoParameterMessage(commandStr,flag,FIELD_FROM_REMOVE)){
			commandType->setFlagRemoveFrom();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveTo()==false){

		if(setNoParameterMessage(commandStr,flag,FIELD_TO_REMOVE)){
			commandType->setFlagRemoveTo();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveAllRemindTimes()==false){

		if(setNoParameterMessage(commandStr,flag,FIELD_RT_REMOVE_ALL)){
			commandType->setFlagRemoveAllRemindTimes();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveAllParticipants()==false){

		if(setNoParameterMessage(commandStr,flag,FIELD_PPL_REMOVE_ALL)){
			commandType->setFlagRemoveAllParticipants();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveAllTags()==false){

		if(setNoParameterMessage(commandStr,flag,FIELD_TAG_REMOVE_ALL)){
			commandType->setFlagRemoveAllTags();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveRemindTimes()==false){

		list<time_t>content;
		if(setRemindTimesMessage(commandStr,flag, content,FIELD_RT_REMOVE)){
			commandType->setRemoveRemindTimes(content);
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveParticipants()==false){

		list<string>content;
		if(setParticipantsMessage(commandStr,flag, content,FIELD_PPL_REMOVE)){
			commandType->setRemoveParticipants(content);
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveTags()==false){

		list<string>content;
		if(setTagsMessage(commandStr,flag, content,FIELD_TAG_REMOVE)){
			commandType->setRemoveTags(content);
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagAddRemindTimes()==false){

		list<time_t>content;
		if(setRemindTimesMessage(commandStr,flag, content,FIELD_RT_ADD)){
			commandType->setAddRemindTimes(content);
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagAddParticipants()==false){

		list<string>content;
		if(setParticipantsMessage(commandStr,flag, content,FIELD_PPL_ADD)){
			commandType->setAddParticipants(content);
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagAddTags()==false){

		list<string>content;
		if(setTagsMessage(commandStr,flag, content,FIELD_TAG_ADD)){
			commandType->setAddTags(content);
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
		response.setCommandType(MOD);
		return (Command*)commandType;
	}
	else return NULL;

}