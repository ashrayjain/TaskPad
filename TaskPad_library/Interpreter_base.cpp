/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter_base.cpp 
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
#include "Interpreter_base.h"
#include "Natty.h"

using namespace std;
using namespace TP;

/************************
*    Error Constants    *
************************/
const string Interpreter_base:: ERROR_TIME_FORMAT="Cannot parse the time format!";
const string Interpreter_base:: ERROR_DUPLICATE="Duplicate command!";

/************************
*    Field Constants    *
************************/
const string Interpreter_base:: FIELD_NAME="(\\s+)name(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_DUE="(\\s+)(due|by)(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_FROM="(\\s+)from(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_TO="(\\s+)to(\\s+)`[^`]*`";   
const string Interpreter_base:: FIELD_AT="(\\s+)(at|location|place)(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_PPL="(\\s+)(ppl|with)(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_NOTE="(\\s+)note(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_PRIORITY="(\\s+)(impt|priority)(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_TAG="\\s(#[^( |`)]*)(\\s|$)";
const string Interpreter_base:: FIELD_RT="(\\s+)(rt|remind)(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_TASK_STATE="\\s(done|undone|overdue)(\\s|$)";
const string Interpreter_base:: FIELD_TASK_TYPE="\\s(timed|deadline|floating)(\\s|$)";
const string Interpreter_base:: FIELD_DUE_REMOVE="\\s(-(due|by))(\\s|$)";
const string Interpreter_base:: FIELD_FROM_REMOVE="\\s(-from)(\\s|$)";
const string Interpreter_base:: FIELD_TO_REMOVE="\\s(-to)(\\s|$)";
const string Interpreter_base:: FIELD_RT_REMOVE_ALL="\\s(-rtall)(\\s|$)";
const string Interpreter_base:: FIELD_PPL_REMOVE_ALL="\\s(-pplall)(\\s|$)";
const string Interpreter_base:: FIELD_TAG_REMOVE_ALL="\\s(-#)(\\s|$)";
const string Interpreter_base:: FIELD_RT_REMOVE="(\\s+)-(rt|remind)(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_PPL_REMOVE="(\\s+)-(ppl|with)(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_TAG_REMOVE="\\s(-#[^( |`)]*)(\\s|$)";
const string Interpreter_base:: FIELD_RT_ADD="(\\s+)\\+(rt|remind)(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_PPL_ADD="(\\s+)\\+(ppl|with)(\\s+)`[^`]*`";
const string Interpreter_base:: FIELD_TAG_ADD="\\s(\\+#[^( |`)]*)(\\s|$)";

/************************
*   Notation Constants  *
************************/
const char Interpreter_base::   NOTATION_COMMA=',';
const char Interpreter_base::   NOTATION_HASH='#';
const char Interpreter_base::   NOTATION_SPACE=' ';
const char Interpreter_base::   NOTATION_SLASH='/';
const char Interpreter_base::   NOTATION_COLON=':';
const char Interpreter_base::   NOTATION_ACCENT_GRAVE='`';

/************************
*   Priority Constants  *
************************/
const string Interpreter_base:: PRIORITY_HIGH_ABBREV="H";
const string Interpreter_base:: PRIORITY_HIGH_FULL="HIGH";
const string Interpreter_base:: PRIORITY_MEDIUM_ABBREV="M";
const string Interpreter_base:: PRIORITY_MEDIUM_FULL="MEDIUM";
const string Interpreter_base:: PRIORITY_LOW_ABBREV="L";
const string Interpreter_base:: PRIORITY_LOW_FULL="LOW";

/************************
* Task Status Constants *
************************/
const string Interpreter_base:: STATUS_UNDONE="undone";
const string Interpreter_base:: STATUS_DONE="done";
const string Interpreter_base:: STATUS_OVERDUE="overdue";

/************************
* Task Type Constants *
************************/
const string Interpreter_base:: TYPE_TIMED="timed";
const string Interpreter_base:: TYPE_DEADLINE="deadline";

/************************
*     Time Constants    *
************************/
const int Interpreter_base::    UNINITIALIZED_TIME=-1;
const int Interpreter_base::	DEFAULT_TIME=0;
const int Interpreter_base::	DEFAULT_CTIME_BASE_YEAR=1900;
const int Interpreter_base::	DEFAULT_MONTH=1;

