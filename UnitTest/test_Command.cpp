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
			//TC0: emu Interpreter
			Command_Add* cmd_add_interpreter = new Command_Add();
			cmd_add_interpreter->setName("FML 2014");//compulsory field
			cmd_add_interpreter->setDueDate(time(NULL));
			Command* cmd = cmd_add_interpreter;//to return
			Assert::AreEqual(static_cast<int>(TP::ADD), static_cast<int>(cmd->getCommandType()));

			//TC0.1: tags
			/*cmd_add_interpreter->setTags("tag1, tag2, tag3");
			Assert::AreEqual((string) "tag1, tag2, tag3", cmd_add_interpreter->getTags());*/

			/*time_t tmp = cmd_add_interpreter->getDueDate();
			Assert::AreEqual((string) "" , string(std::asctime(std::localtime(&tmp))));*/

			//TC0.2: paticipants
			/*cmd_add_interpreter->setParticipants("ppl1, ppl2, ppl3");
			Assert::AreEqual((string) "ppl1, ppl2, ppl3", cmd_add_interpreter->getParticipants());*/

			//TC1: emu Executor
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

		TEST_METHOD(cmd_Sync)
		{
			//TC0
			Command* cmd = new Command_Sync();
			Assert::AreEqual(static_cast<int>(TP::SYNC), static_cast<int>(cmd->getCommandType()));

			delete cmd;
		}
	};
}