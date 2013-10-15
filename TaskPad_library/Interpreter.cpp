#include "Interpreter.h"

using namespace std;

const string Interpreter::DELIMITER = "`";

Interpreter::Interpreter(void)
{
	_isSuccess=true;
}


Interpreter::~Interpreter(void)
{
}
/*
**********************************************************
Component Functions
**********************************************************
*/

void Interpreter::processCommand()
{
	if(_command=="add")
	{
		_response.setCommandType(ADD);
		functionAdd();
	}
	else if(_command=="mod")
	{
		_response.setCommandType(MOD);
		functionMod();
	}
	else if(_command=="del")
	{
		_response.setCommandType(DEL);
		functionDel();
	}
	else if(_command=="find")
	{
		_response.setCommandType(FIND);
		functionFind();
	}
	else
	{
		_response.setStatus(ERROR);
		_response.setErrorMsg("Invalid command");
	}
	return;
}

void Interpreter::processKeyWords()
{
	if(listOfKeyWords.size()!=0){
		eliminateFalseKeyWords();
	}

	if(listOfKeyWords.size()>1){
		checkForDuplicateKeyWords();//messenger
	}
	

	return;
}

/*
************************************************************
Auxillary Functions for Component Functions
************************************************************
*/

void Interpreter::eliminateFalseKeyWords()
{
	int numberOfKeyWords=listOfKeyWords.size();
	int preWordIndex;
	for(int i=0; i<numberOfKeyWords && numberOfKeyWords!=0;i++)
	{
		preWordIndex=listOfKeyWords.at(i).index-1;

		if(!isKeyWordValid(preWordIndex))
		{
			listOfKeyWords.erase(listOfKeyWords.begin()+i);
			i--;
			numberOfKeyWords--;
		}
	}

	return;
}

bool Interpreter::isKeyWordValid(int prevSysWordIndex)
{
	bool wordFlag=false;
	string expectedChar=DELIMITER;
	char prevWordLastChar=listOfWords.at(prevSysWordIndex).back();
	if(prevWordLastChar==expectedChar.at(0))
	{
		wordFlag=true;
	}
	//index case
	else if((_command=="mod"||_command=="del"||_command=="find")&& 
		prevSysWordIndex==1 && isByIndex())
	{
		wordFlag=true;
	}
	//exact case
	else if((_command=="mod"||_command=="del"||_command=="find")&&
			prevSysWordIndex==0 &&
			listOfWords.at(prevSysWordIndex+1)=="exact")
	{
		wordFlag=true;
	}
	else if(listOfWords.at(prevSysWordIndex)=="done"||
		    listOfWords.at(prevSysWordIndex)=="undone"||
			listOfWords.at(prevSysWordIndex)=="timed"||
			listOfWords.at(prevSysWordIndex)=="deadline"||
			listOfWords.at(prevSysWordIndex)=="floating")
	{
		wordFlag=true;
	}
	else if(_command=="find"&&
		    prevSysWordIndex==0)
	{
		wordFlag=true;
	}

	return wordFlag;
}

void Interpreter::checkForDuplicateKeyWords()//ERROR_DUPLICATE
{
	bool duplicate=false;
	string duplicatedKeyWord;
	int numberOfKeyWords=listOfKeyWords.size();
	for(int i=0; i<numberOfKeyWords;i++)
		for(int j=i+1;j<numberOfKeyWords;j++)
		{
			if((listOfKeyWords.at(i).keyWord==
				listOfKeyWords.at(j).keyWord)&&
				listOfKeyWords.at(j).keyWord!="#")
			{
				duplicate=true;
				duplicatedKeyWord=listOfKeyWords.at(i).keyWord;
				_response.setStatus(ERROR);
				_response.setErrorMsg(ERROR_DUPLICATE);
			}
		}
	
	return;
}

