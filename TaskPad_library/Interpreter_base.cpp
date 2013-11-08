#include "Interpreter_base.h"
#include "Natty.h"

using namespace std;


const string Interpreter_base:: FIELD_NAME=" name `[^`]*`";
const string Interpreter_base:: FIELD_DUE=" (due|by) `[^`]*`";
const string Interpreter_base:: FIELD_FROM=" from `[^`]*`";
const string Interpreter_base:: FIELD_TO=" to `[^`]*`";
const string Interpreter_base:: FIELD_AT=" (at|location|place) `[^`]*`";
const string Interpreter_base:: FIELD_PPL=" (ppl|with) `[^`]*`";
const string Interpreter_base:: FIELD_NOTE=" note `[^`]*`";
const string Interpreter_base:: FIELD_PRIORITY=" (impt|priority) `[^`]*`";
const string Interpreter_base:: FIELD_TAG="\\s(#[^( |`)]*)(\\s|$)";
const string Interpreter_base:: FIELD_RT=" (rt|remind) `[^`]*`";
const string Interpreter_base:: FIELD_TASK_STATE="\\s(done|undone|overdue)(\\s|$)";
const string Interpreter_base:: FIELD_TASK_TYPE="\\s(timed|deadline|floating)(\\s|$)";
const string Interpreter_base:: FIELD_DUE_REMOVE="\\s(-(due|by))(\\s|$)";
const string Interpreter_base:: FIELD_FROM_REMOVE="\\s(-from)(\\s|$)";
const string Interpreter_base:: FIELD_TO_REMOVE="\\s(-to)(\\s|$)";
const string Interpreter_base:: FIELD_RT_REMOVE_ALL="\\s(-rtall)(\\s|$)";
const string Interpreter_base:: FIELD_PPL_REMOVE_ALL="\\s(-pplall)(\\s|$)";
const string Interpreter_base:: FIELD_TAG_REMOVE_ALL="\\s(-#)(\\s|$)";
const string Interpreter_base:: FIELD_RT_REMOVE=" -(rt|remind) `[^`]*`";
const string Interpreter_base:: FIELD_PPL_REMOVE=" -(ppl|with) `[^`]*`";
const string Interpreter_base:: FIELD_TAG_REMOVE="\\s(-#[^( |`)]*)(\\s|$)";
const string Interpreter_base:: FIELD_RT_ADD=" \\+(rt|remind) `[^`]*`";
const string Interpreter_base:: FIELD_PPL_ADD=" \\+(ppl|with) `[^`]*`";
const string Interpreter_base:: FIELD_TAG_ADD="\\s(\\+#[^( |`)]*)(\\s|$)";

const string Interpreter_base:: PRIORITY_HIGH_ABBREV="H";
const string Interpreter_base:: PRIORITY_HIGH_FULL="HIGH";
const string Interpreter_base:: PRIORITY_MEDIUM_ABBREV="M";
const string Interpreter_base:: PRIORITY_MEDIUM_FULL="MEDIUM";
const string Interpreter_base:: PRIORITY_LOW_ABBREV="L";
const string Interpreter_base:: PRIORITY_LOW_FULL="LOW";

const string Interpreter_base:: STATUS_UNDONE="undone";
const string Interpreter_base:: STATUS_DONE="done";
const string Interpreter_base:: STATUS_OVERDUE="overdue";

const string Interpreter_base:: TYPE_TIMED="timed";
const string Interpreter_base:: TYPE_DEADLINE="deadline";

const char Interpreter_base:: NOTATION_COMMA=',';
const char Interpreter_base:: NOTATION_HASH='#';
const char Interpreter_base:: NOTATION_SPACE=' ';
const char Interpreter_base:: NOTATION_SLASH='/';
const char Interpreter_base:: NOTATION_COLON=':';
const char Interpreter_base:: NOTATION_ACCENT_GRAVE='`';


const int Interpreter_base:: INCREMENT_BY_ONE=1;
const int Interpreter_base:: UNINITIALIZED_TIME=-1;
const int Interpreter_base:: DEFAULT_TIME=0;
const int Interpreter_base:: DEFAULT_CTIME_BASE_YEAR=1900;
const int Interpreter_base:: DEFAULT_MONTH=1;

const int Interpreter_base:: UPPER_LIMIT_YEAR=2100;
const int Interpreter_base:: UPPER_LIMIT_MONTH=12;
const int Interpreter_base:: UPPER_LIMIT_DAY=31;
const int Interpreter_base:: UPPER_LIMIT_HOUR=24;
const int Interpreter_base:: UPPER_LIMIT_MINUTE=59;
const int Interpreter_base:: LOWER_LIMIT_YEAR=1971;
const int Interpreter_base:: CURRENT_CENTURY=2000;

