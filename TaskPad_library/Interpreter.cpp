#include "Enum.h"
#include "Interpreter.h"

void Interpreter::interpretCommand(unsigned ActualIndex, Command *prevCommand){



	return;
}


int Interpreter::interpretIndex(std::string indexStr, Messenger &response){

	int num;
	if(integerConverter(indexStr,num))
	{
		response.setStatus(TP::SUCCESS);

	}
	else{ 

		response.setStatus(TP::ERROR);
		response.setErrorMsg("The index is invalid");
	}

	return num;
}



Command* Interpreter::interpretCommand(std::string commandStr, Messenger &response)
{
	bool flag=true;
	stringstream extract(commandStr);
	string word;
	Command* returnCommand;

	flag=checkPun(commandStr);

	extract>>word;

	if(flag!=false){
		if(word=="add")
		{
			Command_Add* Add_pointer=new Command_Add();

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ") flag = false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0){
					Add_pointer->setName(word);
					if(commandStr.length()>=6+word.length()){
						returnCommand=interpretAdd(Add_pointer, commandStr.substr(6+word.length()),response,flag);
					}
					else flag=false;
				}

				else flag=false;
			}

		}

		else if(word=="mod")
		{
			Command_Mod* Mod_pointer=new Command_Mod();
			stringstream checkIndex;

			word.clear();
			getline(extract,word,'\'');



			if(word==" "){

				word.clear();
				getline(extract,word,'\'');		


				if(word.length()>0){

					Mod_pointer->setOptName(word);

					if(commandStr.length()>=6+word.length())
					{
						returnCommand=interpretModify(Mod_pointer, commandStr.substr(6+word.length()),response,flag);

					}
					else flag=false;
				}

				else flag=false;	
			}	


			else if(word==" exact ")
			{
				Mod_pointer->setFlagExact();

				word.clear();
				getline(extract,word,'\'');

				if(word.length()>0){
					Mod_pointer->setName(word);

					if(commandStr.length()>=12+word.length()){
						returnCommand=interpretModify(Mod_pointer, commandStr.substr(12+word.length()),response,flag);

					}
					else flag=false;
				}
				else flag=false;

			}


			else 
			{
				int index=atoi(word.c_str());
				checkIndex<<index;

				if(index>0 && checkIndex.str().length()==word.length()-2)
				{
					Mod_pointer->setIndex(index);

					if(commandStr.length()>=6+word.length()){
						returnCommand=interpretModify(Mod_pointer, commandStr.substr(6+word.length()),response,flag);
					}
					else flag=false;
				}
				else flag=false;
			}

		}

		else if(word=="del"){

			Command_Del* Del_pointer=new Command_Del();  
			stringstream checkIndex;

			word.clear();
			getline(extract,word,'\'');

			if(word=="exact")
			{
				Del_pointer->setFlagExact();

				word.clear();
				getline(extract,word,'\'');

				if(word!=" ") flag = false;

				word.clear();
				getline(extract,word,'\'');		

				if(flag!=false){
					if(word.length()>0 && commandStr.find("\'"+word+"\'")!=string::npos){
						Del_pointer->setName(word);

						returnCommand=(Command*)Del_pointer;

					}
					else flag=false;
				}
			}
			else {

				int index=atoi(word.c_str());

				checkIndex<<index;
				if(index>0 && checkIndex.str().length()==word.length())
				{
					Del_pointer->setIndex(index);

					returnCommand=(Command*)Del_pointer;
				}
				else flag=false;
			}

		}

		else if(word=="find"){

			Command_Find* Find_pointer=new Command_Find();

			word.clear();
			getline(extract,word,'\'');

			if(word==" "){

				word.clear();
				getline(extract,word,'\'');		


				if(word.length()>0){

					Find_pointer->setOptName(word);

					if(commandStr.length()>=7+word.length()){
						returnCommand=interpretFind(Find_pointer, commandStr.substr(7+word.length()),response,flag);

					}
					else flag=false;
				}

				else flag=false;	
			}	


			else if(word==" exact ")
			{
				Find_pointer->setFlagExact();

				word.clear();
				getline(extract,word,'\'');

				if(word.length()>0){
					Find_pointer->setOptName(word);
					if(commandStr.length()>=13+word.length()){
						returnCommand=interpretFind(Find_pointer, commandStr.substr(13+word.length()),response,flag);
					}
					else flag=false;
				}
				else flag=false;
			}
		}


		else if(word=="undo"){

			Command_Undo* Undo_pointer=new Command_Undo();
			returnCommand=(Command*)Undo_pointer;
		}

		else if(word=="redo"){

			Command_Redo* Redo_pointer=new Command_Redo();
			returnCommand=(Command*)Redo_pointer;


		}

		else if(word=="sync"){

			Command_Sync* Sync_pointer=new Command_Sync(); 

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ") flag = false;

			word.clear();
			getline(extract,word,'\'');
			if(flag!=false){

				if(word.length()>0 && commandStr.find("\'"+word+"\'")!=string::npos){

					Sync_pointer->setSyncProviderName(word);
					returnCommand=(Command*)Sync_pointer;

				}
				else flag=false;
			}

		}
	}
	if(flag==false){

		response.setStatus(TP::ERROR);
		response.setErrorMsg("invalid command");
		return NULL;
	}

	else return returnCommand;
}

