#include "Interpreter_base.h"
#include "Natty.h"

bool Interpreter_base::checkKeyWord(string command, int position){

	vector<int> positionForNotion; 
	positionForNotion.push_back(-1); // dummy value;
	bool isKeyWord=true;


	for(int i=0;i<command.length();i++){
		if(command.at(i)=='`'){
			positionForNotion.push_back(i);
		}
	}

	int count=1;

	while(isKeyWord &&count<(int)positionForNotion.size()){
		if(position>positionForNotion[count] && position<positionForNotion[count+1]){
			isKeyWord=false;
		}
		count=count+2;
	}

	return isKeyWord;
} 

void Interpreter_base:: extractQuotedMessage(string field, string & QuotedMessage){

		stringstream extract(field);
		getline(extract,QuotedMessage,'`');
		QuotedMessage.clear();
		getline(extract,QuotedMessage,'`');

		return;
}
int Interpreter_base:: getIndexMessage(string command,bool& flag){

	int num;
	flag=integerConverter(command, num);
	return num;
}	

bool Interpreter_base::checkDuplicate(string command, string cmdTemplate,int startPosition){

	string subString=command.substr(startPosition+1);
	smatch match;
	regex extractTemplate(cmdTemplate);
	string field;
	bool isDuplicate=false;

	if (regex_search(subString, match, extractTemplate)){
		field=match[0];
	}
	if(field.length()>0){
		isDuplicate=true;
	}

	return isDuplicate;
}

bool Interpreter_base::extractField(string command, smatch & match, regex extractTemplate, string&field){

	bool isSuccessful=false;

	if (regex_search(command, match, extractTemplate)){
		field=match[0];
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
		field=match[0];
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
			field=match[0];
		}

		if(!field.empty()){
			flag=false;
		}

		field.clear();

		regex checkTo(FIELD_TO);
		
		if (regex_search(command, match, checkTo)){
			field=match[0];
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

		field=match[0];

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

			field=match[0];

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

		field=match[0];

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

			field=match[0];

		}
		if(!field.empty())flag=false;


	}
	else{

		isNotEmpty=false;

	}


	return isNotEmpty;
}



