#include "Interpreter.h"


void Interpreter::interpretCommand(unsigned ActualIndex, Command *prevCommand){

	TP::COMMAND_TYPE taskType;
	taskType=prevCommand->getCommandType();
	Command_Mod* Mod_pointer;
	Command_Del* Del_pointer;

	if(taskType==MOD){
		Mod_pointer=(Command_Mod*)prevCommand;
		Mod_pointer->setIndex(ActualIndex);
		prevCommand=Mod_pointer;
	}

	else if(taskType==DEL){

		Del_pointer=(Command_Del*)prevCommand;
		Del_pointer->setIndex(ActualIndex);
		prevCommand=Del_pointer;

	}


	return;
}


int Interpreter::interpretIndex(std::string indexStr, Messenger &response){

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

bool Interpreter::checkCommand(string command, int& commandType){

	bool flag=false;
	bool testlist[12]={false};
	int num=-1;


	string generalAddCase="((( due| from| to| impt| at| ppl| note| rt) `[^`]*`)|( #[^( |`)]*))*";
	string generalModCase="((( due| from| name| to| impt| at| ppl| note| rt) `[^`]*`)|( done| undone)|( (#)[^( |`)]*))*";
	string generalFindCase="((( due| from| name| to| impt| at| ppl| note| rt) `[^`]*`)|( #[^( |`)]*)|( done| undone)|(( timed| deadline| floating)( |$)))*";



	regex test_add_command("^add `([^`]+)`"+generalAddCase); 
	regex test_mod_command("^mod `([^`]+)`"+generalModCase); 
	regex test_mod_exact_command("^mod exact `([^`]+)`"+generalModCase);
	regex test_mod_index_command("^mod ([0-9]+)"+generalModCase);

	regex test_find_command("^find"+generalFindCase);
	regex test_find_exact_command("^find exact"+generalFindCase);

	regex test_del_command("^del `([^`]+)`");
	regex test_del_exact_command("^del exact `([^`]+)`");
	regex test_del_index_command("^del ([0-9]+)");
	regex test_undo_command("^undo");
	regex test_redo_command("^redo");
	regex test_syn_command("^sync `([^`]+)`");


	testlist[0]=regex_match(command,test_add_command);
	testlist[1]=regex_match(command,test_mod_command);
	testlist[2]=regex_match(command,test_mod_exact_command);
	testlist[3]=regex_match(command,test_mod_index_command);
	testlist[4]=regex_match(command,test_find_command);
	testlist[5]=regex_match(command,test_find_exact_command);
	testlist[6]=regex_match(command,test_del_command);
	testlist[7]=regex_match(command,test_del_exact_command);
	testlist[8]=regex_match(command,test_del_index_command);
	testlist[9]=regex_match(command,test_undo_command);
	testlist[10]=regex_match(command,test_redo_command);
	testlist[11]=regex_match(command,test_syn_command);

	for(int i=0;i<12 && flag!=true;i++){
		if(testlist[i]==true){
			num=i;
			flag=true;
		}
	}

	commandType=num;
	return flag;
}

Command* Interpreter::interpretCommand(std::string commandStr, Messenger &response)
{
	bool flag=true;
	int commandType;

	string word;
	Command* returnCommand;

	flag=checkCommand(commandStr,commandType);



	if(flag!=false){
		switch (commandType){
		case 0:
			{
				Command_Add* Add_pointer=new Command_Add();


				stringstream extractName(commandStr);
				string taskName;
				getline(extractName,taskName,'`');
				taskName.clear();
				getline(extractName,taskName,'`');
				Add_pointer->setName(taskName);

				returnCommand=interpretAdd(Add_pointer, commandStr, response,flag);

				break;
			}

		case 1:
			{
				Command_Mod* Mod_pointer=new Command_Mod();

				stringstream extractName(commandStr);
				string taskName;
				getline(extractName,taskName,'`');
				taskName.clear();
				getline(extractName,taskName,'`');
				Mod_pointer->setName(taskName);

				returnCommand=interpretModify(Mod_pointer, commandStr, response,flag);
				break;
			}

		case 2:
			{
				Command_Mod* Mod_pointer=new Command_Mod();

				Mod_pointer->setFlagExact();

				stringstream extractName(commandStr);
				string taskName;
				getline(extractName,taskName,'`');
				taskName.clear();
				getline(extractName,taskName,'`');
				Mod_pointer->setName(taskName);



				returnCommand=interpretModify(Mod_pointer, commandStr, response,flag);

				break;
			}

		case 3:
			{
				Command_Mod* Mod_pointer=new Command_Mod();
				string getIndex=commandStr;
				stringstream extractIndex(getIndex);
				string content;
				extractIndex>>getIndex;
				getIndex.clear();
				extractIndex>>getIndex;
				int index;

				index=getIndexMessage(getIndex,flag);
				Mod_pointer->setIndex(index);

				returnCommand=interpretModify(Mod_pointer, commandStr, response,flag);
				break;
			}


		case 4:
			{

				Command_Find* Find_pointer=new Command_Find();



				returnCommand=interpretFind(Find_pointer,commandStr, response,flag);
				break;
			}

		case 5:
			{

				Command_Find* Find_pointer=new Command_Find();

				Find_pointer->setFlagExact();
				returnCommand=interpretFind(Find_pointer,commandStr, response,flag);
				break;
			}

		case 6:
			{

				Command_Del* Del_pointer=new Command_Del();
				returnCommand=interpretDelete(Del_pointer,commandStr, response,flag);
				break;

			}
		case 7:
			{
				Command_Del* Del_pointer=new Command_Del();

				Del_pointer->setFlagExact();

				returnCommand=interpretDelete(Del_pointer,commandStr, response,flag);
				break;
			}
		case 8:
			{
				Command_Del* Del_pointer=new Command_Del();
				stringstream extractIndex(commandStr);
				string content;
				extractIndex>>commandStr;
				commandStr.clear();
				extractIndex>>commandStr;
				int index;

				index=getIndexMessage(commandStr,flag);
				Del_pointer->setIndex(index);
				response.setCommandType(DEL);
			
				returnCommand=(Command*)Del_pointer;
				break;


			}
		case 9:
			{
				Command_Undo* Undo_pointer=new Command_Undo();

				returnCommand=interpretUndo(Undo_pointer,commandStr, response,flag);
				break;
			}
		case 10:
			{
				Command_Redo* Redo_pointer=new Command_Redo();

				returnCommand=interpretRedo(Redo_pointer,commandStr, response,flag);
				break;


			}
		case 11:
			{
				Command_Sync *Sync_pointer=new Command_Sync();

				returnCommand=interpretSync(Sync_pointer,commandStr, response,flag);

				break;
			}

		default: flag=false;
			break;
		}

	}
	if(flag==false){

		response.setStatus(ERROR);
		response.setErrorMsg("invalid command");
		return NULL;
	}

	else{

		response.setStatus(SUCCESS);

		return returnCommand;
	}

}

Command* Interpreter:: interpretAdd(Command_Add* commandType, string commandStr,Messenger &response, bool& flag)
{


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

	if(flag==true){

		response.setStatus(SUCCESS);
		response.setCommandType(ADD);
		return (Command*)commandType;
	}

	else return NULL;
}



Command* Interpreter::interpretModify(Command_Mod* commandType, std::string commandStr, Messenger &response, bool& flag){

	if(flag && commandType->getFlagDue()==false){		

		time_t content;
		if(getDueDateMessage(commandStr,flag,content)){		
			commandType->setDueDate(content);		
		}		
	}
	else {

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

	if(flag==true){
		response.setStatus(SUCCESS);
		response.setCommandType(MOD);
		return (Command*)commandType;
	}
	else return NULL;
}



Command* Interpreter::interpretFind(Command_Find* commandType, std::string commandStr, Messenger &response, bool& flag){


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

	if(flag && commandType->getFlagTaskType()==false){
		TASK_TYPE content;
		if(getTaskTypeMessage(commandStr,flag,content)){
			commandType->setTaskType(content);
		}
	}
	else{

		flag=false;

	}

	if(flag==true){
		response.setStatus(SUCCESS);
		response.setCommandType(FIND);
		return (Command*)commandType;
	}
	else return NULL;
}


Command* Interpreter:: interpretDelete(Command_Del*commandType,std::string commandStr, Messenger &response, bool &flag){

	stringstream extractContent(commandStr);
	string content;
	getline(extractContent,content,'`');
	content.clear();
	getline(extractContent,content,'`');
	commandType->setName(content);


	response.setCommandType(DEL);
	response.setStatus(SUCCESS);
	return (Command*)commandType;
}
Command* Interpreter:: interpretRedo(Command_Redo*commandType,std::string commandStr, Messenger &response, bool &flag){

	response.setCommandType(REDO);
	response.setStatus(SUCCESS);
	return (Command*)commandType;
}

Command* Interpreter:: interpretUndo(Command_Undo*commandType,std::string commandStr, Messenger &response, bool &flag){

	response.setCommandType(UNDO);
	response.setStatus(SUCCESS);
	return (Command*)commandType;
}

Command* Interpreter:: interpretSync(Command_Sync*commandType,std::string commandStr, Messenger &response, bool &flag){

	string content;
	if(getSyncProviderNameMessage(commandStr,flag,content)){
		commandType->setSyncProviderName(content);
	}
	response.setCommandType(SYNC);
	response.setStatus(SUCCESS);
	return (Command*)commandType;
}



int Interpreter:: getIndexMessage(string command,bool& flag){

	int num;
	flag=integerConverter(command, num);
	return num;
}	


bool Interpreter::checkKeyWord(string command, int position){

	vector<int> positionForNotion; 
	positionForNotion.push_back(-1); // dummy value;
	bool isKeyWord=true;


	for(int i=0;i<command.length();i++)
	{
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

bool Interpreter::checkDuplicate(string command, string cmdTemplate,int startPosition){

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

bool Interpreter:: getNameMessage(string command, bool&flag, string& content){ 

	regex extractTemplete(" name `[^`]*`");
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

		if(checkDuplicate(command," name `[^`]*`",match.position())==true){

			flag=false;
		}

	}
	else{

		isNotEmpty=false;

	}


	return isNotEmpty;

}
bool Interpreter::getOptNameMessage(string command, bool&flag,string& content){ 

	regex extractTemplete(" name `[^`]*`");
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

		if(checkDuplicate(command," name `[^`]*`",match.position())==true){

			flag=false;
		}

	}
	else{

		isNotEmpty=false;

	}


	return isNotEmpty;
}

bool Interpreter::getDueDateMessage(string command, bool&flag, time_t& content){

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

		content=setTime(preContent,flag);

		//convertToTime(preContent,content,flag);

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

bool	Interpreter::getFromDateMessage(string command, bool&flag, time_t& content){

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

		content=setTime(preContent,flag);

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

bool Interpreter::getToDateMessage(string command, bool&flag, time_t& content){

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

		content=setTime(preContent,flag);

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


bool Interpreter::getLocationMessage(string command, bool&flag, string& content){

	regex extractTemplete(" at `[^`]*`");
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

		if(checkDuplicate(command," at `[^`]*`",match.position())==true){

			flag=false;
		}		

	}
	else{

		isNotEmpty=false;

	}


	return isNotEmpty;

}

bool Interpreter::getParticipantsMessage(string command, bool&flag, list<std::string>& content){
	list<string>pplList;
	regex extractTemplete(" ppl `[^`]*`");
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


		if(checkDuplicate(command," ppl `[^`]*`",match.position())==true){

			flag=false;
		}	

	}
	else{

		isNotEmpty=false;
	}


	return isNotEmpty;

}	

bool	Interpreter::getNoteMessage(string command, bool&flag,string& content){

	regex extractTemplete(" note `[^`]*`");
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

		if(checkDuplicate(command," note `[^`]*`",match.position())==true){

			flag=false;
		}	

	}
	else{

		isNotEmpty=false;

	}


	return isNotEmpty;


}
bool Interpreter::getPriorityMessage(string command, bool&flag,TP::PRIORITY& content){ // need to force type
	regex extractTemplete(" impt `[^`]*`");
	smatch match;
	string commandStr;
	string preContent;
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

		if(preContent=="H" ||preContent=="HIGH"){
			priority=HIGH;

		}
		else if(preContent=="M" || preContent=="MEDIUM"){
			priority=MEDIUM;

		}
		else if(preContent=="L"||preContent=="LOW"){

			priority=LOW;

		}
		else if(preContent.empty()){

			priority=MEDIUM;
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

bool Interpreter::getTagsMessage(string command, bool&flag,list<std::string>& content){

	list<string>tagList;
	regex extractTemplete("\\s(#[^( |`)]*)(\\s|$)");

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
			tagList.push_back(tagContent);
			count++;
		}	


		subStirng=subStirng.substr(match.position()+1);
		commandStr.clear();

		if (regex_search(subStirng, match, extractTemplete)){

			commandStr=match[0];

		}

	}

	if(count==0){
		isNotEmpty=false;
	}

	content=tagList;
	return isNotEmpty;

}	

bool Interpreter::	getRemindTimesMessage(string command, bool&flag,list<std::time_t>&content){

	list<time_t>rtList;
	regex extractTemplete(" rt `[^`]*`");
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
			time_t rtTime=setTime(time,flag);
			rtList.push_back(rtTime);
			time.clear();
			getline(extractIndividual,time,',');
		}
		content=rtList;

		if(checkDuplicate(command," rt `[^`]*`",match.position())==true){

			flag=false;
		}
	}
	else{

		isNotEmpty=false;
	}



	return isNotEmpty;
}

bool Interpreter::getTaskStateMessage(string command, bool&flag, TP::TASK_STATE& content){ 

	TASK_STATE task_state;
	string commandStr;
	smatch match;
	bool isNotEmpty=true;
	regex extractTemplete("\\s(done|undone)(\\s|$)");
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

		else {
			content=DONE;	

		}
	}

	else{

		flag=false;
	}

	return isNotEmpty;
}

