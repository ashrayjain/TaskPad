#ifndef _FILEHANDLER_H_
#define _FILEHANDLER_H_

#include<fstream>

class FileHandler
{
	protected:
		virtual void openFile(const std::string& fileName, std::ios_base::openmode) =0;
		virtual void closeFile() =0;

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