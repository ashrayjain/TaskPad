#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <fstream>

//a singleton for the whole program
class Logger
{
	public:
		enum LOG_TYPE {
			INFO, IMPT_INFO, WARNING, ERROR, FATAL
		};

		void setMinLevelToLog	(LOG_TYPE);

		static	Logger*	getLogger	(std::string logFileName = _defaultLogFileName);
				void	log			(std::string callingClass, std::string message, LOG_TYPE logType = INFO);

		~Logger();
	private:
		std::string _curLogFileName;
		std::ofstream _logWriter;
		static const std::string _defaultLogFileName;

		LOG_TYPE _minLevelToLog;

		//the single object
		static Logger* logger;
		
		//log functions
		void logInfo(std::string message);
		void logImptInfo(std::string message);
		void logWarning(std::string message);
		void logError(std::string message);
		void logFatal(std::string message);

		//helper functions

		std::string getCurTime();
		//constructor
		Logger();
		Logger(Logger const&);
		void operator=(Logger const&);


};

#endif