Command* Interpreter:: interpretAdd(Command_Add* commandType, string commandStr,Messenger &response, bool& flag)
{
	stringstream extract(commandStr);
	string word;
	string timeComponent;



	while(extract>>word && flag)
	{
		if(word=="due" && commandType->getFlagDue()==false && commandType->getFlagFrom()==false && commandType->getFlagTo()==false){

			time_t requiredTime;

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){

				if(word.length()>0){
					requiredTime=setTime(word,flag);

					commandType->setDueDate(requiredTime);
				}
				else flag=false;
			}
		}

		else if(word=="from" && commandType->getFlagFrom()==false && commandType->getFlagTo()==false && commandType->getFlagDue()==false){

			time_t requiredTime;

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0){
					requiredTime=setTime(word,flag);	

					commandType->setFromDate(requiredTime);
				}
				else flag=false;	
			}
		}


		else if(word=="to" && commandType->getFlagFrom()==true && commandType->getFlagDue()==false){
			time_t requiredTime;

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){

				if(word.length()>0){
					requiredTime=setTime(word,flag);	

					commandType->setToDate(requiredTime);	
				}
				else flag=false;
			}
		}

		else if(word=="at" && commandType->getFlagLocation()==false){


			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setLocation(word);

				}
				else flag=false;
			}
		}

		else if(word=="ppl" && (commandType->getFlagParticipants()==false)){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setParticipants(word);
				}
				else flag=false;
			}
		}
		else if(word=="note" && commandType->getFlagNote()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setNote(word);
				}

				else flag=false;
			}
		}

		else if(word=="impt" && commandType->getFlagPriority()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					if(word=="H" ||word=="HIGH"){

						commandType->setPriority(TP::HIGH);
					}
					else if(word=="M" || word=="MEDIUM"){
						commandType->setPriority(TP::MEDIUM);
					}
					else if(word=="L"||word=="LOW"){
						commandType->setPriority(TP::LOW);
					}
					else flag=false;
				}
				else flag=false;
			}
		}


		else if(word=="#"){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0){
					commandType->setNote(word);

				}
				else flag=false;
			}
		}


		else if(word=="rt"){
			time_t requiredTime;

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0){
					requiredTime=setTime(word,flag);	

					commandType->setRemindTime(requiredTime);	
				}
				else flag=false;
			}
		}


		else flag=false;	

	}

	if(flag==false){
		response.setStatus(TP::ERROR);
		response.setErrorMsg("Command invalid!");
		return NULL; 

	}
	else return (Command*)commandType;

}



