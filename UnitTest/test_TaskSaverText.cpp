#include "stdafx.h"
#include "CppUnitTest.h"
#include "Storage_Datastore_stub.h"
#include "../TaskPad_library/Datastore.h"
#include "../TaskPad_library/Task.h"
#include "../TaskPad_library/Enum.h"
#include "../TaskPad_library/TaskSaverText.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace TP;

static const std::string TASK_FILE_NAME = "TaskPad.txt";

const string LABEL_START_OF_TASK	= "StartOfTask";
const string LABEL_NAME				= "name: ";
const string LABEL_INDEX			= "index: ";
const string LABEL_DUE_DATE			= "due: ";
const string LABEL_FROM_DATE		= "from: ";
const string LABEL_TO_DATE			= "to: ";
const string LABEL_LOCATION			= "at: ";
const string LABEL_PARTICIPANT		= "ppl: ";
const string LABEL_NOTE				= "note: ";
const string LABEL_PRIORITY			= "impt: ";
const string LABEL_TAG				= "#: ";
const string LABEL_REMINDER_TIME	= "rt: ";
const string LABEL_STATE			= "state: ";
const string LABEL_END_OF_TASK		= "EndOfTask";

namespace UnitTest
{		
	TEST_CLASS(test_TaskSaverText)
	{
	public:

		TEST_METHOD(save_taskDS)
		{
			// There are 2 partitions: A Datastore with tasks and one without
			// * We can consider subcases where the tasks in the datastore have partially
			// * filled attributes

			Storage_Datastore_stub taskStore, emptyStore;
			TaskSaverText saver;
			stringstream tempStream;

			// Partition 1 & Boundary Condition : no Tasks in the list
			emptyStreams(tempStream);
			saver.save(&emptyStore,TASK_FILE_NAME);
			Assert::IsTrue(compareStreams(tempStream));

			// Boundary condition - All possible attributes for a task, one task given 
			taskStore.emptyStore();
			emptyStreams(tempStream);

			addFirstTask(&taskStore, tempStream);
			saver.save(&taskStore,TASK_FILE_NAME);
			Assert::IsTrue(compareStreams(tempStream));

			// Partition 2: Multiple Tasks in the list with variable attributes 
			taskStore.emptyStore();
			emptyStreams(tempStream);

			addFirstTask(&taskStore, tempStream);
			addSecondTask(&taskStore, tempStream);
			addThirdTask(&taskStore, tempStream);
			saver.save(&taskStore,TASK_FILE_NAME);

			emptyStreams(tempStream);
			taskStore.emptyStore();
		}

		void emptyStreams (stringstream& tempStream) {
			ofstream tempFile(TASK_FILE_NAME);
			tempFile.close();

			tempStream.clear();
		}

		bool compareStreams(stringstream& sTemp)
		{
			ifstream outFile(TASK_FILE_NAME);
			stringstream fileBuf;

			fileBuf << outFile.rdbuf();

			// debugging code for unit test
			ofstream logFileTemp(TASK_FILE_NAME + "_temp.txt");
			logFileTemp << sTemp.str();
			logFileTemp.close();

			outFile.close();

			return (fileBuf.str() == sTemp.str());
		}

		void addFirstTask(StorableTaskDatastore* taskList, stringstream& parallelStream) {
			// partition 1: All attributes of Deadline task given
			// contains all attributes of a task, with lists for Participants, 
			// Reminder Times and Tags

			/* boundary condition with all attributes */
			unsigned long long  taskIndex = 1384184604102;
			std::string taskName = "Deadline Task 1";
			string taskLocation = "somewhere";
			string taskParticipant1 = "ppl 1";
			string taskParticipant2 = "ppl 2";
			string taskParticipant3 = "ppl 3";
			string taskParticipant4 = "ppl 4";
			string taskNote = "this is a sample note";
			string taskTag1 = "tag1";
			string taskTag2 = "tag2";
			PRIORITY taskPriority = HIGH;
			TASK_STATE taskState = UNDONE;

			time_t t = time(NULL);
			time_t taskDueDate = (t + 864000);
			time_t taskRt1 = (t + 432000);
			time_t taskRt2 = (t + 691200);

			Task task(taskIndex);
			task.setName(taskName);
			task.setDueDate(taskDueDate);
			task.setLocation(taskLocation);
			task.setParticipants(taskParticipant1,ADD_ELEMENT);
			task.setParticipants(taskParticipant2,ADD_ELEMENT);
			task.setParticipants(taskParticipant3,ADD_ELEMENT);
			task.setParticipants(taskParticipant4,ADD_ELEMENT);
			task.setNote(taskNote);
			task.setPriority(HIGH);
			task.setTags(taskTag1,ADD_ELEMENT);
			task.setTags(taskTag2,ADD_ELEMENT);
			task.setRemindTimes(taskRt1,ADD_ELEMENT);
			task.setRemindTimes(taskRt2,ADD_ELEMENT);


			parallelStream << endl;
			parallelStream << LABEL_START_OF_TASK << endl;
			parallelStream << LABEL_INDEX << taskIndex << endl;
			parallelStream << LABEL_NAME << taskName << endl;
			parallelStream << LABEL_DUE_DATE << taskDueDate << endl;
			parallelStream << LABEL_FROM_DATE << taskDueDate << endl;
			parallelStream << LABEL_TO_DATE << taskDueDate << endl;
			parallelStream << LABEL_LOCATION << taskLocation << endl;
			parallelStream << LABEL_PARTICIPANT << taskParticipant1 << endl;
			parallelStream << LABEL_PARTICIPANT << taskParticipant2 << endl;
			parallelStream << LABEL_PARTICIPANT << taskParticipant3 << endl;
			parallelStream << LABEL_PARTICIPANT << taskParticipant4 << endl;
			parallelStream << LABEL_NOTE << taskNote << endl;
			parallelStream << LABEL_PRIORITY << PRIORITY_STRING[taskPriority] << endl;
			parallelStream << LABEL_TAG << taskTag1 << endl;
			parallelStream << LABEL_TAG << taskTag2 << endl;
			parallelStream << LABEL_REMINDER_TIME << taskRt1 << endl;
			parallelStream << LABEL_REMINDER_TIME << taskRt2 << endl;
			parallelStream << LABEL_STATE << TASK_STATE_STRING[taskState] << endl;
			parallelStream << LABEL_END_OF_TASK << endl;

			taskList->addTask(task);
			return;
		}

		void addSecondTask(StorableTaskDatastore* taskDS, stringstream& parallelStream) {
			// partition 2: Some attributes of Task missing
			
			unsigned long long taskIndex = 1384184904102;
			std::string taskName = "Floating Task 1";
			string taskLocation = "somewhere";
			string taskNote = "this is a sample note";
			string taskTag1 = "tag1";
			PRIORITY taskPriority = HIGH;
			TASK_STATE taskState = DONE;
			time_t t = time(NULL);
			time_t taskFromDate = (t + 864100);
			time_t taskToDate = (t + 865000);
			time_t taskRt1 = (t + 432000);
			time_t taskRt2 = (t + 691200);

			// create the Task
			Task task(taskIndex);
			task.setName(taskName);
			task.setLocation(taskLocation);
			task.setNote(taskNote);
			task.setPriority(HIGH);
			task.setTags(taskTag1,ADD_ELEMENT);
			task.setRemindTimes(taskRt1,ADD_ELEMENT);
			task.setRemindTimes(taskRt2,ADD_ELEMENT);
			//add task to datastore
			taskDS->addTask(task);

			//generate the expected output
			parallelStream << endl;
			parallelStream << LABEL_START_OF_TASK;
			parallelStream << LABEL_INDEX << taskIndex << endl;
			parallelStream << LABEL_NAME << taskName << endl;
			parallelStream << LABEL_FROM_DATE << taskFromDate << endl;
			parallelStream << LABEL_TO_DATE << taskToDate << endl;
			parallelStream << LABEL_LOCATION << taskLocation << endl;
			parallelStream << LABEL_NOTE << taskNote << endl;
			parallelStream << LABEL_PRIORITY << PRIORITY_STRING[taskPriority] << endl;
			parallelStream << LABEL_TAG << taskTag1 << endl;
			parallelStream << LABEL_REMINDER_TIME << taskRt1 << endl;
			parallelStream << LABEL_REMINDER_TIME << taskRt2 << endl;
			parallelStream << LABEL_STATE << TASK_STATE_STRING[taskState] << endl;
			parallelStream << LABEL_END_OF_TASK << endl;
			return;
		}

		void addThirdTask(StorableTaskDatastore* taskDS, stringstream& parallelStream) {
			// partition 3: Bare minimum attributes
			// Boundary condition with the compulsory attributes

			unsigned long long taskIndex = 1384184914102;
			std::string taskName = "Floating Task 2";
			PRIORITY taskPriority = MEDIUM;
			TASK_STATE taskState = UNDONE;

			// create the Task
			Task task(taskIndex);
			task.setName(taskName);
			//add task to datastore
			taskDS->addTask(task);

			//generate the expected output
			parallelStream << endl;
			parallelStream << LABEL_START_OF_TASK;
			parallelStream << LABEL_INDEX << taskIndex << endl;
			parallelStream << LABEL_NAME << taskName << endl;
			parallelStream << LABEL_PRIORITY << PRIORITY_STRING[taskPriority] << endl;
			parallelStream << LABEL_STATE << TASK_STATE_STRING[taskState] << endl;
			parallelStream << LABEL_END_OF_TASK << endl;
			return;
		}
	};
}