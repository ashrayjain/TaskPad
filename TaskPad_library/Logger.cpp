/*
 * =====================================================================================
 *
 *		Filename:  Logger.cpp
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file implements all of the functions defined in Logger.h file
  * 
 */

#ifndef _LOGGER_CPP_
#define _LOGGER_CPP_

#include <string>
#include <ctime>
#include <sstream>
#include "Logger.h"
#include "Enum.h"

using namespace std;
using namespace TP;

const int Logger::CONFIG_DETAILS_SIZE						= 3;
const int Logger::FILENAME									= 0;
const int Logger::CLASS_TO_MONITOR							= 1;
const int Logger::MIN_LEVEL_TO_LOG							= 2;
	
const std::string Logger::CONFIG_FILE_NAME					= "../configLogger.txt";

const std::string Logger::DEFAULT_LOG_FILE_NAME				= "TaskPadLog.txt";
const std::string Logger::DEFAULT_CLASS_TO_MONITOR			= "";
const std::string Logger::DEFAULT_LEVEL_TO_LOG				= "INFOLOG";
const std::string Logger::DEFAULT_CONFIG_VALUE				= "";
const std::string Logger::DEFAULT_LOG_MESSAGE_LINKER		= ": ";
const std::string Logger::DEFAULT_KEY_VALUE_DELIMITER		= " ";
const std::string Logger::DEFAULT_INFO_LOG_IDENTIFIER		= "\t\t\t";
const std::string Logger::DEFAULT_NOTICE_LOG_IDENTIFIER		= "\t\t";
const std::string Logger::DEFAULT_WARNING_LOG_IDENTIFIER	= "\t";
const std::string Logger::DEFAULT_ERROR_LOG_IDENTIFIER		= "";
const std::string Logger::DEFAULT_FATAL_LOG_IDENTIFIER		= LOG_TYPE_STRING[FATALLOG] + DEFAULT_LOG_MESSAGE_LINKER;

const std::string Logger::MESSAGE_ERROR_FATAL_LOG			= "Fatal log occurred!";
const std::string Logger::MESSAGE_START_LOG					= "Logging Started at: ";

const std::string Logger::CONFIG_LABEL_LIST[CONFIG_DETAILS_SIZE]		= {"FileName:","ClasstoMonitor:","MinLogLevel:"};
const std::string Logger::CONFIG_DEFAULT_VALUES_LIST[CONFIG_DETAILS_SIZE]	= {DEFAULT_LOG_FILE_NAME,DEFAULT_CLASS_TO_MONITOR,DEFAULT_LEVEL_TO_LOG};

std::string Logger::_configDetailList[CONFIG_DETAILS_SIZE]		= {DEFAULT_CONFIG_VALUE,DEFAULT_CONFIG_VALUE,DEFAULT_CONFIG_VALUE};

Logger* Logger::logger;

Logger* Logger::getLogger ()
{
	if(logger == NULL)
	{
		static Logger theOne;
		logger = &theOne;

		logger->configureLogger();
		logger->_logWriter.open(_configDetailList[FILENAME]);

		std::string curTime = logger->getCurTime();
		logger->_logWriter << MESSAGE_START_LOG << curTime << endl;
	}
	return logger;
}

void Logger::configureLogger()
{
	loadConfigData();
	setDefaultValues();
	extractMinLevelToMonitor();
}

void Logger::loadConfigData()
{
	ifstream configFile(CONFIG_FILE_NAME);
	std::string configLabel;
	std::string configData;
	std::string configLine;

	while(configFile.is_open() && configFile.good())
	{
		getline(configFile,configLine);

		configLabel = getNewLabel(configLine);
		configData = getNewData(configLine);

		for(int i = 0;i<CONFIG_DETAILS_SIZE;i++)
		{
			if(configLabel == CONFIG_LABEL_LIST[i])
			{
				_configDetailList[i] = configData;
				break;
			}
		}
	}
}

void Logger::setDefaultValues()
{
	//set the default values for those which are still empty strings
	for(int i=0; i<CONFIG_DETAILS_SIZE;i++)
	{
		if(_configDetailList[i] == DEFAULT_CONFIG_VALUE)
		{
			_configDetailList[i] = CONFIG_DEFAULT_VALUES_LIST[i];
		}
	}
	return;
}

void Logger::extractMinLevelToMonitor()
{
	for (LOG_TYPE logType = INFOLOG; logType < LOG_TYPE_COUNT;logType = static_cast<LOG_TYPE>(logType + 1))
	{
		if(LOG_TYPE_STRING[logType] == _configDetailList[MIN_LEVEL_TO_LOG])
		{
			_minLevelToLog = logType;
			break;
		}
	}
}

std::string Logger::getNewLabel(std::string newLine)
{
	std::string label;
	stringstream tempStream(newLine);

	tempStream >> label;

	return label;
}

std::string Logger::getNewData(std::string newLine)
{
	int pos = newLine.find_first_of(DEFAULT_KEY_VALUE_DELIMITER);
	if(pos != std::string::npos) {
		return newLine.substr(pos+1);
	}
	else {
		return DEFAULT_CONFIG_VALUE;
	}
}

std::string Logger::getCurTime()
{
	char logTime[40];
	time_t rawTime = time(0);
	tm curTimeTm;
	localtime_s(&curTimeTm,&rawTime);
	strftime(logTime, 10, "%A %d-%m-%Y, %T",&curTimeTm);

	return logTime;
}

Logger::Logger()
{
}

void Logger::log (std::string callingClass, std::string message, LOG_TYPE logType)
{
	if(isToBeLogged(callingClass,logType))
	{
		std::string completeMessage = callingClass + DEFAULT_LOG_MESSAGE_LINKER + message;
		switch(logType)
		{
			case INFOLOG:
				logInfo(completeMessage);
				break;
			case NOTICELOG:
				logNotice(completeMessage);
				break;
			case WARNINGLOG:
				logWarning(completeMessage);
				break;
			case ERRORLOG:
				logError(completeMessage);
				break;
			case FATALLOG:
				logFatal(completeMessage);
				throw FatalLogException(MESSAGE_ERROR_FATAL_LOG);
				break;
		}
	}

	return;
}

bool Logger::isToBeLogged(std::string callingClass, LOG_TYPE logType)
{
	bool isOfDesiredLevel = static_cast<int>(logType) >= static_cast<int>(_minLevelToLog);
	bool isOfDesiredClass = (_configDetailList[CLASS_TO_MONITOR].empty()) || (_configDetailList[CLASS_TO_MONITOR] == callingClass);

	return isOfDesiredClass && isOfDesiredLevel;
}

void Logger::logInfo (std::string message) {
	_logWriter << DEFAULT_INFO_LOG_IDENTIFIER << message << endl;
}
void Logger::logNotice (std::string message) {
	_logWriter << DEFAULT_NOTICE_LOG_IDENTIFIER << message << endl;
}
void Logger::logWarning (std::string message) {
	_logWriter << DEFAULT_WARNING_LOG_IDENTIFIER << message << endl;
}
void Logger::logError (std::string message) {
	_logWriter << message << endl;
}
void Logger::logFatal (std::string message) {
	_logWriter << DEFAULT_FATAL_LOG_IDENTIFIER << message << endl;
}

Logger::~Logger()
{
	this->_logWriter.close();
}

#endif