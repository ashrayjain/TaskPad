/*
 * =====================================================================================
 *
 *		Filename:  TaskFileHandler.h
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This header file defines an abstract class called TaskFileHandler and all file handling classes
  * in this program will inherit from this class
  * 
 */

#ifndef _FILEHANDLER_H_
#define _FILEHANDLER_H_

#include<fstream>

class Task;

class TaskFileHandler
{
	protected:
		virtual void		openFile				(const std::string& fileName, std::ios_base::openmode) =0;
		virtual void		closeFile				() =0;
				std::string getTaskFilePath			(const Task& task);
				std::string getDeletedTaskFilePath	(const Task& task);

		const static std::string TASK_DIRECTORY;

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