bool Interpreter_base::setParticipantsMessage(string command, bool&flag, list<std::string>& content,string regexTemplate){
	list<string>pplList;
	regex extractTemplate(regexTemplate);
	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){

		field=match[0];

	}
	if(!field.empty()){

		extractQuotedMessage(field, QuotedMessage);

		stringstream extractIndividual(QuotedMessage);
		string name="";
		getline(extractIndividual,name,',');
		while(!name.empty()){

			pplList.push_back(name);
			name.clear();
			getline(extractIndividual,name,',');
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


bool  Interpreter_base::setGeneralMessage(string command, bool&flag,string& content,string regexTemplate){
	regex extractTemplate(regexTemplate);
	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){

		field=match[0];

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
		field=match[0];
	}

	if(!field.empty()){

		extractQuotedMessage(field, QuotedMessage);


		if(QuotedMessage.empty())isNotEmpty=false;
	
		UpperContent=toUpper(QuotedMessage);
		
		if(UpperContent=="H" ||UpperContent=="HIGH"){
			priority=HIGH;
		}
		else if(UpperContent=="M" || UpperContent=="MEDIUM"){
			priority=MEDIUM;
		}

		else if(UpperContent=="L"||UpperContent=="LOW"){
			priority=LOW;
		}
		
		else {
			flag=false;
		}
		content=priority;

		if(checkDuplicate(command,FIELD_PRIORITY,match.position())==true){

			flag=false;
		}

	}
	else {

		isNotEmpty=false;
	}
	return isNotEmpty;
}

bool Interpreter_base::setTagsMessage(string command, bool&flag,list<std::string>& content,string regexTemplate){

	list<string>tagList;
	regex extractTemplate(regexTemplate);

	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;
	string subStirng=command;

	int count=0;

	if (regex_search(subStirng, match, extractTemplate)){

		field=match[0];

	}

	while(!field.empty()){

		if(checkKeyWord(subStirng,match.position())==true){

			string tagContent;
			stringstream extract(field);
			getline(extract,tagContent,'#');
			tagContent.clear();
			getline(extract,tagContent,' ');

			if(!tagContent.empty()){

				tagList.push_back(tagContent);
			}
			count++;
		}	


		subStirng=subStirng.substr(match.position()+1);
		field.clear();

		if (regex_search(subStirng, match, extractTemplate)){

			field=match[0];

		}

	}

	if(count==0 || tagList.empty()){
		isNotEmpty=false;
	}

	content=tagList;
	return isNotEmpty;

}	

bool Interpreter_base::	setRemindTimesMessage(string command, bool&flag,list<std::time_t>&content, string regexTemplate){

	list<time_t>rtList;
	regex extractTemplate(regexTemplate);
	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){

		field=match[0];

	}
	if(!field.empty()){

		extractQuotedMessage(field, QuotedMessage);

		stringstream extractIndividual(QuotedMessage);
		string time="";
		getline(extractIndividual,time,',');

		while(!time.empty()){
			bool isDue=false;
			time_t rtTime=setTime(time,flag,isDue);
			rtList.push_back(rtTime);
			time.clear();
			getline(extractIndividual,time,',');
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
	

	int count=0;
	bool isNotEmpty=true;

	vector<string>result=extractNoParameterMessage(command,FIELD_TASK_STATE,count);
	

	if(count==0){
		isNotEmpty=false;
	}
	else if(count==1){
		if(result.at(0).find("undone")!=string::npos){

			content=UNDONE;
		}

		else if(result.at(0).find("done")!=string::npos){
			content=DONE;	

		}
		else if(result.at(0).find("overdue")!=string::npos){
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

	int startIndex=0;
	vector<string>result;

	regex extractTemplate(regexTemplate);


	if (regex_search(startPos,endPos, match, extractTemplate)){

		field=match[0];

	}

	while(!field.empty()){

		if(checkKeyWord(command,startIndex+match.position())==true){

			result.push_back(field);
			count++;
		}	

		startIndex=startIndex+match.position()+1;
		startPos=startPos+match.position()+1;

		field.clear();
		if(startPos!=endPos){
			if (regex_search(startPos,endPos, match, extractTemplate)){

				field=match[0];

			}
		}
	}

	return result;
}



bool Interpreter_base::getTaskTypeMessage(string command, bool&flag, TP::TASK_TYPE& content){

	TASK_TYPE task_type;

	
	int count=0;
	bool isNotEmpty=true;

	vector<string>result=extractNoParameterMessage(command,FIELD_TASK_TYPE,count);
	
	

	
	if(count==0){
		isNotEmpty=false;
	}
	else if(count==1){
		if(result.at(0).find("timed")!=string::npos){
			content=TIMED;
		}
		else if(result.at(0).find("deadline")!=string::npos){
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
	
	int count=0;
	bool isNotEmpty=true;

	vector<string>result=extractNoParameterMessage(command,FIELD_TASK_TYPE,count);

	if(count==0){
		isNotEmpty=false;
	}
	else if(count==1){
		isNotEmpty=true;
	}
	else{
		flag=false;
	}

	return isNotEmpty;

}

bool Interpreter_base::getSyncProviderNameMessage(string command, bool&flag, string&content){

	regex extractTemplate("sync `[^`]+`");
	smatch match;
	string field;
	string QuotedMessage;
	bool isNotEmpty=true;

	if (regex_search(command, match, extractTemplate)){

		field=match[0];

	}

	if(!field.empty()){

		stringstream extract(field);
		getline(extract,QuotedMessage,'`');
		content.clear();
		getline(extract,QuotedMessage,'`');

		content=QuotedMessage;

	}
	else {

		isNotEmpty=false;
	}
	return isNotEmpty;
}

time_t Interpreter_base::setTime(string field,bool& flag, bool& isDue){
	
	field = natty::getNatty().parseDateTime(field);
	int year=-1,month=-1,day=-1,hour=-1,min=-1;
	time_t rawtime;
	string inputInfo=field;
	struct tm  timeinfo={0,0,0,0,0,0};

	time (&rawtime);

	localtime_s (&timeinfo,&rawtime);

	int countSlash=0;
	for(int i=0;i<field.length();i++){

		if(field.at(i)=='/'){

			countSlash++;
		}

	}
	string content;
	switch(countSlash){

	case 0: 
		{	
			int countSpace=0;
			for(int i=0;i<field.length();i++){

				if(field.at(i)==' '){

					countSpace++;
				}

			}

			if(countSpace==1){
				stringstream extract(field);
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
				stringstream extract(field);
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
			stringstream extract(field);
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
			stringstream extract(field);
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
		if(day==-1){
			if(isDue){
			if(timeinfo.tm_hour>hour)
				day=timeinfo.tm_mday+1;
			else if(timeinfo.tm_hour==hour && timeinfo.tm_min>min)
				day=timeinfo.tm_mday+1;
			else 
				day=timeinfo.tm_mday;
			}
			else 
				day=timeinfo.tm_mday;
			}
		if(hour==-1)hour=0;
		if(min==-1)min=0;
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
		for(unsigned i=0;i<requiredString.length();i++){
			if(isdigit(requiredString[i])==false){
				flag=false;
			}
		}
	}
	number=atoi(requiredString.c_str());
	return flag;
}

string Interpreter_base::toUpper(string str){
   std::transform(str.begin(), str.end(), str.begin(), ::toupper);
return str;
}