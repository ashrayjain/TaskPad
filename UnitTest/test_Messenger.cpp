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
		
		TEST_METHOD(Messenger_Getter)
		{
			string nameT1 = "t1";
			string nameT2 = "t2";
			string nameT3 = "t3";
			string nameT4 = "t4";
			string nameT5 = "t5";

			Task t1 = Task(nameT1);
			Task t2 = Task(nameT2);
			Task t3 = Task(nameT3);
			Task t4 = Task(nameT4);
			Task t5 = Task(nameT5);

			Task myTasks[] = {t1,t2,t3,t4};

			list<Task> taskList= list<Task>(myTasks,myTasks + 4);

			string testErrorStr = "This is a test error Message";
			Messenger testMessenger(ADD,ERROR_INTERMEDIATE,taskList,t5,10,testErrorStr);

			Assert::AreEqual(testMessenger.getErrorMsg(),string(testErrorStr));
			Assert::AreEqual(testMessenger.getIndex(),10);
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(ERROR_INTERMEDIATE));
			Assert::AreEqual(static_cast<int>(testMessenger.getCommandType()),static_cast<int>(ADD));

			list<Task> returnList = testMessenger.getList();
			list<Task>::iterator it = returnList.begin();
			Assert::AreEqual((int) testMessenger.getList().size(),4);

			Assert::AreEqual(((it++)->getName()),nameT1);
			Assert::AreEqual(((it++)->getName()),nameT2);
			Assert::AreEqual(((it++)->getName()),nameT3);
			Assert::AreEqual(((it++)->getName()),nameT4);

			Assert::AreEqual(testMessenger.getTask().getName(),nameT5);
		}
		TEST_METHOD(Messenger_Set_Error_String)
		{
			Messenger testMessenger;

			string testErrorStr = "This is a test error Message";
			string testErrorStr1 = "";
			string testErrorStr2 = "This is another test error Message";

			testMessenger.setErrorMsg(testErrorStr);
			Assert::AreEqual(testMessenger.getErrorMsg(),string(testErrorStr));

			testMessenger.setErrorMsg(testErrorStr1);
			Assert::AreEqual(testMessenger.getErrorMsg(),testErrorStr1);

			testMessenger.setErrorMsg(testErrorStr2);
			Assert::AreEqual(testMessenger.getErrorMsg(),string(testErrorStr2));
		}
		TEST_METHOD(Messenger_Set_Int)
		{
			Messenger testMessenger;

			int testInt1 = 10;
			int testInt3 = -1;
			int testInt4 = 0;

			testMessenger.setInt(testInt1);
			Assert::AreEqual(testMessenger.getIndex(),testInt1);

			testMessenger.setInt(testInt3);
			Assert::AreEqual(testMessenger.getIndex(),testInt3);

			testMessenger.setInt(testInt4);
			Assert::AreEqual(testMessenger.getIndex(),testInt4);

		}
		TEST_METHOD(Messenger_Set_Status)
		{
			Messenger testMessenger;

			testMessenger.setStatus(SUCCESS);
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(SUCCESS));

			testMessenger.setStatus(ERROR);
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(ERROR));

			testMessenger.setStatus(INTERMEDIATE);
			Assert::AreEqual(static_cast<int>(testMessenger.getStatus()),static_cast<int>(INTERMEDIATE));

		}
		TEST_METHOD(Messenger_Set_List)
		{
			Messenger testMessenger;

			Task myTasks[] = {Task(1),Task(2),Task(3),Task(4)};
			testMessenger.setList(list<Task>(myTasks,myTasks+4));

			list<Task> returnList = testMessenger.getList();
			list<Task>::iterator it = returnList.begin();
			Assert::AreEqual(((it++)->getIndex()),(unsigned long long) 1);
			Assert::AreEqual(((it++)->getIndex()),(unsigned long long) 2);
			Assert::AreEqual(((it++)->getIndex()),(unsigned long long) 3);
			Assert::AreEqual(((it++)->getIndex()),(unsigned long long) 4);

			testMessenger.setList(list<Task>());

			Assert::IsTrue((testMessenger.getList().size() == 0));
		}
		TEST_METHOD(Messenger_Set_CommandType)
		{
			Messenger testMessenger;

			testMessenger.setCommandType(DEL);
			Assert::AreEqual(static_cast<int>(DEL),static_cast<int>(testMessenger.getCommandType()));

			testMessenger.setCommandType(ADD);
			Assert::AreEqual(static_cast<int>(ADD),static_cast<int>(testMessenger.getCommandType()));

			testMessenger.setCommandType(SYNC);
			Assert::AreEqual(static_cast<int>(SYNC),static_cast<int>(testMessenger.getCommandType()));

			testMessenger.setCommandType(UNDO);
			Assert::AreEqual(static_cast<int>(UNDO),static_cast<int>(testMessenger.getCommandType()));

		}
		TEST_METHOD(Messenger_Set_Task)
		{
			Messenger testMessenger;

			Task tempTask("tempName");
			testMessenger.setTask(tempTask);
			Assert::AreEqual(testMessenger.getTask().getName(),tempTask.getName());
		}
		TEST_METHOD(Messenger_Reset)
		{
			string nameT1 = "t1";
			string nameT2 = "t2";
			string nameT3 = "t3";
			string nameT4 = "t4";
			string nameT5 = "t5";

			Task t1 = Task(nameT1);
			Task t2 = Task(nameT2);
			Task t3 = Task(nameT3);
			Task t4 = Task(nameT4);
			Task t5 = Task(nameT5);

			Task myTasks[] = {t1,t2,t3,t4};

			list<Task> taskList= list<Task>(myTasks,myTasks + 4);

			string testErrorStr = "This is a test error Message";
			Messenger testMessenger(ADD,ERROR_INTERMEDIATE,taskList,t5,10,testErrorStr);

			testMessenger.resetMessenger();

			Assert::AreEqual(testMessenger.getErrorMsg()		,	Messenger::DEFAULT_ERROR_MESSAGE_VALUE);
			Assert::AreEqual(testMessenger.getIndex()			,	Messenger::DEFAULT_INDEX_VALUE);
			Assert::IsTrue	(testMessenger.getStatus()			==	Messenger::DEFAULT_STATUS_VALUE);
			Assert::IsTrue	(testMessenger.getCommandType()		==	Messenger::DEFAULT_COMMAND_TYPE_VALUE);
			Assert::IsTrue	(testMessenger.getTask().getIndex()	==	Messenger::DEFAULT_TASK_VALUE.getIndex());
			Assert::IsTrue	(testMessenger.getList().size()		==	Messenger::DEFAULT_TASK_LIST_VALUE.size());
		}
	};
}