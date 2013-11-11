#include <sstream>
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
	TEST_CLASS(test_TaskSaverText) {
	public:

		TEST_METHOD(save_taskDS) {
			// There are 2 partitions: A Datastore with tasks and one without
			// * We can consider subcases where the tasks in the datastore have partially
			// * filled attributes

			Storage_Datastore_stub taskStore, emptyStore;
			TaskSaverText saver;
			stringstream tempStream;
			Task task;

			// Partition 1 & Boundary Condition : no Tasks in the list
			emptyStreams(tempStream);
			saver.save(&emptyStore,TASK_FILE_NAME);
			Assert::IsTrue(compareStreams(tempStream));

			// Boundary condition - All possible attributes for a task, one task given 
			taskStore.emptyStore();
			emptyStreams(tempStream);

			task = getFirstTask();
			taskStore.addTask(task);
			generateExpectedOutput(tempStream,task);
			saver.save(&taskStore,TASK_FILE_NAME);
			Assert::IsTrue(compareStreams(tempStream));

			// Partition 2: Multiple Tasks in the list with variable attributes 
			taskStore.emptyStore();
			emptyStreams(tempStream);

			task = getFirstTask();
			taskStore.addTask(task);
			generateExpectedOutput(tempStream,task);
			getSecondTask();
			taskStore.addTask(task);
			generateExpectedOutput(tempStream,task);
			getThirdTask();
			taskStore.addTask(task);
			generateExpectedOutput(tempStream,task);

			saver.save(&taskStore,TASK_FILE_NAME);

			emptyStreams(tempStream);
			taskStore.emptyStore();
			std::remove(TASK_FILE_NAME.c_str());

		}

		TEST_METHOD(save_task) {
			// This method is called after every user command
			// There are 3 partitions: (i) a mod/add command (ii) a del command
			Task::flushAllIndices();
			Task tempTask = getFirstTask();
			ifstream tempIfstream;
			COMMAND_TYPE tempType = ADD;
			TaskSaverText saver;
			stringstream tempStream;
			stringstream taskPath;
			taskPath << TASK_FILE_DIRECTORY << tempTask.getIndex() << TASK_FILE_EXTENSION;
			
			emptyStreams(tempStream);

			// Partition 1: ADD type
			generateExpectedOutput(tempStream,tempTask);
			saver.save(tempTask,tempType);
			Assert::IsTrue(compareStreams(tempStream,taskPath.str()));

			//clean up
			std::remove(taskPath.str().c_str());

			//Partition 2: DEL type
			taskPath.str("");
			taskPath << TASK_FILE_DIRECTORY << tempTask.getIndex() << TASK_FILE_DELETED_EXTENSION;

			emptyStreams(tempStream);
			tempStream << tempTask.getIndex() << endl;
			tempType = DEL;
			saver.save(tempTask,tempType);
			Assert::IsTrue(compareStreams(tempStream,taskPath.str()));

			//cleanup
			std::remove(taskPath.str().c_str());
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
			std::remove(fileName.c_str());
			tempStream.str("");
		}

		bool compareStreams(stringstream& sTemp, string fileName = TASK_FILE_NAME) {
			ifstream outFile(fileName);
			stringstream fileBuf;

			fileBuf << outFile.rdbuf();

			// debugging code for unit test
			//ofstream logFileTemp(TASK_FILE_NAME + "_temp.txt");
			//logFileTemp << sTemp.str();
			//logFileTemp.close();

			outFile.close();

			return (fileBuf.str() == sTemp.str());
		}

		void generateExpectedOutput (stringstream& parallelStream, const Task& task) {
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
		}
	};
}