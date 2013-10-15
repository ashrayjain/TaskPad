#ifndef _STORAGE_H_
#define _STORAGE_H_

#include<fstream>
#include<list>
#include "Enum.h"

//forward definitions
class Task;
class Command;

using namespace std;
using namespace TP;

class Storage
{
	private:
		ofstream _fileWriter;
		ifstream _fileReader;
		bool _isFileMishandled;

		static const string _fileName;

		static const string LABEL_NAME;
		static const string LABEL_INDEX;
		static const string LABEL_DUE_DATE;
		static const string LABEL_FROM_DATE;
		static const string LABEL_TO_DATE;
		static const string LABEL_LOCATION;
		static const string LABEL_PARTICIPANT;
		static const string LABEL_NOTE;
		static const string LABEL_PRIORITY;
		static const string LABEL_TAG;
		static const string LABEL_REMINDER_TIME;
		static const string LABEL_STATE;
		static const string LABEL_END_OF_TASK;
		static const string LABEL_START_OF_TASK;

		////////////////////////
		////Saving Functions///
		//////////////////////

		//savers
		void saveTaskList		(const list<Task>& taskList);
		void saveTask			(const Task& task);
		void saveTaskAttributes	(const Task& tempTaskTask);

		//file level attributes
		void saveTaskLevelLabel			(std::string LabelStr);
		void saveAttributeLevelLabel	(std::string LabelStr);

		//saving generic attributes of all tasks
		void saveIndex				(const Task& tempTask);
		void saveName				(const Task& tempTask);
		void saveLocation			(const Task& tempTask);
		void saveParticipants		(const Task& tempTask);
		void saveNote				(const Task& tempTask);
		void savePriority			(const Task& tempTask);
		void saveTags				(const Task& tempTask);
		void saveReminderTimes		(const Task& tempTask);
		void saveState				(const Task& tempTask);
		void saveDueDate			(const Task& tempTask);
		void saveFromDate			(const Task& tempTask);
		void saveToDate				(const Task& tempTask);

		static string convertTimeToString(time_t time);

		// string converters
		string convertToString	(int num);
		string convertToString	(unsigned long long index);
		string convertToString	(time_t time);
		string convertToString	(PRIORITY priority);
		string convertToString	(TASK_STATE state);

		//writers
		void writeLineToFile	(string line, bool newLine = true);
		void emptyTheFile		();

		//file opener/closer
		void openTheFileToWrite		(std::string fileName = _fileName, std::ios_base::openmode = ios::trunc);
		void closeTheWrittenFile	();
		
		//removes Task files
		void removeTaskFiles();

		///////////////////////////
		//// Loading Functions ////
		///////////////////////////

		//loader
		void loadTaskList(list<Task>& taskList);

		//helper functions
		std::string getNewLabel(std::string);
		std::string getNewData (std::string);
		Task getNextTask();
		unsigned long long getTaskIndex (std::string);
		bool hasNextTask();

		// task constructing functions
		Task createNewTask(unsigned long long index);
		void setTaskName	(Task& task,std::string name);
		void setTaskDueDate	(Task& task, std::string dueDate);
		void setTaskFromDate(Task& task, std::string fromDate);
		void setTaskToDate(Task& task, std::string toDate);
		void setTaskLocation(Task& task, std::string location);
		void setTaskParticipant(Task& task, std::string participant);
		void setTaskNote(Task& task, std::string note);
		void setTaskPriority(Task& task, std::string taskPriority);
		void setTaskTag(Task& task, std::string tag);
		void setTaskReminderTime(Task& task, std::string reminderTime);
		void setTaskState(Task& task, std::string taskState);
		//reader
		std::string getNextLineFromFile();

		// reverse string converters
		int					getIntFromString		(std::string attribute);
		unsigned long long	getIndexFromString		(std::string attribute);
		time_t				getTimeFromString		(std::string attribute);
		PRIORITY			getPriorityFromString	(std::string attribute);
		TASK_STATE			getTaskStateFromString	(std::string attribute);

		//file opener/closer
		void openTheFileToRead(std::string fileName = _fileName);
		void closeTheReadFile();

	public:
		Storage(list<Task>&);

		bool save	(const list<Task>&);
		bool save	(const Command&);
		bool save	(const Task& task);

		void load	(list<Task>& taskList);

		const string FILENAME;

		~Storage();
};
#endif