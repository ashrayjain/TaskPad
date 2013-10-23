#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TaskPad_library/Executor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

	TEST_CLASS(test_Executor)
	{
	private:
		list<Task> data;

	public:
		TEST_METHOD_INITIALIZE(Generate_Test_Data)
		{
			data.clear();
			
			time_t now = time(NULL);

			Task testTask_1 = Task(1);
			testTask_1.setName("Test Task 1");
			testTask_1.setDueDate(now + 864000);
			testTask_1.setLocation("Someplace");
			testTask_1.setNote("This is a sample note!");
			testTask_1.setParticipants(list<string>(5, "Participant X"));
			testTask_1.setPriority(TP::PRIORITY::HIGH);
			testTask_1.setRemindTimes(now + 432000, TP::LIST_OP::ADD_ELEMENT);
			testTask_1.setRemindTimes(now + 691000, TP::LIST_OP::ADD_ELEMENT);
			
			data.push_back(testTask_1);

			Task testTask_2 = Task(2);
			testTask_2.setName("Test Task 2");
			testTask_2.setDueDate(now + 1664000);
			testTask_2.setLocation("Someplace elsewhere");
			testTask_2.setNote("This is another sample note!");
			testTask_2.setParticipants(list<string>(3, "Participant X"));
			testTask_2.setPriority(TP::PRIORITY::LOW);
			testTask_2.setRemindTimes(now + 1032000, TP::LIST_OP::ADD_ELEMENT);
			
			data.push_back(testTask_2);

			Task testTask_3 = Task(3);
			testTask_3.setName("Test Task 3");
			testTask_3.setLocation("Another location");
			testTask_3.setNote("This is the final sample note!");
			testTask_3.setParticipants(list<string>(2, "Participant X"));
			testTask_3.setRemindTimes(now + 232000, TP::LIST_OP::ADD_ELEMENT);
			
			data.push_back(testTask_3);
		}

		TEST_METHOD(Execute_Add_Command_Test)
		{
			Command* testCmd = new Command_Add();
			Command_Add* testAddCmd = dynamic_cast<Command_Add*>(testCmd);
			time_t now = time(NULL);
			Executor testExecutor(&data);
			Messenger testResponse;

			testAddCmd->setName("Test Task");
			testAddCmd->setDueDate(now + 864000);
			testAddCmd->setLocation("Someplace");
			testAddCmd->setNote("This is a sample note!");
			testAddCmd->setTags(list<string>(1, "#testhashtag"));
			testAddCmd->setParticipants(list<string>(5, "Participant X"));
			testAddCmd->setPriority(TP::PRIORITY::HIGH);
			testAddCmd->setRemindTimes(list<time_t>(1, now + 432000));
			
			testExecutor.executeCommand(testCmd, testResponse);

			Assert::IsTrue(string("Test Task") == data.back().getName());
			Assert::IsTrue((now + 864000) == data.back().getDueDate());
			Assert::IsTrue(string("Someplace") == data.back().getLocation());
			Assert::IsTrue(string("This is a sample note!") == data.back().getNote());
			Assert::IsTrue(list<string>(1, "#testhashtag") == data.back().getTags());
			Assert::IsTrue(list<string>(5, "Participant X") == data.back().getParticipants());
			Assert::IsTrue(TP::PRIORITY::HIGH == data.back().getPriority());
			Assert::IsTrue(list<time_t>(1, now + 432000) == data.back().getRemindTimes());
			
			delete testCmd;
		}

		TEST_METHOD(Execute_Del_Command_Test)
		{
			Command* testCmd = new Command_Del();
			Command_Del* testDelCmd = dynamic_cast<Command_Del*>(testCmd);
			Executor testExecutor(&data);
			Messenger testResponse;

			testDelCmd->setCreatedTime(data.back().getIndex());
			testExecutor.executeCommand(testCmd, testResponse);
			Assert::IsTrue(2 == data.size());

			testDelCmd->setCreatedTime(time(NULL));
			testExecutor.executeCommand(testCmd, testResponse);
			Assert::IsTrue(TP::STATUS::ERROR==testResponse.getStatus());

			Command* testCmd_2 = new Command_Del();
			Command_Del* testDelCmd_2 = dynamic_cast<Command_Del*>(testCmd_2);
			testDelCmd_2->setName("Test Task 1");
			testExecutor.executeCommand(testCmd_2, testResponse);
			Assert::IsTrue(1 == data.size());

			Command* testCmd_3 = new Command_Del();
			Command_Del* testDelCmd_3 = dynamic_cast<Command_Del*>(testCmd_3);
			testDelCmd_3->setName("Test Task 2");
			testDelCmd_3->setFlagExact();
			testExecutor.executeCommand(testCmd_3, testResponse);
			Assert::IsTrue(0 == data.size());

			delete testCmd;
			delete testCmd_2;
			delete testCmd_3;
		}

		TEST_METHOD(Execute_Mod_Command_Test)
		{
			Command* testCmd = new Command_Mod();
			Command_Mod* testModCmd = dynamic_cast<Command_Mod*>(testCmd);
			Executor testExecutor(&data);
			Messenger testResponse;

			testModCmd->setCreatedTime(data.back().getIndex());
			testModCmd->setOptName("New Task Name");
			testModCmd->setTaskState(TP::TASK_STATE::DONE);
			testExecutor.executeCommand(testCmd, testResponse);
			Assert::IsTrue("New Task Name" == data.back().getName());
			Assert::IsTrue(TP::TASK_STATE::DONE == data.back().getState());

			testModCmd->setCreatedTime(time(NULL));
			testExecutor.executeCommand(testCmd, testResponse);
			Assert::IsTrue(TP::STATUS::ERROR==testResponse.getStatus());

			time_t now = time(NULL);
			Command* testCmd_2 = new Command_Mod();
			Command_Mod* testModCmd_2 = dynamic_cast<Command_Mod*>(testCmd_2);
			testModCmd_2->setName("Test Task 2");
			testModCmd_2->setFlagExact();
			testModCmd_2->setTags(list<string>(1, "#hashtag"));
			testModCmd_2->setRemindTimes(list<time_t>(1, now + 32000));
			testModCmd_2->setTaskState(TP::TASK_STATE::DONE);
			testExecutor.executeCommand(testCmd_2, testResponse);
			Assert::IsTrue(list<string>(1, "#hashtag") == (++data.begin())->getTags());
			Assert::IsTrue(list<time_t>(1, now + 32000) == (++data.begin())->getRemindTimes());
			Assert::IsTrue(TP::TASK_STATE::DONE == (++data.begin())->getState());
			
			delete testCmd;
			delete testCmd_2;
		}

		TEST_METHOD(Execute_Find_Command_Test)
		{
			Command* testCmd = new Command_Find();
			Command_Find* testFindCmd = dynamic_cast<Command_Find*>(testCmd);
			time_t now = time(NULL);
			Executor testExecutor(&data);
			Messenger testResponse;

			testFindCmd->setOptName("Test Task");
			testFindCmd->setFlagExact();

			testExecutor.executeCommand(testCmd, testResponse);
			Assert::IsTrue(0 == testResponse.getList().size());
			
			testFindCmd->setOptName("Test Task 1");
			testExecutor.executeCommand(testCmd, testResponse);
			Assert::IsTrue(1 == testResponse.getList().size());
			
			Command* testCmd_2 = new Command_Find();
			Command_Find* testFindCmd_2 = dynamic_cast<Command_Find*>(testCmd_2);

			testFindCmd_2->setOptName("Test");
			testExecutor.executeCommand(testCmd_2, testResponse);
			Assert::IsTrue(3 == testResponse.getList().size());
			
			testFindCmd_2->setToDate(time(NULL) + 2000000);
			testExecutor.executeCommand(testCmd_2, testResponse);
			Assert::IsTrue(0 == testResponse.getList().size());
			
			testFindCmd_2->setTaskType(TP::TASK_TYPE::DEADLINE);
			testFindCmd_2->setFromDate(time(NULL) - 100000);
			testExecutor.executeCommand(testCmd_2, testResponse);
			Assert::IsTrue(2 == testResponse.getList().size());
			
			Command* testCmd_3 = new Command_Find();
			Command_Find* testFindCmd_3 = dynamic_cast<Command_Find*>(testCmd_3);

			testFindCmd_3->setIndex(2);
			testExecutor.executeCommand(testCmd_3, testResponse);
			Assert::IsTrue(2 == testResponse.getTask().getIndex());

			Command* testCmd_4 = new Command_Find();
			Command_Find* testFindCmd_4 = dynamic_cast<Command_Find*>(testCmd_4);

			testFindCmd_4->setTaskType(TP::TASK_TYPE::FLOATING);
			testExecutor.executeCommand(testCmd_4, testResponse);
			Assert::IsTrue(1 == testResponse.getList().size());

			testFindCmd_4->setTaskType(TP::TASK_TYPE::DEADLINE);
			testFindCmd_4->setFromDate(time(NULL) - 1000000);
			testExecutor.executeCommand(testCmd_4, testResponse);
			Assert::IsTrue(2 == testResponse.getList().size());
			delete testCmd;
			delete testCmd_2;
			delete testCmd_3;
			delete testCmd_4;
		}
	};
}