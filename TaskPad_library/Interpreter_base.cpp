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

int Interpreter_base:: getIndexMessage(string command,bool& flag){

	int num;
	flag=integerConverter(command, num);
	return num;
}	

bool Interpreter_base::checkDuplicate(string command, string cmdTemplate,int startPosition){

	string subString=command.substr(startPosition+1);
	smatch match;
	regex extractTemplete(cmdTemplate);
	string commandStr;
	bool isDuplicate=false;

	if (regex_search(subString, match, extractTemplete)){

		commandStr=match[0];

	}
	if(commandStr.length()>0){

		isDuplicate=true;
	}

	return isDuplicate;
}


bool Interpreter_base::getDueDateMessage(string command, bool&flag, time_t& content){

	regex extractTemplete(" due `[^`]*`");
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
		if(preContent.empty()){

			isNotEmpty=false;
		}
		else{
			bool isDue=true;
			content=setTime(preContent,flag,isDue);
		}

		if(checkDuplicate(command," due `[^`]*`",match.position())==true){

			flag=false;
		}
		// check from
		commandStr.clear();
		regex checkFrom("from `[^`]*`");
		if (regex_search(command, match,checkFrom)){

			commandStr=match[0];

		}
		if(!commandStr.empty()) flag=false;

		// check to

		commandStr.clear();
		regex checkTo("to `[^`]*`");
		if (regex_search(command, match,checkFrom)){

			commandStr=match[0];

		}
		if(!commandStr.empty()) flag=false;


	}
	else{

		isNotEmpty=false;

	}

	return isNotEmpty;
}

bool Interpreter_base::getFromDateMessage(string command, bool&flag, time_t& content){

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
			bool isDue=false;
			content=setTime(preContent,flag,isDue);
		}
		if(checkDuplicate(command," from `[^`]*`",match.position())==true){

			flag=false;
		}		


		regex checkDue(" due `[^`]*`");
		commandStr.clear();
		if (regex_search(command, match, checkDue)){

			commandStr=match[0];

		}
		if(!commandStr.empty())flag=false;



	}
	else{

		isNotEmpty=false;

	}


	return isNotEmpty;

}

bool Interpreter_base::getToDateMessage(string command, bool&flag, time_t& content){

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
			bool isDue=false;
			content=setTime(preContent,flag, isDue);
		}


		if(checkDuplicate(command," to `[^`]*`",match.position())==true){

			flag=false;
		}


		regex checkDue(" due `[^`]*`");
		commandStr.clear();
		if (regex_search(command, match, checkDue)){

			commandStr=match[0];

		}
		if(!commandStr.empty())flag=false;


	}
	else{

		isNotEmpty=false;

	}


	return isNotEmpty;
}



bool Interpreter_base::setParticipantsMessage(string command, bool&flag, list<std::string>& content,string regexTemplate){
	list<string>pplList;
	regex extractTemplete(regexTemplate);
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
		content.clear();
		getline(extract,preContent,'`');

		stringstream extractIndividual(preContent);
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


bool  Interpreter_base::setGeneralMessage(string command, bool&flag,string& content,string regexTemplete){
	regex extractTemplete(regexTemplete);
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

		content=preContent;
		if(content.empty())isNotEmpty=false;
		if(checkDuplicate(command,regexTemplete,match.position())==true){

			flag=false;
		}	

	}
	else{

		isNotEmpty=false;

	}


	return isNotEmpty;
}



