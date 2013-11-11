#include "Interpreter_Find.h"
#include "Natty.h"

using namespace std;
using namespace TP;

const int PULLED_DOWN_HOUR=0;
const int PULLED_DOWN_MIN=0;
const int PUSHED_UP_HOUR=23;
const int PUSHED_UP_MIN=59;


Command* Interpreter_Find::interpretFind(Command_Find* commandType, string commandStr, Messenger &response, bool &flag){
	PRIORITY		contentPriority;
	string			contentString;
	list<string>	contentStringList;
	TASK_STATE		contentTaskState;
	TASK_TYPE		contentTaskType;
	time_t			contentTime;
	list<time_t>	contentTimeList;
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
	if(setTagsMessage(commandStr,flag,contentStringList,FIELD_TAG)){
		commandType->setTags(contentStringList);
	}
	if(setGeneralMessage(commandStr,flag,contentString,FIELD_NAME)){
		commandType->setOptName(contentString);
	}
	if(getTaskStateMessage(commandStr,flag,contentTaskState)){
		commandType->setTaskState(contentTaskState);
	}
	if(getTaskTypeMessage(commandStr,flag,contentTaskType)){
		commandType->setTaskState(contentTaskState);
	}	
	if(getPriorityMessage(commandStr,flag,contentPriority)){
		commandType->setPriority(contentPriority);
	}
	
	if(commandType->getFlagFrom()==true && commandType->getFlagTo()==true){
		if(commandType->getFromDate()>commandType->getToDate()){
			flag=false;
		}
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

time_t Interpreter_Find::setTime(string commandStr, bool&flag){
	commandStr = natty::getNatty().parseDateTime(commandStr);
	int year=UNINITIALIZED_TIME,month=UNINITIALIZED_TIME,day=UNINITIALIZED_TIME,hour=UNINITIALIZED_TIME,min=UNINITIALIZED_TIME,second=CHANGE_BY_ONE;
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
	case EMPTY_ITEM: {	
			extractTimeWithNoSlashForFind(commandStr, content, flag, day, hour, min);
			break;
		}
	case ONE_ITEM:{	
			extractTimeWithOneSlashForFind(commandStr, content, flag, day, month, hour, min);
			break;
		}
	case TWO_ITEMS:{	
			extractTimeWithTwoSlashForFind(commandStr, content, flag, day, month, year, hour, min);
			break;
		}
	default:
		break;
	}
	if(flag!=false){
		setUnitializeTimeForFind(year, timeinfo, month, day, hour, min, second);
	}
	struct tm  timeMessage={DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME};
	checkTimeValidityForFind(year, flag, month, day, hour, min);
	if(flag!=false){
		timeMessage.tm_year=year-DEFAULT_CTIME_BASE_YEAR;
		timeMessage.tm_mon=month-CHANGE_BY_ONE;
		timeMessage.tm_mday=day;
		timeMessage.tm_hour=hour;
		timeMessage.tm_min=min;
		timeMessage.tm_sec=DEFAULT_TIME;
	}
	else{
		throw TIME_FORMAT_ERROR;
	}
	return mktime(&timeMessage);
}

bool Interpreter_Find::getFromDateMessage(string command, bool&flag, time_t& content){

	regex extractTemplete(FIELD_FROM);
	smatch match;
	string commandStr;
	string preContent;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplete)){
		commandStr=match[START_POSITION];
	}
	if(!commandStr.empty()){

		stringstream extract(commandStr);
		getline(extract,preContent,NOTATION_ACCENT_GRAVE);
		preContent.clear();
		getline(extract,preContent,NOTATION_ACCENT_GRAVE);

		if(preContent.empty())isNotEmpty=false;
		else{

			content=setTime(preContent,flag);
		}
		if(checkDuplicate(command,FIELD_FROM,match.position(),commandStr.length())==true){

			flag=false;
		}		
	}
	else{

		isNotEmpty=false;

	}

	return isNotEmpty;
}

bool Interpreter_Find::getToDateMessage(string command, bool&flag, time_t& content){

	regex extractTemplete(FIELD_TO);
	smatch match;
	string commandStr;
	string preContent;
	bool isNotEmpty=true;
	if (regex_search(command, match, extractTemplete)){
		commandStr=match[START_POSITION];
	}
	if(!commandStr.empty()){
		stringstream extract(commandStr);
		getline(extract,preContent,NOTATION_ACCENT_GRAVE);
		preContent.clear();
		getline(extract,preContent,NOTATION_ACCENT_GRAVE);
		if(preContent.empty())isNotEmpty=false;
		else{
			content=setTime(preContent,flag);
		}
		if(checkDuplicate(command,FIELD_TO,match.position(),commandStr.length())==true){
			flag=false;
		}
	}
	else{
		isNotEmpty=false;
	}
	return isNotEmpty;
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

bool Interpreter_Find::extractTimeWithNoSlashForFind( std::string &commandStr, std::string &content, bool& flag, int& day, int& hour, int& min )
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
	return flag;
}

bool Interpreter_Find::extractTimeWithOneSlashForFind( std::string commandStr, std::string &content, bool& flag, int& day, int& month, int& hour, int& min )
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
	return flag;
}

void Interpreter_Find::extractTimeWithTwoSlashForFind( std::string commandStr, std::string &content, bool &flag, int& day, int& month, int &year, int& hour, int& min )
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
}

void Interpreter_Find::setUnitializeTimeForFind( int &year, struct tm &timeinfo, int &month, int &day, int &hour, int &min, int &second ){
	if(year==UNINITIALIZED_TIME)year=timeinfo.tm_year+DEFAULT_CTIME_BASE_YEAR;
	if(month==UNINITIALIZED_TIME)month=timeinfo.tm_mon+CHANGE_BY_ONE;
	if(day==UNINITIALIZED_TIME)day=timeinfo.tm_mday;
	if(hour==UNINITIALIZED_TIME && min ==UNINITIALIZED_TIME){
		second=UPPER_LIMIT_MINUTE;
	}
	if(hour==UNINITIALIZED_TIME)hour=DEFAULT_TIME;
	if(min==UNINITIALIZED_TIME) min=DEFAULT_TIME;
}

bool Interpreter_Find::checkTimeValidityForFind( int year, bool& flag, int month, int day, int hour, int min ){
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
	return flag;
}


