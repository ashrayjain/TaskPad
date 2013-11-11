#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TaskPad_library/Manager.h"
#include "../TaskPad_library/Task.h"
#include "../TaskPad_library/Messenger.h"
#include "../TaskPad_library/Enum.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace TP;

namespace UnitTest{
	TEST_CLASS(test_System_Test){
	public:
		
		TEST_METHOD(System_Testing_Basic_CRUD){
			//@XIE KAI A0102016E
			////*************************************** simple test
			////use case:
			////1. user add a task
			////2. user delete the added task
			////
			////TC0: WORK FLOW ADD
			Manager scheduler;
			const char* TC0_TASK_NAME = "Project Kai Testing";
			const char* TEST_ADD_TASK_1 = "add `Project Kai Testing`";
			Messenger msg = scheduler.processCommand(TEST_ADD_TASK_1);
			Assert::AreEqual((int)msg.getStatus(), (int) SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)ADD);
			Assert::AreEqual(msg.getTask().getName(), (string)TC0_TASK_NAME);

			////TC1: WORK FLOW DEL
			const char* TEST_DEL_TASK_1 = "del exact `Project Kai Testing`";
			msg = scheduler.processCommand(TEST_DEL_TASK_1);
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)DEL);
			Assert::AreEqual(msg.getTask().getName(), (string)TC0_TASK_NAME);

			////***************************************
			////use case:
			////1. user add a task
			////2. user modify the added task's note
			////3. user undo the modification
			////4. user redo the modification
			////5. user comes to find a tag
			////6. user deleted the task
			////
			////TC2: WORK FLOW ADD 2
			const char* TEST_ADD_TASK_2 = "add `Project22 Kai Testing` note `TEST NOTE` #KaiTesting";
			msg = scheduler.processCommand(TEST_ADD_TASK_2);
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)ADD);
			Assert::AreEqual(msg.getTask().getName(), (string)"Project22 Kai Testing");
			Assert::AreEqual(msg.getTask().getNote(), (string)"TEST NOTE");

			////TC3: WORK FLOW MOD
			msg = scheduler.processCommand("mod exact `Project22 Kai Testing` note `TEST NOTE2`");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)MOD);
			Assert::AreEqual(msg.getTask().getNote(), (string)"TEST NOTE2");

			////TC4: UNDO
			msg = scheduler.processCommand("undo");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)UNDO);
			Assert::AreEqual(msg.getTask().getNote(), (string)"TEST NOTE");

			////TC5: REDO
			msg = scheduler.processCommand("redo");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)REDO);
			Assert::AreEqual(msg.getTask().getNote(), (string)"TEST NOTE2");

			////TC6: FIND
			msg = scheduler.processCommand("find #KaiTesting");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)FIND);
			Assert::AreEqual(msg.getList().front().getName(), (string)"Project22 Kai Testing");

			////reset
			scheduler.resetStatus();

			////TC7: DEL 2
			msg = scheduler.processCommand("del exact `Project22 Kai Testing`");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(),(int) DEL);
			Assert::AreEqual(msg.getTask().getName(), (string)"Project22 Kai Testing");
		}
	};
}