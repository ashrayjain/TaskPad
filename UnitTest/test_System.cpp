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
			const char* TEST_ADD_SIMPLE_TASK = "add `Project Kai Testing`";
			Messenger msg = scheduler.processCommand(TEST_ADD_SIMPLE_TASK);
			Assert::AreEqual((int)msg.getStatus(), (int) SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)ADD);
			Assert::AreEqual(msg.getTask().getName(), (string)TC0_TASK_NAME);

			////TC1: WORK FLOW DEL
			const char* TEST_DEL_SIMPLE_TASK = "del exact `Project Kai Testing`";
			msg = scheduler.processCommand(TEST_DEL_SIMPLE_TASK);
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
			const char* TEST_ADD_TASK_ID_2 = "add `Project22 Kai Testing` note `TEST NOTE` #KaiTesting";
			const char* TC2_TASK_NAME = "Project22 Kai Testing";
			const char* TC2_NOTE = "TEST NOTE";
			msg = scheduler.processCommand(TEST_ADD_TASK_ID_2);
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)ADD);
			Assert::AreEqual(msg.getTask().getName(), (string)TC2_TASK_NAME);
			Assert::AreEqual(msg.getTask().getNote(), (string)TC2_NOTE);

			////TC3: WORK FLOW MOD
			const char* TEST_MOD_TASK_ID_2 = "mod exact `Project22 Kai Testing` note `TEST NOTE2`";
			const char* TC3_NOTE = "TEST NOTE2";
			msg = scheduler.processCommand(TEST_MOD_TASK_ID_2);
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)MOD);
			Assert::AreEqual(msg.getTask().getNote(), (string)TC3_NOTE);

			////TC4: UNDO
			const char* TEST_UNDO_CMD_MOD = "undo";
			msg = scheduler.processCommand(TEST_UNDO_CMD_MOD);
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)UNDO);
			Assert::AreEqual(msg.getTask().getNote(), (string)TC2_NOTE);

			////TC5: REDO
			const char* TEST_REDO_CMD_MOD = "redo";
			msg = scheduler.processCommand(TEST_REDO_CMD_MOD);
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)REDO);
			Assert::AreEqual(msg.getTask().getNote(), (string)TC3_NOTE);

			////TC6: FIND
			const char* TEST_FIND_TAG = "find #KaiTesting";
			msg = scheduler.processCommand(TEST_FIND_TAG);
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)FIND);
			Assert::AreEqual(msg.getList().front().getName(), (string)TC2_TASK_NAME);

			////reset
			scheduler.resetStatus();

			////TC7: DEL 2
			const char* TEST_DEL_EXACT = "del exact `Project22 Kai Testing`";
			msg = scheduler.processCommand(TEST_DEL_EXACT);
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(),(int) DEL);
			Assert::AreEqual(msg.getTask().getName(), (string)TC2_TASK_NAME);
		}
	};
}