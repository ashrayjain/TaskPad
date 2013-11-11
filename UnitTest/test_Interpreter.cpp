#include "stdafx.h"
#include "CppUnitTest.h"

#include "../TaskPad_library/Interpreter.h"
#include "../TaskPad_library/Messenger.h"
#include "../TaskPad_library/Command.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

//valid command
const string ADD_CASE_1="add `test` ppl `jiangze` note `CS2103` at `nus`";
const string ADD_CASE_2="add `test` impt `H` #: #:P #T.T";
const string ADD_CASE_3="add `test` impt `low`";
const string ADD_CASE_4="add `test` note `study`";
const string ADD_CASE_5="add `test` #a #b #c note `_|_`";
const string ADD_CASE_6="add `test` at `nus`  note `  note   `";
//invalid command
const string ADD_CASE_7="add `test` note `nus` note `111`";
const string ADD_CASE_8="add `test` at `home` at `wc`";
const string ADD_CASE_9="add `test` 10";
const string ADD_CASE_10="add `test` dddddddd";
//valid command
const string MOD_CASE_1="mod `test` ppl `jiangze` note `CS2103` at `nus`";
const string MOD_CASE_2="mod `test` impt `H` #: #:P #T.T";
const string MOD_CASE_3="mod 1 from at `home`";
const string MOD_CASE_4="mod  exact `test` note `study`";
const string MOD_CASE_5="add  exact `test`";
const string MOD_CASE_6="mod `test` at `nus` -note `  note   `";
const string MOD_CASE_7="mod `test` -ppl `a`";
const string MOD_CASE_8="mod `test` -from";
const string MOD_CASE_9="mod `test` -due";
const string MOD_CASE_10="mod `test` -rtall";
//invalid command
const string MOD_CASE_11="mod `test` note `dd` note `123`";
const string MOD_CASE_12="mod `test` at `home` at `nus`";
const string MOD_CASE_13="mod `test` 1111";
const string MOD_CASE_14="mod `test` dddddddd";
const string MOD_CASE_15="mod exact exact";
//valid command
const string FIND_CASE_1="find name `test`";
const string FIND_CASE_2="find undone";
const string FIND_CASE_3="find #123";
const string FIND_CASE_4="find timed";
const string FIND_CASE_5="find floating";
const string FIND_CASE_6="find overdue";
//invalid command
const string FIND_CASE_7="find duee `tomorrow`";
const string FIND_CASE_8="find `test`";
const string FIND_CASE_9="find timed timed";
const string FIND_CASE_10="find `test` dddddddd";

//valid command
const string DEL_CASE_1="del `test`";
const string DEL_CASE_2="del exact `test`";
const string DEL_CASE_3="del 1"; 

namespace UnitTest
{
	TEST_CLASS(test_Interpreter)
	{
	public:


		TEST_METHOD(Interpreter_Add)
		{
			// TODO: Your test code here

			Interpreter test;
			Command* testCommand;  

			Messenger response;

			//CASE 1 to 6 are valid, 7 to 10 are invalid	
			testCommand=test.interpretCommand(ADD_CASE_1,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(ADD_CASE_2,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(ADD_CASE_3,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(ADD_CASE_4,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(ADD_CASE_5,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(ADD_CASE_6,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(ADD_CASE_7,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(ADD_CASE_8,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(ADD_CASE_9,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(ADD_CASE_10,response);
			Assert::AreEqual(0,(int)response.getStatus());

		}


		TEST_METHOD(Interpreter_Mod)
		{

			//Case 1 to 10 are valid, 11 to 15 are invalid
			Interpreter test;
			Command* testCommand;  
			Messenger response;

			testCommand=test.interpretCommand(MOD_CASE_1,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_2,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_3,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_4,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_5,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_6,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_7,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_8,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_9,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_10,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_11,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_12,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_13,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_14,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(MOD_CASE_15,response);
			Assert::AreEqual(0,(int)response.getStatus());

		}


		TEST_METHOD(Interpreter_Find)
		{
			Interpreter test;
			Command* testCommand;  
			Messenger response;


			//CASE 1 to 6 are valid, 7 to 10 are invalid	
			testCommand=test.interpretCommand(FIND_CASE_1,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(FIND_CASE_2,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(FIND_CASE_3,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(FIND_CASE_4,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(FIND_CASE_5,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(FIND_CASE_6,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(FIND_CASE_7,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(FIND_CASE_8,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(FIND_CASE_9,response);
			Assert::AreEqual(0,(int)response.getStatus());

			testCommand=test.interpretCommand(FIND_CASE_10,response);
			Assert::AreEqual(0,(int)response.getStatus());

		}		

		TEST_METHOD(Interpreter_DEL){
			Interpreter test;
			Command* testCommand;  
			Messenger response;
			testCommand=test.interpretCommand(DEL_CASE_1,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(DEL_CASE_2,response);
			Assert::AreEqual(2,(int)response.getStatus());

			testCommand=test.interpretCommand(DEL_CASE_3,response);
			Assert::AreEqual(2,(int)response.getStatus());


		}

	};
}