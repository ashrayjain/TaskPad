/*
 *
=================================================================
=================================================================
 *
 *  Filename: Interpreter.cpp   
 *  
 *  Version V0.5
 *  Created 15/10/13 12:00
 *
 *  Author: An Jiangze(A0105729A),Li Zixuan(A0096582R)
 *  Organization: NUS,SOC
 *
==================================================================
==================================================================
 */



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

const string GENERAL_ADD_CASE			="((((\\s+)(due|by)|(\\s+)from|(\\s+)to|(\\s+)(impt|priority)|(\\s+)(at|place|location)|(\\s+)(ppl|with)|(\\s+)note|(\\s+)(rt|remind))(\\s+)`[^`]*`)|((\\s+)#[^( |`)]*))*(\\s*)";
const string GENERAL_MOD_CASE			="((((\\s+)(due|by)|(\\s+)from|(\\s+)name|(\\s+)to|(\\s+)(impt|priority)|(\\s+)(at|place|location)|(\\s+)(ppl|with)|(\\s+)note|(\\s+)(rt|remind)|(\\s+)-(rt|remind)|(\\s+)-(ppl|with)|(\\s+)\\+(rt|remind)|(\\s+)\\+(ppl|with))(\\s+)`[^`]*`)|((\\s+)done|(\\s+)undone)|((\\s+)-(due|by)|(\\s+)-from|(\\s+)-to)|((\\s+)-rtall|(\\s+)-pplall|(\\s+)-#)|((\\s+)(#|-#|\\+#)[^( |`)]*))*(\\s*)";
const string GENERAL_FIND_CASE			="((((\\s+)from|(\\s+)name|(\\s+)to|(\\s+)(impt|priority)|(\\s+)(at|place|location)|(\\s+)(ppl|with)|(\\s+)note|(\\s+)(rt|remind))(\\s+)`[^`]*`)|((\\s+)#[^( |`)]*)|((\\s+)done|(\\s+)undone|(\\s+)overdue)|(((\\s+)timed|(\\s+)deadline|(\\s+)floating)))*(\\s*)";
const string COMMAND_ADD				="^add(\\s+)`([^`]+)`(\\s*)";
const string COMMAND_MOD				="^mod(\\s+)`([^`]+)`(\\s*)";
const string COMMAND_MOD_EXACT			="^mod(\\s+)exact(\\s+)`([^`]+)`(\\s*)";
const string COMMAND_MOD_INDEX			="^mod(\\s+)([0-9]+)(\\s*)";
const string COMMAND_FIND				="^find(\\s*)";
const string COMMAND_FIND_EXACT			="^find(\\s+)exact(\\s*)";
const string COMMAND_DEL				="^del(\\s+)`([^`]+)`(\\s*)";
const string COMMAND_DEL_EXACT			="^del(\\s+)exact(\\s+)`([^`]+)`(\\s*)";
const string COMMAND_DEL_INDEX			="^del(\\s+)([0-9]+)(\\s*)";
const string COMMAND_UNDO				="^undo(\\s*)";
const string COMMAND_REDO				="^redo(\\s*)";
const string COMMAND_INDEX				="^([0-9]+)(\\s*)";
const string ERROR_COMMAND				="invalid command";

const int	 TOTAL_TEST_CASE			=12;
const int	 DUMMY_VALUE				=-1;
const int	 START_POSITION_VALUE		=0;
const int    START_POSITION				=0;
const char   NOTATION_ACCENT_GRAVE		='`';

enum COMMAND_CATEGORY{ADD_COMMAND,MOD_COMMAND,MOD_EXACT_COMMAND,MOD_INDEX_COMMAND,FIND_COMMAND,FIND_EXACT_COMMAND,DEL_COMMAND, 
	DEL_EXACT_COMMAND, DEL_INDEX_COMMAND, UNDO_COMMAND,REDO_COMMAND,INDEX_COMMAND};