bool Interpreter_base::getPriorityMessage(string command, bool&flag,TP::PRIORITY& content){ // need to force type
	regex extractTemplete(" impt `[^`]*`");
	smatch match;
	string commandStr;
	string preContent;
	string UpperContent;
	
	PRIORITY priority=MEDIUM;
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
	
		UpperContent=toUpper(preContent);
		
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

		if(checkDuplicate(command," impt `[^`]*`",match.position())==true){

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
	regex extractTemplete(regexTemplate);

	smatch match;
	string commandStr;
	string preContent;
	bool isNotEmpty=true;
	string subStirng=command;

	int count=0;

	if (regex_search(subStirng, match, extractTemplete)){

		commandStr=match[0];

	}

	while(!commandStr.empty()){

		if(checkKeyWord(subStirng,match.position())==true){

			stringstream extract(commandStr);
			string tagContent;
			getline(extract,tagContent,'#');
			tagContent.clear();
			getline(extract,tagContent,' ');

			if(!tagContent.empty()){

				tagList.push_back(tagContent);
			}
			count++;
		}	


		subStirng=subStirng.substr(match.position()+1);
		commandStr.clear();

		if (regex_search(subStirng, match, extractTemplete)){

			commandStr=match[0];

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
	regex extractTemplete(regexTemplate);
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

		stringstream extractIndividual(preContent);
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
	string commandStr;
	smatch match;
	bool isNotEmpty=true;
	regex extractTemplete(FIELD_TASK_STATE);
	string subStirng=command;

	int startIndex=0;
	string::const_iterator startPos = command.begin();
	string::const_iterator endPos = command.end();

	vector<string>result;
	int count=0;
	string test;
	if (regex_search(subStirng, match, extractTemplete)){

		commandStr=match[0];

	}

	while(!commandStr.empty()){

		if(checkKeyWord(command,startIndex+match.position())==true){

			result.push_back(commandStr);
			count++;
		}	

		startIndex=startIndex+match.position()+1;
		startPos=startPos+match.position()+1;

		commandStr.clear();
		if(startPos!=endPos){
			if (regex_search(startPos,endPos, match, extractTemplete)){

				commandStr=match[0];

			}
		}
	}

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

bool Interpreter_base::getTaskTypeMessage(string command, bool&flag, TP::TASK_TYPE& content){

	TASK_TYPE task_type;
	string commandStr;
	smatch match;
	regex extractTemplete("\\s(timed|deadline|floating)(\\s|$)");
	string subStirng=command;

	string::const_iterator startPos = command.begin();
	string::const_iterator endPos = command.end();

	int startIndex=0;
	vector<string>result;
	int count=0;
	bool isNotEmpty=true;


	if (regex_search(startPos,endPos, match, extractTemplete)){

		commandStr=match[0];

	}

	while(!commandStr.empty()){

		if(checkKeyWord(command,startIndex+match.position())==true){

			result.push_back(commandStr);
			count++;
		}	

		startIndex=startIndex+match.position()+1;
		startPos=startPos+match.position()+1;

		commandStr.clear();
		if(startPos!=endPos){
			if (regex_search(startPos,endPos, match, extractTemplete)){

				commandStr=match[0];

			}
		}
	}

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
	string commandStr;
	smatch match;
	regex extractTemplete(regexTemplate);
	string subStirng=command;

	string::const_iterator startPos = command.begin();
	string::const_iterator endPos = command.end();

	int startIndex=0;
	int count=0;
	bool isNotEmpty=true;


	if (regex_search(startPos,endPos, match, extractTemplete)){

		commandStr=match[0];

	}

	while(!commandStr.empty()){

		if(checkKeyWord(command,startIndex+match.position())==true){
			count++;
		}	

		startIndex=startIndex+match.position()+1;
		startPos=startPos+match.position()+1;

		commandStr.clear();
		if(startPos!=endPos){
			if (regex_search(startPos,endPos, match, extractTemplete)){

				commandStr=match[0];

			}
		}
	}

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

	regex extractTemplete("sync `[^`]+`");
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
		content.clear();
		getline(extract,preContent,'`');

		content=preContent;

	}
	else {

		isNotEmpty=false;
	}
	return isNotEmpty;
}

time_t Interpreter_base::setTime(string commandStr,bool& flag, bool& isDue){
	commandStr = natty::getNatty().parseDateTime(commandStr);
	int year=-1,month=-1,day=-1,hour=-1,min=-1;
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