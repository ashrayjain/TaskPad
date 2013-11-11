#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TaskPad_library/Command.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest
{		
	TEST_CLASS(test_Command)
	{
	public:
		
		TEST_METHOD(cmd_Add)
		{
			//TC0: Command_Add --> Command
			Command_Add* cmd_add_interpreter = new Command_Add();
			cmd_add_interpreter->setName("FML 2014");//compulsory field
			cmd_add_interpreter->setDueDate(time(NULL));
			Command* cmd = cmd_add_interpreter;//to return
			Assert::AreEqual(static_cast<int>(TP::ADD), static_cast<int>(cmd->getCommandType()));

			//TC1: Command --> Command_Add
			Command_Add* cmd_add_executor;
			bool isDueDateSet = false;
			if( cmd->getCommandType() == TP::ADD )
				cmd_add_executor = dynamic_cast<Command_Add*>(cmd);
			if( cmd_add_executor->getFlagDue() )
				isDueDateSet = true;
			Assert::AreEqual(true, isDueDateSet);
			Assert::AreEqual((string)"FML 2014", cmd_add_executor->getName());
			
			delete cmd;
		}

		TEST_METHOD(cmd_Modify)
		{
			//TC0
			Command* cmd = new Command_Mod();
			Assert::AreEqual(static_cast<int>(TP::MOD), static_cast<int>(cmd->getCommandType()));
			
			Command_Mod* cmd_mod = dynamic_cast<Command_Mod*>(cmd);
			cmd_mod->setPriority(TP::HIGH);
			Assert::AreEqual(cmd_mod->getFlagPriority(), true);

			delete cmd;
		}

		TEST_METHOD(cmd_Delete)
		{
			//TC0
			Command* cmd = new Command_Del();
			Assert::AreEqual(static_cast<int>(TP::DEL), static_cast<int>(cmd->getCommandType()));

			delete cmd;
		}

		TEST_METHOD(cmd_Find)
		{
			//TC0
			Command* cmd = new Command_Find();
			Assert::AreEqual(static_cast<int>(TP::FIND), static_cast<int>(cmd->getCommandType()));

			delete cmd;
		}

		TEST_METHOD(cmd_Undo)
		{
			//TC0
			Command* cmd = new Command_Undo();
			Assert::AreEqual(static_cast<int>(TP::UNDO), static_cast<int>(cmd->getCommandType()));

			delete cmd;
		}

		TEST_METHOD(cmd_Redo)
		{
			//TC0
			Command* cmd = new Command_Redo();
			Assert::AreEqual(static_cast<int>(TP::REDO), static_cast<int>(cmd->getCommandType()));

			delete cmd;
		}

		TEST_METHOD(cmd_Show) {
			int testInt = 0;
			Command* cmd = new Command_Show();
			Assert::AreEqual(static_cast<int>(TP::SHOW), static_cast<int>(cmd->getCommandType()));

			Command_Show* cmd_show = dynamic_cast<Command_Show*> (cmd);

			Assert::IsFalse(cmd_show->getFlagIndex());

			cmd_show->setIndex(testInt);
			Assert::IsTrue(cmd_show->getFlagIndex());

			Assert::AreEqual(cmd_show->getIndex(), testInt);

			delete cmd;
		}
	};
}