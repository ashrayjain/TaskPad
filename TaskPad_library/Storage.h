#ifndef _STORAGE_H_
#define _STORAGE_H_

#include<fstream>
#include<list>
#include "Enum.h"

//forward definitions
class Task;
class Command;
class DeadlineTask;
class TimedTask;
class FloatingTask;

using namespace std;
using namespace TP;

class Storage
{
	private:
		ofstream _fileWriter;
		ifstream _fileReader;

		static const string _fileName;

		void saveGenericAttributes(Task* tempTask);
		void saveDeadlineTaskSpecificAttributes(Task* tempTask);
		void saveTimedTaskSpecificAttributes(Task* tempTask);
		void saveFloatingTaskSpecificAttributes(Task* tempTask);
		void saveString(string line);

		//generic attributes of all tasks
		void saveTaskType(Task* T);
		void saveIndex(Task* T);
		void saveName(Task* T);
		void saveLocation(Task* T);
		void saveParticipants(Task* T);
		void saveNote(Task* T);
		void savePriority(Task* T);
		void saveTags(Task* T);
		void saveReminderTimes(Task* T);
		void saveState(Task* T);

		// Task specific attributes
		template <class taskTypePointer> void saveDueDate(taskTypePointer T);
		template <class taskTypePointer> void saveFromDate(taskTypePointer T);
		template <class taskTypePointer> void saveToDate(taskTypePointer T);

		static string convertTimeToString(time_t time);

		// stringconverters
		string convertToString(unsigned index);
		string convertToString(TASK_TYPE type);
		string convertToString(time_t time);
		string convertToString(list<time_t> timeList);
		string convertToString(list<string> strList);
		string convertToString(PRIORITY priority);
		string convertToString(TASK_STATE state);

		//writers
		void writeLineToFile(string line);
		void writeNumberToFile(int num);
		void emptyTheFile();
	public:
		Storage(list<Task>&);

		bool save(list<Task>&);
		bool save(const Command&);

		~Storage();
};
#endif