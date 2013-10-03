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

		void saveGenericAttributes					(Task* tempTaskempTask);
		void saveDeadlineTaskSpecificAttributes		(Task* tempTaskempTask);
		void saveTimedTaskSpecificAttributes		(Task* tempTaskempTask);
		void saveFloatingTaskSpecificAttributes		(Task* tempTaskempTask);

		//file level attributes
				void saveCount				(unsigned count);
		inline	void saveTaskCount			(unsigned taskCount);
		inline	void saveParticipantCount	(unsigned participantCount);
		inline	void saveReminderCount		(unsigned reminderCount);
		inline	void saveTagCount			(unsigned tagCount);

		//generic attributes of all tasks
		void saveTaskType			(Task* tempTask);
		void saveIndex				(Task* tempTask);
		void saveName				(Task* tempTask);
		void saveLocation			(Task* tempTask);
		void saveParticipants		(Task* tempTask);
		void saveNote				(Task* tempTask);
		void savePriority			(Task* tempTask);
		void saveTags				(Task* tempTask);
		void saveReminderTimes		(Task* tempTask);
		void saveState				(Task* tempTask);

		// Task specific attributes
		template <class taskTypePointer> void saveDueDate	(taskTypePointer T);
		template <class taskTypePointer> void saveFromDate	(taskTypePointer T);
		template <class taskTypePointer> void saveToDate	(taskTypePointer T);

		static string convertTimeToString(time_t time);

		// stringconverters
		string convertToString	(unsigned index);
		string convertToString	(TASK_TYPE type);
		string convertToString	(time_t time);
		string convertToString	(list<time_t> timeList);
		string convertToString	(list<string> strList);
		string convertToString	(PRIORITY priority);
		string convertToString	(TASK_STATE state);

		//writers
		void writeLineToFile	(string line);
		void emptyTheFile		();
	public:
		Storage(list<Task>&);

		bool save	(list<Task>&);
		bool save	(const Command&);

		~Storage();
};
#endif