const int Interpreter_base::	UPPER_LIMIT_YEAR=2100;
const int Interpreter_base::	UPPER_LIMIT_MONTH=12;
const int Interpreter_base::	UPPER_LIMIT_DAY=31;
const int Interpreter_base::	UPPER_LIMIT_HOUR=24;
const int Interpreter_base::	UPPER_LIMIT_MINUTE=59;
const int Interpreter_base::	LOWER_LIMIT_YEAR=1971;
const int Interpreter_base::	CURRENT_CENTURY=2000;

/************************
*   General Constants   *
************************/
const int Interpreter_base::	INCREMENT_BY_ONE=1;
const int Interpreter_base::	EMPTY_STRING=0;
const int Interpreter_base::	START_POSITION = 0;
const int Interpreter_base::	DUMMY_VALUE = -1;
const int Interpreter_base::	CHANGE_BY_ONE=1;
const int Interpreter_base::	CHANGE_BY_TWO=2;


//@AN JIANGZE A0105729A 
bool Interpreter_base::getDueDateMessage(string command, bool&isSuccessful, time_t& content){

	regex extractTemplate(FIELD_DUE);
	smatch match;
	string field;
	string quotedMessage;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){
		field=match[START_POSITION];
	}

	if(!field.empty()){

		extractQuotedMessage(field, quotedMessage);

		if(!quotedMessage.empty()){
			bool isDue=true;
			content=setTime(quotedMessage,isSuccessful,isDue);
		}
		else{
			isNotEmpty=false;
		}
		if(checkDuplicate(command,FIELD_DUE,match.position(),field.length())==true){
			isSuccessful=false;
		}
		
		//"Due" ,and "from" and "to", are mutually exclusive.

		// Check whether the command has "from" or not.
		field.clear();
		isFromExistance(command, match, field, isSuccessful);
		// Check whether the command has "to" or not.
		field.clear();
		isToExistance(command, match, field, isSuccessful);
	}
	else{
		isNotEmpty=false;
	}

	return isNotEmpty;
}
//@AN JIANGZE A0105729A
bool Interpreter_base::getFromDateMessage(string command, bool&isSuccessful, time_t& content){

	regex extractTemplate(FIELD_FROM);
	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){
		field=match[START_POSITION];
	}

	if(!field.empty()){
		extractQuotedMessage(field, QuotedMessage);
		if(QuotedMessage.empty())isNotEmpty=false;
		else{
			bool isDue=false;
			content=setTime(QuotedMessage,isSuccessful,isDue);

		}
		if(checkDuplicate(command,FIELD_FROM,match.position(),field.length())==true){

			isSuccessful=false;
		}		

		//"Due" ,and "from" and "to", are mutually exclusive.

		// Check whether the command has "due" or not.
		field.clear();
		isDueExistance(field, command, match, isSuccessful);
	}
	else{
		isNotEmpty=false;
	}
	return isNotEmpty;
}
//@AN JIANGZE A0105729A
bool Interpreter_base::getToDateMessage(string command, bool&isSuccessful, time_t& content){

	regex extractTemplate(FIELD_TO);
	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){
		field=match[START_POSITION];
	}
	if(!field.empty()){
		extractQuotedMessage(field, QuotedMessage);
		if(QuotedMessage.empty())isNotEmpty=false;
		else{
			bool isDue=false;
			content=setTime(QuotedMessage,isSuccessful, isDue);
		}

		if(checkDuplicate(command,FIELD_TO,match.position(),field.length())==true){
			isSuccessful=false;
		}

		//"Due" ,and "from" and "to", are mutually exclusive.

		// Check whether the command has the "due" or not.
		isDueExistance(field, command, match, isSuccessful);
	}
	else{
		isNotEmpty=false;
	}
	return isNotEmpty;
}
//@AN JIANGZE A0105729A
bool Interpreter_base::setParticipantsMessage(string command, bool&isSuccessful, list<string>& content,string regexTemplate){
	list<string>pplList;
	regex extractTemplate(regexTemplate);
	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){
		field=match[START_POSITION];
	}
	if(!field.empty()){
		extractQuotedMessage(field, QuotedMessage);
		stringstream extractIndividual(QuotedMessage);
		string name;
		getline(extractIndividual,name,NOTATION_COMMA);
		while(!name.empty()){

			pplList.push_back(trim(name));
			name.clear();
			getline(extractIndividual,name,NOTATION_COMMA);
		}
		content=pplList;
		if(pplList.empty())isNotEmpty=false;
		if(checkDuplicate(command,regexTemplate,match.position(),field.length())==true){
			isSuccessful=false;
		}	
	}
	else{
		isNotEmpty=false;
	}
	return isNotEmpty;
}	

