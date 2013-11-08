#include "Interpreter.h"
#include "Interpreter_Add.h"
#include "Interpreter_Mod.h"
#include "Interpreter_Find.h"
#include "Interpreter_Delete.h"
#include "Interpreter_Redo.h"
#include "Interpreter_Undo.h"
#include "Interpreter_Sync.h"
#include <regex>

const string GENERAL_ADD_CASE="((( (due|by)| from| to| (impt|priority)| (at|place|location)| (ppl|with)| note| (rt|remind)) `[^`]*`)|( #[^( |`)]*))*(\\s*)";
const string GENERAL_MOD_CASE="((( (due|by)| from| name| to| (impt|priority)| (at|place|location)| (ppl|with)| note| (rt|remind)| -(rt|remind)| -(ppl|with)| \\+(rt|remind)| \\+(ppl|with)) `[^`]*`)|( done| undone)|( -(due|by)| -from| -to)|( -rtall| -pplall| -#)|( (#|-#|\\+#)[^( |`)]*))*(\\s*)";
const string GENERAL_FIND_CASE="((( from| name| to| (impt|priority)| (at|place|location)| (ppl|with)| note| (rt|remind)) `[^`]*`)|( #[^( |`)]*)|( done| undone| overdue)|(( timed| deadline| floating)))*(\\s*)";
const string COMMAND_ADD="^add `([^`]+)`";
const string COMMAND_MOD="^mod `([^`]+)`";
const string COMMAND_MOD_EXACT="^mod exact `([^`]+)`";
const string COMMAND_MOD_INDEX="^mod ([0-9]+)";
const string COMMAND_FIND="^find";
const string COMMAND_FIND_EXACT="^find exact";
const string COMMAND_DEL="^del `([^`]+)`(\\s*)";
const string COMMAND_DEL_EXACT="^del exact `([^`]+)`(\\s*)";
const string COMMAND_DEL_INDEX="^del ([0-9]+)(\\s*)";
const string COMMAND_UNDO="^undo(\\s*)";
const string COMMAND_REDO="^redo(\\s*)";
const string ERROR_MSG="invalid command";

bool Interpreter::checkCommand(string command, int& commandType){

	bool flag=false;
	bool testlist[12]={false};
	int num=-1;

	regex test_add_command(COMMAND_ADD+GENERAL_ADD_CASE); 

	regex test_mod_command(COMMAND_MOD+GENERAL_MOD_CASE); 
	regex test_mod_exact_command(COMMAND_MOD_EXACT+GENERAL_MOD_CASE);
	regex test_mod_index_command(COMMAND_MOD_INDEX+GENERAL_MOD_CASE);

	regex test_find_command(COMMAND_FIND+GENERAL_FIND_CASE);
	regex test_find_exact_command(COMMAND_FIND_EXACT+GENERAL_FIND_CASE);

	regex test_del_command(COMMAND_DEL);
	regex test_del_exact_command(COMMAND_DEL_EXACT);
	regex test_del_index_command(COMMAND_DEL_INDEX);

	regex test_undo_command(COMMAND_UNDO);

	regex test_redo_command(COMMAND_REDO);

	regex test_sync_command("^sync `([^`]+)`(\\s*)");


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
	testlist[11]=regex_match(command,test_sync_command);

	for(int i=0;i<12 && flag!=true;i++){
		if(testlist[i]==true){
			num=i;
			flag=true;
		}
	}

	commandType=num;
	return flag;
}


bool Interpreter::integerConvert(string& requiredString, int& number){
	
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
	if(integerConvert(indexStr,num))
	{
		response.setStatus(SUCCESS);

	}
	else{ 

		response.setStatus(ERR);
		response.setErrorMsg(ERROR_MSG);
	}

	return num;

}

