/*
 * =====================================================================================
 *
 *		Filename:  Logger.h
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file defines all the functions and attributes of the Logger Class
  * This class serves as a common logger for all other components to use
  * 
  * Gives five levels of importance to logged messages and allows for configuration through
  * an external file to specify (i) the minimum level to log, (ii) The specific class (if any) to monitor
  * and (iii) The file name to do the logging, default is set to TaskLogger.txt
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <fstream>
#include "Enum.h"

//a singleton for the whole program
class Logger {
	public:
		void setMinLevelToLog		(TP::LOG_TYPE);

		static	Logger*	getLogger	();
				void	log			(std::string  callingClass, 
									 std::string  message, 
									 TP::LOG_TYPE logType = DEFAULT_LOGTYPE_TO_USE);
	private:
		std::ofstream _logWriter;
		TP::LOG_TYPE  _minLevelToLog;
		static Logger* logger; //the single object

		static		 std::string  _configDetailList[];

		static const std::string  CONFIG_LABEL_LIST[];
		static const std::string  CONFIG_DEFAULT_VALUES_LIST[];
		static const std::string  CONFIG_FILE_NAME;

		static const std::string  DEFAULT_UNCONFIGURED_VALUE;
		static const std::string  DEFAULT_LOG_FILE_NAME;
		static const std::string  DEFAULT_CLASS_TO_MONITOR;
		static const std::string  DEFAULT_LEVEL_TO_LOG;
		static const std::string  DEFAULT_LOG_MESSAGE_LINKER;
		static const std::string  DEFAULT_KEY_VALUE_DELIMITER;
		static const std::string  DEFAULT_INFO_LOG_IDENTIFIER;
		static const std::string  DEFAULT_NOTICE_LOG_IDENTIFIER;
		static const std::string  DEFAULT_WARNING_LOG_IDENTIFIER;
		static const std::string  DEFAULT_ERROR_LOG_IDENTIFIER;
		static const std::string  DEFAULT_FATAL_LOG_IDENTIFIER;
		static const TP::LOG_TYPE DEFAULT_LOGTYPE_TO_USE;

		static const std::string  MESSAGE_ERROR_FATAL_LOG;
		static const std::string  MESSAGE_START_LOG;

		static const int		  CONFIG_DETAILS_SIZE;
		static const int		  FILENAME;
		static const int		  CLASS_TO_MONITOR;
		static const int		  MIN_LEVEL_TO_LOG;

		// initialisation functions
		static void initialiseLogger	();
		static void createLogger		();
		
		//log functions
		void logTheMessage				(std::string message, TP::LOG_TYPE logType);
		void logInfo					(std::string message);
		void logNotice					(std::string message);
		void logWarning					(std::string message);
		void logError					(std::string message);
		void logFatal					(std::string message);

		//helper functions
		void openLogFileToWrite			();
		void setDefaultValues			();
		void extractMinLevelToMonitor	();
		void printStartMessage			();
		std::string getNewData			(std::string newLine);
		std::string getNewLabel			(std::string newLine);
		bool isToBeLogged				(std::string  callingClass,
										 TP::LOG_TYPE logType);

		std::string getCurTime			();

		//configuration functions
		void configureLogger	();
		void loadConfigData		();
		void extractInfo		(const std::string& configLine,
									   std::string&	configLabel,
									   std::string&	configData);

		void saveExtractedInfo	(const std::string& configLabel, 
								 const std::string& configData);

		//constructor
		Logger	();
		Logger	(Logger const&);

		//equals operator overload
		void operator=(Logger const&);

		//destructor
		~Logger();
};

#endif