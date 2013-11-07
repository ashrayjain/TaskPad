#include "Interpreter_base.h"


class Interpreter_Find: public Interpreter_base
{

public:
	Interpreter_Find(){};
	~Interpreter_Find(){};


	Command* interpretFind(Command_Find* commandType, std::string commandStr, Messenger &response, bool &flag);

	time_t            setTime(string commandStr, bool& flag);                       
	bool              integerConverter(string& requiredString, int& number)               { return Interpreter_base::integerConverter(requiredString,number);}
	string            toUpper(string str)                                                 { return Interpreter_base::toUpper(str);}
	
	int	              getIndexMessage(string command,bool&flag)                           { return Interpreter_base::getIndexMessage(command,flag); }			


    bool              setGeneralMessage(string command, bool&flag,string& content,string regexTemplete) { return Interpreter_base::setGeneralMessage(command,flag,content,regexTemplete);}


	bool		      getFromDateMessage(string command, bool&flag,time_t& content);      
	bool			  getToDateMessage(string command, bool&flag,time_t& content);  

//	bool              getNameMessage(string command,bool&flag,string& content)            { return Interpreter_base::getNameMessage(command,flag,content); }
//	bool              getOptNameMessage(string command, bool&flag, string& content)       { return Interpreter_base:: getOptNameMessage(command,flag,content); }
	      
//	bool		      getLocationMessage(string command, bool&flag,string& content)       { return Interpreter_base::getLocationMessage(command,flag,content); }
//	bool	          getParticipantsMessage(string command, bool&flag,list<std::string>& content){return Interpreter_base::getParticipantsMessage(command,flag,content); }
//	bool		      getNoteMessage(string command, bool&flag,string& content)           { return Interpreter_base::getNoteMessage(command,flag,content); }
	bool			  getPriorityMessage(string command, bool&flag,PRIORITY& content)     { return Interpreter_base::getPriorityMessage(command, flag,content); }
//	bool	          getTagsMessage(string command, bool&flag,list<std::string>& content){ return Interpreter_base::getTagsMessage(command,flag,content); }
//	bool	          getRemindTimesMessage(string command, bool&flag,list<std::time_t>& content){ return Interpreter_base::getRemindTimesMessage(command,flag,content);}
	bool			  getTaskStateMessage(string command, bool&flag,TASK_STATE& content)  { return Interpreter_base::getTaskStateMessage(command,flag,content); }
	bool			  getTaskTypeMessage(string command, bool&flag,TASK_TYPE& content)    { return Interpreter_base::getTaskTypeMessage(command,flag,content);}
	bool			  getSyncProviderNameMessage(string command, bool&flag,string& content){ return Interpreter_base::getSyncProviderNameMessage(command,flag,content); }
	bool              checkDuplicate(string command, string cmdTemplate,int startPosition){ return Interpreter_base::checkDuplicate(command, cmdTemplate,startPosition);}
	bool              checkKeyWord(string command, int position)                          { return Interpreter_base::checkKeyWord(command,position); }      
	void              convertToTime(string timeStr, time_t &result,bool&flag)             { Interpreter_base::convertToTime(timeStr,result,flag);}


};

