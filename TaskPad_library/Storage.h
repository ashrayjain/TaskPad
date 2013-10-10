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

		static const string _fileName;

		static const string LABEL_TASK_COUNT;
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

		void saveTaskList		(const list<Task>& taskList);
		void saveTask			(const Task& task);
		void saveTaskAttributes	(const Task& tempTaskTask);

		//enum to decide the way to open the file

		//file level attributes
				void saveHeader				(std::string headerStr);
				void saveCount				(unsigned long long count);
		inline	void saveTaskCount			(unsigned long long taskCount);

		//generic attributes of all tasks
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

		// stringconverters
		string convertToString	(int num);
		string convertToString	(unsigned long long index);
		string convertToString	(time_t time);
		string convertToString	(list<time_t> timeList);
		string convertToString	(list<string> strList);
		string convertToString	(PRIORITY priority);
		string convertToString	(TASK_STATE state);

		//file opener/closer

		void openTheFileToRead();
		void openTheFileToWrite(std::ios_base::openmode);
		void closeTheWrittenFile();
		void closeTheReadFile();
		//writers
		void writeLineToFile	(string line, bool newLine);
		void writeLineToFile	(string line);
		void emptyTheFile		();
	public:
		Storage(list<Task>&);

		bool save	(const list<Task>&);
		bool save	(const Command&);
		bool save	(const Task& task);

		const string FILENAME;

		~Storage();
};
#endif