#include "Interpreter_Find.h"
#include "Natty.h"

time_t Interpreter_Find::setTime(string commandStr, bool&flag){
	commandStr = natty::getNatty().parseDateTime(commandStr);
	int year=UNINITIALIZED_TIME,month=UNINITIALIZED_TIME,day=UNINITIALIZED_TIME,hour=UNINITIALIZED_TIME,min=UNINITIALIZED_TIME,second=UNINITIALIZED_TIME;
	time_t rawtime;
	string inputInfo=commandStr;
	struct tm  timeinfo={DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME};

	time (&rawtime);

	localtime_s (&timeinfo,&rawtime);

	int countSlash=EMPTY_ITEM;
	for(int i=START_POSITION;i<commandStr.length();i++){

		if(commandStr.at(i)==NOTATION_SLASH){

			countSlash++;
		}

	}
	string content;
	switch(countSlash){

	case EMPTY_ITEM: 
		{	
			int countSpace=EMPTY_ITEM;
			for(int i=START_POSITION;i<commandStr.length();i++){

				if(commandStr.at(i)==NOTATION_SPACE){

					countSpace++;
				}

			}

			if(countSpace==ONE_ITEM){
				stringstream extract(commandStr);
				getline(extract,content,NOTATION_SPACE);
				if(!content.empty()){
					flag=integerConverter(content,day);
				}
				else{
					flag=false;
				}
				content.clear();
				getline(extract,content,NOTATION_COLON);
				if(!content.empty()){

					flag=integerConverter(content,hour);
				}
				content.clear();
				getline(extract,content);
				if(!content.empty()){

					flag=integerConverter(content,min);
				}
			}
			else if(countSpace==EMPTY_ITEM){
				stringstream extract(commandStr);
				getline(extract,content,NOTATION_COLON);
				if(!content.empty()){

					flag=integerConverter(content,hour);
				}
				content.clear();
				getline(extract,content);
				if(!content.empty()){

					flag=integerConverter(content,min);
				}

			}
			else{
				flag=false;
			}
			break;
		}
	case ONE_ITEM:
		{	
			stringstream extract(commandStr);
			content.clear();
			getline(extract,content,NOTATION_SLASH);  
			flag=integerConverter(content,day);
			content.clear();
			getline(extract,content,NOTATION_SPACE); 
			flag=integerConverter(content,month);

			content.clear();
			getline(extract,content,NOTATION_COLON);
			if(!content.empty()){

				flag=integerConverter(content,hour);
			}
			content.clear();
			getline(extract,content);
			if(!content.empty()){

				flag=integerConverter(content,min);
			}

			break;
		}
	case TWO_ITEMS:
		{	
			stringstream extract(commandStr);
			content.clear();
			getline(extract,content,NOTATION_SLASH);  
			flag=integerConverter(content,day);
			content.clear();
			getline(extract,content,NOTATION_SLASH); 
			flag=integerConverter(content,month);
			content.clear();
			getline(extract,content,NOTATION_SPACE); 
			flag=integerConverter(content,year);
			if(year<100)year=year+CURRENT_CENTURY;
			content.clear();
			getline(extract,content,NOTATION_COLON);
			if(!content.empty()){

				flag=integerConverter(content,hour);
			}
			content.clear();
			getline(extract,content);
			if(!content.empty()){

				flag=integerConverter(content,min);
			}
			break;
		}
	default:
		break;
	}
	if(flag!=false){

		if(year==UNINITIALIZED_TIME)year=timeinfo.tm_year+DEFAULT_CTIME_BASE_YEAR;
		if(month==UNINITIALIZED_TIME)month=timeinfo.tm_mon+CHANGE_BY_ONE;
		if(day==UNINITIALIZED_TIME)day=timeinfo.tm_mday;


		if(hour==UNINITIALIZED_TIME && min ==UNINITIALIZED_TIME){
			second=UPPER_LIMIT_MINUTE;
		}

		if(hour==UNINITIALIZED_TIME)hour=DEFAULT_TIME;
		if(min==UNINITIALIZED_TIME) min=DEFAULT_TIME;


	}

	struct tm  timeMessage={DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME};

	if(year>UPPER_LIMIT_YEAR || year<LOWER_LIMIT_YEAR)
		flag=false;
	else if(month>UPPER_LIMIT_MONTH)
		flag=false;
	else if(day>UPPER_LIMIT_DAY)
		flag=false;
	else if(hour>UPPER_LIMIT_HOUR)
		flag=false;
	else if(min>UPPER_LIMIT_MINUTE)
		flag=false;

	if(flag!=false){
		timeMessage.tm_year=year-DEFAULT_CTIME_BASE_YEAR;
		timeMessage.tm_mon=month-CHANGE_BY_ONE;
		timeMessage.tm_mday=day;
		timeMessage.tm_hour=hour;
		timeMessage.tm_min=min;
		timeMessage.tm_sec=DEFAULT_TIME;
	}

	return mktime(&timeMessage);
}



