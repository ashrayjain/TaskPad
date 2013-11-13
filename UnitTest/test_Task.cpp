#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TaskPad_library/Task.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest
{
	static const time_t			now						= time(NULL);
	static const string			PARTICIPANT_NAME		= "Participant X";
	static const string			TAG_NAME				= "Tag";
	static const time_t			REMIND_TIME				= now + 4000;
	static const string			TEST_TASK_NAME			= "Test Task";
	static const string			TEST_TASK_LOCATION		= "Someplace";
	static const string			TEST_TASK_NOTE			= "This is a sample note!";
	static const list<string>	TEST_TASK_PARTICIPANTS	= list<string>(5, "Participant X");
	static const list<string>	TEST_TASK_TAGS			= list<string>(1, "Tag");
	static const time_t			TEST_TASK_DUE_DATE		= now + 864000;
	static const time_t			TEST_TASK_FROM_DATE		= now + 1864000;
	static const time_t			TEST_TASK_TO_DATE		= now - 464000;
	static const TP::PRIORITY	TEST_TASK_PRIORITY		= TP::PRIORITY::HIGH;
	static const time_t			TEST_TASK_REMIND_TIME	= now + 4000;
	static const time_t			TEST_TASK_PAST_DUE_DATE	= now - 864000;
	static const time_t			TEST_TASK_PAST_FROM_DATE= now - 464000;
	static const time_t			TEST_TASK_PAST_TO_DATE	= now - 1864000;

	

	TEST_CLASS(test_Task)
	{
	private:
		void compareTaskAttributes(Task &lhs, Task &rhs) {
			Assert::IsTrue(lhs.getName()==rhs.getName());
			Assert::IsTrue(lhs.getIndex()==rhs.getIndex());
			Assert::IsTrue(lhs.getLocation()==rhs.getLocation());
			Assert::IsTrue(lhs.getNote()==rhs.getNote());
			Assert::IsTrue(lhs.getPriority()==rhs.getPriority());
			Assert::IsTrue(lhs.getParticipants()==rhs.getParticipants());
			Assert::IsTrue(lhs.getTags()==rhs.getTags());
			Assert::IsTrue(lhs.getHashTagPtrs()==rhs.getHashTagPtrs());
			Assert::IsTrue(lhs.getRemindTimes()==rhs.getRemindTimes());
			Assert::IsTrue(lhs.getRemindTimesPtrs()==rhs.getRemindTimesPtrs());
			Assert::IsTrue(lhs.getFromDate()==rhs.getFromDate());
			Assert::IsTrue(lhs.getToDate()==rhs.getToDate());
			Assert::IsTrue(lhs.getState()==rhs.getState());
			Assert::IsTrue(lhs.getTaskType()==rhs.getTaskType());
		}

	public:
		TEST_METHOD_CLEANUP(Clean_Up)
		{
			Task::flushAllIndices();
		}
		
		TEST_METHOD(Task_Default_Values_Test)
		{
			Task t = Task();
			// Attribute Testing
			Assert::IsTrue(t.getName()==Task::DEFAULT_NAME);
			Assert::IsTrue(t.getIndex()==Task::DEFAULT_INDEX);
			Assert::IsTrue(t.getLocation()==Task::DEFAULT_LOCATION);
			Assert::IsTrue(t.getNote()==Task::DEFAULT_NOTE);
			Assert::IsTrue(t.getPriority()==Task::DEFAULT_PRIORITY);
			Assert::IsTrue(t.getParticipants()==Task::DEFAULT_PARTICIPANTS);
			Assert::IsTrue(t.getTags()==Task::DEFAULT_TAGS);
			Assert::IsTrue(t.getHashTagPtrs()==Task::DEFAULT_HASHTAG_PTRS);
			Assert::IsTrue(t.getRemindTimes()==Task::DEFAULT_REMINDTIMES);
			Assert::IsTrue(t.getRemindTimesPtrs()==Task::DEFAULT_REMINDTIMES_PTRS);
			Assert::IsTrue(t.getFromDate()==Task::DEFAULT_FROMDATE);
			Assert::IsTrue(t.getToDate()==Task::DEFAULT_TODATE);
			Assert::IsTrue(t.getState()==Task::DEFAULT_STATE);
			Assert::IsTrue(t.getTaskType()==Task::DEFAULT_TYPE);
			
			// Flag Testing
			Assert::IsFalse(t.getFlagName());
			Assert::IsFalse(t.getFlagIndex());
			Assert::IsFalse(t.getFlagLocation());
			Assert::IsFalse(t.getFlagNote());
			Assert::IsFalse(t.getFlagPriority());
			Assert::IsFalse(t.getFlagParticipants());
			Assert::IsFalse(t.getFlagTags());
			Assert::IsFalse(t.getFlagRemindTimes());
			Assert::IsFalse(t.getFlagFromDate());
			Assert::IsFalse(t.getFlagToDate());
			Assert::IsFalse(t.getFlagState());
		}

		TEST_METHOD(Task_Normal_Deadline_Test)
		{
			Task t = Task(TEST_TASK_NAME);
			t.setDueDate(TEST_TASK_DUE_DATE);
			Assert::IsTrue(t.getTaskType()==TP::DEADLINE);
		}
		
		TEST_METHOD(Task_Normal_Floating_Test)
		{
			Task t = Task(TEST_TASK_NAME);
			Assert::IsTrue(t.getTaskType()==TP::FLOATING);
		}
		
		TEST_METHOD(Task_Normal_Timed_Test)
		{
			Task t = Task(TEST_TASK_NAME);
			t.setFromDate(TEST_TASK_FROM_DATE);
			t.setToDate(TEST_TASK_TO_DATE);
			Assert::IsTrue(t.getTaskType()==TP::TIMED);
		}
		
		TEST_METHOD(Task_Type_Stress_Test)
		{
			Task t = Task(TEST_TASK_NAME);
			t.setFromDate(TEST_TASK_FROM_DATE);
			t.setToDate(TEST_TASK_TO_DATE);
			t.setDueDate(TEST_TASK_DUE_DATE);
			Assert::IsTrue(t.getTaskType()==TP::DEADLINE);
			t.setFromDate(TEST_TASK_FROM_DATE);
			Assert::IsTrue(t.getTaskType()==TP::TIMED);
			t.setToDate(TEST_TASK_TO_DATE);
			Assert::IsTrue(t.getTaskType()==TP::TIMED);
			t.setFromDate(Task::DEFAULT_FROMDATE);
			t.setToDate(Task::DEFAULT_TODATE);
			Assert::IsTrue(t.getTaskType()==TP::FLOATING);
		}
		
		TEST_METHOD(Task_State_Undone_Overdue_Test)
		{
			Task t = Task(TEST_TASK_NAME);
			t.setDueDate(TEST_TASK_DUE_DATE);
			Assert::IsTrue(t.getState()==TP::UNDONE);
			t.setDueDate(TEST_TASK_PAST_DUE_DATE);
			Assert::IsTrue(t.getState()==TP::OVERDUE);
			t.setFromDate(TEST_TASK_PAST_FROM_DATE);
			Assert::IsTrue(t.getState()==TP::OVERDUE);
			t.setFromDate(TEST_TASK_FROM_DATE);
			t.setToDate(TEST_TASK_FROM_DATE);
			Assert::IsTrue(t.getState()==TP::UNDONE);
		}
		
		TEST_METHOD(Task_Valid_Index_Test)
		{
			Task t = Task(1);
			try {
				Task _t = Task(2);
				Assert::IsTrue(true);
			}
			catch(string s) {
				Assert::IsTrue(false);
			}
			Task::flushAllIndices();
		}
		
		TEST_METHOD(Task_Invalid_Index_Test)
		{
			Task t = Task(1);
			try {
				Task _t = Task(1);
				Assert::IsTrue(false);
			}
			catch(string s) {
				Assert::IsTrue(true);
			}
			Task::flushAllIndices();
		}
		
		TEST_METHOD(Task_Add_Remove_Participant_Test)
		{
			Task t = Task(1);
			t.setParticipants(TEST_TASK_PARTICIPANTS);
			t.setParticipants(PARTICIPANT_NAME, TP::REMOVE_ELEMENT);
			Assert::IsTrue(t.getParticipants()==list<string>(4, PARTICIPANT_NAME));
			t.setParticipants(PARTICIPANT_NAME, TP::ADD_ELEMENT);
			t.setParticipants(PARTICIPANT_NAME, TP::ADD_ELEMENT);
			Assert::IsTrue(t.getParticipants()==list<string>(6, PARTICIPANT_NAME));
		}
		
		TEST_METHOD(Task_Add_Remove_Tags_Test)
		{
			Task t = Task(1);
			t.setTags(TEST_TASK_TAGS);
			t.setTags(TAG_NAME, TP::REMOVE_ELEMENT);
			Assert::IsTrue(t.getTags()==list<string>());
			t.setTags(TAG_NAME, TP::ADD_ELEMENT);
			Assert::IsTrue(t.getTags()==list<string>(1, TAG_NAME));
		}
		
		TEST_METHOD(Task_Add_Remove_RemindTimes_Test)
		{
			Task t = Task(1);
			t.setRemindTimes(list<time_t>(1, TEST_TASK_REMIND_TIME));
			t.setRemindTimes(REMIND_TIME, TP::REMOVE_ELEMENT);
			Assert::IsTrue(t.getRemindTimes()==list<time_t>());
			t.setRemindTimes(REMIND_TIME, TP::ADD_ELEMENT);
			t.setRemindTimes(REMIND_TIME, TP::ADD_ELEMENT);
			Assert::IsTrue(t.getRemindTimes()==list<time_t>(2, REMIND_TIME));
		}
		
	};
}