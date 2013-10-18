#include "stdafx.h"
#include "CppUnitTest.h"

#include "../TaskPad_library/Interpreter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(test_Interpreter)
	{
	public:
		
		
		TEST_METHOD(Add)
		{
			// TODO: Your test code here
		
		Interpreter test;
		Command* tt;
		string pp="add `ww` at `nus` note `study`";
		
		Messenger response;
		
		tt=test.interpretCommand(pp,response);
		
		Command_Add* ttt=(Command_Add*)tt;
		
		string testone = ttt->getLocation();
		
		TP::STATUS get=response.getStatus();
		Assert:: AreEqual((string)"nus", (string)testone);
		
		}
		
		TEST_METHOD(Mod)
		{
			// TODO: Your test code here
		
		Interpreter test;
		Command* tt;
		string pp="mod `aaa` at `nus`";
		
		Messenger response;
		
		tt=test.interpretCommand(pp,response);
		
		Command_Mod* ttt=(Command_Mod*)tt;
		
		string testone = ttt->getNote();
		
		TP::STATUS get=response.getStatus();
		Assert:: AreEqual((string)"", (string)testone);
		
		}

		
		TEST_METHOD(Find)
		{
			// TODO: Your test code here
		
		Interpreter test;
		Command* tt;
		string pp="find at `nus` note `study` undone";
		
		Messenger response;
		
		tt=test.interpretCommand(pp,response);
		
		Command_Find* ttt=(Command_Find*)tt;
		
		string testone = ttt->getLocation();
		
		TP::STATUS get=response.getStatus();
		Assert:: AreEqual((string)"nus", (string)testone);
		
		}



	};
}