/**********************************************************************************
*Input: command->original command line input by user						      *
*		regexTemplate->a FIELD CONSTANT	e.g FIELD_AT							  *
*																				  *
*Function: This function applies to a field of the format yy `xxx` e.g at `nus`,  *
*			note `hello`. It finds the corresponding field from command using     *
*			regexTemplate and extract the message from the field            	  *
*																				  *
*Ouput:	isSuccessful->indicates whether error is present in the field					  *
*		content->extracted message												  *
*		isEmpty->indicates whether extracted message is empty					  *
**********************************************************************************/
//@AN JIANGZE A0105729A
bool  Interpreter_base::setGeneralMessage(string command, bool&isSuccessful,string& content,string regexTemplate){
	regex extractTemplate(regexTemplate);
	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;
	if (regex_search(command, match, extractTemplate)){
		field=match[START_POSITION];
	}
	if(!field.empty()){
		extractQuotedMessage(field, QuotedMessage);
		content=QuotedMessage;
		if(content.empty())isNotEmpty=false;
		if(checkDuplicate(command,regexTemplate,match.position(),field.length())==true){
			isSuccessful=false;
		}	
	}
	else{
		isNotEmpty=false;
	}
	return isNotEmpty;
}
//@AN JIANGZE A0105729A
bool Interpreter_base::getPriorityMessage(string command, bool& isSuccessful, TP::PRIORITY& content){ 
	regex extractTemplate(FIELD_PRIORITY);
	smatch match;
	string field;
	string QuotedMessage;
	string UpperContent;
	PRIORITY priority=MEDIUM;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){
		field=match[START_POSITION];
	}
	if(!field.empty()){
		extractQuotedMessage(field, QuotedMessage);
		if(QuotedMessage.empty())isNotEmpty=false;
		if(isNotEmpty){
			UpperContent=toUpper(QuotedMessage);
			if(UpperContent==PRIORITY_HIGH_ABBREV ||UpperContent==PRIORITY_HIGH_FULL){
				priority=HIGH;
			}
			else if(UpperContent==PRIORITY_MEDIUM_ABBREV || UpperContent==PRIORITY_MEDIUM_FULL){
				priority=MEDIUM;
			}
			else if(UpperContent==PRIORITY_LOW_ABBREV||UpperContent==PRIORITY_LOW_FULL){
				priority=LOW;
			}
			else {
				isSuccessful=false;
			}
			content=priority;
		}
		if(isSuccessful==true){
			if(checkDuplicate(command,FIELD_PRIORITY,match.position(),field.length())==true){
				isSuccessful=false;
			}
		}
	}
	else {
		isNotEmpty=false;
	}
	return isNotEmpty;
}
//@AN JIANGZE A0105729A
bool Interpreter_base::setTagsMessage(string command, bool&isSuccessful,list<string>& content,string regexTemplate){

	list<string>tagList;
	regex extractTemplate(regexTemplate);
	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;
	string subString=command;
	int count=EMPTY_ITEM;
	if (regex_search(subString, match, extractTemplate)){
		field=match[START_POSITION];
	}
	count = extractTagMessage(field, subString, match, tagList, count, extractTemplate);
	if(count==EMPTY_ITEM || tagList.empty()){
		isNotEmpty=false;
	}
	content=tagList;
	return isNotEmpty;

}
//@AN JIANGZE A0105729A
bool Interpreter_base::setRemindTimesMessage(string command, bool&isSuccessful,list<time_t>&content, string regexTemplate){

	list<time_t>rtList;
	regex extractTemplate(regexTemplate);
	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){
		field=match[START_POSITION];
	}
	if(!field.empty()){
		extractQuotedMessage(field, QuotedMessage);
		stringstream extractIndividual(QuotedMessage);
		string time;
		getline(extractIndividual,time,NOTATION_COMMA);
		while(!time.empty()){
			bool isDue=false;
			time_t rtTime=setTime(time,isSuccessful,isDue);

			rtList.push_back(rtTime);
			time.clear();
			getline(extractIndividual,time,NOTATION_COMMA);
		}
		content=rtList;
		if(content.empty())isNotEmpty=false;
		if(checkDuplicate(command,regexTemplate,match.position(),field.length())==true){
			isSuccessful=false;
		}
	}
	else{
		isNotEmpty=false;
	}
	return isNotEmpty;
}
//@AN JIANGZE A0105729A
bool Interpreter_base::getTaskStateMessage(string command, bool&isSuccessful, TP::TASK_STATE& content){ 

	TASK_STATE task_state;
	int count=EMPTY_ITEM;
	bool isNotEmpty=true;
	vector<string>result=extractNoParameterMessage(command,FIELD_TASK_STATE,count);
	if(count==EMPTY_ITEM){
		isNotEmpty=false;
	}
	else if(count==ONE_ITEM){
		if(result.at(START_POSITION).find(STATUS_UNDONE)!=string::npos){
			content=UNDONE;
		}
		else if(result.at(START_POSITION).find(STATUS_DONE)!=string::npos){
			content=DONE;	
		}
		else if(result.at(START_POSITION).find(STATUS_OVERDUE)!=string::npos){
			content=OVERDUE;
		}
		else{
			isSuccessful=false;
		}
	}
	else{
		isSuccessful=false;
	}
	return isNotEmpty;
}

