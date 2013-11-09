#include "Interpreter.h"
#include "Interpreter_Add.h"
#include "Interpreter_Mod.h"
#include "Interpreter_Find.h"
#include "Interpreter_Delete.h"
#include "Interpreter_Redo.h"
#include "Interpreter_Undo.h"

#include <regex>

using namespace std;
using namespace TP;

const string GENERAL_ADD_CASE="((((\\s+)(due|by)|(\\s+)from|(\\s+)to|(\\s+)(impt|priority)|(\\s+)(at|place|location)|(\\s+)(ppl|with)|(\\s+)note|(\\s+)(rt|remind))(\\s+)`[^`]*`)|((\\s+)#[^( |`)]*))*(\\s*)";
const string GENERAL_MOD_CASE="((((\\s+)(due|by)|(\\s+)from|(\\s+)name|(\\s+)to|(\\s+)(impt|priority)|(\\s+)(at|place|location)|(\\s+)(ppl|with)|(\\s+)note|(\\s+)(rt|remind)|(\\s+)-(rt|remind)|(\\s+)-(ppl|with)|(\\s+)\\+(rt|remind)|(\\s+)\\+(ppl|with))(\\s+)`[^`]*`)|((\\s+)done|(\\s+)undone)|((\\s+)-(due|by)|(\\s+)-from|(\\s+)-to)|((\\s+)-rtall|(\\s+)-pplall|(\\s+)-#)|((\\s+)(#|-#|\\+#)[^( |`)]*))*(\\s*)";
const string GENERAL_FIND_CASE="((((\\s+)from|(\\s+)name|(\\s+)to|(\\s+)(impt|priority)|(\\s+)(at|place|location)|(\\s+)(ppl|with)|(\\s+)note|(\\s+)(rt|remind))(\\s+)`[^`]*`)|((\\s+)#[^( |`)]*)|((\\s+)done|(\\s+)undone|(\\s+)overdue)|(((\\s+)timed|(\\s+)deadline|(\\s+)floating)))*(\\s*)";
const string COMMAND_ADD="^add(\\s+)`([^`]+)`(\\s*)";
const string COMMAND_MOD="^mod(\\s+)`([^`]+)`(\\s*)";
const string COMMAND_MOD_EXACT="^mod(\\s+)exact(\\s+)`([^`]+)`(\\s*)";
const string COMMAND_MOD_INDEX="^mod(\\s+)([0-9]+)(\\s*)";
const string COMMAND_FIND="^find(\\s*)";
const string COMMAND_FIND_EXACT="^find(\\s+)exact(\\s*)";
const string COMMAND_DEL="^del(\\s+)`([^`]+)`(\\s*)";
const string COMMAND_DEL_EXACT="^del(\\s+)exact(\\s+)`([^`]+)`(\\s*)";
const string COMMAND_DEL_INDEX="^del(\\s+)([0-9]+)(\\s*)";
const string COMMAND_UNDO="^undo(\\s*)";
const string COMMAND_REDO="^redo(\\s*)";
const string ERROR_MSG="invalid command";
const char NOTATION_ACCENT_GRACE='`';
const int TOTAL_TEST_CASE=12;
const int DUMMY_VALUE=-1;
const int START_POSITION_VALUE=0;

enum COMMAND_CATEGORY{ADD_COMMAND,MOD_COMMAND,MOD_EXACT_COMMAND,MOD_INDEX_COMMAND,FIND_COMMAND,FIND_EXACT_COMMAND,DEL_COMMAND, 
	DEL_EXACT_COMMAND, DEL_INDEX_COMMAND, UNDO_COMMAND,REDO_COMMAND,SYNC_COMMAND};