bool Interpreter_Find::getFromDateMessage(std::string command, bool&flag, std::time_t& content){

	regex extractTemplete(FIELD_FROM);
	smatch match;
	std::string commandStr;
	std::string preContent;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplete)){

		commandStr=match[0];

	}
	if(!commandStr.empty()){

		std::stringstream extract(commandStr);
		getline(extract,preContent,NOTATION_ACCENT_GRAVE);
		preContent.clear();
		getline(extract,preContent,NOTATION_ACCENT_GRAVE);

		if(preContent.empty())isNotEmpty=false;
		else{

			content=setTime(preContent,flag);
		}
		if(checkDuplicate(command,FIELD_FROM,match.position())==true){

			flag=false;
		}		
	}
	else{

		isNotEmpty=false;

	}

	return isNotEmpty;
}

bool Interpreter_Find::getToDateMessage(std::string command, bool&flag, std::time_t& content){

	regex extractTemplete(FIELD_TO);
	smatch match;
	std::string commandStr;
	std::string preContent;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplete)){

		commandStr=match[0];

	}
	if(!commandStr.empty()){

		std::stringstream extract(commandStr);
		getline(extract,preContent,NOTATION_ACCENT_GRAVE);
		preContent.clear();
		getline(extract,preContent,NOTATION_ACCENT_GRAVE);
		if(preContent.empty())isNotEmpty=false;
		else
		{
			content=setTime(preContent,flag);
		}


		if(checkDuplicate(command,FIELD_TO,match.position())==true){

			flag=false;
		}
	}
	else{

		isNotEmpty=false;

	}
	return isNotEmpty;
}

Command* Interpreter_Find::interpretFind(Command_Find* commandType, std::string commandStr, Messenger &response, bool &flag){

	if(flag && commandType->getFlagFrom() ==false){

		std::time_t content;
		if(getFromDateMessage(commandStr,flag,content)){
			commandType->setFromDate(content);
		}
	    if (flag==false){
			throw (string)"Sorry, this is invalid time format!";
		}	
	}
	else {
		flag=false;

	}

	if(flag && commandType->getFlagTo() ==false){
		std::time_t content;
		struct tm timeinfo;
		struct tm timeinfoForFrom;
		
		if(getToDateMessage(commandStr,flag,content)){
			localtime_s(&timeinfo, &content); 

			if(commandType->getFlagFrom() ==true && content==commandType->getFromDate()){
				
				time_t contentForFrom=commandType->getFromDate();
				localtime_s(&timeinfoForFrom, &contentForFrom);
				timeinfo.tm_hour=23;
				timeinfo.tm_min=59;
			    timeinfoForFrom.tm_hour=0;
			    timeinfoForFrom.tm_min=0;
			    commandType->setFromDate(mktime(&timeinfoForFrom));
				
			
			}
			commandType->setToDate(mktime(&timeinfo));
		}
	
	      if (flag==false){
			throw (string)"Sorry, this is invalid time format!";
		}          
	}
	else{

		flag=false;
	}


	if(flag && commandType->getFlagParticipants()==false){

		list<std::string>content;
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
		list<std::string>content;

		if(setTagsMessage(commandStr,flag,content,FIELD_TAG)){
			commandType->setTags(content);
		}
	}
	else {

		flag=false;
	}

	if(flag && commandType->getFlagOptName()==false){

		std::string content;
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

	if(flag && commandType->getFlagTaskType()==false){
		TASK_TYPE content;
		if(getTaskTypeMessage(commandStr,flag,content)){
			commandType->setTaskType(content);
		}
	}
	else{

		flag=false;

	}
	if(flag && commandType->getFlagRemindTimes()==false){
		list<std::time_t> content;
		if(setRemindTimesMessage(commandStr,flag,content,FIELD_RT)){
			commandType->setRemindTimes(content);
		}
          if (flag==false){
			throw (string)"Sorry, this is invalid time format!";
		}
	}
	else{

		flag=false;

	}
	if(commandType->getFlagFrom()==true && commandType->getFlagTo()==true){

		if(commandType->getFromDate()>commandType->getToDate())flag=false;
	}

    if(flag==true){

		response.setStatus(SUCCESS);
		response.setCommandType(FIND);
		
	}

	else{ 
		
		response.setStatus(ERR);
		commandType=NULL;
	}
    return (Command*)commandType;

}

