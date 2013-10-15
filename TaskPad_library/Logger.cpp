#ifndef _LOGGER_CPP_
#define _LOGGER_CPP_

#include <string>
#include <ctime>
#include "Logger.h"

using namespace std;

const std::string Logger::_defaultLogFileName = "TaskPadLog.txt";
Logger* Logger::logger;

Logger* Logger::getLogger (std::string logFileName)
{
	if(logger == NULL)
	{
		static Logger theOne;
		logger = &theOne;

		logger->_minLevelToLog = INFO;
		logger->_curLogFileName = logFileName;
		logger->_logWriter.open(logger->_curLogFileName);

		std::string curTime = logger->getCurTime();
		logger->_logWriter << "Logging Started at: " << curTime << endl;
	}
	return logger;
}

std::string Logger::getCurTime()
{
	time_t rawTime;
	time(&rawTime);

	return ctime(&rawTime);

}

Logger::Logger()
{
	_curLogFileName = "";
}

void Logger::log (std::string callingClass, std::string message, LOG_TYPE logType)
{
	if(static_cast<int>(logType) >= static_cast<int>(_minLevelToLog))
	{
		std::string completeMessage = callingClass + ": " +message;
		switch(logType)
		{
			case INFO:
				logInfo(completeMessage);
				break;
			case IMPT_INFO:
				logImptInfo(completeMessage);
				break;
			case WARNING:
				logWarning(completeMessage);
				break;
			case ERROR:
				logError(completeMessage);
				break;
			case FATAL:
				logFatal(completeMessage);
				throw "Fatal log occurred!";
				break;
		}
		_logWriter.flush();
	}

	return;
}

void Logger::logInfo (std::string message) {
	_logWriter << "\t\t\t" << message << endl;
}
void Logger::logImptInfo (std::string message) {
	_logWriter << "\t\t" << message << endl;
}
void Logger::logWarning (std::string message) {
	_logWriter << "\t" << message << endl;
}
void Logger::logError (std::string message) {
	_logWriter << message << endl;
}
void Logger::logFatal (std::string message) {
	_logWriter << "FATAL: " << message << endl;
}

Logger::~Logger()
{
	this->_logWriter.close();
}

#endif