/**********************************************************************************
*Input: command->original command line input by user						      *
*		regexTemplate->a FIELD CONSTANT	e.g FIELD_AT							  *
*																				  *
*Function: This function applies to a field without message e.g -rtall,done,timed.*
*		   It extracts all occurance the corresponding field from command using   *
*		   regexTemplate and stores them in a string vector                       *
*																				  *
*Ouput: count->number of occurance												  *
*		result->a vector containing all occurances								  *
**********************************************************************************/
//@LI ZIXUAN A0096582R
vector<string> Interpreter_base::extractNoParameterMessage(string command, string regexTemplate,int& count){
	string field;
	smatch match;
//	string subString=command;
	string::const_iterator startPos = command.begin();
	string::const_iterator endPos = command.end();
	int startIndex=START_POSITION;
	vector<string>result;
	regex extractTemplate(regexTemplate);
	if (regex_search(startPos,endPos, match, extractTemplate)){
		field=match[START_POSITION];
	}
	while(!field.empty()){
		if(checkKeyWord(command,startIndex+match.position())==true){
			result.push_back(field);
			count++;
		}	
		startIndex=startIndex+match.position()+CHANGE_BY_ONE;
		startPos=startPos+match.position()+CHANGE_BY_ONE;
		field.clear();
		if(startPos!=endPos){
			if (regex_search(startPos,endPos, match, extractTemplate)){
				field=match[START_POSITION];
			}
		}
	}
	return result;
}
//@AN JIANGZE A0105729A
bool Interpreter_base::getTaskTypeMessage(string command, bool&isSuccessful, TP::TASK_TYPE& content){
	TASK_TYPE task_type;
	int count=EMPTY_ITEM;
	bool isNotEmpty=true;
	vector<string>result=extractNoParameterMessage(command,FIELD_TASK_TYPE,count);
	if(count==EMPTY_ITEM){
		isNotEmpty=false;
	}
	else if(count==ONE_ITEM){
		if(result.at(START_POSITION).find(TYPE_TIMED)!=string::npos){
			content=TIMED;
		}
		else if(result.at(START_POSITION).find(TYPE_DEADLINE)!=string::npos){
			content=DEADLINE;
		}
		else {
			content=FLOATING;
		}
	}
	else{
		isSuccessful=false;
	}

	return isNotEmpty;
}
//@AN JIANGZE A0105729A
bool  Interpreter_base::setNoParameterMessage(string command, bool&isSuccessful, string regexTemplate){
	int count=EMPTY_ITEM;
	bool isNotEmpty=true;

	vector<string>result=extractNoParameterMessage(command, regexTemplate, count);

	if(count==EMPTY_ITEM){
		isNotEmpty=false;
	}
	else if(count==ONE_ITEM){
		isNotEmpty=true;
	}
	else{
		isSuccessful=false;
	}
	return isNotEmpty;
}

