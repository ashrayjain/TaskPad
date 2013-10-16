#ifndef _TASKLOADER_H-
#define _TASKLOADER_H_

#include <list>

class Task;

class TaskLoader
{
	public:
		virtual bool load (std::list<Task>& taskList, std::string fileName);
	protected:
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

const std::string TaskLoader::LABEL_START_OF_TASK = "StartOfTask";
const std::string TaskLoader::LABEL_NAME = "name:";
const std::string TaskLoader::LABEL_INDEX = "index:";
const std::string TaskLoader::LABEL_DUE_DATE = "due:";
const std::string TaskLoader::LABEL_FROM_DATE = "from:";
const std::string TaskLoader::LABEL_TO_DATE = "to:";
const std::string TaskLoader::LABEL_LOCATION = "at:";
const std::string TaskLoader::LABEL_PARTICIPANT = "ppl:";
const std::string TaskLoader::LABEL_NOTE = "note:";
const std::string TaskLoader::LABEL_PRIORITY = "impt:";
const std::string TaskLoader::LABEL_TAG = "#:";
const std::string TaskLoader::LABEL_REMINDER_TIME = "rt:";
const std::string TaskLoader::LABEL_STATE = "state:";
const std::string TaskLoader::LABEL_END_OF_TASK = "EndOfTask";

#endif