Command* Interpreter::interpretModify(Command_Mod* commandType, std::string commandStr, Messenger &response, bool& flag)
{

	stringstream extract(commandStr);
	string word;
	string timeComponent;

	while(extract>>word && flag)
	{
		if(word=="due" && commandType->getFlagDue()==false && commandType->getFlagFrom()==false && commandType->getFlagTo()==false){

			time_t requiredTime;

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){

				if(word.length()>0){
					requiredTime=setTime(word,flag);

					commandType->setDueDate(requiredTime);
				}
				else flag=false;
			}
		}


		else if(word=="at" && commandType->getFlagLocation()==false){


			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setLocation(word);

				}
				else flag=false;
			}
		}
		else if(word=="ppl" && (commandType->getFlagParticipants()==false)){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setParticipants(word);
				}
				else flag=false;
			}
		}
		else if(word=="note" && commandType->getFlagNote()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setNote(word);

				}

				else flag=false;
			}
		}

		else if(word=="impt" && commandType->getFlagPriority()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					if(word=="H" ||word=="HIGH"){

						commandType->setPriority(TP::HIGH);
					}
					else if(word=="M" || word=="MEDIUM"){
						commandType->setPriority(TP::MEDIUM);
					}
					else if(word=="L"||word=="LOW"){
						commandType->setPriority(TP::LOW);
					}
					else flag=false;
				}
				else flag=false;
			}
		}

		else if(word=="#" && commandType->getFlagTags()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0){

					commandType->setNote(word);

				}
				else flag=false;
			}
		}


		else if(word=="rt" && commandType->getFlagRemindTime()==false){
			time_t requiredTime;

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0){
					requiredTime=setTime(word,flag);	

					commandType->setRemindTime(requiredTime);	
				}
				else flag=false;	
			}
		}




		else if(word=="-at" && commandType->getFlagLocation()==false){

			commandType->setLocation("");

		}




		else if(word=="-ppl" && commandType->getFlagParticipants()==false){
			commandType->setParticipants("");

		}



		else if(word=="-note" && commandType->getFlagNote()==false)
		{
			commandType->setNote("");

		}

		else if(word=="-#" && commandType->getFlagTags()==false){

			commandType->setTags("");
		}



		else if(word=="-rt" && commandType->getFlagRemindTime()==false)
		{
			time_t timeNull;
			timeNull=time(0);
			commandType->setRemindTime(timeNull);
		}


		else if(word=="done" && commandType->getFlagTaskState()==false){

			commandType->setTaskState(TP::DONE);
		}
		else if(word=="undone" && commandType->getFlagTaskState()==false){

			commandType->setTaskState(TP::UNDONE);

		}

		else flag=false;	

	}

	if(flag==false){
		response.setStatus(TP::ERROR);
		response.setErrorMsg("Command invalid!");
		return NULL; 

	}
	else return (Command*)commandType;
}



