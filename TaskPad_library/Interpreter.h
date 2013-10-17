#pragma once
#include "Command.h"
#include "Messenger.h"
#include "Enum.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Interpreter
{
private:
	static const string DELIMITER;

	typedef struct KEY_WORDS
	{
		int index;
		string keyWord;
	};

	//variables
	vector<string>listOfWords;
	vector<KEY_WORDS>listOfKeyWords;

	vector<string>strUT;
	list<string>pplUT;
	list<string>tagsUT;

	string _command;
	bool _isSuccess;
	Command* _result;
	Messenger _response;

public:
	Interpreter(void);
	~Interpreter(void);

	//auxillary functions for UnitTest
	string getIndividualWord(int userIndex);
	string getstrUT(int userIndex);
	string getKeyWord(int userIndex);
	int getListOfKeyWordsSize();
	Command* get_cmdObject(string command);
	string getPplUT(int userIndex);
	string getTagsUT(int userIndex);

	//auxillary functions for component functions
	bool isKeyWordValid(int prevSysWordIndex);
	void eliminateFalseKeyWords();
	void checkForDuplicateKeyWords();
	string reconstructInputInfo(int incrementIdx, int numberOfKeyWords);
	string reconstructName(int startIndex, int endIndex);
	void allocatePpl(string keyWord, list<string>&ppl, string inputInfo);
	void allocateRt(string keyWord, list<time_t>&remindTime, string inputInfo);
	void allocateTags(string keyWord, list<string>&tags, string inputInfo);

	
	unsigned int convertStrToUI(string keyWord, string inputInfo);
	PRIORITY convertStrToPriority(string keyWord, string inputInfo);
	time_t setTime(string keyWord, string inputInfo);
	bool integerConverter(string& requiredString, int& number);
	bool isByIndex();

	void setCmdObj_Add(Command_Add* cmd, string keyWord, string inputInfo, unsigned int time, PRIORITY priority, list<string>ppl, list<string>tags, list<time_t>remindTime);
	void setCmdObj_Mod(Command_Mod* cmd, string keyWord, string inputInfo, unsigned int time, PRIORITY priority, list<string>ppl, list<string>tags, list<time_t>remindTime);
	void setCmdObj_Find(Command_Find* cmd, string keyWord, string inputInfo, unsigned int time, PRIORITY priority, list<string>ppl, list<string>tags, list<time_t>remindTime);
	//void setCmdObj_Del(Command_Del* cmd);

	//component functions
	void functionAdd();
	void functionMod();
	void functionDel();
	void functionFind();

	//principal functions
	void extractIndividualWords(string userInput);
	void extractCommand();
	void extractKeyWords();
	void processKeyWords();
	void processCommand();
	
public:
	//APIs
	Command * interpretCommand(string userInput, Messenger & response);
	int interpretIndex(string indexStr, Messenger & response);
	void interpretCommand(unsigned int index, Command* prevCommand);

};

