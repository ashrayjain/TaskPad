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
			Messenger testMessenger(ADD,ERROR_INTERMEDIATE,list<Task>(myTasks,myTasks + 4),10,"This is a test error Message");

			Assert::AreEqual(testMessenger.getErrorMsg(),string("This is a test error Message"));
			Assert::AreEqual(testMessenger.getIndex(),10);
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(ERROR_INTERMEDIATE));
			Assert::AreEqual(static_cast<int>(testMessenger.getCommandType()),static_cast<int>(ADD));

			list<Task>::iterator returnList = testMessenger.getList().begin();
			Assert::AreEqual(static_cast<int>((returnList++)->getTaskType()),static_cast<int>(DEADLINE));
			Assert::AreEqual(static_cast<int>((returnList++)->getTaskType()),static_cast<int>(TIMED));
			Assert::AreEqual(static_cast<int>((returnList++)->getTaskType()),static_cast<int>(FLOATING));
			Assert::AreEqual(static_cast<int>((returnList++)->getTaskType()),static_cast<int>(DEADLINE));
		}
		TEST_METHOD(Setter_Messenger)
		{
			Messenger testMessenger;

			//************* TEST SetErrorMsg() *********************//
			Assert::IsTrue(testMessenger.setErrorMsg("This is a test error Message"));
			Assert::AreEqual(testMessenger.getErrorMsg(),string("This is a test error Message"));

			Assert::IsFalse(testMessenger.setErrorMsg(""));
			Assert::AreEqual(testMessenger.getErrorMsg(),string("This is a test error Message"));

			Assert::IsTrue(testMessenger.setErrorMsg("a"));
			Assert::AreEqual(testMessenger.getErrorMsg(),string("a"));
			//************* ENDTEST SetErrorMsg() *********************//

			//************* TEST SetInt() *********************//
			Assert::IsTrue(testMessenger.setInt(10));
			Assert::AreEqual(testMessenger.getIndex(),10);

			Assert::IsTrue(testMessenger.setInt(1));
			Assert::AreEqual(testMessenger.getIndex(),1);

			Assert::IsFalse(testMessenger.setInt(-1));
			Assert::AreEqual(testMessenger.getIndex(),1);

			Assert::IsFalse(testMessenger.setInt(0));
			Assert::AreEqual(testMessenger.getIndex(),1);

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

			list<Task>::iterator returnList = testMessenger.getList().begin();
			Assert::AreEqual(static_cast<int>((returnList++)->getTaskType()),static_cast<int>(DEADLINE));
			Assert::AreEqual(static_cast<int>((returnList++)->getTaskType()),static_cast<int>(TIMED));
			Assert::AreEqual(static_cast<int>((returnList++)->getTaskType()),static_cast<int>(FLOATING));
			Assert::AreEqual(static_cast<int>((returnList++)->getTaskType()),static_cast<int>(DEADLINE));

			Task myTasks1[] = {TimedTask(),TimedTask(),FloatingTask()};
			Assert::IsTrue(testMessenger.setList(list<Task>(myTasks1,myTasks1+3)));

			list<Task>::iterator returnList1 = testMessenger.getList().begin();
			Assert::AreEqual(static_cast<int>((returnList1++)->getTaskType()),static_cast<int>(TIMED));
			Assert::AreEqual(static_cast<int>((returnList1++)->getTaskType()),static_cast<int>(TIMED));
			Assert::AreEqual(static_cast<int>((returnList1++)->getTaskType()),static_cast<int>(FLOATING));

			Assert::IsTrue(testMessenger.setList(list<Task>()));

			list<Task>::iterator returnList2 = testMessenger.getList().begin();
			Assert::IsTrue((returnList2 == testMessenger.getList().end()));

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