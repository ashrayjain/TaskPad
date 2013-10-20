//#include "Interpreter.h"
//
//
//void Interpreter::interpretCommand(unsigned ActualIndex, Command *prevCommand){
//
//	TP::COMMAND_TYPE taskType;
//	taskType=prevCommand->getCommandType();
//	Command_Mod* Mod_pointer;
//	Command_Del* Del_pointer;
//
//	if(taskType==MOD){
//		Mod_pointer=(Command_Mod*)prevCommand;
//		Mod_pointer->setIndex(ActualIndex);
//		prevCommand=Mod_pointer;
//	}
//
//	else if(taskType==DEL){
//
//		Del_pointer=(Command_Del*)prevCommand;
//		Del_pointer->setIndex(ActualIndex);
//		prevCommand=Del_pointer;
//
//	}
//
//
//	return;
//}
//
//
//int Interpreter::interpretIndex(std::string indexStr, Messenger &response){
//
//	int num;
//	if(integerConverter(indexStr,num))
//	{
//		response.setStatus(SUCCESS);
//
//	}
//	else{ 
//
//		response.setStatus(ERROR);
//		response.setErrorMsg("The index is invalid");
//	}
//
//	return num;
//}
//
//bool Interpreter::checkCommand(string command, int& commandType){
//
//	bool flag=false;
//	bool testlist[12]={false};
//	int num=-1;
//
//
//	string generalAddCase="((( due| from| to| impt| at| ppl| note| rt) `[^`]*`)|( #[^( |`)]*))*";
//	string generalModCase="((( due| from| name| to| impt| at| ppl| note| rt) `[^`]*`)|( done| undone)|( (#)[^( |`)]*))*";
//	string generalFindCase="((( due| from| name| to| impt| at| ppl| note| rt) `[^`]*`)|( #[^( |`)]*)|( done| undone)|(( timed| deadline| floating)( |$)))*";
//
//
//
//	regex test_add_command("^add `([^`]+)`"+generalAddCase); 
//	regex test_mod_command("^mod `([^`]+)`"+generalModCase); 
//	regex test_mod_exact_command("^mod exact `([^`]+)`"+generalModCase);
//	regex test_mod_index_command("^mod ([0-9]+)"+generalModCase);
//
//	regex test_find_command("^find"+generalFindCase);
//	regex test_find_exact_command("^find exact"+generalFindCase);
//
//	regex test_del_command("^del `([^`]+)`");
//	regex test_del_exact_command("^del exact `([^`]+)`");
//	regex test_del_index_command("^del ([0-9]+)");
//	regex test_undo_command("^undo");
//	regex test_redo_command("^redo");
//	regex test_syn_command("^sync `([^`]+)`");
//
//
//	testlist[0]=regex_match(command,test_add_command);
//	testlist[1]=regex_match(command,test_mod_command);
//	testlist[2]=regex_match(command,test_mod_exact_command);
//	testlist[3]=regex_match(command,test_mod_index_command);
//	testlist[4]=regex_match(command,test_find_command);
//	testlist[5]=regex_match(command,test_find_exact_command);
//	testlist[6]=regex_match(command,test_del_command);
//	testlist[7]=regex_match(command,test_del_exact_command);
//	testlist[8]=regex_match(command,test_del_index_command);
//	testlist[9]=regex_match(command,test_undo_command);
//	testlist[10]=regex_match(command,test_redo_command);
//	testlist[11]=regex_match(command,test_syn_command);
//
//	for(int i=0;i<12 && flag!=true;i++){
//		if(testlist[i]==true){
//			num=i;
//			flag=true;
//		}
//	}
//
//	commandType=num;
//	return flag;
//}
//
//Command* Interpreter::interpretCommand(std::string commandStr, Messenger &response)
//{
//	bool flag=true;
//	int commandType;
//
//	string word;
//	Command* returnCommand;
//
//	flag=checkCommand(commandStr,commandType);
//
//
//
//	if(flag!=false){
//		switch (commandType){
//		case 0:
//			{
//				Command_Add* Add_pointer=new Command_Add();
//
//
//				stringstream extractName(commandStr);
//				string taskName;
//				getline(extractName,taskName,'`');
//				taskName.clear();
//				getline(extractName,taskName,'`');
//				Add_pointer->setName(taskName);
//
//				returnCommand=interpretAdd(Add_pointer, commandStr, response,flag);
//
//				break;
//			}
//
//		case 1:
//			{
//				Command_Mod* Mod_pointer=new Command_Mod();
//
//				stringstream extractName(commandStr);
//				string taskName;
//				getline(extractName,taskName,'`');
//				taskName.clear();
//				getline(extractName,taskName,'`');
//				Mod_pointer->setName(taskName);
//
//				returnCommand=interpretModify(Mod_pointer, commandStr, response,flag);
//				break;
//			}
//
//		case 2:
//			{
//				Command_Mod* Mod_pointer=new Command_Mod();
//
//				Mod_pointer->setFlagExact();
//
//				stringstream extractName(commandStr);
//				string taskName;
//				getline(extractName,taskName,'`');
//				taskName.clear();
//				getline(extractName,taskName,'`');
//				Mod_pointer->setName(taskName);
//
//
//
//				returnCommand=interpretModify(Mod_pointer, commandStr, response,flag);
//
//				break;
//			}
//
//		case 3:
//			{
//				Command_Mod* Mod_pointer=new Command_Mod();
//				string getIndex=commandStr;
//				stringstream extractIndex(getIndex);
//				string content;
//				extractIndex>>getIndex;
//				getIndex.clear();
//				extractIndex>>getIndex;
//				int index;
//
//				index=getIndexMessage(getIndex,flag);
//				Mod_pointer->setIndex(index);
//
//				returnCommand=interpretModify(Mod_pointer, commandStr, response,flag);
//				break;
//			}
//
//
//		case 4:
//			{
//
//				Command_Find* Find_pointer=new Command_Find();
//
//
//
//				returnCommand=interpretFind(Find_pointer,commandStr, response,flag);
//				break;
//			}
//
//		case 5:
//			{
//
//				Command_Find* Find_pointer=new Command_Find();
//
//				Find_pointer->setFlagExact();
//				returnCommand=interpretFind(Find_pointer,commandStr, response,flag);
//				break;
//			}
//
//		case 6:
//			{
//
//				Command_Del* Del_pointer=new Command_Del();
//				returnCommand=interpretDelete(Del_pointer,commandStr, response,flag);
//				break;
//
//			}
//		case 7:
//			{
//				Command_Del* Del_pointer=new Command_Del();
//
//				Del_pointer->setFlagExact();
//
//				returnCommand=interpretDelete(Del_pointer,commandStr, response,flag);
//				break;
//			}
//		case 8:
//			{
//				Command_Del* Del_pointer=new Command_Del();
//				stringstream extractIndex(commandStr);
//				string content;
//				extractIndex>>commandStr;
//				commandStr.clear();
//				extractIndex>>commandStr;
//				int index;
//
//				index=getIndexMessage(commandStr,flag);
//				Del_pointer->setIndex(index);
//
//
//				returnCommand=(Command*)Del_pointer;
//				break;
//
//
//			}
//		case 9:
//			{
//				Command_Undo* Undo_pointer=new Command_Undo();
//
//				returnCommand=interpretUndo(Undo_pointer,commandStr, response,flag);
//				break;
//			}
//		case 10:
//			{
//				Command_Redo* Redo_pointer=new Command_Redo();
//
//				returnCommand=interpretRedo(Redo_pointer,commandStr, response,flag);
//				break;
//
//
//			}
//		case 11:
//			{
//				Command_Sync *Sync_pointer=new Command_Sync();
//
//				returnCommand=interpretSync(Sync_pointer,commandStr, response,flag);
//
//				break;
//			}
//
//		default: flag=false;
//			break;
//		}
//
//	}
//	if(flag==false){
//
//		response.setStatus(ERROR);
//		response.setErrorMsg("invalid command");
//		return NULL;
//	}
//
//	else{
//
//		response.setStatus(SUCCESS);
//
//		return returnCommand;
//	}
//
//}
//
//Command* Interpreter:: interpretAdd(Command_Add* commandType, string commandStr,Messenger &response, bool& flag)
//{
//
//
//	if(flag && commandType->getFlagDue()==false){		
//
//		time_t content;
//		if(getDueDateMessage(commandStr,flag,content)){		
//			commandType->setDueDate(content);		
//		}		
//	}
//	else {
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagFrom() ==false){
//
//		time_t content;
//		if(getFromDateMessage(commandStr,flag,content)){
//			commandType->setFromDate(content);
//		}
//	}
//	else {
//		flag=false;
//
//	}
//
//	if(flag && commandType->getFlagTo() ==false){
//		time_t content;
//
//		if(getToDateMessage(commandStr,flag,content)){
//			commandType->setToDate(content);
//		}
//	}
//	else{
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagParticipants()==false){
//
//		list<string>content;
//		if(getParticipantsMessage(commandStr,flag,content)){
//			commandType->setParticipants(content);
//		}
//
//	}
//	else{
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagNote()==false){
//
//		string content;
//		if(getNoteMessage(commandStr,flag,content)){
//			commandType->setNote(content);
//		}
//	}
//	else{
//
//		flag=false;
//
//	}
//
//	if(flag && commandType->getFlagLocation()==false){
//		string content;
//		if(getLocationMessage(commandStr,flag,content)){
//			commandType->setLocation(content);
//		}
//	}
//	else{
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagRemindTimes()==false){
//		list<time_t>content;
//		if(getRemindTimesMessage(commandStr,flag,content)){
//			commandType->setRemindTimes(content);
//		}
//	}
//	else{
//
//		flag=false;
//
//	}
//
//	if(flag && commandType->getFlagPriority()==false){
//		PRIORITY content;
//		if(getPriorityMessage(commandStr,flag,content)){
//			commandType->setPriority(content);
//		}
//	}
//	else {
//		flag=false;
//
//	}
//
//	if(flag && commandType->getFlagTags()==false){
//		list<string>content;
//
//		if(getTagsMessage(commandStr,flag,content)){
//			commandType->setTags(content);
//		}
//	}
//	else {
//
//		flag=false;
//	}
//
//	if(flag==true){
//
//		response.setStatus(SUCCESS);
//		response.setCommandType(ADD);
//		return (Command*)commandType;
//	}
//
//	else return NULL;
//}
//
//
//
//Command* Interpreter::interpretModify(Command_Mod* commandType, std::string commandStr, Messenger &response, bool& flag){
//
//	if(flag && commandType->getFlagDue()==false){		
//
//		time_t content;
//		if(getDueDateMessage(commandStr,flag,content)){		
//			commandType->setDueDate(content);		
//		}		
//	}
//	else {
//
//		flag=false;
//	}
//
//
//	if(flag && commandType->getFlagParticipants()==false){
//
//		list<string>content;
//		if(getParticipantsMessage(commandStr,flag,content)){
//			commandType->setParticipants(content);
//		}
//
//	}
//	else{
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagNote()==false){
//
//		string content;
//		if(getNoteMessage(commandStr,flag,content)){
//			commandType->setNote(content);
//		}
//	}
//	else{
//
//		flag=false;
//
//	}
//
//	if(flag && commandType->getFlagLocation()==false){
//		string content;
//		if(getLocationMessage(commandStr,flag,content)){
//			commandType->setLocation(content);
//		}
//	}
//	else{
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagRemindTimes()==false){
//		list<time_t>content;
//		if(getRemindTimesMessage(commandStr,flag,content)){
//			commandType->setRemindTimes(content);
//		}
//	}
//	else{
//
//		flag=false;
//
//	}
//
//	if(flag && commandType->getFlagPriority()==false){
//		PRIORITY content;
//		if(getPriorityMessage(commandStr,flag,content)){
//			commandType->setPriority(content);
//		}
//	}
//	else {
//		flag=false;
//
//	}
//
//	if(flag && commandType->getFlagTags()==false){
//		list<string>content;
//
//		if(getTagsMessage(commandStr,flag,content)){
//			commandType->setTags(content);
//		}
//	}
//	else {
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagOptName()==false){
//
//		string content;
//		if(getNameMessage(commandStr,flag,content)){
//			commandType->setOptName(content);
//		}
//	}
//	else {
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagTaskState()==false){
//
//		TASK_STATE content;
//		if(getTaskStateMessage(commandStr,flag,content)){
//			commandType->setTaskState(content);
//		}
//	}
//	else {
//
//		flag=false;
//	}
//
//	if(flag==true){
//		response.setStatus(SUCCESS);
//		response.setCommandType(MOD);
//		return (Command*)commandType;
//	}
//	else return NULL;
//}
//
//
//
//Command* Interpreter::interpretFind(Command_Find* commandType, std::string commandStr, Messenger &response, bool& flag){
//
//
//	if(flag && commandType->getFlagFrom() ==false){
//
//		time_t content;
//		if(getFromDateMessage(commandStr,flag,content)){
//			commandType->setFromDate(content);
//		}
//	}
//	else {
//		flag=false;
//
//	}
//
//	if(flag && commandType->getFlagTo() ==false){
//		time_t content;
//
//		if(getToDateMessage(commandStr,flag,content)){
//			commandType->setToDate(content);
//		}
//	}
//	else{
//
//		flag=false;
//	}
//
//
//	if(flag && commandType->getFlagParticipants()==false){
//
//		list<string>content;
//		if(getParticipantsMessage(commandStr,flag,content)){
//			commandType->setParticipants(content);
//		}
//
//	}
//	else{
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagNote()==false){
//
//		string content;
//		if(getNoteMessage(commandStr,flag,content)){
//			commandType->setNote(content);
//		}
//	}
//	else{
//
//		flag=false;
//
//	}
//
//	if(flag && commandType->getFlagLocation()==false){
//		string content;
//		if(getLocationMessage(commandStr,flag,content)){
//			commandType->setLocation(content);
//		}
//	}
//	else{
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagPriority()==false){
//		PRIORITY content;
//		if(getPriorityMessage(commandStr,flag,content)){
//			commandType->setPriority(content);
//		}
//	}
//	else {
//		flag=false;
//
//	}
//
//	if(flag && commandType->getFlagTags()==false){
//		list<string>content;
//
//		if(getTagsMessage(commandStr,flag,content)){
//			commandType->setTags(content);
//		}
//	}
//	else {
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagOptName()==false){
//
//		string content;
//		if(getNameMessage(commandStr,flag,content)){
//			commandType->setOptName(content);
//		}
//	}
//	else {
//		flag=false;
//	}
//
//
//	if(flag && commandType->getFlagTaskState()==false){
//
//		TASK_STATE content;
//		if(getTaskStateMessage(commandStr,flag,content)){
//			commandType->setTaskState(content);
//		}
//	}
//	else {
//
//		flag=false;
//	}
//
//	if(flag && commandType->getFlagTaskType()==false){
//		TASK_TYPE content;
//		if(getTaskTypeMessage(commandStr,flag,content)){
//			commandType->setTaskType(content);
//		}
//	}
//	else{
//
//		flag=false;
//
//	}
//
//	if(flag==true){
//		response.setStatus(SUCCESS);
//		response.setCommandType(FIND);
//		return (Command*)commandType;
//	}
//	else return NULL;
//}
//
//
//Command* Interpreter:: interpretDelete(Command_Del*commandType,std::string commandStr, Messenger &response, bool &flag){
//
//	stringstream extractContent(commandStr);
//	string content;
//	getline(extractContent,content,'`');
//	content.clear();
//	getline(extractContent,content,'`');
//	commandType->setName(content);
//
//
//	response.setCommandType(DEL);
//	response.setStatus(SUCCESS);
//	return (Command*)commandType;
//}
//Command* Interpreter:: interpretRedo(Command_Redo*commandType,std::string commandStr, Messenger &response, bool &flag){
//
//	response.setCommandType(REDO);
//	response.setStatus(SUCCESS);
//	return (Command*)commandType;
//}
//
//Command* Interpreter:: interpretUndo(Command_Undo*commandType,std::string commandStr, Messenger &response, bool &flag){
//
//	response.setCommandType(UNDO);
//	response.setStatus(SUCCESS);
//	return (Command*)commandType;
//}
//
//Command* Interpreter:: interpretSync(Command_Sync*commandType,std::string commandStr, Messenger &response, bool &flag){
//
//	string content;
//	if(getSyncProviderNameMessage(commandStr,flag,content)){
//		commandType->setSyncProviderName(content);
//	}
//	response.setCommandType(SYNC);
//	response.setStatus(SUCCESS);
//	return (Command*)commandType;
//}
//
//
//
//int Interpreter:: getIndexMessage(string command,bool& flag){
//
//	int num;
//	flag=integerConverter(command, num);
//	return num;
//}	
//
//
//bool Interpreter::checkKeyWord(string command, int position){
//
//	vector<int> positionForNotion; 
//	positionForNotion.push_back(-1); // dummy value;
//	bool isKeyWord=true;
//
//
//	for(int i=0;i<command.length();i++)
//	{
//		if(command.at(i)=='`'){
//
//			positionForNotion.push_back(i);
//		}
//
//	}
//	int count=1;
//
//	while(isKeyWord &&count<(int)positionForNotion.size()){
//
//		if(position>positionForNotion[count] && position<positionForNotion[count+1]){
//
//			isKeyWord=false;
//
//		}
//		count=count+2;
//	}
//
//	return isKeyWord;
//} 
//
//bool Interpreter::checkDuplicate(string command, string cmdTemplate,int startPosition){
//
//	string subString=command.substr(startPosition+1);
//	smatch match;
//	regex extractTemplete(cmdTemplate);
//	string commandStr;
//	bool isDuplicate=false;
//
//	if (regex_search(subString, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(commandStr.length()>0){
//
//		isDuplicate=true;
//	}
//
//	return isDuplicate;
//}
//
//bool Interpreter:: getNameMessage(string command, bool&flag, string& content){ 
//
//	regex extractTemplete(" name `[^`]*`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		preContent.clear();
//		getline(extract,preContent,'`');
//
//		content=preContent;
//
//		if(checkDuplicate(command," name `[^`]*`",match.position())==true){
//
//			flag=false;
//		}
//
//	}
//	else{
//
//		isNotEmpty=false;
//
//	}
//
//
//	return isNotEmpty;
//
//}
//bool Interpreter::getOptNameMessage(string command, bool&flag,string& content){ 
//
//	regex extractTemplete(" name `[^`]*`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		preContent.clear();
//		getline(extract,preContent,'`');
//
//		content=preContent;
//
//		if(checkDuplicate(command," name `[^`]*`",match.position())==true){
//
//			flag=false;
//		}
//
//	}
//	else{
//
//		isNotEmpty=false;
//
//	}
//
//
//	return isNotEmpty;
//}
//
//bool Interpreter::getDueDateMessage(string command, bool&flag, time_t& content){
//
//	regex extractTemplete(" due `[^`]*`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		preContent.clear();
//		getline(extract,preContent,'`');
//
//		content=setTime(preContent,flag);
//
//		//convertToTime(preContent,content,flag);
//
//		if(checkDuplicate(command," due `[^`]*`",match.position())==true){
//
//			flag=false;
//		}
//		// check from
//		commandStr.clear();
//		regex checkFrom("from `[^`]*`");
//		if (regex_search(command, match,checkFrom)){
//
//			commandStr=match[0];
//
//		}
//		if(!commandStr.empty()) flag=false;
//
//		// check to
//
//		commandStr.clear();
//		regex checkTo("to `[^`]*`");
//		if (regex_search(command, match,checkFrom)){
//
//			commandStr=match[0];
//
//		}
//		if(!commandStr.empty()) flag=false;
//
//
//	}
//	else{
//
//		isNotEmpty=false;
//
//	}
//
//	return isNotEmpty;
//}
//
//bool	Interpreter::getFromDateMessage(string command, bool&flag, time_t& content){
//
//	regex extractTemplete(" from `[^`]*`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		preContent.clear();
//		getline(extract,preContent,'`');
//
//		content=setTime(preContent,flag);
//
//		if(checkDuplicate(command," from `[^`]*`",match.position())==true){
//
//			flag=false;
//		}		
//
//
//		regex checkDue(" due `[^`]*`");
//		commandStr.clear();
//		if (regex_search(command, match, checkDue)){
//
//			commandStr=match[0];
//
//		}
//		if(!commandStr.empty())flag=false;
//
//
//
//	}
//	else{
//
//		isNotEmpty=false;
//
//	}
//
//
//	return isNotEmpty;
//
//}
//
//bool Interpreter::getToDateMessage(string command, bool&flag, time_t& content){
//
//	regex extractTemplete(" to `[^`]*`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		preContent.clear();
//		getline(extract,preContent,'`');
//
//		content=setTime(preContent,flag);
//
//		if(checkDuplicate(command," to `[^`]*`",match.position())==true){
//
//			flag=false;
//		}
//
//
//		regex checkDue(" due `[^`]*`");
//		commandStr.clear();
//		if (regex_search(command, match, checkDue)){
//
//			commandStr=match[0];
//
//		}
//		if(!commandStr.empty())flag=false;
//
//
//	}
//	else{
//
//		isNotEmpty=false;
//
//	}
//
//
//	return isNotEmpty;
//}
//
//
//bool Interpreter::getLocationMessage(string command, bool&flag, string& content){
//
//	regex extractTemplete(" at `[^`]*`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		preContent.clear();
//		getline(extract,preContent,'`');
//
//		content=preContent;
//
//		if(checkDuplicate(command," at `[^`]*`",match.position())==true){
//
//			flag=false;
//		}		
//
//	}
//	else{
//
//		isNotEmpty=false;
//
//	}
//
//
//	return isNotEmpty;
//
//}
//
//bool Interpreter::getParticipantsMessage(string command, bool&flag, list<std::string>& content){
//	list<string>pplList;
//	regex extractTemplete(" ppl `[^`]*`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		content.clear();
//		getline(extract,preContent,'`');
//
//		stringstream extractIndividual(preContent);
//		string name="";
//		getline(extractIndividual,name,',');
//		while(!name.empty()){
//
//			pplList.push_back(name);
//			name.clear();
//			getline(extractIndividual,name,',');
//		}
//
//		content=pplList;
//
//
//		if(checkDuplicate(command," ppl `[^`]*`",match.position())==true){
//
//			flag=false;
//		}	
//
//	}
//	else{
//
//		isNotEmpty=false;
//	}
//
//
//	return isNotEmpty;
//
//}	
//
//bool	Interpreter::getNoteMessage(string command, bool&flag,string& content){
//
//	regex extractTemplete(" note `[^`]*`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		preContent.clear();
//		getline(extract,preContent,'`');
//
//		content=preContent;
//
//		if(checkDuplicate(command," note `[^`]*`",match.position())==true){
//
//			flag=false;
//		}	
//
//	}
//	else{
//
//		isNotEmpty=false;
//
//	}
//
//
//	return isNotEmpty;
//
//
//}
//bool Interpreter::getPriorityMessage(string command, bool&flag,TP::PRIORITY& content){ // need to force type
//	regex extractTemplete(" impt `[^`]*`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	PRIORITY priority=MEDIUM;
//	bool isNotEmpty=true;
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		preContent.clear();
//		getline(extract,preContent,'`');
//
//		if(preContent=="H" ||preContent=="HIGH"){
//			priority=HIGH;
//
//		}
//		else if(preContent=="M" || preContent=="MEDIUM"){
//			priority=MEDIUM;
//
//		}
//		else if(preContent=="L"||preContent=="LOW"){
//
//			priority=LOW;
//
//		}
//		else if(preContent.empty()){
//
//			priority=MEDIUM;
//		}
//		else {
//			flag=false;
//		}
//		content=priority;
//
//		if(checkDuplicate(command," impt `[^`]*`",match.position())==true){
//
//			flag=false;
//		}
//
//	}
//	else {
//
//		isNotEmpty=false;
//	}
//	return isNotEmpty;
//}
//
//bool Interpreter::getTagsMessage(string command, bool&flag,list<std::string>& content){
//
//	list<string>tagList;
//	regex extractTemplete("\\s(#[^( |`)]*)(\\s|$)");
//
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//	string subStirng=command;
//
//	int count=0;
//
//	if (regex_search(subStirng, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//
//	while(!commandStr.empty()){
//
//		if(checkKeyWord(subStirng,match.position())==true){
//
//			stringstream extract(commandStr);
//			string tagContent;
//			getline(extract,tagContent,'#');
//			tagContent.clear();
//			getline(extract,tagContent,' ');
//			tagList.push_back(tagContent);
//			count++;
//		}	
//
//
//		subStirng=subStirng.substr(match.position()+1);
//		commandStr.clear();
//
//		if (regex_search(subStirng, match, extractTemplete)){
//
//			commandStr=match[0];
//
//		}
//
//	}
//
//	if(count==0){
//		isNotEmpty=false;
//	}
//
//	content=tagList;
//	return isNotEmpty;
//
//}	
//
//bool Interpreter::	getRemindTimesMessage(string command, bool&flag,list<std::time_t>&content){
//
//	list<time_t>rtList;
//	regex extractTemplete(" rt `[^`]*`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		preContent.clear();
//		getline(extract,preContent,'`');
//
//		stringstream extractIndividual(preContent);
//		string time="";
//		getline(extractIndividual,time,',');
//
//		while(!time.empty()){
//			time_t rtTime=setTime(time,flag);
//			rtList.push_back(rtTime);
//			time.clear();
//			getline(extractIndividual,time,',');
//		}
//		content=rtList;
//
//		if(checkDuplicate(command," rt `[^`]*`",match.position())==true){
//
//			flag=false;
//		}
//	}
//	else{
//
//		isNotEmpty=false;
//	}
//
//
//
//	return isNotEmpty;
//}
//
//bool Interpreter::getTaskStateMessage(string command, bool&flag, TP::TASK_STATE& content){ 
//
//	TASK_STATE task_state;
//	string commandStr;
//	smatch match;
//	bool isNotEmpty=true;
//	regex extractTemplete("\\s(done|undone)(\\s|$)");
//	string subStirng=command;
//
//	int startIndex=0;
//	string::const_iterator startPos = command.begin();
//	string::const_iterator endPos = command.end();
//
//	vector<string>result;
//	int count=0;
//	string test;
//	if (regex_search(subStirng, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//
//	while(!commandStr.empty()){
//
//		if(checkKeyWord(command,startIndex+match.position())==true){
//
//			result.push_back(commandStr);
//			count++;
//		}	
//
//		startIndex=startIndex+match.position()+1;
//		startPos=startPos+match.position()+1;
//
//		commandStr.clear();
//		if(startPos!=endPos){
//			if (regex_search(startPos,endPos, match, extractTemplete)){
//
//				commandStr=match[0];
//
//			}
//		}
//	}
//
//	if(count==0){
//		isNotEmpty=false;
//	}
//	else if(count==1){
//		if(result.at(0).find("undone")!=string::npos){
//
//			content=UNDONE;
//		}
//
//		else {
//			content=DONE;	
//
//		}
//	}
//
//	else{
//
//		flag=false;
//	}
//
//	return isNotEmpty;
//}
//
//bool Interpreter::getTaskTypeMessage(string command, bool&flag, TP::TASK_TYPE& content){
//
//	TASK_TYPE task_type;
//	string commandStr;
//	smatch match;
//	regex extractTemplete("\\s(timed|deadline|floating)(\\s|$)");
//	string subStirng=command;
//
//	string::const_iterator startPos = command.begin();
//	string::const_iterator endPos = command.end();
//
//	int startIndex=0;
//	vector<string>result;
//	int count=0;
//	bool isNotEmpty=true;
//
//
//	if (regex_search(startPos,endPos, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//
//	while(!commandStr.empty()){
//
//		if(checkKeyWord(command,startIndex+match.position())==true){
//
//			result.push_back(commandStr);
//			count++;
//		}	
//
//		startIndex=startIndex+match.position()+1;
//		startPos=startPos+match.position()+1;
//
//		commandStr.clear();
//		if(startPos!=endPos){
//			if (regex_search(startPos,endPos, match, extractTemplete)){
//
//				commandStr=match[0];
//
//			}
//		}
//	}
//
//	if(count==0){
//		isNotEmpty=false;
//	}
//	else if(count==1){
//		if(result.at(0).find("timed")!=string::npos){
//
//			content=TIMED;
//		}
//		else if(result.at(0).find("deadline")!=string::npos){
//
//			content=DEADLINE;
//
//		}
//		else {
//			content=FLOATING;
//
//
//		}
//	}
//	else{
//
//		flag=false;
//	}
//
//	return isNotEmpty;
//}
//
//bool Interpreter::getSyncProviderNameMessage(string command, bool&flag, string&content){
//
//	regex extractTemplete("sync `[^`]+`");
//	smatch match;
//	string commandStr;
//	string preContent;
//	bool isNotEmpty=true;
//
//	if (regex_search(command, match, extractTemplete)){
//
//		commandStr=match[0];
//
//	}
//
//	if(!commandStr.empty()){
//
//		stringstream extract(commandStr);
//		getline(extract,preContent,'`');
//		content.clear();
//		getline(extract,preContent,'`');
//
//		content=preContent;
//
//	}
//	else {
//
//		isNotEmpty=false;
//	}
//	return isNotEmpty;
//}
///*
//time_t Interpreter::setTime(string commandStr,bool& _isSuccess)
//{
//int year=-1,month=-1,day=-1,hour=-1,min=-1;
//time_t rawtime;
//string inputInfo=commandStr;
//struct tm  timeinfo={0,0,0,0,0,0};
//
//time (&rawtime);
//
//localtime_s (&timeinfo,&rawtime);
//
//
////	string timeTemplete="dd/mo/yy hh:mm";
//
//if(inputInfo.length()>1){
//
//_isSuccess=integerConverter(inputInfo.substr(0,2),day);
//
//if(_isSuccess!=false && inputInfo.length()>4 ){
//
//if(inputInfo.at(2)=='/'){
//_isSuccess=integerConverter(inputInfo.substr(3,2),month);		
//
//}
//else _isSuccess=false;
//}	
//
//if(_isSuccess!=false && inputInfo.length()>7){
//if(inputInfo.at(5)=='/'){
//_isSuccess=integerConverter(inputInfo.substr(6,2),year);
//year=year+2000;
//}
//else _isSuccess=false;
//}
//
//if(_isSuccess!=false && inputInfo.length()>10){
//
//if(inputInfo.at(8)==' '){
//_isSuccess=integerConverter(inputInfo.substr(9,2),hour);
//}
//else _isSuccess=false;
//
//}
//
//if(_isSuccess!=false && inputInfo.length()>13){
//
//if(inputInfo.at(11)==':'){
//_isSuccess=integerConverter(inputInfo.substr(12),min);
//}
//else _isSuccess=false;
//}
//
//if(_isSuccess!=false){
//
//if(year==-1)year=timeinfo.tm_year+1900;
//if(month==-1)month=timeinfo.tm_mon+1;
//if(day==-1)day=timeinfo.tm_mday+1;
//if(hour==-1)hour=timeinfo.tm_hour;
//if(min==-1)min=timeinfo.tm_min;
//}
//
//if(_isSuccess!=false){
//struct tm  timeMessage={0,0,0,0,0,0};
//
//timeMessage.tm_year=year-1900;
//timeMessage.tm_mon=month-1;
//timeMessage.tm_mday=day;
//timeMessage.tm_hour=hour;
//timeMessage.tm_min=min;
//
//return mktime(&timeMessage);
//}
//
//
//}
//
//return mktime(&timeinfo);
//}
//
//*/
//time_t Interpreter::setTime(string commandStr,bool& flag){
//	int year=-1,month=-1,day=-1,hour=-1,min=-1;
//	time_t rawtime;
//	string inputInfo=commandStr;
//	struct tm  timeinfo={0,0,0,0,0,0};
//
//	time (&rawtime);
//
//	localtime_s (&timeinfo,&rawtime);
//
//	int countSlash=0;
//	for(int i=0;i<commandStr.length();i++){
//
//		if(commandStr.at(i)=='/'){
//
//			countSlash++;
//		}
//
//	}
//	string content;
//	switch(countSlash){
//
//	case 0: 
//	{	
//		stringstream extract(commandStr);
//		getline(extract,content,' ');
//		flag=integerConverter(content,day);
//		content.clear();
//		getline(extract,content,':');
//		if(!content.empty()){
//
//			flag=integerConverter(content,hour);
//		}
//		content.clear();
//		getline(extract,content);
//		if(!content.empty()){
//
//			flag=integerConverter(content,min);
//		}
//		break;
//	}
//	case 1:
//	{	
//		stringstream extract(commandStr);
//		content.clear();
//		getline(extract,content,'/');  
//		flag=integerConverter(content,day);
//		content.clear();
//		getline(extract,content,' '); 
//		flag=integerConverter(content,month);
//		
//		content.clear();
//		getline(extract,content,':');
//		if(!content.empty()){
//
//			flag=integerConverter(content,hour);
//		}
//		content.clear();
//		getline(extract,content);
//		if(!content.empty()){
//
//			flag=integerConverter(content,min);
//		}
//
//		break;
//	}
//	case 2:
//	{	
//		stringstream extract(commandStr);
//		content.clear();
//		getline(extract,content,'/');  
//		flag=integerConverter(content,day);
//		content.clear();
//		getline(extract,content,'/'); 
//		flag=integerConverter(content,month);
//		content.clear();
//		getline(extract,content,' '); 
//		flag=integerConverter(content,year);
//		if(year<100)year=year+2000;
//		content.clear();
//		getline(extract,content,':');
//		if(!content.empty()){
//
//			flag=integerConverter(content,hour);
//		}
//		content.clear();
//		getline(extract,content);
//		if(!content.empty()){
//
//			flag=integerConverter(content,min);
//		}
//		 break;
//	}
//	default:
//		break;
//	}
//if(flag!=false){
//
//if(year==-1)year=timeinfo.tm_year+1900;
//if(month==-1)month=timeinfo.tm_mon+1;
//if(day==-1)day=timeinfo.tm_mday+1;
//if(hour==-1)hour=12;
//if(min==-1)min=0;
//}
//
//struct tm  timeMessage={0,0,0,0,0,0};
//
//timeMessage.tm_year=year-1900;
//timeMessage.tm_mon=month-1;
//timeMessage.tm_mday=day;
//timeMessage.tm_hour=hour;
//timeMessage.tm_min=min;
//
//return mktime(&timeMessage);
//
//
//}
//
//
//
//
//bool Interpreter::integerConverter(string& requiredString, int& number)
//{
//
//	bool flag=true;
//
//	if(requiredString.empty()==true){
//		flag=false;
//	}
//	else{
//		for(unsigned i=0;i<requiredString.length();i++){
//			if(isdigit(requiredString[i])==false){
//				flag=false;
//			}
//		}
//	}
//	number=atoi(requiredString.c_str());
//
//	return flag;
//
//}
//
//

/*
 * =====================================================================================
 *
 *       Filename:  Interpreter.cpp
 *
 *        Version:  1.0
 *        Created:  16/10/13 18:13:09
 *
 *         Author:  XIE KAI (A0102016E), gigikie@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Interpreter.h"
#include "Enum.h"
#include <ctime>
#include <iostream>
#include <sstream>

using namespace std;
using namespace TP;

const string Interpreter::EMPTY = "";
const string Interpreter::SHARP = "#";
const string Interpreter::SPACE = " ";
const time_t Interpreter::DEFAULT_TIME = 0;
const string Interpreter::STR_DONE = "DONE";
const string Interpreter::STR_UNDONE = "UNDONE";
const string Interpreter::STR_TIMED = "TIMED";
const string Interpreter::STR_DEADLINE = "DEADLINE";
const string Interpreter::STR_FLOATING = "FLOATING";
const string Interpreter::STR_H = "H";
const string Interpreter::STR_M = "M";
const string Interpreter::STR_L = "L";
const string Interpreter::STR_HIGH = "HIGH";
const string Interpreter::STR_MEDIUM = "MEDIUM";
const string Interpreter::STR_LOW = "LOW";
const string Interpreter::ERROR_WRONG_COMMAND = "Undefined command";
const string Interpreter::ERROR_WRONG_FORMAT = "Invalid format";
const string Interpreter::ERROR_WRONG_TIME_FORMAT = "Invalid time format";
const string Interpreter::CMD_ADD = "ADD";
const string Interpreter::CMD_MOD = "MOD";
const string Interpreter::CMD_DEL = "DEL";
const string Interpreter::CMD_FIND = "FIND";
const string Interpreter::CMD_UNDO = "UNDO";
const string Interpreter::CMD_REDO = "REDO";
const string Interpreter::CMD_SYNC = "SYNC";
const string Interpreter::DELIMITER = "`";
const char Interpreter::DELIMITER_COMMA = ',';
const string Interpreter::DELIMITER_PAIR = "`[^`]*`";
const string Interpreter::FORMAT_INTEGER = "\\d{1,8}";
const char* Interpreter::FORMAT_DD_MM = "%d/%d";
const char* Interpreter::FORMAT_DD_MM_HOUR_MIN = "%d/%d %d:%d";
const char* Interpreter::FORMAT_DD_MM_YY = "%d/%d/%d";
const char* Interpreter::FORMAT_DD_MM_YY_HOUR_MIN = "%d/%d/%d %d:%d";
const string Interpreter::REGEXP_DATE_DD_MM = "((0[1-9])|(1[0-9])|(2[0-9])|(3[0-1])|([1-9]))/((0[1-9])|(1[0-2])|([1-9]))";
const string Interpreter::REGEXP_DATE_DD_MM_HOUR_MIN = "((0[1-9])|(1[0-9])|(2[0-9])|(3[0-1])|([1-9]))/((0[1-9])|(1[0-2])|([1-9])) ((0[0-9])|(1[0-9])|(2[0-3])|([0-9])):((0[0-9])|(1[0-9])|(2[0-9])|(3[0-9])|(4[0-9])|(5[0-9])|([0-9]))";
const string Interpreter::REGEXP_DATE_DD_MM_YY = "((0[1-9])|(1[0-9])|(2[0-9])|(3[0-1])|([1-9]))/((0[1-9])|(1[0-2])|([1-9]))/\\d+";
const string Interpreter::REGEXP_DATE_DD_MM_YY_HOUR_MIN = "((0[1-9])|(1[0-9])|(2[0-9])|(3[0-1])|([1-9]))/((0[1-9])|(1[0-2])|([1-9]))/\\d+ ((0[0-9])|(1[0-9])|(2[0-3])|([0-9])):((0[0-9])|(1[0-9])|(2[0-9])|(3[0-9])|(4[0-9])|(5[0-9])|([0-9]))";
const string Interpreter::REGEXP_VALUE_INDEX = "(\\s+)[0-9]+";
const string Interpreter::REGEXP_VALUE_EXACT = "(\\s+)exact";
const string Interpreter::REGEXP_VALUE_NAME = "(^add(\\s+)`([^`]+)`)|(^mod((\\s+)exact|)(\\s+)`([^`]+)`)|(^del((\\s+)exact|)(\\s+)`([^`]+)`)";
const string Interpreter::REGEXP_VALUE_OPT_NAME = "(\\s+)name(\\s+)`[^`]*`";
const string Interpreter::REGEXP_VALUE_DUE_DATE = "(\\s+)due(\\s+)`[^`]*`";
const string Interpreter::REGEXP_VALUE_FROM_DATE = "(\\s+)from(\\s+)`[^`]*`";
const string Interpreter::REGEXP_VALUE_TO_DATE = "(\\s+)to(\\s+)`[^`]*`";
const string Interpreter::REGEXP_VALUE_LOCATION = "(\\s+)at(\\s+)`[^`]*`";
const string Interpreter::REGEXP_VALUE_PARTICIPANTS = "(\\s+)ppl(\\s+)`[^`]*`";
const string Interpreter::REGEXP_VALUE_NOTE = "(\\s+)note(\\s+)`[^`]*`";
const string Interpreter::REGEXP_VALUE_PRIORITY = "(\\s+)impt(\\s+)`[^`]*`";
const string Interpreter::REGEXP_VALUE_TAGS = "(\\s+)#[^ `]*";
const string Interpreter::REGEXP_VALUE_REMIND_TIME = "(\\s+)rt(\\s+)`[^`]*`";
const string Interpreter::REGEXP_VALUE_SYNC = "^sync(\\s+)`([^`]+)`";
const string Interpreter::REGEXP_VALUE_TASK_STATE = "(\\s+)(done|undone)";
const string Interpreter::REGEXP_VALUE_TASK_TYPE = "(\\s+)(timed|deadline|floating)";
const string Interpreter::REGEXP_CMD = "^\\w+";
const string Interpreter::REGEXP_CMD_ADD = "^add(\\s+)`([^`]+)`(((\\s+)(due|from|to|impt|at|ppl|note|rt)(\\s+)`[^`]*`)|((\\s+)#[^ `]*))*(\\s*)";
const string Interpreter::REGEXP_CMD_MOD = "^mod(((\\s+)exact|)(\\s+)`([^`]+)`|(\\s+)[0-9]+)(((\\s+)(name|due|from|to|impt|at|ppl|note|rt)(\\s+)`[^`]*`)|((\\s+)done|(\\s+)undone)|((\\s+)#[^ `]*))*(\\s*)";
const string Interpreter::REGEXP_CMD_DEL = "^del(((\\s+)exact|)(\\s+)`([^`]+)`|(\\s+)[0-9]+)(\\s*)";
const string Interpreter::REGEXP_CMD_FIND = "^find((\\s+)exact|)(((\\s+)(name|from|to|impt|at|ppl|note)(\\s+)`[^`]*`)|((\\s+)#[^ `]*)|(((\\s+)done|(\\s+)undone))|(((\\s+)timed|(\\s+)deadline|(\\s+)floating)))*(\\s*)";
const string Interpreter::REGEXP_CMD_UNDO = "^undo(\\s*)";
const string Interpreter::REGEXP_CMD_REDO = "^redo(\\s*)";
const string Interpreter::REGEXP_CMD_SYNC = "^sync(\\s+)`([^`]+)`(\\s*)";

/**************
* Public APIs *
***************/

Command* Interpreter::interpretCommand(string commandStr, Messenger &response){
	string commandType_Str;
	Command* result = NULL;
	_commandStr = commandStr;
	try{
		buildDelimiterPairs();
		if(getCommand(commandType_Str)){
			if(commandType_Str == CMD_ADD)
				result = handleCmdAdd(response);
			else if(commandType_Str == CMD_MOD)
				result = handleCmdMod(response);
			else if(commandType_Str == CMD_DEL)
				result = handleCmdDel(response);
			else if(commandType_Str == CMD_FIND)
				result = handleCmdFind(response);
			else if(commandType_Str == CMD_UNDO)
				result = handleCmdUndo(response);
			else if(commandType_Str == CMD_REDO)
				result = handleCmdRedo(response);
			else if(commandType_Str == CMD_SYNC)
				result = handleCmdSync(response);
		}
	}
	catch(string exceptionStr){
		if(garbagePtr != NULL){
			delete garbagePtr;
			garbagePtr = NULL;
		}
		setError(response, exceptionStr);
	}
	return result;
}

int Interpreter::interpretIndex(string indexStr, Messenger &response){
	int result;
	if(integerConverter(indexStr, result))
	{
		response.setStatus(SUCCESS);
	}
	else{
		response.setStatus(ERROR);
		response.setErrorMsg(ERROR_WRONG_COMMAND);
	}
	return result;
}

/*****************
* Format Handler *
******************/

void Interpreter::buildDelimiterPairs() throw(string) {
	string searchStr = _commandStr;
	string::const_iterator startPos = searchStr.begin();
	string::const_iterator endPos = searchStr.end();
	smatch result_match;
	regex pattern(DELIMITER_PAIR);
	int lastTimeEndPos = 0;
	_delimiterPairs.clear();

	while(regex_search(startPos, endPos, result_match, pattern)){
		pair<int, int> delimPair;

		int delimPos = result_match.position() + lastTimeEndPos;
		delimPair.first = delimPos;
		delimPair.second = delimPos + result_match.length() - 1;
		lastTimeEndPos = delimPos + result_match.length();
		_delimiterPairs.push_back(delimPair);
		if(searchStr.length() > lastTimeEndPos)
			startPos = searchStr.begin() + lastTimeEndPos;
		else
			break;
	}
	if(searchStr.length() != lastTimeEndPos && regex_search(startPos, endPos, result_match, regex(DELIMITER)))
		throw ERROR_WRONG_FORMAT;
}

bool Interpreter::getCommand(string &result) throw(string){
	bool isSuccessful;
	smatch result_match;
	regex pattern(REGEXP_CMD);

	isSuccessful = regex_search(_commandStr, result_match, pattern);
	if(isSuccessful)
		result = toUpper(result_match.str());
	else
		throw ERROR_WRONG_COMMAND;
	return isSuccessful;
}

void Interpreter::validateFormat(TP::COMMAND_TYPE cmdType) throw(string){
	bool isSuccessful;
	switch (cmdType)
	{
	case ADD:
		isSuccessful = regex_match(_commandStr, regex(REGEXP_CMD_ADD));
		break;
	case MOD:
		isSuccessful = regex_match(_commandStr, regex(REGEXP_CMD_MOD));
		break;
	case DEL:
		isSuccessful = regex_match(_commandStr, regex(REGEXP_CMD_DEL));
		break;
	case FIND:
		isSuccessful = regex_match(_commandStr, regex(REGEXP_CMD_FIND));
		break;
	case UNDO:
		isSuccessful = regex_match(_commandStr, regex(REGEXP_CMD_UNDO));
		break;
	case REDO:
		isSuccessful = regex_match(_commandStr, regex(REGEXP_CMD_REDO));
		break;
	case SYNC:
		isSuccessful = regex_match(_commandStr, regex(REGEXP_CMD_SYNC));
		break;
	}
	if(!isSuccessful)
		throw ERROR_WRONG_FORMAT;
}

/******************
* Command Handler *
*******************/

Command* Interpreter::handleCmdAdd(Messenger &response) throw(string){
	return handleCmdCommon<Command_Add>(response);
}

Command* Interpreter::handleCmdMod(Messenger &response) throw(string){
	return handleCmdCommon<Command_Mod>(response);
}
Command* Interpreter::handleCmdDel(Messenger &response) throw(string){
	return handleCmdCommon<Command_Del>(response);
}
Command* Interpreter::handleCmdFind(Messenger &response) throw(string){
	return handleCmdCommon<Command_Find>(response);
}
Command* Interpreter::handleCmdUndo(Messenger &response) throw(string){
	return handleCmdSimple<Command_Undo>(response);
}
Command* Interpreter::handleCmdRedo(Messenger &response) throw(string){
	return handleCmdSimple<Command_Redo>(response);
}
Command* Interpreter::handleCmdSync(Messenger &response) throw(string){
	return handleCmdCommon<Command_Sync>(response);
}

void Interpreter::setSuccess(Messenger &response, COMMAND_TYPE ct){
	response.setCommandType(ct);
	response.setStatus(SUCCESS);
}

void Interpreter::setError(Messenger &response, string errorMsg){
	response.setStatus(ERROR);
	response.setErrorMsg(errorMsg);
}

/************
* Extractor *
*************/

bool Interpreter::extractKeyValuePair(regex pattern, string &result) throw(string) {
	bool isSuccessful;
	smatch result_match;
	int valueStartPos;
	int valueEndPos;
	int valueLength;

	isSuccessful = regex_search(_commandStr, result_match, pattern);
	if(isSuccessful){
		string result_tmp = result_match.str();
		valueStartPos = result_tmp.find(DELIMITER) + 1;
		valueEndPos = result_tmp.length() - 1;
		valueLength = valueEndPos - valueStartPos;
		result = result_tmp.substr(valueStartPos, valueLength);
		checkDuplicant(result_match.suffix().str(), pattern);
		//if have DUE DATE, FROM DATE & TO DATE are not allowed
		if(regex_match(result_tmp, regex(REGEXP_VALUE_DUE_DATE))){
			checkDuplicant(result_match.suffix().str(), regex(REGEXP_VALUE_FROM_DATE));
			checkDuplicant(result_match.suffix().str(), regex(REGEXP_VALUE_TO_DATE));
		}
		//if have FROM DATE or TO DATE, DUE DATE is not allowed
		else if(regex_match(result_tmp, regex(REGEXP_VALUE_FROM_DATE)) ||
			regex_match(result_tmp, regex(REGEXP_VALUE_TO_DATE))){
			checkDuplicant(result_match.suffix().str(), regex(REGEXP_VALUE_DUE_DATE));
		}
	}
	else
		result = EMPTY;

	return isSuccessful;
}

bool Interpreter::extractValue(regex pattern, string &result, bool needAppend) throw(string) {
	bool isSuccessful = false;
	string searchStr = _commandStr;
	string::const_iterator startPos = searchStr.begin();
	string::const_iterator endPos = searchStr.end();
	smatch result_match;
	int index;
	int lastTimeEndPos = 0;

	while(regex_search(startPos, endPos, result_match, pattern)){
		index = result_match.position() + lastTimeEndPos;
		lastTimeEndPos = index + result_match.length();

		if(!isWithinDelimiterPair(index)){
			isSuccessful = true;
			if(!needAppend){
				result = result_match.str();
				break;
			}
			else{
				result += DELIMITER_COMMA + result_match.str();
			}
		}
		if(searchStr.length() > lastTimeEndPos)
			startPos = searchStr.begin() + lastTimeEndPos;
		else
			break;
	}
	if(isSuccessful && !needAppend){
		checkDuplicant(lastTimeEndPos, pattern);
	}
	return isSuccessful;
}

void Interpreter::checkDuplicant(string searchStr, regex pattern) throw(string) {
	if(regex_search(searchStr, pattern))
		throw ERROR_WRONG_FORMAT;
}

void Interpreter::checkDuplicant(int lastTimeEndPos, regex pattern) throw(string) {
	string searchStr = _commandStr;
	string::const_iterator startPos;
	string::const_iterator endPos = searchStr.end();
	smatch result_match;
	int index;

	if(searchStr.length() > lastTimeEndPos){
		startPos = searchStr.begin() + lastTimeEndPos;
		while(regex_search(startPos, endPos, result_match, pattern)){
			index = result_match.position() + lastTimeEndPos;
			lastTimeEndPos = index + result_match.length();

			if(!isWithinDelimiterPair(index)){
				throw ERROR_WRONG_FORMAT;
			}
			if(searchStr.length() > lastTimeEndPos)
				startPos = searchStr.begin() + lastTimeEndPos;
			else
				break;
		}
	}
}

bool Interpreter::isWithinDelimiterPair(int index){
	bool result = false;
	for(int i = 0; i < _delimiterPairs.size(); i++){
		pair<int, int> delimPair = _delimiterPairs[i];
		if(delimPair.first < index && index < delimPair.second){
			result = true;
			break;
		}
	}
	return result;
}

/************
* Converter *
*************/

bool Interpreter::integerConverter(string& requiredString, int& number)
{
	bool isSuccessful;
	requiredString = trim(requiredString);

	if(requiredString.empty()){
		isSuccessful = false;
	}
	else{
		isSuccessful = regex_match(requiredString, regex(FORMAT_INTEGER));
	}
	number = isSuccessful? atoi(requiredString.c_str()): -1;
	return isSuccessful;
}

//CAN BE SHORTER
void Interpreter::convertToTime(string timeStr, time_t &result) throw(string){
	bool FLAG_empty = false;
	struct tm time = {DEFAULT_TIME};
	int day, month, year, hour, min;
	day = month = year = hour = min = DEFAULT_TIME;
	time_t currentTime;
	struct tm* currentTimeInfo;
	::time(&currentTime);
	currentTimeInfo = localtime(&currentTime);

	timeStr = trim(timeStr);
	if(regex_match(timeStr, regex(REGEXP_DATE_DD_MM_YY))){
		sscanf_s(timeStr.c_str(), FORMAT_DD_MM_YY, &day, &month, &year);
	}
	else if(regex_match(timeStr, regex(REGEXP_DATE_DD_MM_YY_HOUR_MIN))){
		sscanf_s(timeStr.c_str(), FORMAT_DD_MM_YY_HOUR_MIN, &day, &month, &year, &hour, &min);
	}
	else if(regex_match(timeStr, regex(REGEXP_DATE_DD_MM))){
		sscanf_s(timeStr.c_str(), FORMAT_DD_MM, &day, &month);
	}
	else if(regex_match(timeStr, regex(REGEXP_DATE_DD_MM_HOUR_MIN))){
		sscanf_s(timeStr.c_str(), FORMAT_DD_MM_HOUR_MIN, &day, &month, &hour, &min);
	}
	else if(timeStr == EMPTY){
		FLAG_empty = true;
	}
	else{
		throw ERROR_WRONG_TIME_FORMAT;
	}

	time.tm_mday = day;
	time.tm_mon = month != DEFAULT_TIME? month - 1: month;
	if(year == DEFAULT_TIME)
		time.tm_year = currentTimeInfo->tm_year;
	else
		time.tm_year = year > 1900? year - 1900: year + 100;
	time.tm_hour = hour;
	time.tm_min = min;

	result = FLAG_empty? DEFAULT_TIME: mktime(&time);
}

list<string> Interpreter::convertToList(string listStr){
	list<string> result;
	string listItemStr;
	stringstream listStrStream(listStr);
	while(getline(listStrStream, listItemStr, DELIMITER_COMMA)){
		list<string>::const_iterator startPos = result.begin();
		list<string>::const_iterator endPos = result.end();
		if(find(startPos, endPos, trim(listItemStr)) == endPos &&
			trim(listItemStr) != EMPTY &&
			trim(listItemStr) != SHARP)
			result.push_back(trim(listItemStr));
		listItemStr.clear();
	}
	return result;
}

list<time_t> Interpreter::convertToTimeList(string listStr) throw(string){
	list<time_t> result;
	string listItemStr;
	stringstream listStrStream(listStr);
	while(getline(listStrStream, listItemStr, DELIMITER_COMMA)){
		list<time_t>::const_iterator startPos = result.begin();
		list<time_t>::const_iterator endPos = result.end();
		time_t listItem;
		convertToTime(listItemStr, listItem);
		//neglect duplicant
		if(find(startPos, endPos, listItem) == endPos &&
			listItem != DEFAULT_TIME)
			result.push_back(listItem);
		listItemStr.clear();
	}
	return result;
}

int Interpreter::convertToEnum(string enumStr) throw(string) {
	int result;
	enumStr = trim(enumStr);
	enumStr = toUpper(enumStr);
	if(enumStr == EMPTY)
		result = -1;
	else if(enumStr == STR_DONE)
		result = DONE;
	else if(enumStr == STR_UNDONE)
		result = UNDONE;
	else if(enumStr == STR_TIMED)
		result = TIMED;
	else if(enumStr == STR_DEADLINE)
		result = DEADLINE;
	else if(enumStr == STR_FLOATING)
		result = FLOATING;
	else if(enumStr == STR_H || enumStr == STR_HIGH)
		result = HIGH;
	else if(enumStr == STR_M || enumStr == STR_MEDIUM)
		result = MEDIUM;
	else if(enumStr == STR_L || enumStr == STR_LOW)
		result = LOW;
	else
		throw ERROR_WRONG_FORMAT;
	return result;
}

/*****************
* Format Handler *
******************/

string Interpreter::trim(string str){
	stringstream trimmer;
	string substr;
	trimmer << str;
	str.clear();
	trimmer >> str;
	while(trimmer >> substr){
		str += SPACE + substr;//magic string
		substr.clear();
	}
	return str;
}

string Interpreter::toUpper(string str){
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}