/**********************************************************************************
*Input: isDue->indicates whether the call of this function is for FIELD_DUE	      *
*		timeInput->message in time format										  *
*																				  *
*Function: This function accepts 4 time formats hh:mm, dd hh:mm, dd/mm hh:mm	  *
*			dd/mm/yy hh:mm and converts them from string to time_t.				  *
*																				  *
*Ouput: mktime(&timeMessage)->time in time_t format								  *
*		isSucessful->indicates whether error is present								  *
**********************************************************************************/
//@AN JIANGZE A0105729A
time_t Interpreter_base::setTime(string timeInput,bool& isSuccessful, bool& isDue){
	timeInput = natty::getNatty().parseDateTime(timeInput);
	int year=UNINITIALIZED_TIME,month=UNINITIALIZED_TIME,day=UNINITIALIZED_TIME,hour=UNINITIALIZED_TIME,min=UNINITIALIZED_TIME;
	time_t rawtime;
	string inputInfo=timeInput;
	string content;
	struct tm  timeinfo={DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME};
	time (&rawtime);
	localtime_s (&timeinfo,&rawtime);
	int countSlash=EMPTY_ITEM;
	for(int i=START_POSITION;i<timeInput.length();i++){
		if(timeInput.at(i)==NOTATION_SLASH){
			countSlash++;
		}
	}	
	switch(countSlash){
	case EMPTY_ITEM: {	
			extractTimeWithEmptySlash(timeInput, content, isSuccessful, day, hour, min);
			break;
		}
	case ONE_ITEM:{	
			extractTimeWithOneSlash(timeInput, content, isSuccessful, day, month, hour, min);
			break;
		}
	case TWO_ITEMS:{	
			extractTimeWithTwoSlash(timeInput, content, isSuccessful, day, month, year, hour, min);
			break;
		}
	default:
		break;
	}
	if(isSuccessful!=false){
		setUnitializeTime(year, timeinfo, month, day, isDue, hour, min);
	}
	struct tm  timeMessage={DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME};
    checkTimeValidity(year, isSuccessful, month, day, hour, min);
	if(isSuccessful!=false){
		timeMessage.tm_year=year-DEFAULT_CTIME_BASE_YEAR;
		timeMessage.tm_mon=month-CHANGE_BY_ONE;
		timeMessage.tm_mday=day;
		timeMessage.tm_hour=hour;
		timeMessage.tm_min=min;
	    timeMessage.tm_sec=DEFAULT_TIME;
	}
	else{
		throw ERROR_TIME_FORMAT;
	}
	return mktime(&timeMessage);
}
//@AN JIANGZE A0105729A
bool Interpreter_base::integerConverter(string& requiredString, int& number){
	bool isSuccessful=true;
	if(requiredString.empty()==true){
		isSuccessful=false;
	}
	else{
		for(unsigned i=START_POSITION;i<requiredString.length();i++){
			if(isdigit(requiredString[i])==false){
				isSuccessful=false;
			}
		}
	}
	number=atoi(requiredString.c_str());
	return isSuccessful;
}
//@LI ZIXUAN A0096582R
string Interpreter_base::toUpper(string str){
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}
//@LI ZIXUAN A0096582R
void Interpreter_base::extractTimeWithEmptySlash( std::string &timeMessage, std::string &content, bool& isSuccessful, int& day, int& hour, int& min )
{
	int countSpace=EMPTY_ITEM;
	for(int i=START_POSITION;i<timeMessage.length();i++){
		if(timeMessage.at(i)==NOTATION_SPACE){
			countSpace++;
		}
	}
	if(countSpace==ONE_ITEM){
		stringstream extract(timeMessage);
		getline(extract,content,NOTATION_SPACE);
		if(!content.empty()){
			isSuccessful=integerConverter(content,day);
		}
		else{
			isSuccessful=false;
		}
		content.clear();
		getline(extract,content,NOTATION_COLON);
		if(!content.empty()){

			isSuccessful=integerConverter(content,hour);
		}
		content.clear();
		getline(extract,content);
		if(!content.empty()){

			isSuccessful=integerConverter(content,min);
		}
	}
	else if(countSpace==EMPTY_ITEM){
		stringstream extract(timeMessage);
		getline(extract,content,NOTATION_COLON);
		if(!content.empty()){

			isSuccessful=integerConverter(content,hour);
		}
		content.clear();
		getline(extract,content);
		if(!content.empty()){
			isSuccessful=integerConverter(content,min);
		}
	}
	else{
		isSuccessful=false;
	}
	
}
//@LI ZIXUAN A0096582R
void Interpreter_base::extractTimeWithOneSlash( std::string timeMessage, std::string &content, bool& isSuccessful, int& day, int& month, int& hour, int& min )
{
	stringstream extract(timeMessage);
	content.clear();
	getline(extract,content,NOTATION_SLASH);  
	isSuccessful=integerConverter(content,day);
	content.clear();
	getline(extract,content,NOTATION_SPACE); 
	isSuccessful=integerConverter(content,month);
	content.clear();
	getline(extract,content,NOTATION_COLON);
	if(!content.empty()){
		isSuccessful=integerConverter(content,hour);
	}
	content.clear();
	getline(extract,content);
	if(!content.empty()){
		isSuccessful=integerConverter(content,min);
	}	
}
//@LI ZIXUAN A0096582R
void Interpreter_base::extractTimeWithTwoSlash( std::string timeMessage, std::string &content, bool &isSuccessful, int& day, int& month, int &year, int& hour, int& min )
{
	
	stringstream extract(timeMessage);
	content.clear();
	getline(extract,content,NOTATION_SLASH);  
	isSuccessful=integerConverter(content,day);
	content.clear();
	getline(extract,content,NOTATION_SLASH); 
	isSuccessful=integerConverter(content,month);
	content.clear();
	getline(extract,content,NOTATION_SPACE); 
	isSuccessful=integerConverter(content,year);
	if(year<100)year=year+CURRENT_CENTURY;
	content.clear();
	getline(extract,content,NOTATION_COLON);
	if(!content.empty()){
		isSuccessful=integerConverter(content,hour);
	}
	content.clear();
	getline(extract,content);
	if(!content.empty()){
		isSuccessful=integerConverter(content,min);
	}
}
//@AN JIANGZE A0105729A
void Interpreter_base::setUnitializeTime( int &year, struct tm &timeinfo, int &month, int &day, bool& isDue, int &hour, int &min ){
	if(year==UNINITIALIZED_TIME)year=timeinfo.tm_year+DEFAULT_CTIME_BASE_YEAR;
	if(month==UNINITIALIZED_TIME)month=timeinfo.tm_mon+CHANGE_BY_ONE;
	if(day==UNINITIALIZED_TIME){
		if(isDue){
			if(timeinfo.tm_hour>hour)
				day=timeinfo.tm_mday+CHANGE_BY_ONE;
			else if(timeinfo.tm_hour==hour && timeinfo.tm_min>min)
				day=timeinfo.tm_mday+CHANGE_BY_ONE;
			else 
				day=timeinfo.tm_mday;
		}
		else 
			day=timeinfo.tm_mday;
	}
	if(hour==UNINITIALIZED_TIME)hour=DEFAULT_TIME;
	if(min==UNINITIALIZED_TIME)min=DEFAULT_TIME;
}
//@LI ZIXUAN A0096582R
bool Interpreter_base::checkTimeValidity( int year, bool& isSuccessful, int month, int day, int hour, int min ){
	if(year>UPPER_LIMIT_YEAR || year<LOWER_LIMIT_YEAR)
		isSuccessful=false;
	else if(month>UPPER_LIMIT_MONTH)
		isSuccessful=false;
	else if(day>UPPER_LIMIT_DAY)
		isSuccessful=false;
	else if(hour>UPPER_LIMIT_HOUR)
		isSuccessful=false;
	else if(min>UPPER_LIMIT_MINUTE)
		isSuccessful=false;
	return isSuccessful;
}
//@LI ZIXUAN A0096582R
bool Interpreter_base::isFromExistance( string command, smatch match, string &field, bool& isSuccessful ){
	regex checkFrom(FIELD_FROM);
	if (regex_search(command, match, checkFrom)){
		field=match[START_POSITION];
	}

	if(!field.empty()){
		isSuccessful=false;
	}	
	return isSuccessful;
}
//@LI ZIXUAN A0096582R
bool Interpreter_base::isDueExistance( string &field, string command, smatch match, bool& isSuccessful ){
	regex checkDue(FIELD_DUE);
	field.clear();
	if (regex_search(command, match, checkDue)){
		field=match[START_POSITION];
	}
	if(!field.empty())isSuccessful=false;
	return isSuccessful;
}
//@LI ZIXUAN A0096582R
bool Interpreter_base::isToExistance( string command, smatch match, string &field, bool& isSuccessful ){
	regex checkTo(FIELD_TO);
	if (regex_search(command, match, checkTo)){
		field=match[START_POSITION];
	}
	if(!field.empty()){
		isSuccessful=false;
	}	
	return isSuccessful;
}
//@LI ZIXUAN A0096582R
int Interpreter_base::extractTagMessage( string &field, string &subString, smatch &match, list<string> &tagList, int count, regex extractTemplate ){
	while(!field.empty()){
		if(checkKeyWord(subString,match.position())==true){
			string tagContent;
			stringstream extract(field);
			getline(extract,tagContent,NOTATION_HASH);
			tagContent.clear();
			getline(extract,tagContent,NOTATION_SPACE);
			if(!tagContent.empty()){
				tagList.push_back(tagContent);
			}
			count++;
		}	
		subString=subString.substr(match.position()+CHANGE_BY_ONE);
		field.clear();
		if (regex_search(subString, match, extractTemplate)){
			field=match[START_POSITION];
		}
	}	
	return count;
}
//@AN JIANGZE A0105729A
bool Interpreter_base::checkKeyWord(string command, int position){

	vector<int> positionForNotion; 
	positionForNotion.push_back(DUMMY_VALUE);
	bool isKeyWord=true;
	int count=ONE_ITEM;

	for(int i=START_POSITION;i<command.length();i++){
		if(command.at(i)==NOTATION_ACCENT_GRAVE){
			positionForNotion.push_back(i);
		}
	}
	while(isKeyWord &&count<(int)positionForNotion.size()){
		if(position>positionForNotion[count] && position<positionForNotion[count+CHANGE_BY_ONE]){
			isKeyWord=false;
		}
		count=count+CHANGE_BY_TWO;
	}
	return isKeyWord;
} 

