#include <sstream>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Storage_Datastore_stub.h"
#include "../TaskPad_library/Datastore.h"
#include "../TaskPad_library/Task.h"
#include "../TaskPad_library/Enum.h"
#include "../TaskPad_library/TaskLoaderText.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace TP;

static const std::string TASK_FILE_NAME					= "TaskPad.txt";
static const std::string TASK_FILE_DIRECTORY			= "Tasks\\";
static const std::string TASK_FILE_EXTENSION			= ".task";
static const std::string TASK_FILE_DELETED_EXTENSION	= ".deltask";

static const string LABEL_START_OF_TASK		= "StartOfTask";
static const string LABEL_NAME				= "name: ";
static const string LABEL_INDEX				= "index: ";
static const string LABEL_DUE_DATE			= "due: ";
static const string LABEL_FROM_DATE			= "from: ";
static const string LABEL_TO_DATE			= "to: ";
static const string LABEL_LOCATION			= "at: ";
static const string LABEL_PARTICIPANT		= "ppl: ";
static const string LABEL_NOTE				= "note: ";
static const string LABEL_PRIORITY			= "impt: ";
static const string LABEL_TAG				= "#: ";
static const string LABEL_REMINDER_TIME		= "rt: ";
static const string LABEL_STATE				= "state: ";
static const string LABEL_END_OF_TASK		= "EndOfTask";

//@Manikandan Thyagesh A0100124J

namespace UnitTest {

	TEST_CLASS(test_TaskLoaderText) {
	public:

		TEST_METHOD(load_taskDS) {
			//There are 3 partitions for this function:-
			//(i) no main load, no changes to recover
			//(ii) has main load, no changes to recover
			//(iii) has main load, changes to recover

			Task::flushAllIndices();
			stringstream	taskPath;
			string			path1, path2;
			Storage_Datastore_stub taskDS;
			TaskLoaderText	loader(&taskDS);
			Task task1 = getFirstTask();
			Task task2 = getSecondTask();
			Task task3 = getThirdTask();
			Task::flushAllIndices();
			list<Task> returnList;
			list<Task>::iterator it;

			string newName = "New Task Name";


			// Partition 1, no main load/recover
			emptyStreams(taskPath);
			loader.load(TASK_FILE_NAME);
			Assert::IsTrue(taskDS.getTaskList().empty());

			//Partition 2, has main load.
			generateExpectedOutput(task1);
			generateExpectedOutput(task2);
			generateExpectedOutput(task3);

			loader.load(TASK_FILE_NAME);
			
			returnList = taskDS.getTaskList();
			it = returnList.begin();
			Assert::AreEqual(3, (int) returnList.size());

			Assert::IsTrue((*it++) == task1);
			Assert::IsTrue((*it++) == task2);
			Assert::IsTrue((*it++) == task3);

			//cleanup
			taskDS.emptyStore();

			//Partition 3, main load and recover changes

			//simulate a modified task
			task1.setName(newName);
			taskPath.str("");
			taskPath << TASK_FILE_DIRECTORY << task1.getIndex() << TASK_FILE_EXTENSION;		
			path1 = taskPath.str();
			generateExpectedOutput(task1,path1);
			// simulate a deleted task
			path2 = generateExpectedOutput(task2.getIndex());

			loader.load(TASK_FILE_NAME);
			returnList = taskDS.getTaskList();
			it = returnList.begin();
			Assert::AreEqual(2,(int) returnList.size());

			Assert::IsTrue((*it++) == task1);
			Assert::IsTrue((*it++) == task3);

			taskDS.emptyStore();
			std::remove(path1.c_str());
			std::remove(path2.c_str());
			emptyStreams(taskPath);
			emptyStreams(taskPath,TASK_FILE_NAME);
			std::remove(TASK_FILE_NAME.c_str());
		}

		Task getFirstTask() {
			// partition 1: All attributes of Deadline task given
			// contains all attributes of a task, with lists for Participants, 
			// Reminder Times and Tags

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

			// create the Task
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

			return task;
		}

		Task getSecondTask() {
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
			
			return task;
		}

		Task getThirdTask() {
			// partition 3: Bare minimum attributes
			// Boundary condition with the compulsory attributes

			unsigned long long taskIndex = 1384184914102;
			std::string taskName = "Floating Task 2";
			PRIORITY taskPriority = MEDIUM;
			TASK_STATE taskState = UNDONE;

			// create the Task
			Task task(taskIndex);
			task.setName(taskName);

			return task;
		}

		void emptyStreams (stringstream& tempStream, string fileName = TASK_FILE_NAME) {
			ofstream tempFile(fileName);
			tempFile.close();
			tempStream.str("");
		}

		void generateExpectedOutput (const Task& task, string fileName = TASK_FILE_NAME) {

			ofstream parallelStream(fileName, ofstream::app);

			//generate the expected output
			parallelStream << endl;
			parallelStream << LABEL_START_OF_TASK << endl;
			parallelStream << LABEL_INDEX << task.getIndex() << endl;
			parallelStream << LABEL_NAME << task.getName() << endl;
			if (task.getFlagDueDate()) {
				parallelStream << LABEL_DUE_DATE << task.getDueDate() << endl;
			}
			if(task.getFlagFromDate()) {
				parallelStream << LABEL_FROM_DATE << task.getFromDate() << endl;
			}
			if(task.getFlagToDate()) {
				parallelStream << LABEL_TO_DATE << task.getToDate() << endl;
			}
			if (task.getFlagLocation()) {
				parallelStream << LABEL_LOCATION << task.getLocation() << endl;
			}
			if (task.getFlagParticipants()) {
				list<string> participants = task.getParticipants();
				list<string>::iterator it = participants.begin();
				while (it != participants.end()) {
					parallelStream << LABEL_PARTICIPANT << *it << endl;
					it++;
				}
			}
			if(task.getFlagNote()) {
				parallelStream << LABEL_NOTE << task.getNote() << endl;
			}
			parallelStream << LABEL_PRIORITY << PRIORITY_STRING[task.getPriority()] << endl;
			if (task.getFlagTags()) {
				list<string> tags = task.getTags();
				list<string>::iterator it = tags.begin();
				while (it != tags.end()) {
					parallelStream << LABEL_TAG << *it << endl;
					it++;
				}
			}
			if (task.getFlagRemindTimes()) {
				list<time_t> reminders = task.getRemindTimes();
				list<time_t>::iterator it = reminders.begin();
				while (it != reminders.end()) {
					parallelStream << LABEL_REMINDER_TIME << *it << endl;
					it++;
				}
			}
			parallelStream << LABEL_STATE << TASK_STATE_STRING[task.getState()] << endl;
			parallelStream << LABEL_END_OF_TASK << endl;

			parallelStream.close();
		}

		string generateExpectedOutput (unsigned long long index) {
			stringstream taskPath;
			taskPath.str("");
			taskPath << TASK_FILE_DIRECTORY << index << TASK_FILE_DELETED_EXTENSION;

			ofstream delTaskFile(taskPath.str());
			delTaskFile << index << endl;

			delTaskFile.close();

			return taskPath.str();
		}
	};
}