const int Interpreter_base::EMPTY_STRING=0;
const int Interpreter_base::START_POSITION = 0;
const int Interpreter_base::DUMMY_VALUE = -1;
const int Interpreter_base::CHANGE_BY_ONE=1;
const int Interpreter_base::CHANGE_BY_TWO=2;


bool Interpreter_base::checkKeyWord(string command, int position){

	vector<int> positionForNotion; 
	positionForNotion.push_back(DUMMY_VALUE); // dummy value;
	bool isKeyWord=true;


	for(int i=START_POSITION;i<command.length();i++){
		if(command.at(i)==NOTATION_ACCENT_GRAVE){
			positionForNotion.push_back(i);
		}
	}

	int count=ONE_ITEM;

	while(isKeyWord &&count<(int)positionForNotion.size()){
		if(position>positionForNotion[count] && position<positionForNotion[count+CHANGE_BY_ONE]){
			isKeyWord=false;
		}
		count=count+CHANGE_BY_TWO;
	}

	return isKeyWord;
} 

void Interpreter_base:: extractQuotedMessage(string field, string & QuotedMessage){

	stringstream extract(field);
	getline(extract,QuotedMessage,NOTATION_ACCENT_GRAVE);
	QuotedMessage.clear();
	getline(extract,QuotedMessage,NOTATION_ACCENT_GRAVE);

	return;
}
int Interpreter_base:: getIndexMessage(string command,bool& flag){

	int num;
	flag=integerConverter(command, num);
	return num;
}	

bool Interpreter_base::checkDuplicate(string command, string cmdTemplate,int startPosition){

	string subString=command.substr(startPosition+CHANGE_BY_ONE);
	smatch match;
	regex extractTemplate(cmdTemplate);
	string field;
	bool isDuplicate=false;

	if (regex_search(subString, match, extractTemplate)){
		field=match[START_POSITION];
	}
	if(field.length()>EMPTY_STRING){
		isDuplicate=true;
	}

	return isDuplicate;
}

bool Interpreter_base::extractField(string command, smatch & match, regex extractTemplate, string&field){

	bool isSuccessful=false;

	if (regex_search(command, match, extractTemplate)){
		field=match[START_POSITION];
		isSuccessful=true;
	}



	return isSuccessful;
}

bool Interpreter_base::getDueDateMessage(string command, bool&flag, time_t& content){

	regex extractTemplate(FIELD_DUE);
	smatch match;
	string field;
	string quotedMessage;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){
		field=match[START_POSITION];
	}
	//extractField(command,match,extractTemplate,field);

	if(!field.empty()){

		extractQuotedMessage(field, quotedMessage);

		if(!quotedMessage.empty()){
			bool isDue=true;
			content=setTime(quotedMessage,flag,isDue);
		}
		else{
			isNotEmpty=false;
		}

		if(checkDuplicate(command,FIELD_DUE,match.position())==true){
			flag=false;
		}

		field.clear();

		regex checkFrom(FIELD_FROM);
		if (regex_search(command, match, checkFrom)){
			field=match[START_POSITION];
		}

		if(!field.empty()){
			flag=false;
		}

		field.clear();

		regex checkTo(FIELD_TO);

		if (regex_search(command, match, checkTo)){
			field=match[START_POSITION];
		}

		if(!field.empty()){
			flag=false;
		}

	}
	else{
		isNotEmpty=false;
	}

	return isNotEmpty;
}

bool Interpreter_base::getFromDateMessage(string command, bool&flag, time_t& content){

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
			content=setTime(QuotedMessage,flag,isDue);
		}
		if(checkDuplicate(command,FIELD_FROM,match.position())==true){

			flag=false;
		}		


		regex checkDue(FIELD_DUE);
		field.clear();
		if (regex_search(command, match, checkDue)){

			field=match[START_POSITION];

		}
		if(!field.empty())flag=false;



	}
	else{

		isNotEmpty=false;

	}


	return isNotEmpty;

}

bool Interpreter_base::getToDateMessage(string command, bool&flag, time_t& content){

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
		else
		{
			bool isDue=false;
			content=setTime(QuotedMessage,flag, isDue);
		}


		if(checkDuplicate(command,FIELD_TO,match.position())==true){

			flag=false;
		}


		regex checkDue(FIELD_DUE);
		field.clear();
		if (regex_search(command, match, checkDue)){

			field=match[START_POSITION];

		}
		if(!field.empty())flag=false;


	}
	else{

		isNotEmpty=false;

	}


	return isNotEmpty;
}



bool Interpreter_base::setParticipantsMessage(string command, bool&flag, list<string>& content,string regexTemplate){
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

		if(checkDuplicate(command,regexTemplate,match.position())==true){

			flag=false;
		}	

	}
	else{

		isNotEmpty=false;
	}


	return isNotEmpty;

}	

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