/**********************************************************************************
*Input: field->a Field Constant e.g FIELD_NAME								      *
*																				  *
*Ouput:	quotedMessage->the message in between the two ACCENT_GRAVE of the field   *
*					   e.g Zixuan of name `Zixuan`								  *
**********************************************************************************/
//@LI ZIXUAN A0096582R
void Interpreter_base::extractQuotedMessage(string field, string& QuotedMessage){

	stringstream extract(field);
	getline(extract,QuotedMessage,NOTATION_ACCENT_GRAVE);
	QuotedMessage.clear();
	getline(extract,QuotedMessage,NOTATION_ACCENT_GRAVE);

	return;
}
//@AN JIANGZE A0105729A
int Interpreter_base::getIndexMessage(string command,bool& isSuccessful){
	int num;
	isSuccessful=integerConverter(command, num);
	return num;
}	

/**********************************************************************************
*Input: command->original command line input by user						      *
*		field->a FIELD CONSTANT	e.g FIELD_AT									  *
*		startPosition->position first character of first occurance of the field   *
*		matchLength->length of the first occurance								  *
*																				  *
*Function: This function checks for multiple occurance of the same field.		  *
*																				  *
*Ouput: isDuplicate->indicates whether duplicates field exists					  *
**********************************************************************************/
//@AN JIANGZE A0105729A
bool Interpreter_base::checkDuplicate(string command, string field,int startPosition, int matchLength){

	string subString=command.substr(startPosition+matchLength);
	smatch match;
	regex extractTemplate(field);
	string duplicate;
	bool isDuplicate=false;

	if (regex_search(subString, match, extractTemplate)){
		duplicate=match[START_POSITION];
	}
	if(duplicate.length()>EMPTY_STRING){
		isDuplicate=true;
	}
	if(isDuplicate==true) throw ERROR_DUPLICATE;
	return isDuplicate;
}
//@LI ZIXUAN A0096582R
bool Interpreter_base::extractField(string command, smatch &match, regex extractTemplate, string&field){

	bool isSuccessful=false;

	if (regex_search(command, match, extractTemplate)){
		field=match[START_POSITION];
		isSuccessful=true;
	}
	return isSuccessful;
}
//@AN JIANGZE A0105729A
string Interpreter_base::trim(string str){
	stringstream trimmer;
	string substr;
	trimmer << str;
	str.clear();
	trimmer >> str;
	while(trimmer >> substr){
		str += NOTATION_SPACE + substr;
		substr.clear();
	}
	return str;
}