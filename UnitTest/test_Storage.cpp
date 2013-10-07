#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TaskPad_library/Task.h"
#include "../TaskPad_library/Enum.h"
#include "../TaskPad_library/Storage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace TP;

namespace UnitTest
{		
	TEST_CLASS(test_Storage)
	{
	public:
		
		TEST_METHOD(saveList)
		{
			list<Task> taskList;
			setupTaskList(taskList);

			Storage testStore(taskList);

			Assert::IsTrue(testStore.save(taskList));
		}

		void setupTaskList(list<Task>& taskList)
		{
			Task task,task1;
			time_t t = time(NULL);
			task.setName("newDeadline Task");
			task.setDueDate((t + 864000));
			task.setLocation("somewhere");
			task.setNote("this is a sample note");
			task.setParticipants("ppl 1",ADD_ELEMENT);
			task.setParticipants("ppl 2",ADD_ELEMENT);
			task.setParticipants("ppl 3",ADD_ELEMENT);
			task.setParticipants("ppl 4",ADD_ELEMENT);
			task.setPriority(HIGH);
			task.setRemindTimes((t + 432000),ADD_ELEMENT);
			task.setRemindTimes((t + 691200),ADD_ELEMENT);

			taskList.push_back(task);


			task1.setName("task 2");
			task1.setState(DONE);
			task1.setTags("test1",ADD_ELEMENT);
			task1.setTags("test2",ADD_ELEMENT);

			taskList.push_back(task1);

			return;


		}
	};
}