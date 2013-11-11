#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TaskPad_library/Command.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest{		
	TEST_CLASS(test_Command){
	public:
		
		//@XIE KAI A0102016E
		TEST_METHOD(cmd_Add){
			const char* BEST_NAME_EVER = "FML 2014";

			//TC0: Command_Add converts to Command
			//Assume it happens in Interpreter
			Command_Add* cmdAddInInterpreter = new Command_Add();
			//compulsory field
			cmdAddInInterpreter->setName(BEST_NAME_EVER);
			cmdAddInInterpreter->setDueDate(time(NULL));
			//to return by Interpreter
			Command* cmd = cmdAddInInterpreter;
			Assert::AreEqual(static_cast<int>(TP::ADD), static_cast<int>(cmd->getCommandType()));

			//TC1: Command converts back to Command_Add
			//Assume it happens in Executor
			Command_Add* cmdAddInExecutor;
			bool isDueDateSet = false;
			if( cmd->getCommandType() == TP::ADD )
				cmdAddInExecutor = dynamic_cast<Command_Add*>(cmd);
			if( cmdAddInExecutor->getFlagDue() )
				isDueDateSet = true;
			Assert::AreEqual(true, isDueDateSet);
			Assert::AreEqual((string)BEST_NAME_EVER, cmdAddInExecutor->getName());
			
			delete cmd;
		}

		TEST_METHOD(cmd_Modify){
			//TC0 test Priority
			Command* cmd = new Command_Mod();
			Assert::AreEqual(static_cast<int>(TP::MOD), static_cast<int>(cmd->getCommandType()));
			
			Command_Mod* cmd_mod = dynamic_cast<Command_Mod*>(cmd);
			cmd_mod->setPriority(TP::HIGH);
			Assert::AreEqual(cmd_mod->getFlagPriority(), true);

			delete cmd;
		}

		TEST_METHOD(cmd_Delete){
			//TC0 test Exact Flag
			Command* cmd = new Command_Del();
			Assert::AreEqual(static_cast<int>(TP::DEL), static_cast<int>(cmd->getCommandType()));

			Command_Del* cmd_del = dynamic_cast<Command_Del*>(cmd);
			cmd_del->setFlagExact();
			Assert::AreEqual(cmd_del->getFlagExact(), true);

			delete cmd;
		}

		TEST_METHOD(cmd_Find){
			//TC0 test From and To date
			Command* cmd = new Command_Find();
			Assert::AreEqual(static_cast<int>(TP::FIND), static_cast<int>(cmd->getCommandType()));

			Command_Find* cmd_find = dynamic_cast<Command_Find*>(cmd);
			time_t currTime = time(NULL);
			cmd_find->setFromDate(currTime);
			cmd_find->setToDate(currTime);
			Assert::IsTrue(cmd_find->getFromDate() == cmd_find->getToDate());
			Assert::IsTrue(cmd_find->getFlagFrom());
			Assert::IsTrue(cmd_find->getFlagTo());

			delete cmd;
		}

		TEST_METHOD(cmd_Undo){
			//TC0
			Command* cmd = new Command_Undo();
			Assert::AreEqual(static_cast<int>(TP::UNDO), static_cast<int>(cmd->getCommandType()));

			delete cmd;
		}

		TEST_METHOD(cmd_Redo){
			//TC0
			Command* cmd = new Command_Redo();
			Assert::AreEqual(static_cast<int>(TP::REDO), static_cast<int>(cmd->getCommandType()));

			delete cmd;
		}

		//@Thyagesh A0100124J
		TEST_METHOD(cmd_Show){
			int testInt = 0;
			Command* cmd = new Command_Show();
			Assert::AreEqual(static_cast<int>(TP::SHOW), static_cast<int>(cmd->getCommandType()));

			Command_Show* cmd_show = dynamic_cast<Command_Show*>(cmd);

			Assert::IsFalse(cmd_show->getFlagIndex());

			cmd_show->setIndex(testInt);
			Assert::IsTrue(cmd_show->getFlagIndex());

			Assert::AreEqual(cmd_show->getIndex(), testInt);

			delete cmd;
		}
	};
}