#include "stdafx.h"
#include "CppUnitTest.h"

#include "../TaskPad_library/Interpreter.h"
#include "../TaskPad_library/Messenger.h"
#include "../TaskPad_library/Command.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(test_Interpreter)
	{
	public:
		
		
		TEST_METHOD(Add)
		{
			// TODO: Your test code here
		
		Interpreter test;
		Command* testCommand;  
		
		Messenger response;
	


        // For test case, normally, I use equivalence pattitioning to find bugs.
		// The first partition: Normal case.
		string case1="add `ww` at `nus` note `study`";
		testCommand=test.interpretCommand(case1,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		string case2="add `ww` #qq #pp";
		testCommand=test.interpretCommand(case2,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		string case4="add `ww` ppl `a,b,c` note `study`";
		testCommand=test.interpretCommand(case4,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		// The second partition: empty content. 
		string case5="add `ww`"; 
		testCommand=test.interpretCommand(case5,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		
		string case6="add `ww` at ``";
		testCommand=test.interpretCommand(case6,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		string case7="add `ww` note ``";
		testCommand=test.interpretCommand(case7,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		string case9="add `ww` at `nus` note `study";
		testCommand=test.interpretCommand(case9,response);
		Assert::AreEqual(0,(int)response.getStatus());
		
		string case10="add `ww``nus` note `study`";
		testCommand=test.interpretCommand(case10,response);
		Assert::AreEqual(0,(int)response.getStatus());
		}
		
		TEST_METHOD(Mod)
		{
			// TODO: Your test code here
		
		Interpreter test;
		Command* testCommand;  
		Messenger response;
	   
		// The first partition: Normal case.
		string case1="mod `ww` at `nus` note `study`";
		testCommand=test.interpretCommand(case1,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		string case2="mod `ww` #qq #pp";
		testCommand=test.interpretCommand(case2,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		string case4="mod `ww` ppl `a,b,c` note `study`";
		testCommand=test.interpretCommand(case4,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		// The second partition: empty content. 
		string case5="mod `ww`"; 
		testCommand=test.interpretCommand(case5,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		string case6="mod `ww` at ``";
		testCommand=test.interpretCommand(case6,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		string case7="mod `ww` note ``";
		testCommand=test.interpretCommand(case7,response);
		Assert::AreEqual(2,(int)response.getStatus());
		
		string case9="mod `ww` at `nus` note `study";
		testCommand=test.interpretCommand(case9,response);
		Assert::AreEqual(0,(int)response.getStatus());
		
		string case10="mod `ww``nus` note `study`";
	    testCommand=test.interpretCommand(case10,response);
		Assert::AreEqual(0,(int)response.getStatus());
	
		
		// For modify, also could check the boundary condition

		 string case11 = "mod -1 name `pp`";
	     testCommand=test.interpretCommand(case11,response);
		 Assert::AreEqual(0,(int)response.getStatus());
		
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