Command*  Interpreter::interpretCommand(std::string commandStr, Messenger &response){

	bool flag=true;
	int commandType;

	string word;
	Command* returnCommand;

	flag=checkCommand(commandStr,commandType);



	if(flag!=false){
		switch (commandType){
		case ADD_COMMAND:
			{
				Command_Add* Add_pointer=new Command_Add();
				Interpreter_Add interpretAdd;
				
				stringstream extractName(commandStr);
				string taskName;
				getline(extractName,taskName,'`');
				taskName.clear();
				getline(extractName,taskName,'`');
				Add_pointer->setName(taskName);

				

				returnCommand=interpretAdd.interpretAdd(Add_pointer, commandStr, response,flag);

				break;
			}

		case 1:
			{
				Command_Mod* Mod_pointer=new Command_Mod();
				Interpreter_Mod interpretMod;

				stringstream extractName(commandStr);
				string taskName;
				getline(extractName,taskName,'`');
				taskName.clear();
				getline(extractName,taskName,'`');
				Mod_pointer->setName(taskName);

				
				
				returnCommand=interpretMod.interpretModify(Mod_pointer, commandStr, response,flag);
				break;
			}

		case 2:
			{
				Command_Mod* Mod_pointer=new Command_Mod();
				Interpreter_Mod interpretMod;

				Mod_pointer->setFlagExact();

				stringstream extractName(commandStr);
				string taskName;
				getline(extractName,taskName,'`');
				taskName.clear();
				getline(extractName,taskName,'`');
				Mod_pointer->setName(taskName);



				returnCommand=interpretMod.interpretModify(Mod_pointer, commandStr, response,flag);

				break;
			}

		case 3:
			{
				Command_Mod* Mod_pointer=new Command_Mod();
				Interpreter_Mod interpretMod;
				
				string getIndex=commandStr;
				stringstream extractIndex(getIndex);
				string content;
				extractIndex>>getIndex;
				getIndex.clear();
				extractIndex>>getIndex;
				int index;

				index=interpretMod.getIndexMessage(getIndex,flag);
				
				Mod_pointer->setIndex(index);

				returnCommand=interpretMod.interpretModify(Mod_pointer, commandStr, response,flag);
				break;
			}


		case 4:
			{

				Command_Find* Find_pointer=new Command_Find();
				Interpreter_Find interpretFind;


				returnCommand=interpretFind.interpretFind(Find_pointer,commandStr, response,flag);
				break;
			}

		case 5:
			{

				Command_Find* Find_pointer=new Command_Find();
				Interpreter_Find interpretFind;


				Find_pointer->setFlagExact();
				returnCommand=interpretFind.interpretFind(Find_pointer,commandStr, response,flag);
				break;
			}

		case 6:
			{

				Command_Del* Del_pointer=new Command_Del();
				Interpreter_Delete interpretDel;
				
				
				returnCommand=interpretDel.interpretDelete(Del_pointer,commandStr, response,flag);
				break;

			}
		case 7:
			{
				Command_Del* Del_pointer=new Command_Del();
               Interpreter_Delete interpretDel;
				
				
				Del_pointer->setFlagExact();

				returnCommand=interpretDel.interpretDelete(Del_pointer,commandStr, response,flag);
				break;
			}
		case 8:
			{
				Command_Del* Del_pointer=new Command_Del();
				Interpreter_Delete interpretDel;
				
				stringstream extractIndex(commandStr);
				string content;
				extractIndex>>commandStr;
				commandStr.clear();
				extractIndex>>commandStr;
				int index;

				index=interpretDel.getIndexMessage(commandStr,flag);
				
				Del_pointer->setIndex(index);
				response.setCommandType(DEL);

				returnCommand=(Command*)Del_pointer;
				break;


			}
		case 9:
			{
				Command_Undo* Undo_pointer=new Command_Undo();
				Interpreter_Undo interpretUndo;

				returnCommand=interpretUndo.interpretUndo(Undo_pointer,commandStr, response,flag);
				break;
			}
		case 10:
			{
				Command_Redo* Redo_pointer=new Command_Redo();
				Interpreter_Redo interpretRedo;

				returnCommand=interpretRedo.interpretRedo(Redo_pointer,commandStr, response,flag);
				break;


			}
		case 11:
			{
				Command_Sync *Sync_pointer=new Command_Sync();
				Interpreter_Sync interpretSync;
				
				
				returnCommand=interpretSync.interpretSync(Sync_pointer,commandStr, response,flag);

				break;
			}

		default: flag=false;
			break;
		}

	}
	if(flag==false){

		response.setStatus(ERR);
		response.setErrorMsg(ERROR_MSG);
		return NULL;
	}

	else{

		response.setStatus(SUCCESS);

		return returnCommand;
	}
}



