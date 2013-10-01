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
			string testErrorStr = "This is a test error Message";
			Messenger testMessenger(ADD,ERROR_INTERMEDIATE,list<Task>(myTasks,myTasks + 4),10,testErrorStr);

			Assert::AreEqual(testMessenger.getErrorMsg(),string(testErrorStr));
			Assert::AreEqual(testMessenger.getIndex(),10);
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(ERROR_INTERMEDIATE));
			Assert::AreEqual(static_cast<int>(testMessenger.getCommandType()),static_cast<int>(ADD));

			list<Task> returnList = testMessenger.getList();
			list<Task>::iterator it = returnList.begin();
			Assert::AreEqual((int) testMessenger.getList().size(),4);
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(DEADLINE));
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(TIMED));
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(FLOATING));
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(DEADLINE));
		}
		TEST_METHOD(Setter_Messenger)
		{
			Messenger testMessenger;

			//************* TEST SetErrorMsg() *********************//
			string testErrorStr = "This is a test error Message";
			string testErrorStr2 = "This is another test error Message";

			Assert::IsTrue(testMessenger.setErrorMsg(testErrorStr));
			Assert::AreEqual(testMessenger.getErrorMsg(),string(testErrorStr));

			Assert::IsFalse(testMessenger.setErrorMsg(""));
			Assert::AreEqual(testMessenger.getErrorMsg(),string(testErrorStr));

			Assert::IsTrue(testMessenger.setErrorMsg(testErrorStr2));
			Assert::AreEqual(testMessenger.getErrorMsg(),string(testErrorStr2));
			//************* ENDTEST SetErrorMsg() *********************//

			//************* TEST SetInt() *********************//
			int testInt1 = 10;
			int testInt2 = 1;
			int testInt3 = -1;
			int testInt4 = 0;


			Assert::IsTrue(testMessenger.setInt(testInt1));
			Assert::AreEqual(testMessenger.getIndex(),testInt1);

			Assert::IsTrue(testMessenger.setInt(testInt2));
			Assert::AreEqual(testMessenger.getIndex(),testInt2);

			Assert::IsFalse(testMessenger.setInt(testInt3));
			Assert::AreEqual(testMessenger.getIndex(),testInt2);

			Assert::IsFalse(testMessenger.setInt(testInt4));
			Assert::AreEqual(testMessenger.getIndex(),testInt2);

			//************* END TEST SetInt() *********************//

			//************* TEST SetStatus() *********************//
			Assert::IsTrue(testMessenger.setStatus(SUCCESS));
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(SUCCESS));

			Assert::IsTrue(testMessenger.setStatus(SUCCESS_INDEXED_COMMAND));
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(SUCCESS_INDEXED_COMMAND));

			Assert::IsTrue(testMessenger.setStatus(ERROR));
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(ERROR));

			Assert::IsTrue(testMessenger.setStatus(ERROR_INTERMEDIATE));
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(ERROR_INTERMEDIATE));

			Assert::IsTrue(testMessenger.setStatus(INTERMEDIATE));
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(INTERMEDIATE));

			//************* END TEST SetStatus() *********************//

			//************* TEST SetList() *********************//
			Task myTasks[] = {DeadlineTask(),TimedTask(),FloatingTask(),DeadlineTask()};
			Assert::IsTrue(testMessenger.setList(list<Task>(myTasks,myTasks+4)));

			list<Task> returnList = testMessenger.getList();
			list<Task>::iterator it = returnList.begin();
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(DEADLINE));
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(TIMED));
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(FLOATING));
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(DEADLINE));

			Task myTasks1[] = {TimedTask(),TimedTask(),FloatingTask()};
			Assert::IsTrue(testMessenger.setList(list<Task>(myTasks1,myTasks1+3)));

			returnList = testMessenger.getList();
			it = returnList.begin();
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(TIMED));
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(TIMED));
			Assert::AreEqual(static_cast<int>((it++)->getTaskType()),static_cast<int>(FLOATING));

			Assert::IsTrue(testMessenger.setList(list<Task>()));

			Assert::IsTrue((testMessenger.getList().size() == 0));

			//************* END TEST SetList() *********************//

			//************* END TEST SetCommandType() *********************//

			Assert::IsTrue(testMessenger.setCommandType(DEL));
			Assert::AreEqual(static_cast<int>(DEL),static_cast<int>(testMessenger.getCommandType()));

			Assert::IsTrue(testMessenger.setCommandType(ADD));
			Assert::AreEqual(static_cast<int>(ADD),static_cast<int>(testMessenger.getCommandType()));

			Assert::IsTrue(testMessenger.setCommandType(MOD));
			Assert::AreEqual(static_cast<int>(MOD),static_cast<int>(testMessenger.getCommandType()));

			Assert::IsTrue(testMessenger.setCommandType(SYNC));
			Assert::AreEqual(static_cast<int>(SYNC),static_cast<int>(testMessenger.getCommandType()));

			Assert::IsTrue(testMessenger.setCommandType(UNDO));
			Assert::AreEqual(static_cast<int>(UNDO),static_cast<int>(testMessenger.getCommandType()));

			//************* TEST SetCommandType() *********************//
		}
	};
}