bool  Interpreter_base::setGeneralMessage(string command, bool&flag,string& content,string regexTemplate){
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
		if(checkDuplicate(command,regexTemplate,match.position())==true){
			flag=false;
		}	
	}
	else{
		isNotEmpty=false;
	}

	return isNotEmpty;
}



bool Interpreter_base::getPriorityMessage(string command, bool&flag,TP::PRIORITY& content){ // need to force type

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
				flag=false;
			}

			content=priority;
		}
		if(flag==true){
			if(checkDuplicate(command,FIELD_PRIORITY,match.position())==true){

				flag=false;
		 	}
		}
	}
	else {

		isNotEmpty=false;
	}
	return isNotEmpty;
}

bool Interpreter_base::setTagsMessage(string command, bool&flag,list<string>& content,string regexTemplate){

	list<string>tagList;
	regex extractTemplate(regexTemplate);

	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;
	string subStirng=command;

	int count=EMPTY_ITEM;

	if (regex_search(subStirng, match, extractTemplate)){

		field=match[START_POSITION];

	}

	while(!field.empty()){

		if(checkKeyWord(subStirng,match.position())==true){

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


		subStirng=subStirng.substr(match.position()+CHANGE_BY_ONE);
		field.clear();

		if (regex_search(subStirng, match, extractTemplate)){

			field=match[START_POSITION];

		}

	}

	if(count==EMPTY_ITEM || tagList.empty()){
		isNotEmpty=false;
	}

	content=tagList;
	return isNotEmpty;

}	

bool Interpreter_base::	setRemindTimesMessage(string command, bool&flag,list<time_t>&content, string regexTemplate){

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
		string time="";
		getline(extractIndividual,time,NOTATION_COMMA);

		while(!time.empty()){
			bool isDue=false;
			time_t rtTime=setTime(time,flag,isDue);
			rtList.push_back(rtTime);
			time.clear();
			getline(extractIndividual,time,NOTATION_COMMA);
		}
		content=rtList;
		if(content.empty())isNotEmpty=false;
		if(checkDuplicate(command,regexTemplate,match.position())==true){

			flag=false;
		}
	}
	else{

		isNotEmpty=false;
	}



	return isNotEmpty;
}

bool Interpreter_base::getTaskStateMessage(string command, bool&flag, TP::TASK_STATE& content){ 

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
			flag=false;
		}
	}

	else{

		flag=false;
	}

	return isNotEmpty;
}

vector<string> Interpreter_base::extractNoParameterMessage(string command, string regexTemplate,int& count){

	string field;
	smatch match;

	string subStirng=command;

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



bool Interpreter_base::getTaskTypeMessage(string command, bool&flag, TP::TASK_TYPE& content){

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

		flag=false;
	}

	return isNotEmpty;
}



bool  Interpreter_base::setNoParameterMessage(string command, bool&flag, string regexTemplate){

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
		flag=false;
	}

	return isNotEmpty;

}



time_t Interpreter_base::setTime(string field,bool& flag, bool& isDue){

	field = natty::getNatty().parseDateTime(field);
	int year=UNINITIALIZED_TIME,month=UNINITIALIZED_TIME,day=UNINITIALIZED_TIME,hour=UNINITIALIZED_TIME,min=UNINITIALIZED_TIME;
	time_t rawtime;
	string inputInfo=field;
	struct tm  timeinfo={DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME,DEFAULT_TIME};

	time (&rawtime);

	localtime_s (&timeinfo,&rawtime);

	int countSlash=EMPTY_ITEM;
	for(int i=START_POSITION;i<field.length();i++){

		if(field.at(i)==NOTATION_SLASH){

			countSlash++;
		}

	}
	string content;
	switch(countSlash){

	case EMPTY_ITEM: 
		{	
			int countSpace=EMPTY_ITEM;
			for(int i=START_POSITION;i<field.length();i++){

				if(field.at(i)==NOTATION_SPACE){

					countSpace++;
				}

			}

			if(countSpace==ONE_ITEM){
				stringstream extract(field);
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
				stringstream extract(field);
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
			stringstream extract(field);
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
			stringstream extract(field);
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
	}

	return mktime(&timeMessage);


}




bool Interpreter_base::integerConverter(string& requiredString, int& number)
{

	bool flag=true;

	if(requiredString.empty()==true){
		flag=false;
	}
	else{
		for(unsigned i=START_POSITION;i<requiredString.length();i++){
			if(isdigit(requiredString[i])==false){
				flag=false;
			}
		}
	}
	number=atoi(requiredString.c_str());
	return flag;
}

string Interpreter_base::toUpper(string str){
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}