#ifndef _TASKLOADER_H_
#define _TASKLOADER_H_

#include <list>
#include <fstream>

class Task;

class TaskLoader
{
	public:
		virtual void load (std::list<Task>& taskList, const std::string& fileName) =0;
		~TaskLoader() {}
	protected:
		std::ifstream _fileReader;
		bool _isFileMishandled;

		static const std::string LABEL_START_OF_TASK;
		static const std::string LABEL_NAME;
		static const std::string LABEL_INDEX;
		static const std::string LABEL_DUE_DATE;
		static const std::string LABEL_FROM_DATE;
		static const std::string LABEL_TO_DATE;
		static const std::string LABEL_LOCATION;
		static const std::string LABEL_PARTICIPANT;
		static const std::string LABEL_NOTE;
		static const std::string LABEL_PRIORITY;
		static const std::string LABEL_TAG;
		static const std::string LABEL_REMINDER_TIME;
		static const std::string LABEL_STATE;
		static const std::string LABEL_END_OF_TASK;
};

#endif