Command* Interpreter::interpretFind(Command_Find* commandType, std::string commandStr, Messenger &response, bool& flag){

	stringstream extract(commandStr);
	string word;
	string timeComponent;

	while(extract>>word && flag){

		if(word=="timed" && commandType->getFlagTaskType()==false){

			commandType->setTaskType(TP::TIMED);

			word.clear();
			extract>>word;

			if(word=="from"){

				time_t requiredTime;

				word.clear();
				getline(extract,word,'\'');

				if(word!=" ")flag=false;

				word.clear();
				getline(extract,word,'\'');	

				if(flag!=false){
					if(word.length()>0){

						requiredTime=setTime(word,flag);

						commandType->setFromDate(requiredTime);
					}
					else flag=false;
				}
			}

			else flag=false;

			if(flag){

				word.clear();
				extract>>word;

				if(word=="to"){

					time_t requiredTime;

					word.clear();
					getline(extract,word,'\'');

					if(word!=" ")flag=false;

					word.clear();
					getline(extract,word,'\'');	

					if(flag!=false){
						if(word.length()>0){

							requiredTime=setTime(word,flag);

							commandType->setToDate(requiredTime);

						}
						else flag=false;
					}

				}		
				else flag=false;
			}
		}

		else if(word=="deadline" && commandType->getFlagTaskType()==false){
			time_t requiredTime;

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');		

			if(flag!=false){
				if(word.length()>0){
					requiredTime=setTime(word,flag);

					commandType->setDueDate(requiredTime);
					commandType->setTaskType(TP::DEADLINE);
				}

				else flag=false;
			}
		}

		else if(word=="floating" && commandType->getFlagTaskType()==false){

			commandType->setTaskType(TP::FLOATING);		

		}

		else if(word=="at" && commandType->getFlagLocation()==false){


			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setLocation(word);

				}
				else flag=false;
			}
		}

		else if(word=="ppl" && (commandType->getFlagParticipants()==false)){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setParticipants(word);
				}
				else flag=false;
			}
		}

		else if(word=="note" && commandType->getFlagNote()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setNote(word);
				}

				else flag=false;
			}
		}

		else if(word=="impt" && commandType->getFlagPriority()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					if(word=="H" ||word=="HIGH"){

						commandType->setPriority(TP::HIGH);
					}
					else if(word=="M" || word=="MEDIUM"){
						commandType->setPriority(TP::MEDIUM);
					}
					else if(word=="L"||word=="LOW"){
						commandType->setPriority(TP::LOW);
					}
					else flag=false;
				}
				else flag=false;
			}
		}


		else if(word=="#" && commandType->getFlagTags()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');
			if(flag!=false){
				if(word.length()>0){

					commandType->setNote(word);

				}
				else flag=false;
			}
		}

		else if(word=="-at" && commandType->getFlagLocation()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setLocation(word);

				}
				else flag=false;
			}
		}



		else if(word=="-ppl" && commandType->getFlagParticipants()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setParticipants(word);
				}
				else flag=false;
			}
		}


		else if(word=="-note" && commandType->getFlagNote()==false)
		{
			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0)
				{
					commandType->setNote(word);

				}

				else flag=false;
			}
		}


		else if(word=="-#" && commandType->getFlagTags()==false){

			word.clear();
			getline(extract,word,'\'');

			if(word!=" ")flag=false;

			word.clear();
			getline(extract,word,'\'');

			if(flag!=false){
				if(word.length()>0){

					commandType->setNote(word);

				}
				else flag=false;
			}
		}


		else if(word=="done" && commandType->getFlagTaskState()==false){

			commandType->setTaskState(TP::DONE);
		}
		else if(word=="undone" && commandType->getFlagTaskState()==false ){

			commandType->setTaskState(TP::UNDONE);

		}	
		else flag=false;
	}

	if(flag==false){
		response.setStatus(TP::ERROR);
		response.setErrorMsg("Command invalid!");
		return NULL; 

	}
	else return (Command*)commandType;

}


bool Interpreter:: checkPun(string requiredString){


	int counter=0;
	for(int i=0;i<(int)requiredString.length();i++)
	{
		if(requiredString.at(i)=='\'')
			counter++;

	}
	if(counter%2 ==0)return true;

	else return false;


}



time_t Interpreter::setTime(string word, bool& flag)
{
	int year=-1,month=-1,day=-1,hour=-1,min=-1;
	string timeComponent;

	if(word.length()>0)
	{
		timeComponent=word;

		stringstream timeExtracter(timeComponent);


		getline(timeExtracter,timeComponent,'/');		

		if(!integerConverter(timeComponent,day))
		{	
			flag=false;

		}


		timeComponent.clear(); 			
		getline(timeExtracter,timeComponent,'/');

		if(timeComponent.length()>0)
		{

			if(!integerConverter(timeComponent,month))
			{		
				flag=false;

			}
		}


		timeComponent.clear(); 			
		getline(timeExtracter,timeComponent,' ');
		if(timeComponent.length()>0)
		{

			if(!integerConverter(timeComponent,year))
			{	
				flag=false;

			}
			year+=2000;

		}

		timeComponent.clear(); 			
		getline(timeExtracter,timeComponent,':');
		if(timeComponent.length()>0)
		{

			if(!integerConverter(timeComponent,hour))
			{	
				flag=false;

			}
		}

		timeComponent.clear(); 			
		getline(timeExtracter,timeComponent);
		if(timeComponent.length()>0)
		{

			if(!integerConverter(timeComponent,min))
			{	
				flag=false;

			}
		}
	}
	else flag=false;

	struct tm * timeinfo;
	memset(&timeinfo, 0, sizeof(struct tm));
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = month - 1;
	timeinfo->tm_mday = day;
	timeinfo->tm_hour=hour;
	timeinfo->tm_min=min;

	return mktime(timeinfo);
}

bool integerConverter(string& requiredString, int& number)
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