time_t Interpreter::setTime(string keyWord,string inputInfo)
{
    int year=-1,month=-1,day=-1,hour=-1,min=-1;
	time_t rawtime;

	struct tm  timeinfo={0,0,0,0,0,0};

	time (&rawtime);

	localtime_s (&timeinfo,&rawtime);


	if(keyWord=="due"||
		keyWord=="from"||
		keyWord=="to"||
		keyWord=="rt")
	{

		//	string timeTemplete="dd/mo/yy hh:mm";


		if(inputInfo.length()==0){
			return 0;
		}
		if(inputInfo.length()>1){

			_isSuccess=integerConverter(inputInfo.substr(0,2),day);

			if(_isSuccess!=false && inputInfo.length()>4 ){

				if(inputInfo.at(2)=='/'){
					_isSuccess=integerConverter(inputInfo.substr(3,2),month);		

				}
				else _isSuccess=false;
			}	

			if(_isSuccess!=false && inputInfo.length()>7){
				if(inputInfo.at(5)=='/'){
					_isSuccess=integerConverter(inputInfo.substr(6,2),year);
					year=year+2000;
				}
				else _isSuccess=false;
			}

			if(_isSuccess!=false && inputInfo.length()>10){

				if(inputInfo.at(8)==' '){
					_isSuccess=integerConverter(inputInfo.substr(9,2),hour);
				}
				else _isSuccess=false;

			}
			else{
				hour = 0;
			}

			if(_isSuccess!=false && inputInfo.length()>13){

				if(inputInfo.at(11)==':'){
					_isSuccess=integerConverter(inputInfo.substr(12),min);
				}
				else _isSuccess=false;
			}
			else{
				min = 0;
			}

			if(_isSuccess!=false){

				if(year==-1)year=timeinfo.tm_year+1900;
				if(month==-1)month=timeinfo.tm_mon+1;
				if(day==-1)day=timeinfo.tm_mday+1;
				if(hour==-1)hour=timeinfo.tm_hour;
				if(min==-1)min=timeinfo.tm_min;
			}
		}
		else {

			_isSuccess=false;
		}

		
		if(!_isSuccess){
			_response.setStatus(ERROR);
			_response.setErrorMsg("Invalid time");
		}

		if(_isSuccess!=false){
			struct tm  timeMessage={0,0,0,0,0,0};

			timeMessage.tm_year=year-1900;
			timeMessage.tm_mon=month-1;
			timeMessage.tm_mday=day;
			timeMessage.tm_hour=hour;
			timeMessage.tm_min=min;

			return mktime(&timeMessage);
		}
	}

	return mktime(&timeinfo);
}