bool Interpreter::checkCommand(string command, int& commandType){

	bool flag=false;
	bool testlist[TOTAL_TEST_CASE]={false};
	int num=DUMMY_VALUE;

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


	testlist[ADD_COMMAND]=regex_match(command,test_add_command);
	testlist[MOD_COMMAND]=regex_match(command,test_mod_command);
	testlist[MOD_EXACT_COMMAND]=regex_match(command,test_mod_exact_command);
	testlist[MOD_INDEX_COMMAND]=regex_match(command,test_mod_index_command);
	testlist[FIND_COMMAND]=regex_match(command,test_find_command);
	testlist[FIND_EXACT_COMMAND]=regex_match(command,test_find_exact_command);
	testlist[DEL_COMMAND]=regex_match(command,test_del_command);
	testlist[DEL_EXACT_COMMAND]=regex_match(command,test_del_exact_command);
	testlist[DEL_INDEX_COMMAND]=regex_match(command,test_del_index_command);
	testlist[UNDO_COMMAND]=regex_match(command,test_undo_command);
	testlist[REDO_COMMAND]=regex_match(command,test_redo_command);
	testlist[SYNC_COMMAND]=regex_match(command,test_sync_command);

	for(int i=START_POSITION_VALUE;i<TOTAL_TEST_CASE && flag!=true;i++){
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
		for(unsigned i=START_POSITION_VALUE;i<requiredString.length();i++){
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
	string timeFormatIssue;
	Command* returnCommand = NULL;

	flag=checkCommand(commandStr,commandType);



	if(flag!=false){
		switch (commandType){
		case ADD_COMMAND:
			{
				Command_Add* Add_pointer=new Command_Add();
				Interpreter_Add interpretAdd;
				
				stringstream extractName(commandStr);
				string taskName;
				getline(extractName,taskName,NOTATION_ACCENT_GRACE);
				taskName.clear();
				getline(extractName,taskName,NOTATION_ACCENT_GRACE);
				Add_pointer->setName(taskName);

				
				try{
				returnCommand=interpretAdd.interpretAdd(Add_pointer, commandStr, response,flag);
				}catch(string timeFormatError){

					timeFormatIssue=timeFormatError;
			     
					if(flag==false){	
					delete Add_pointer;
					returnCommand=NULL;
					}
				
				}
				
				break;
			}

		case MOD_COMMAND:
			{
				Command_Mod* Mod_pointer=new Command_Mod();
				Interpreter_Mod interpretMod;

				stringstream extractName(commandStr);
				string taskName;
				getline(extractName,taskName,NOTATION_ACCENT_GRACE);
				taskName.clear();
				getline(extractName,taskName,NOTATION_ACCENT_GRACE);
				Mod_pointer->setName(taskName);

				returnCommand=interpretMod.interpretModify(Mod_pointer, commandStr, response,flag);
				
				if(flag==false){
				   delete Mod_pointer;
				   returnCommand=NULL;
				}
				break;
			}
		
		case MOD_EXACT_COMMAND:
			{
				Command_Mod* Mod_pointer=new Command_Mod();
				Interpreter_Mod interpretMod;

				Mod_pointer->setFlagExact();

				stringstream extractName(commandStr);
				string taskName;
				getline(extractName,taskName,NOTATION_ACCENT_GRACE);
				taskName.clear();
				getline(extractName,taskName,NOTATION_ACCENT_GRACE);
				Mod_pointer->setName(taskName);



				returnCommand=interpretMod.interpretModify(Mod_pointer, commandStr, response,flag);

				break;
			}

		case MOD_INDEX_COMMAND:
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
				if(flag==false){
				
				delete Mod_pointer;
				returnCommand=NULL;
				}
				
				break;
			}


		case FIND_COMMAND:
			{

				Command_Find* Find_pointer=new Command_Find();
				Interpreter_Find interpretFind;


				returnCommand=interpretFind.interpretFind(Find_pointer,commandStr, response,flag);
				break;
			}

		case FIND_EXACT_COMMAND:
			{

				Command_Find* Find_pointer=new Command_Find();
				Interpreter_Find interpretFind;


				Find_pointer->setFlagExact();
				returnCommand=interpretFind.interpretFind(Find_pointer,commandStr, response,flag);
				break;
			}

		case DEL_COMMAND:
			{

				Command_Del* Del_pointer=new Command_Del();
				Interpreter_Delete interpretDel;
				
				
				returnCommand=interpretDel.interpretDelete(Del_pointer,commandStr, response,flag);
				break;

			}
		case DEL_EXACT_COMMAND:
			{
				Command_Del* Del_pointer=new Command_Del();
               Interpreter_Delete interpretDel;
				
				
				Del_pointer->setFlagExact();

				returnCommand=interpretDel.interpretDelete(Del_pointer,commandStr, response,flag);
				break;
			}
		case DEL_INDEX_COMMAND:
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
		case UNDO_COMMAND:
			{
				Command_Undo* Undo_pointer=new Command_Undo();
				Interpreter_Undo interpretUndo;

				returnCommand=interpretUndo.interpretUndo(Undo_pointer,commandStr, response,flag);
				break;
			}
		case REDO_COMMAND:
			{
				Command_Redo* Redo_pointer=new Command_Redo();
				Interpreter_Redo interpretRedo;

				returnCommand=interpretRedo.interpretRedo(Redo_pointer,commandStr, response,flag);
				break;


			}
		default: flag=false;
			break;
		}

	}
	if(flag==false){

		response.setStatus(ERR);
		
		if(!timeFormatIssue.empty()){
		
			response.setErrorMsg(timeFormatIssue);
		
		}
		
		else{

			response.setErrorMsg(ERROR_MSG);
		}
		return NULL;
	}

	else{

		response.setStatus(SUCCESS);

		return returnCommand;
	}
}



