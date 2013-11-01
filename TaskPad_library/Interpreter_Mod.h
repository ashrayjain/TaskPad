
#include "Interpreter_base.h"


class Interpreter_Mod: public Interpreter_base
{
public:
	Interpreter_Mod(){};
	~Interpreter_Mod(){};

	Command* interpretModify(Command_Mod* commandType, std::string commandStr, Messenger &response, bool &flag);

	time_t            setTime(string commandStr, bool& _isSuccess)                        { return Interpreter_base::setTime(commandStr, _isSuccess); }
	bool              integerConverter(string& requiredString, int& number)               { return Interpreter_base::integerConverter(requiredString,number);}

	int	              getIndexMessage(string command,bool&flag)                           { return Interpreter_base::getIndexMessage(command,flag); }			
	bool              getNameMessage(string command,bool&flag,string& content)            { return Interpreter_base::getNameMessage(command,flag,content); }
	bool              getOptNameMessage(string command, bool&flag, string& content)       { return Interpreter_base:: getOptNameMessage(command,flag,content); }
	bool			  getDueDateMessage(string command, bool&flag,time_t& content)        { return Interpreter_base::getDueDateMessage(command,flag,content); }
	bool		      getFromDateMessage(string command, bool&flag,time_t& content)       { return Interpreter_base:: getFromDateMessage(command,flag,content); }
	bool			  getToDateMessage(string command, bool&flag,time_t& content)         { return Interpreter_base::getToDateMessage(command,flag,content); }
	bool		      getLocationMessage(string command, bool&flag,string& content)       { return Interpreter_base::getLocationMessage(command,flag,content); }
	bool	          getParticipantsMessage(string command, bool&flag,list<std::string>& content){return Interpreter_base::getParticipantsMessage(command,flag,content); }
	bool		      getNoteMessage(string command, bool&flag,string& content)           { return Interpreter_base::getNoteMessage(command,flag,content); }
	bool			  getPriorityMessage(string command, bool&flag,PRIORITY& content)     { return Interpreter_base::getPriorityMessage(command, flag,content); }
	bool	          getTagsMessage(string command, bool&flag,list<std::string>& content){ return Interpreter_base::getTagsMessage(command,flag,content); }
	bool	          getRemindTimesMessage(string command, bool&flag,list<std::time_t>& content){ return Interpreter_base::getRemindTimesMessage(command,flag,content);}
	bool			  getTaskStateMessage(string command, bool&flag,TASK_STATE& content)  { return Interpreter_base::getTaskStateMessage(command,flag,content); }
	bool			  getTaskTypeMessage(string command, bool&flag,TASK_TYPE& content)    { return Interpreter_base::getTaskTypeMessage(command,flag,content);}
	
	//bool			  getRemoveDueDateInstruction(string command, bool&flag)			  { return Interpreter_base::getRemoveDueDateInstruction(command,flag);}
	
	bool			  getSyncProviderNameMessage(string command, bool&flag,string& content){ return Interpreter_base::getSyncProviderNameMessage(command,flag,content); }
	bool              checkDuplicate(string command, string cmdTemplate,int startPosition){ return Interpreter_base::checkDuplicate(command, cmdTemplate,startPosition);}
	bool              checkKeyWord(string command, int position)                          { return Interpreter_base::checkKeyWord(command,position); }      
	void              convertToTime(string timeStr, time_t &result,bool&flag)             { Interpreter_base::convertToTime(timeStr,result,flag);}
};


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
		if(getParticipantsMessage(commandStr,flag,content)){
			commandType->setParticipants(content);
		}

	}
	else{

		flag=false;
	}

	if(flag && commandType->getFlagNote()==false){

		string content;
		if(getNoteMessage(commandStr,flag,content)){
			commandType->setNote(content);
		}
	}
	else{

		flag=false;

	}

	if(flag && commandType->getFlagLocation()==false){
		string content;
		if(getLocationMessage(commandStr,flag,content)){
			commandType->setLocation(content);
		}
	}
	else{

		flag=false;
	}

	if(flag && commandType->getFlagRemindTimes()==false){
		list<time_t>content;
		if(getRemindTimesMessage(commandStr,flag,content)){
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

		if(getTagsMessage(commandStr,flag,content)){
			commandType->setTags(content);
		}
	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagOptName()==false){

		string content;
		if(getNameMessage(commandStr,flag,content)){
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

		if(getRemoveDueDateInstruction(commandStr,flag)){
			commandType->setFlagRemoveDue();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveFrom()==false){

		if(getRemoveFromDateInstruction(commandStr,flag)){
			commandType->setFlagRemoveFrom();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveTo()==false){

		if(getRemoveToDateInstruction(commandStr,flag)){
			commandType->setFlagRemoveTo();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveAllRemindTimes()==false){

		if(getRemoveAllRemindTimesInstruction(commandStr,flag)){
			commandType->setFlagRemoveAllRemindTimes();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveAllParticipants()==false){

		if(getRemoveAllParticipantsInstruction(commandStr,flag)){
			commandType->setFlagRemoveAllParticipants();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveAllTags()==false){

		if(getRemoveAllTagsInstruction(commandStr,flag)){
			commandType->setFlagRemoveAllTags();
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveRemindTimes()==false){

		list<time_t>content;
		if(getRemoveRemindTimesMessage(commandStr,flag, content)){
			commandType->setRemoveRemindTimes(content);
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveParticipants()==false){

		list<string>content;
		if(getRemoveParticipantsMessage(commandStr,flag, content)){
			commandType->setRemoveParticipants(content);
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagRemoveTags()==false){

		list<string>content;
		if(getRemoveTagsMessage(commandStr,flag, content)){
			commandType->setRemoveTags(content);
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagAddRemindTimes()==false){

		list<time_t>content;
		if(getAddRemindTimesMessage(commandStr,flag, content)){
			commandType->setAddRemindTimes(content);
		}

	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagAddParticipants()==false){

		list<string>content;
		if(getAddParticipantsMessage(commandStr,flag, content)){
			commandType->setAddParticipants(content);
		}

	}
	else {

		flag=false;
	}

	/*if(flag && commandType->getFlagAddTags()==false){

		list<string>content;
		if(getAddTagsMessage(commandStr,flag, content)){
			commandType->setAddParticipants(content);
		}

	}
	else {

		flag=false;
	}*/


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

