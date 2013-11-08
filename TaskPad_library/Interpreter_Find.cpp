#include "Interpreter_Find.h"
#include "Natty.h"

time_t Interpreter_Find::setTime_F(string commandStr, bool&flag){
	commandStr = natty::getNatty().parseDateTime(commandStr);
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