/**********************************************************************************
*Input: commandStr->original command line input by user						      *
*		response->a Messenger object that contains status and error message		  *
*																				  *
*Function: This function checks whether the user input command matches with one   *
*		   of the 11 general cases and parses the command. The success of		  *
*		   the interpretation and any error message is set in response			  *
*																				  *
*Ouput: returnCommand->a Command object with all fields set by Interpreter		  *
*		response->a Messenger object with status of interpretation and error      *
*				  message if any												  *
**********************************************************************************/
//AN JIANGZE (FUNCTIONALITY) LI ZIXUAN (REFACTORING)
//@AN JIANGZE A0105729A 
//@LI ZIXUAN  A0096582R
Command*  Interpreter::interpretCommand(std::string commandStr, Messenger &response){

	bool isValidCommand=true;
	int commandType;

	string word;
	string issueCollector;
	Command* returnCommand=NULL;
	isValidCommand=checkCommand(commandStr,commandType);

	if(isValidCommand!=false){
		switch (commandType){
		case ADD_COMMAND:{
				Command_Add* Add_pointer=new Command_Add();
				Interpreter_Add interpretAdd;
				string taskName;
				extractQuotedMessage(commandStr, taskName);
				Add_pointer->setName(taskName);

				
				try{
					returnCommand=interpretAdd.interpretAdd(Add_pointer, commandStr, response,isValidCommand);
				}catch(string errorMessage){
	                 isValidCommand=false;					
					 issueCollector=errorMessage;
					 if(Add_pointer!=NULL){
						delete Add_pointer;
					}
				}
				break;
			}

		case MOD_COMMAND:{
				Command_Mod* Mod_pointer=new Command_Mod();
				Interpreter_Mod interpretMod;
				string taskName;
				extractQuotedMessage(commandStr, taskName);
				Mod_pointer->setName(taskName);

				try{
					returnCommand=interpretMod.interpretModify(Mod_pointer, commandStr, response,isValidCommand);
				}catch(string errorMessage){
					isValidCommand=false;					
					issueCollector=errorMessage;			     					
					if(Mod_pointer!=NULL){
						delete Mod_pointer;
					}				
				}
				break;
			}
		
		case MOD_EXACT_COMMAND:{
				Command_Mod* Mod_pointer=new Command_Mod();
				Interpreter_Mod interpretMod;
				string taskName;
				Mod_pointer->setFlagExact();	
				extractQuotedMessage(commandStr, taskName);
				Mod_pointer->setName(taskName);

				try{
					returnCommand=interpretMod.interpretModify(Mod_pointer, commandStr, response,isValidCommand);
				}catch(string errorMessage){
					isValidCommand=false;					
					issueCollector=errorMessage;
					if(Mod_pointer!=NULL){
						delete Mod_pointer;
					}			
				}
				break;
			}

		case MOD_INDEX_COMMAND:{
				Command_Mod* Mod_pointer=new Command_Mod();
				Interpreter_Mod interpretMod;	
				string getIndex=commandStr;
				stringstream extractIndex(getIndex);
				string content;
				extractIndex>>getIndex;
				getIndex.clear();
				extractIndex>>getIndex;
				int index;

				index=getIndexMessage(getIndex,isValidCommand);
				
				Mod_pointer->setIndex(index);

				try{
					returnCommand=interpretMod.interpretModify(Mod_pointer, commandStr, response,isValidCommand);
				}catch(string errorMessage){
					isValidCommand=false;					
					issueCollector=errorMessage;	
					if(Mod_pointer!=NULL){
						delete Mod_pointer;
					}				
				}							
				break;
			}


		case FIND_COMMAND:{
				Command_Find* Find_pointer=new Command_Find();
				Interpreter_Find interpretFind;
				try{
					returnCommand=interpretFind.interpretFind(Find_pointer,commandStr, response,isValidCommand);
				}catch(string errorMessage){
					isValidCommand=false;					
					issueCollector=errorMessage;					
					if(Find_pointer!=NULL){
						delete Find_pointer;
					}				
				}
				break;
			}

		case FIND_EXACT_COMMAND:{

				Command_Find* Find_pointer=new Command_Find();
				Interpreter_Find interpretFind;
				Find_pointer->setFlagExact();			
				try{
					returnCommand=interpretFind.interpretFind(Find_pointer,commandStr, response,isValidCommand);
				}catch(string errorMessage){
					isValidCommand=false;					
					issueCollector=errorMessage;			    				
					if(Find_pointer!=NULL){
						delete Find_pointer;
					}					
				}	
				break;
			}
		case DEL_COMMAND:{
				Command_Del* Del_pointer=new Command_Del();
				Interpreter_Delete interpretDel;								
				returnCommand=interpretDel.interpretDelete(Del_pointer,commandStr, response,isValidCommand);
				break;
			}
		case DEL_EXACT_COMMAND:{
				Command_Del* Del_pointer=new Command_Del();
                Interpreter_Delete interpretDel;						
				Del_pointer->setFlagExact();
				returnCommand=interpretDel.interpretDelete(Del_pointer,commandStr, response,isValidCommand);
				break;
			}
		case DEL_INDEX_COMMAND:{
				Command_Del* Del_pointer=new Command_Del();
				Interpreter_Delete interpretDel;
				stringstream extractIndex(commandStr);
				string content;
				int index;
				extractIndex>>commandStr;
				commandStr.clear();
				extractIndex>>commandStr;				
				index=getIndexMessage(commandStr,isValidCommand);				
				Del_pointer->setIndex(index);
				response.setCommandType(DEL);
				returnCommand=(Command*)Del_pointer;
				break;
			}
		case UNDO_COMMAND:{
				Command_Undo* Undo_pointer=new Command_Undo();
				Interpreter_Undo interpretUndo;
				returnCommand=interpretUndo.interpretUndo(Undo_pointer,commandStr, response,isValidCommand);
				break;
			}
		case REDO_COMMAND:{
				Command_Redo* Redo_pointer=new Command_Redo();
				Interpreter_Redo interpretRedo;
				returnCommand=interpretRedo.interpretRedo(Redo_pointer,commandStr, response,isValidCommand);
				break;
			}
		case INDEX_COMMAND:{
				int num;
				Command_Show* Show_pointer=new Command_Show();
				integerConvert(commandStr,num);
				Show_pointer->setIndex(num);
				response.setCommandType(SHOW);
				returnCommand=(Command*)Show_pointer;
				break;
			}
		default: isValidCommand=false;
			break;
		}

	}
	if(isValidCommand==false){
		response.setStatus(ERR);
		//if has not encountered any throw yet has error, command format is wrong
		if(!issueCollector.empty()){	
			response.setErrorMsg(issueCollector);	
		}		
		else{
			response.setErrorMsg(ERROR_COMMAND);
		}
	}

	else{
		response.setStatus(SUCCESS);
	}
	return returnCommand;
}

