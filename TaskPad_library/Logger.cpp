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

const int Logger::CONFIG_DETAILS_SIZE = 3;
const int Logger::FILENAME = 0;
const int Logger::CLASS_TO_MONITOR = 1;
const int Logger::MIN_LEVEL_TO_LOG = 2;

const std::string Logger::_configFileName = "../configLogger.txt";
const std::string Logger::DEFAULT_CONFIG_FILE = "TaskPadLog.txt";
const std::string Logger::DEFAULT_CLASS_TO_MONITOR = "";
const std::string Logger::DEFAULT_LEVEL_TO_LOG = "INFOLOG";

const std::string Logger::configLabelList[CONFIG_DETAILS_SIZE] = {"FileName:","ClasstoMonitor:","MinLogLevel:"};
	  std::string Logger::configDetailList[CONFIG_DETAILS_SIZE] = {"","",""};
const std::string Logger::configDefaultValues[CONFIG_DETAILS_SIZE] = {DEFAULT_CONFIG_FILE,DEFAULT_CLASS_TO_MONITOR,DEFAULT_LEVEL_TO_LOG};

Logger* Logger::logger;

Logger* Logger::getLogger ()
{
	if(logger == NULL)
	{
		static Logger theOne;
		logger = &theOne;

		logger->configureLogger();
		logger->_logWriter.open(configDetailList[FILENAME]);

		std::string curTime = logger->getCurTime();
		logger->_logWriter << "Logging Started at: " << curTime << endl;
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
	ifstream configFile(_configFileName);
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
			if(configLabel == configLabelList[i])
			{
				configDetailList[i] = configData;
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
		if(configDetailList[i] == "")
		{
			configDetailList[i] = configDefaultValues[i];
		}
	}
	return;
}

void Logger::extractMinLevelToMonitor()
{
	for (TP::LOG_TYPE logType = TP::INFOLOG; logType < TP::LOG_TYPE_COUNT;logType = static_cast<TP::LOG_TYPE>(logType + 1))
	{
		if(TP::LOG_TYPE_STRING[logType] == configDetailList[MIN_LEVEL_TO_LOG])
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
	int pos = newLine.find_first_of(" ");
	if(pos != std::string::npos) {
		return newLine.substr(pos+1);
	}
	else {
		return "";
	}
}

std::string Logger::getCurTime()
{
	time_t rawTime;
	time(&rawTime);

	return ctime(&rawTime);
}

Logger::Logger()
{
}

void Logger::log (std::string callingClass, std::string message, TP::LOG_TYPE logType)
{
	if(isToBeLogged(callingClass,logType))
	{
		std::string completeMessage = callingClass + ": " +message;
		switch(logType)
		{
			case TP::INFOLOG:
				logInfo(completeMessage);
				break;
			case TP::NOTICELOG:
				logNotice(completeMessage);
				break;
			case TP::WARNINGLOG:
				logWarning(completeMessage);
				break;
			case TP::ERRORLOG:
				logError(completeMessage);
				break;
			case TP::FATALLOG:
				logFatal(completeMessage);
				throw "Fatal log occurred!";
				break;
		}
	}

	return;
}

bool Logger::isToBeLogged(std::string callingClass, TP::LOG_TYPE logType)
{
	bool isOfDesiredLevel = static_cast<int>(logType) >= static_cast<int>(_minLevelToLog);
	bool isOfDesiredClass = (configDetailList[CLASS_TO_MONITOR].empty()) || (configDetailList[CLASS_TO_MONITOR] == callingClass);

	return isOfDesiredClass && isOfDesiredLevel;
}

void Logger::logInfo (std::string message) {
	_logWriter << "\t\t\t" << message << endl;
}
void Logger::logNotice (std::string message) {
	_logWriter << "\t\t" << message << endl;
}
void Logger::logWarning (std::string message) {
	_logWriter << "\t" << message << endl;
}
void Logger::logError (std::string message) {
	_logWriter << message << endl;
}
void Logger::logFatal (std::string message) {
	_logWriter << TP::LOG_TYPE_STRING[TP::FATALLOG] << ": " << message << endl;
}

Logger::~Logger()
{
	this->_logWriter.close();
}

#endif