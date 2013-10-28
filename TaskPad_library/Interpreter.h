//#include "Command.h"
#include<string>
// #include<regex>
//#include "Messenger.h"

/*
#include "Interpreter_Add.h"
#include "Interpreter_Mod.h"
#include "Interpreter_Find.h"
#include "Interpreter_Delete.h"
#include "Interpreter_Redo.h"
#include "Interpreter_Undo.h"
#include "Interpreter_Sync.h"
*/

class Messenger;
class Command;

using namespace std;

/*
string GENERAL_ADD_REGEX="((( due| from| to| impt| at| ppl| note| rt) `[^`]*`)|( #[^( |`)]*))*(\\s*)";
string GENERAL_MOD_REGEX="((( due| from| name| to| impt| at| ppl| note| rt) `[^`]*`)|( done| undone)|( (#)[^( |`)]*))*(\\s*)";
string GENERAL_FIND_REGEX="((( from| name| to| impt| at| ppl| note| rt) `[^`]*`)|( #[^( |`)]*)|( done| undone)|(( timed| deadline| floating)))*(\\s*)";
*/

enum COMMAND_CATEGORY{ADD_COMMAND,MOD_COMMAND,MOD_EXACT_COMMAND,MOD_INDEX_COMMAND,FIND_COMMAND,FIND_EXACT_COMMAND,DEL_COMMAND, 
	DEL_EXACT_COMMAND, DEL_INDEX_COMMAND, UNDO_COMMAND,REDO_COMMAND,SYNC_COMMAND};


class Interpreter{


public:
	Interpreter() {};

	Command* interpretCommand(std::string commandStr, Messenger &response);
	
	void interpretCommand(unsigned ActualIndex, Command *prevCommand);
	
	int interpretIndex(std::string indexStr, Messenger &response); 

protected:
	bool checkCommand(string command, int& commandType);
    bool integerConvert(string& requiredString, int& number);


};

