#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TaskPad_library/Messenger.h"
#include "../TaskPad_library/Task.h"
#include "../TaskPad_library/Enum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace TP;

namespace UnitTest
{		
	TEST_CLASS(test_Messenger)
	{
	public:
		
		TEST_METHOD(Getter_Messenger)
		{
			Task myTasks[] = {DeadlineTask(),TimedTask(),FloatingTask(),DeadlineTask()};
			Messenger testMessenger(ERROR_INTERMEDIATE,list<Task>(myTasks,myTasks + 4),10,"This is a test error Message");
			const char* msg= "Status not equal!";

			Assert::AreEqual(testMessenger.getErrorMsg(),string("This is a test error Message"));
			Assert::AreEqual(testMessenger.getIndex(),10);
			//Assert::AreEqual(testMessenger.getStatus(),ERROR_INTERMEDIATE,false);

			list<Task> returnList = testMessenger.getList();
			//Assert::AreEqual(returnList.front().getTaskType(),DEADLINE);


		}
	};
}