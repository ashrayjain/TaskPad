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

		void saveTaskList	(list<Task>& taskList);
		void saveTask		(Task& tempTaskTask);

		//enum to decide the way to open the file

		//file level attributes
				void saveHeader				(std::string headerStr);
				void saveCount				(unsigned count);
		inline	void saveTaskCount			(unsigned taskCount);

		//generic attributes of all tasks
		void saveIndex				(Task& tempTask);
		void saveName				(Task& tempTask);
		void saveLocation			(Task& tempTask);
		void saveParticipants		(Task& tempTask);
		void saveNote				(Task& tempTask);
		void savePriority			(Task& tempTask);
		void saveTags				(Task& tempTask);
		void saveReminderTimes		(Task& tempTask);
		void saveState				(Task& tempTask);
		void saveDueDate			(Task& tempTask);
		void saveFromDate			(Task& tempTask);
		void saveToDate				(Task& tempTask);

		static string convertTimeToString(time_t time);

		// stringconverters
		string convertToString	(int num);
		string convertToString	(unsigned index);
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

		bool save	(list<Task>&);
		bool save	(const Command&);

		const string FILENAME;

		~Storage();
};
#endif