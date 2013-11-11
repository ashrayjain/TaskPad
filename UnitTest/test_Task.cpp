#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TaskPad_library/Task.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest
{
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
		
		TEST_METHOD(Task_Default_Values_Test)
		{
			Task t = Task();
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
		}
	};
}