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

namespace UnitTest
{
	TEST_CLASS(test_System_Test)
	{
	public:
		
		TEST_METHOD(System_Testing_Basic_CRUD)
		{
			//*************************************** simple test
			//TC0: WORK FLOW ADD
			Manager scheduler;
			Messenger msg = scheduler.processCommand("add `Project Kai Testing`");
			Assert::AreEqual((int)msg.getStatus(), (int) SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)ADD);
			Assert::AreEqual(msg.getTask().getName(), (string)"Project Kai Testing");

			//TC1: WORK FLOW DEL
			msg = scheduler.processCommand("del exact `Project Kai Testing`");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)DEL);
			Assert::AreEqual(msg.getTask().getName(), (string)"Project Kai Testing");

			//***************************************
			//TC2: WORK FLOW ADD 2
			msg = scheduler.processCommand("add `Project22 Kai Testing` note `TEST NOTE` #KaiTesting");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)ADD);
			Assert::AreEqual(msg.getTask().getName(), (string)"Project22 Kai Testing");
			Assert::AreEqual(msg.getTask().getNote(), (string)"TEST NOTE");

			//TC3: WORK FLOW MOD
			msg = scheduler.processCommand("mod exact `Project22 Kai Testing` note `TEST NOTE2`");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)MOD);
			Assert::AreEqual(msg.getTask().getNote(), (string)"TEST NOTE2");

			//TC4: UNDO
			msg = scheduler.processCommand("undo");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)UNDO);
			Assert::AreEqual(msg.getTask().getNote(), (string)"TEST NOTE");

			//TC5: REDO
			msg = scheduler.processCommand("redo");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)REDO);
			Assert::AreEqual(msg.getTask().getNote(), (string)"TEST NOTE2");

			//TC6: FIND
			msg = scheduler.processCommand("find #KaiTesting");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(), (int)FIND);
			Assert::AreEqual(msg.getList().front().getName(), (string)"Project22 Kai Testing");

			//reset
			scheduler.resetStatus();

			//TC7: DEL 2
			msg = scheduler.processCommand("del exact `Project22 Kai Testing`");
			Assert::AreEqual((int)msg.getStatus(), (int)SUCCESS);
			Assert::AreEqual((int)msg.getCommandType(),(int) DEL);
			Assert::AreEqual(msg.getTask().getName(), (string)"Project22 Kai Testing");
		}
	};
}