bool Interpreter::getTaskTypeMessage(string command, bool&flag, TP::TASK_TYPE& content){

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

bool Interpreter::getSyncProviderNameMessage(string command, bool&flag, string&content){

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
/*
time_t Interpreter::setTime(string commandStr,bool& _isSuccess)
{
int year=-1,month=-1,day=-1,hour=-1,min=-1;
time_t rawtime;
string inputInfo=commandStr;
struct tm  timeinfo={0,0,0,0,0,0};

time (&rawtime);

localtime_s (&timeinfo,&rawtime);


//	string timeTemplete="dd/mo/yy hh:mm";

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

if(_isSuccess!=false && inputInfo.length()>13){

if(inputInfo.at(11)==':'){
_isSuccess=integerConverter(inputInfo.substr(12),min);
}
else _isSuccess=false;
}

if(_isSuccess!=false){

if(year==-1)year=timeinfo.tm_year+1900;
if(month==-1)month=timeinfo.tm_mon+1;
if(day==-1)day=timeinfo.tm_mday+1;
if(hour==-1)hour=timeinfo.tm_hour;
if(min==-1)min=timeinfo.tm_min;
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

*/
time_t Interpreter::setTime(string commandStr,bool& flag){
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
		stringstream extract(commandStr);
		getline(extract,content,' ');
		flag=integerConverter(content,day);
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
if(day==-1)day=timeinfo.tm_mday+1;
if(hour==-1)hour=0;
if(min==-1)min=0;
}

struct tm  timeMessage={0,0,0,0,0,0};

timeMessage.tm_year=year-1900;
timeMessage.tm_mon=month-1;
timeMessage.tm_mday=day;
timeMessage.tm_hour=hour;
timeMessage.tm_min=min;

return mktime(&timeMessage);


}




bool Interpreter::integerConverter(string& requiredString, int& number)
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


