#ifndef _TASKSAVERTEXT_H_
#define _TASKSAVERTEXT_H_

#include <list>
#include "TaskSaver.h"
#include "Enum.h"

class TaskSaverText: public TaskSaver
{
	public:
		void save (const std::list<Task>& taskList, const std::string& fileName);
		void save (const Task& task);
	private:
		//savers
		void saveTaskList		(const std::list<Task>& taskList);
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

		static std::string convertTimeToString(time_t time);

		// std::string converters
		std::string convertToString	(int num);
		std::string convertToString	(unsigned long long index);
		std::string convertToString	(time_t time);
		std::string convertToString	(TP::PRIORITY priority);
		std::string convertToString	(TP::TASK_STATE state);

		//writers
		void writeLineToFile	(std::string line, bool newLine = true);
		void emptyTheFile		();
				
		//removes Task files
		void removeTaskFiles();
};

#endif