bool Interpreter::integerConverter(string& requiredString, int& number)
{

        bool flag=true;

        if(requiredString.empty()==true){
            flag=false;
        }
		else if(requiredString.size() >= 9){
			flag = false;
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

PRIORITY Interpreter::convertStrToPriority(string keyWord, string inputInfo)
{
	PRIORITY priority;
	if(keyWord=="impt")
	{
		if(inputInfo=="H" ||inputInfo=="HIGH")
		{
			priority=HIGH;
		}
		else if(inputInfo=="M" ||inputInfo=="MEDIUM")
		{
			priority=MEDIUM;
		}
		else if(inputInfo=="L" ||inputInfo=="LOW")
		{
			priority=LOW;
		}
		else{
			_response.setStatus(ERROR);
			_response.setErrorMsg("Invalid priority");
		}
	}

	return priority;

}

bool Interpreter::isByIndex()
{
	bool flag=false;
	int counter=0;
	string index=listOfWords.at(1);

	for(int i=0; i<index.size(); i++)
	{
		if(index.at(i)<='9'&& index.at(i)>='0')
		{
			counter++;
		}
	}

	if(counter==index.size())
	{
		flag = true;
	}

	return flag;
}

string Interpreter::reconstructInputInfo(int incrementIdx, int numberOfKeyWords)
{
	string inputInfo;
	string expectedChar=DELIMITER;
	int firstDataWord=0;
	int lastDataWord=0;

	if(incrementIdx==numberOfKeyWords-1)
	{
		firstDataWord=listOfKeyWords.at(numberOfKeyWords-1).index+1;
		lastDataWord=listOfWords.size()-1;
	}
	else
	{
		firstDataWord=listOfKeyWords.at(incrementIdx).index+1;
		lastDataWord=listOfKeyWords.at(incrementIdx+1).index-1;
	}
		
	for(int j=firstDataWord; j<=lastDataWord&&_isSuccess!=false; j++)
	{
		if(j==firstDataWord)
		{
			inputInfo+=listOfWords.at(j);
			if(inputInfo.at(0)!=expectedChar.at(0)){
				_isSuccess=false;
				_response.setStatus(ERROR);
				_response.setErrorMsg(ERROR_DATA);
			}
		}
		else
		{
			inputInfo=inputInfo+" "+listOfWords.at(j);
		}
	}

	if(inputInfo.size() != 0 && inputInfo.back()!=expectedChar.at(0)){
		_isSuccess=false;
		_response.setStatus(ERROR);
		_response.setErrorMsg(ERROR_DATA);
	}

	//eliminate ''		
	if(inputInfo.size() != 0){
		inputInfo.erase(inputInfo.begin());
	}
	if(inputInfo.size() != 0){
		inputInfo.erase(inputInfo.end()-1);
	}

	return inputInfo;
}

string Interpreter::reconstructName(int startIndex, int endIndex)//ERROR_NAME
{
	string name="";
	string expectedChar=DELIMITER;
		
	for(int j=startIndex; j<=endIndex; j++)
	{
		if(j==startIndex)
		{
			name+=listOfWords.at(j);
			//check first '
			if(name.at(0)!=expectedChar.at(0)){
				_response.setStatus(ERROR);
				_response.setErrorMsg(ERROR_NAME);
			}
		}
		else
		{
			name=name+" "+listOfWords.at(j);
		}
	}

	//check last '		
	if(name.back()!=expectedChar.at(0)){
		_response.setStatus(ERROR);
		_response.setErrorMsg(ERROR_NAME);
			}

	//eliminate ''		
	name.erase(name.begin());
	name.erase(name.end()-1);

	return name;


}

void Interpreter::allocatePpl(string keyWord, list<string>&ppl, string inputInfo)
{
	if(keyWord=="ppl"){
		string pplComponent=inputInfo;
		stringstream timeExtracter(pplComponent);

		while(getline(timeExtracter,pplComponent,',')){
			std::stringstream trimmer;
			//trim!
			trimmer << pplComponent;
			pplComponent.clear();
			trimmer >> pplComponent;
			ppl.push_back(pplComponent);
		}
	}

	return;
}

void Interpreter::allocateRt(string keyWord, list<time_t>&remindTime, string inputInfo)
{
	if(keyWord=="rt"){

	
	time_t rtTime;
	string timeComponent=inputInfo;
	stringstream timeExtracter(timeComponent);

	while(getline(timeExtracter,timeComponent,',')){

		rtTime=setTime(keyWord,timeComponent);
		remindTime.push_back(rtTime);
		}
	}

	return;

}

void Interpreter::allocateTags(string keyWord, list<string>&tags, string inputInfo)
{
	if(keyWord=="#"){
	string pplComponent=inputInfo;
	stringstream timeExtracter(pplComponent);

	while(getline(timeExtracter,pplComponent,',')){
		std::stringstream trimmer;
		//trim!
		trimmer << pplComponent;
		pplComponent.clear();
		trimmer >> pplComponent;
		tags.push_back(pplComponent);
		}
	}
	
	return;
}

void Interpreter::setCmdObj_Add(Command_Add* cmd, string keyWord, string inputInfo, unsigned int time, PRIORITY priority, list<string>ppl, list<string>tags, list<time_t>remindTime)
{
	if(keyWord=="due")
	{
		cmd->setDueDate(time);
	}
	else if(keyWord=="from")
	{
		cmd->setFromDate(time);
	}
	else if(keyWord=="to")
	{
		cmd->setToDate(time);
	}
	else if(keyWord=="at")
	{
		cmd->setLocation(inputInfo);
	}
	else if(keyWord=="ppl")
	{
		cmd->setParticipants(ppl);
	}
	else if(keyWord=="note")
	{
		cmd->setNote(inputInfo);
	}
	else if(keyWord=="impt")
	{
		cmd->setPriority(priority);
	}
	else if(keyWord=="#")
	{
		cmd->setTags(tags);
	}
	else if(keyWord=="rt")
	{
		cmd->setRemindTimes(remindTime);
	}

	return;
}

void Interpreter::setCmdObj_Mod(Command_Mod* cmd, string keyWord, string inputInfo, unsigned int time, PRIORITY priority, list<string>ppl, list<string>tags, list<time_t>remindTime)
{
	if(keyWord=="name")
	{
		cmd->setOptName(inputInfo);
	}
	else if(keyWord=="due")
	{
		cmd->setDueDate(time);
	}
	else if(keyWord=="from")
	{
		cmd->setFromDate(time);
	}
	else if(keyWord=="to")
	{
		cmd->setToDate(time);
	}
	else if(keyWord=="at")
	{
		cmd->setLocation(inputInfo);
	}
	else if(keyWord=="ppl")
	{
		cmd->setParticipants(ppl);
	}
	else if(keyWord=="note")
	{
		cmd->setNote(inputInfo);
	}
	else if(keyWord=="impt")
	{
		cmd->setPriority(priority);
	}
	else if(keyWord=="#")
	{
		cmd->setTags(tags);
	}
	else if(keyWord=="rt")
	{
		cmd->setRemindTimes(remindTime);
	}
	else if(keyWord=="done")
	{
		cmd->setTaskState(DONE);
	}
	else if(keyWord=="undone")
	{
		cmd->setTaskState(UNDONE);
	}
	else if(keyWord=="-at")
	{
		inputInfo="";
		cmd->setLocation(inputInfo);
	}
	else if(keyWord=="-ppl")
	{
		inputInfo="";
		cmd->setParticipants(ppl);
	}
	else if(keyWord=="-note")
	{
		inputInfo="";
		cmd->setNote(inputInfo);
	}
	else if(keyWord=="-#")
	{
		inputInfo="";
		cmd->setTags(tags);
	}
	else if(keyWord=="-rt")
	{
		time=-1;
		cmd->setRemindTimes(remindTime);
	}

	return;
}

/*void Interpreter::setCmdObj_Del(Command_Del* cmd)
{

		
	return;
}*/

void Interpreter::setCmdObj_Find(Command_Find* cmd, string keyWord, string inputInfo, unsigned int time, PRIORITY priority, list<string>ppl, list<string>tags, list<time_t>remindTime)
{
	if(keyWord=="name")
	{
		cmd->setOptName(inputInfo);
	}
	else if(keyWord=="from")
	{
		cmd->setFromDate(time);
	}
	else if(keyWord=="to")
	{
		cmd->setToDate(time);
	}
	else if(keyWord=="at")
	{
		cmd->setLocation(inputInfo);
	}
	else if(keyWord=="ppl")
	{
		cmd->setParticipants(ppl);
	}
	else if(keyWord=="note")
	{
		cmd->setNote(inputInfo);
	}
	else if(keyWord=="impt")
	{
		cmd->setPriority(priority);
	}
	else if(keyWord=="#")
	{
		cmd->setTags(tags);
	}
	else if(keyWord=="rt")
	{
		cmd->setRemindTimes(remindTime);
	}
	else if(keyWord=="done")
	{
		cmd->setTaskState(DONE);
	}
	else if(keyWord=="undone")
	{
		cmd->setTaskState(UNDONE);
	}
	else if(keyWord=="timed")
	{
		cmd->setTaskType(TIMED);
	}
	else if(keyWord=="deadline")
	{
		cmd->setTaskType(DEADLINE);
	}
	else if(keyWord=="floating")
	{
		cmd->setTaskType(FLOATING);
	}

	/*else if(keyWord=="-at")
	{
		inputInfo="";
		cmd->setLocation(inputInfo);
	}
	else if(keyWord=="-ppl")
	{
		inputInfo="";
		cmd->setParticipants(inputInfo);
	}
	else if(keyWord=="-note")
	{
		inputInfo="";
		cmd->setNote(inputInfo);
	}
	else if(keyWord=="-#")
	{
		inputInfo="";
		cmd->setTags(inputInfo);
	}
	else if(keyWord=="-rt")
	{
		time=-1;
		cmd->setRemindTime(time);
	}*/

	return;
}

/*
************************************************************
Principal Functions
************************************************************
*/

void Interpreter::extractIndividualWords(string userInput)
{
	//extract individual word
	string individualWord;
	istringstream ss(userInput);
	
	while(ss>>individualWord)
	{
		listOfWords.push_back(individualWord);
	}
	//empty or contains only command word
	if(listOfWords.size()<2){
		_isSuccess=false;
		_response.setStatus(ERROR);
		_response.setErrorMsg("Command with no parameters / Invalid command");
	}

	return;
}

void Interpreter::extractCommand()
{
	_command=listOfWords.at(0);

	if(_command!="add"&&_command!="mod"&&_command!="del"&&_command!="find"){
		_isSuccess=false;
		_response.setStatus(ERROR);
		_response.setErrorMsg("Invalid command");
	}

	return;
}

void Interpreter::extractKeyWords()
{
	//check for keyword
	KEY_WORDS individualWord;
	int numberOfWords=listOfWords.size();
	for(int i=0; i<numberOfWords;i++)
	{
		individualWord.keyWord=listOfWords.at(i);
		individualWord.index=i;
		
		if(individualWord.keyWord=="name"||
		   individualWord.keyWord=="exact"||
		   individualWord.keyWord=="ppl"||
		   individualWord.keyWord=="due"||
		   individualWord.keyWord=="from"||
		   individualWord.keyWord=="to"||
		   individualWord.keyWord=="at"||
		   individualWord.keyWord=="ppl"||
		   individualWord.keyWord=="note"||
		   individualWord.keyWord=="impt"||
		   individualWord.keyWord=="#"||
		   individualWord.keyWord=="rt"||
		   individualWord.keyWord=="done"||
		   individualWord.keyWord=="undone"||
		   individualWord.keyWord=="-at"||
		   individualWord.keyWord=="-ppl"||
		   individualWord.keyWord=="-note"||
		   individualWord.keyWord=="-#"||
		   individualWord.keyWord=="-rt"||
		   individualWord.keyWord=="timed"||
		   individualWord.keyWord=="deadline"||
		   individualWord.keyWord=="floating")
		{
			listOfKeyWords.push_back(individualWord);
		}

		//if(listOfKeyWords.empty())
		
	}
	return;
}

void Interpreter::functionAdd()
{
	Command_Add *cmd = new Command_Add;
	int numberOfKeyWords=listOfKeyWords.size();
	string name="";
	string inputInfo="";
	

	list<string>tags;
	list<string>ppl;
	list<time_t>remindTime;

	//set name
	if(listOfKeyWords.size()!=0){
		name=reconstructName(1,listOfKeyWords.at(0).index-1);
	}
	else{
		name=reconstructName(1,listOfWords.size()-1);
	}
	cmd->setName(name);
	
	for(int i=0; i<numberOfKeyWords && listOfKeyWords.size()!=0; i++)
	{
		string keyWord=listOfKeyWords.at(i).keyWord;
		
		if(	keyWord!="timed"&&
			keyWord!="deadline"&&
			keyWord!="floating")
		{
			//reconstruct
			inputInfo=reconstructInputInfo(i,numberOfKeyWords);
		}

		//unit test
		strUT.push_back(inputInfo);

		//convert str to time_t

		time_t time=setTime( keyWord, inputInfo);
		
		//convert str to PRIORITY
		PRIORITY priority=convertStrToPriority(keyWord,inputInfo);

		//set list ppl, list tags, list remindTime
		allocatePpl(keyWord,ppl,inputInfo);
		allocateRt(keyWord,remindTime,inputInfo);
		allocateTags(keyWord,tags,inputInfo);
		
		//unit test
		pplUT=ppl;
		tagsUT=tags;

		//allocate	
		setCmdObj_Add(cmd, keyWord,inputInfo,time,priority,ppl,tags,remindTime);

		//reset inputInfo after each iteration
		inputInfo.clear();
	}

	
	if(!_isSuccess){
	delete cmd;
	}
	else _result = cmd;
	cmd=NULL;

	return;
}

void Interpreter::functionMod()
{
	Command_Mod *cmd= new Command_Mod;
	int numberOfKeyWords=listOfKeyWords.size();
	string inputInfo="";

	list<string>tags;
	list<string>ppl;
	list<time_t>remindTime;


	if(isByIndex())
	{
		if(listOfWords.at(1).size() < 9 && (listOfKeyWords.size()==0 ||listOfKeyWords.at(0).index==2)){

			int index=stoi(listOfWords.at(1),nullptr,10);
			cmd->setIndex(index);
		}
		else{
			_isSuccess=false;
			_response.setStatus(ERROR);
			_response.setErrorMsg(ERROR_INDEX);
		}
	}
	else if(listOfWords.at(1)=="exact")
	{
		string name=reconstructName(2,listOfKeyWords.at(1).index-1);
		cmd->setName(name);
		cmd->setFlagExact();
	}
	else
	{
		string name;
		if(listOfKeyWords.size()!=0){
			name=reconstructName(1,listOfKeyWords.at(0).index-1);
		}
		else{
			name=reconstructName(1,listOfWords.size()-1);
		}
		cmd->setName(name);
	}

	for(int i=0; i<numberOfKeyWords&&listOfKeyWords.size()!=0; i++)
	{
		
		string keyWord=listOfKeyWords.at(i).keyWord;

		if(keyWord!="done"&&
			keyWord!="undone"&&
			keyWord!="timed"&&
			keyWord!="deadline"&&
			keyWord!="floating")
		{
			//reconstruct
			inputInfo=reconstructInputInfo(i,numberOfKeyWords);
		}
		
		
		//unit test
		strUT.push_back(inputInfo);
		
		//convert str to time_t
		time_t time=setTime( keyWord, inputInfo);

		//convert str to PRIORITY
		PRIORITY priority=convertStrToPriority(keyWord,inputInfo);

		//set list ppl, list tags, list remindTime
		allocatePpl(keyWord,ppl,inputInfo);
		allocateRt(keyWord,remindTime,inputInfo);
		allocateTags(keyWord,tags,inputInfo);

		//unit test
		pplUT=ppl;
		tagsUT=tags;

		//allocate function-specific
		setCmdObj_Mod(cmd, keyWord,inputInfo,time,priority,ppl,tags,remindTime);

		//reset inputInfo after each iteration
		inputInfo.clear();
	}


	if(!_isSuccess){
	delete cmd;
	}
	else _result = cmd;
	cmd=NULL;

	
	return;
}

void Interpreter:: functionDel()
{
	Command_Del *cmd= new Command_Del;
	
	if(isByIndex())
	{
		if(listOfWords.size()==2){
			int index=stoi(listOfWords.at(1),nullptr,10);
			cmd->setIndex(index);
		}
		else{
			_isSuccess=false;
			_response.setStatus(ERROR);
			_response.setErrorMsg(ERROR_INDEX);
		}
	}
	else if(listOfWords.at(1)=="exact")
	{
		string name=reconstructName(2,listOfWords.size()-1);
		cmd->setName(name);
		cmd->setFlagExact();
	}
	else
	{
		string name;
		if(listOfKeyWords.size()!=0){
			name=reconstructName(1,listOfKeyWords.at(0).index-1);
		}
		else{
			name=reconstructName(1,listOfWords.size()-1);
		}
		cmd->setName(name);
	}
	
	if(!_isSuccess){
	delete cmd;
	}
	else _result = cmd;
	cmd=NULL;
	return;
}

void Interpreter:: functionFind()
{
	Command_Find *cmd= new Command_Find;
	int numberOfKeyWords=listOfKeyWords.size();
	string inputInfo="";
	
	
	list<string>tags;
	list<string>ppl;
	list<time_t>remindTime;

	if(isByIndex())
	{
		if(listOfKeyWords.size()==0){
			int index=stoi(listOfWords.at(1),nullptr,10);
			cmd->setIndex(index);
		}
		else{
			_isSuccess=false;
			_response.setStatus(ERROR);
			_response.setErrorMsg(ERROR_INDEX);
		}

		//return;// for find only
	}
	else if(listOfWords.at(1)=="exact")
	{
		string name =reconstructName(2,listOfWords.size()-1);
		cmd->setOptName(name);
		cmd->setFlagExact();

		//return; //find only
	}
	else if(listOfKeyWords.empty())
	{
		string name=reconstructName(1,listOfWords.size()-1);
		cmd->setOptName(name);
	}
	else if(!listOfKeyWords.empty() && listOfKeyWords.at(0).index!=1)
	{
		string name=reconstructName(1,listOfKeyWords.at(0).index-1);
		cmd->setOptName(name);
	}
	else
	{
		//do nothing
	}


	for(int i=0; i<numberOfKeyWords && !listOfKeyWords.empty(); i++)
	{
		string keyWord=listOfKeyWords.at(i).keyWord;
		
		if(keyWord!="done"&&
		   keyWord!="undone"&&
		   keyWord!="timed"&&
		   keyWord!="deadline"&&
		   keyWord!="floating")
		{
			//reconstruct
			inputInfo=reconstructInputInfo(i,numberOfKeyWords);
		}
		

		//unit test
		strUT.push_back(inputInfo);

		//convert str to time_t
		time_t time=setTime( keyWord, inputInfo);

		//convert str to PRIORITY
		PRIORITY priority=convertStrToPriority(keyWord,inputInfo);

		//set list ppl, list tags, list remindTime
		allocatePpl(keyWord,ppl,inputInfo);
		allocateRt(keyWord,remindTime,inputInfo);
		allocateTags(keyWord,tags,inputInfo);

		//unit test
		pplUT=ppl;
		tagsUT=tags;

		//allocate function-specific
		setCmdObj_Find(cmd, keyWord,inputInfo,time,priority,ppl,tags,remindTime);

		//reset inputInfo after each iteration
		inputInfo.clear();
	}

	if(!_isSuccess){
		delete cmd;
	}
	else _result = cmd;
	cmd=NULL;

	
	return;
}

/*
************************************************************
Auxillary Functions for UnitTest
************************************************************
*/
string Interpreter::getIndividualWord(int userIndex)
{
	
	int systemIndex=userIndex-1;
	string requestedWord=listOfWords.at(systemIndex);

	return requestedWord;
}

string Interpreter::getstrUT(int userIndex)
{
	int systemIndex=userIndex-1;
	string requestedWord=strUT.at(systemIndex);

	return requestedWord;
}

string Interpreter:: getKeyWord(int userIndex)
{
	int systemIndex=userIndex-1;
	string keyWord=listOfKeyWords.at(systemIndex).keyWord;

	return keyWord;
}

int Interpreter:: getListOfKeyWordsSize()
{
	int size=listOfKeyWords.size();

	return size;
}

Command* Interpreter::get_cmdObject(string command)
{
	return _result;
}

string Interpreter::getPplUT(int userIndex)
{
	auto item=pplUT.begin();
	std::advance(item,userIndex-1);

	return *item;
}

string Interpreter::getTagsUT(int userIndex)
{
	auto item=tagsUT.begin();
	std::advance(item,userIndex-1);

	return *item;
}

/*
************************************************************
APIs
************************************************************
*/
Command * Interpreter::interpretCommand(string userInput, Messenger & response)
{
	bool success;
	_response = response;
	_response.setStatus(SUCCESS);

	extractIndividualWords(userInput);
	
	if(_isSuccess){
		extractCommand();
	}

	if(_isSuccess){
		extractKeyWords();
	}

	if(_isSuccess){
		processKeyWords();
	}
	
	if(_isSuccess){
		processCommand();
	}

	response = _response;
	success=_isSuccess;
	//reset
	listOfKeyWords = vector<KEY_WORDS>();
	listOfWords = vector<string>();
	_isSuccess=true;

	if(!success){
		return NULL;
	}
	else return _result;

}

int Interpreter::interpretIndex(string indexStr, Messenger & response)
{
	int num;
	if(integerConverter(indexStr,num))
	{
		response.setStatus(SUCCESS);

	}
	else{ 

		response.setStatus(ERROR);
		response.setErrorMsg("The index is invalid");
	}

	return num;
}

void Interpreter::interpretCommand(unsigned int index, Command* prevCommand)
{
	
	return;
}