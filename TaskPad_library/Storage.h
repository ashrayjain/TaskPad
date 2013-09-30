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

		void saveDeadlineTask(Task* tempTask);
		void saveTimedTask(Task* tempTask);
		void saveFloatingTask(Task* tempTask);
		void saveString(string line);
		template <class taskTypePointer> void saveTaskType(taskTypePointer T);
		template <class taskTypePointer> void saveIndex(taskTypePointer T);
		template <class taskTypePointer> void saveName(taskTypePointer T);
		template <class taskTypePointer> void saveDueDate(taskTypePointer T);
		template <class taskTypePointer> void saveFromDate(taskTypePointer T);
		template <class taskTypePointer> void saveToDate(taskTypePointer T);
		template <class taskTypePointer> void saveLocation(taskTypePointer T);
		template <class taskTypePointer> void saveParticipants(taskTypePointer T);
		template <class taskTypePointer> void saveNote(taskTypePointer T);
		template <class taskTypePointer> void savePriority(taskTypePointer T);
		template <class taskTypePointer> void saveTags(taskTypePointer T);
		template <class taskTypePointer> void saveReminderTimes(taskTypePointer T);
		template <class taskTypePointer> void saveState(taskTypePointer T);

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
		bool writeLineToFile(string line);
	public:
		Storage(list<Task>&);

		bool save(list<Task>&);
		bool save(const Command&);

		~Storage();
};
#endif