/**********************************************************************************
*Input: ActualIndex->index input by the user								      *
*		prevCommand->the previous Command object for the previous user input	  *
*																				  *
*Function: This function is called during "intermediate" case where the system    *
*		   returns multiple results with due to an ambiguous find/mod/del command.*
*		   These results have allocated indices and user can input an index to    *
*		   choose the corresponding task. This function parses the index and sets *
*		   it as an attribute of the previous Command object.                     *
*   	   					 													  *
*Ouput: prevCommand->previous Command object with index set						  *
*																				  *
**********************************************************************************/
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

bool Interpreter::checkCommand(string command, int& commandType){

	bool isValidCommand=false;
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
	regex test_index_command(COMMAND_INDEX);


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
	testlist[INDEX_COMMAND]=regex_match(command,test_index_command);

	for(int i=START_POSITION_VALUE;i<TOTAL_TEST_CASE && isValidCommand!=true;i++){
		if(testlist[i]==true){
			num=i;
			isValidCommand=true;
		}
	}

	commandType=num;
	return isValidCommand;
}

void Interpreter:: extractQuotedMessage(string field, string& quotedMessage){

	stringstream extract(field);
	getline(extract,quotedMessage,NOTATION_ACCENT_GRAVE);
	quotedMessage.clear();
	getline(extract,quotedMessage,NOTATION_ACCENT_GRAVE);
	return;
}

int Interpreter:: getIndexMessage(string command,bool& isSuccessful){

	int num;
	isSuccessful=integerConvert(command, num);
	return num;
}	

bool Interpreter::integerConvert(string& requiredString, int& number){
	
	bool isSuccessful=true;

	if(requiredString.empty()==true){
		isSuccessful=false;
	}
	else{
		for(unsigned i=START_POSITION_VALUE;i<requiredString.length();i++){
			if(isdigit(requiredString[i])==false){
				isSuccessful=false;
			}
		}
	}
	number=atoi(requiredString.c_str());

	return isSuccessful;
}