Command* Interpreter_Find::interpretFind(Command_Find* commandType, std::string commandStr, Messenger &response, bool &flag){

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
		struct tm timeinfo;

		if(getToDateMessage(commandStr,flag,content)){
			localtime_s(&timeinfo, &content); 

			if(commandType->getFlagFrom() ==true){
				if(timeinfo.tm_sec==59){

					timeinfo.tm_hour=23;
					timeinfo.tm_min=59;
				}
			}
			commandType->setToDate(mktime(&timeinfo));
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
		if(setGeneralMessage(commandStr,flag,content," name `[^`]*`")){
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
		list<time_t> content;
		if(setRemindTimesMessage(commandStr,flag,content,FIELD_RT)){
			commandType->setRemindTimes(content);
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
		return (Command*)commandType;
	}
	else return NULL;

}

time_t Interpreter_Find::setTime(string commandStr, bool&flag){

	int year=-1,month=-1,day=-1,hour=-1,min=-1,second=1;
	time_t rawtime;
	string inputInfo=commandStr;
	struct tm  timeinfo={0,0,0,0,0,0};

	time (&rawtime);

	localtime_s (&timeinfo,&rawtime);

	int countSlash=0;
	for(int i=0;i<commandStr.length();i++){

		if(commandStr.at(i)=='/'){

			countSlash++;
		}

	}
	string content;
	switch(countSlash){

	case 0: 
		{	
			int countSpace=0;
			for(int i=0;i<commandStr.length();i++){

				if(commandStr.at(i)==' '){

					countSpace++;
				}

			}

			if(countSpace==1){
				stringstream extract(commandStr);
				getline(extract,content,' ');
				if(!content.empty()){
					flag=integerConverter(content,day);
				}
				else{
					flag=false;
				}
				content.clear();
				getline(extract,content,':');
				if(!content.empty()){

					flag=integerConverter(content,hour);
				}
				content.clear();
				getline(extract,content);
				if(!content.empty()){

					flag=integerConverter(content,min);
				}
			}
			else if(countSpace==0){
				stringstream extract(commandStr);
				getline(extract,content,':');
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
	case 1:
		{	
			stringstream extract(commandStr);
			content.clear();
			getline(extract,content,'/');  
			flag=integerConverter(content,day);
			content.clear();
			getline(extract,content,' '); 
			flag=integerConverter(content,month);

			content.clear();
			getline(extract,content,':');
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
	case 2:
		{	
			stringstream extract(commandStr);
			content.clear();
			getline(extract,content,'/');  
			flag=integerConverter(content,day);
			content.clear();
			getline(extract,content,'/'); 
			flag=integerConverter(content,month);
			content.clear();
			getline(extract,content,' '); 
			flag=integerConverter(content,year);
			if(year<100)year=year+2000;
			content.clear();
			getline(extract,content,':');
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

		if(year==-1)year=timeinfo.tm_year+1900;
		if(month==-1)month=timeinfo.tm_mon+1;
		if(day==-1)day=timeinfo.tm_mday;


		if(hour==-1 && min ==-1){
			second=59;
		}

		if(hour==-1)hour=0;
		if(min==-1) min=0;


	}

	struct tm  timeMessage={0,0,0,0,0,0};

	if(year>2100 || year<1971)
		flag=false;
	else if(month>12)
		flag=false;
	else if(day>31)
		flag=false;
	else if(hour>24)
		flag=false;
	else if(min>59)
		flag=false;

	if(flag!=false){
		timeMessage.tm_year=year-1900;
		timeMessage.tm_mon=month-1;
		timeMessage.tm_mday=day;
		timeMessage.tm_hour=hour;
		timeMessage.tm_min=min;
		timeMessage.tm_sec=second;
	}

	return mktime(&timeMessage);
}




bool Interpreter_Find::getFromDateMessage(string command, bool&flag, time_t& content){

	regex extractTemplete(" from `[^`]*`");
	smatch match;
	string commandStr;
	string preContent;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplete)){

		commandStr=match[0];

	}
	if(!commandStr.empty()){

		stringstream extract(commandStr);
		getline(extract,preContent,'`');
		preContent.clear();
		getline(extract,preContent,'`');

		if(preContent.empty())isNotEmpty=false;
		else{

			content=setTime(preContent,flag);
		}
		if(checkDuplicate(command," from `[^`]*`",match.position())==true){

			flag=false;
		}		
	}
	else{

		isNotEmpty=false;

	}

	return isNotEmpty;
}

bool Interpreter_Find::getToDateMessage(string command, bool&flag, time_t& content){

	regex extractTemplete(" to `[^`]*`");
	smatch match;
	string commandStr;
	string preContent;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplete)){

		commandStr=match[0];

	}
	if(!commandStr.empty()){

		stringstream extract(commandStr);
		getline(extract,preContent,'`');
		preContent.clear();
		getline(extract,preContent,'`');

		if(preContent.empty())isNotEmpty=false;
		else
		{
			content=setTime(preContent,flag);
		}


		if(checkDuplicate(command," to `[^`]*`",match.position())==true){

			flag=false;
		}
	}
	else{

		isNotEmpty=false;

	}
	return isNotEmpty;
}





