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

#include <string>
#include <ctime>
#include <sstream>
#include "Logger.h"
#include "Enum.h"

using namespace std;
using namespace TP;

const int	 Logger::CONFIG_DETAILS_SIZE					= 3;
const int	 Logger::FILENAME								= 0;
const int	 Logger::CLASS_TO_MONITOR						= 1;
const int	 Logger::MIN_LEVEL_TO_LOG						= 2;
	
const string Logger::CONFIG_FILE_NAME						= "../configLogger.txt";

const string	Logger::DEFAULT_UNCONFIGURED_VALUE			= "";
const string	Logger::DEFAULT_LOG_FILE_NAME				= "TaskPadLog.txt";
const string	Logger::DEFAULT_CLASS_TO_MONITOR			= "";
const string	Logger::DEFAULT_LEVEL_TO_LOG				= "INFOLOG";
const string	Logger::DEFAULT_LOG_MESSAGE_LINKER			= ": ";
const string	Logger::DEFAULT_KEY_VALUE_DELIMITER			= " ";
const string	Logger::DEFAULT_INFO_LOG_IDENTIFIER			= "\t\t\t";
const string	Logger::DEFAULT_NOTICE_LOG_IDENTIFIER		= "\t\t";
const string	Logger::DEFAULT_WARNING_LOG_IDENTIFIER		= "\t";
const string	Logger::DEFAULT_ERROR_LOG_IDENTIFIER		= "";
const string	Logger::DEFAULT_FATAL_LOG_IDENTIFIER		= LOG_TYPE_STRING[FATALLOG] + DEFAULT_LOG_MESSAGE_LINKER;
const LOG_TYPE	Logger::DEFAULT_LOGTYPE_TO_USE				= INFOLOG;

const string	Logger::MESSAGE_ERROR_FATAL_LOG				= "Fatal log occurred!";
const string	Logger::MESSAGE_START_LOG					= "Logging Started at: ";



const string Logger::CONFIG_LABEL_LIST[CONFIG_DETAILS_SIZE]				= {"FileName:","ClasstoMonitor:","MinLogLevel:"};
const string Logger::CONFIG_DEFAULT_VALUES_LIST[CONFIG_DETAILS_SIZE]	= 
						{DEFAULT_LOG_FILE_NAME,DEFAULT_CLASS_TO_MONITOR,DEFAULT_LEVEL_TO_LOG};

string Logger::_configDetailList[CONFIG_DETAILS_SIZE]			= 
						{DEFAULT_UNCONFIGURED_VALUE,DEFAULT_UNCONFIGURED_VALUE,DEFAULT_UNCONFIGURED_VALUE};

Logger* Logger::logger;



Logger::Logger() {
}

Logger* Logger::getLogger () {
	bool isFirstTimeToGetLogger = (logger == NULL);

	if(isFirstTimeToGetLogger) {
		initialiseLogger();
	}
	return logger;
}

void Logger::initialiseLogger() {
	createLogger();

	logger->configureLogger();
	logger->printStartMessage();
}

/****************************************************/
/************* Initialisation Methods ***************/
/****************************************************/

void Logger::createLogger () {
	static Logger theOne;
	logger = &theOne;
}

void Logger::configureLogger() {
	loadConfigData();
	setDefaultValues();
	extractMinLevelToMonitor();
	openLogFileToWrite();
}

void Logger::printStartMessage() {
	string curTime = getCurTime();
	logger->_logWriter << MESSAGE_START_LOG << curTime << endl;
}

void Logger::openLogFileToWrite() {
	logger->_logWriter.open(_configDetailList[FILENAME]);
}

/****************************************************/
/************** Configuration Methods ***************/
/****************************************************/

void Logger::loadConfigData() {
	ifstream configFile(CONFIG_FILE_NAME);
	string configLabel;
	string configData;
	string configLine;

	while(configFile.is_open() && configFile.good()) {
		getline(configFile,configLine);
		extractInfo(configLine, configLabel,configData);
		saveExtractedInfo (configLabel, configData);
	}
}

void Logger::extractInfo(const string& configLine,string& configLabel,string& configData) {
	configLabel = getNewLabel(configLine);
	configData	= getNewData(configLine);

	return;
}

void Logger::saveExtractedInfo	(const string& configLabel, const string& configData) {
	for(int i = 0;i<CONFIG_DETAILS_SIZE;i++) {
		if(configLabel == CONFIG_LABEL_LIST[i]) {
			_configDetailList[i] = configData;
			break;
		}
	}
}

void Logger::setDefaultValues()
{
	//set the default values for those which are still empty strings
	for(int i=0; i<CONFIG_DETAILS_SIZE;i++) {
		if(_configDetailList[i] == DEFAULT_UNCONFIGURED_VALUE) {
			_configDetailList[i] = CONFIG_DEFAULT_VALUES_LIST[i];
		}
	}

	return;
}

void Logger::extractMinLevelToMonitor() {
	for (LOG_TYPE logType = INFOLOG; logType < LOG_TYPE_COUNT; logType = static_cast<LOG_TYPE>(logType + 1)) {
		if(LOG_TYPE_STRING[logType] == _configDetailList[MIN_LEVEL_TO_LOG]) {
			_minLevelToLog = logType;
			break;
		}
	}
}

string Logger::getNewLabel(string newLine) {
	string label;
	stringstream tempStream(newLine);

	tempStream >> label;

	return label;
}

string Logger::getNewData(string newLine) {
	string newData = DEFAULT_UNCONFIGURED_VALUE;
	int pos = newLine.find_first_of(DEFAULT_KEY_VALUE_DELIMITER);

	if(pos != string::npos) {
		newData = newLine.substr(pos+1);
	}

	return newData;
}

string Logger::getCurTime()	{
	char logTime[40];
	time_t rawTime = time(0);
	tm curTimeTm;
	localtime_s(&curTimeTm, &rawTime);
	strftime(logTime, 40, "%d-%m-%Y",&curTimeTm);

	return logTime;
}

void Logger::log (string callingClass, string message, LOG_TYPE logType) {
	if(isToBeLogged(callingClass,logType)) {
		string completeMessage = callingClass + DEFAULT_LOG_MESSAGE_LINKER + message;
		logTheMessage(completeMessage, logType);
	}

	return;
}

bool Logger::isToBeLogged(string callingClass, LOG_TYPE logType) {
	bool isOfDesiredLevel = static_cast<int>(logType) >= static_cast<int>(_minLevelToLog);
	bool isOfDesiredClass = (_configDetailList[CLASS_TO_MONITOR].empty()) || (_configDetailList[CLASS_TO_MONITOR] == callingClass);

	return isOfDesiredClass && isOfDesiredLevel;
}

void Logger::logTheMessage(string message, LOG_TYPE logType) {
	switch(logType) {
		case INFOLOG:
			logInfo(message);
			break;
		case NOTICELOG:
			logNotice(message);
			break;
		case WARNINGLOG:
			logWarning(message);
			break;
		case ERRORLOG:
			logError(message);
			break;
		case FATALLOG:
			logFatal(message);
			throw FatalLogException(MESSAGE_ERROR_FATAL_LOG);
			break;
	}
}

void Logger::logInfo (string message) {
	_logWriter << DEFAULT_INFO_LOG_IDENTIFIER << message << endl;
}
void Logger::logNotice (string message) {
	_logWriter << DEFAULT_NOTICE_LOG_IDENTIFIER << message << endl;
}
void Logger::logWarning (string message) {
	_logWriter << DEFAULT_WARNING_LOG_IDENTIFIER << message << endl;
}
void Logger::logError (string message) {
	_logWriter << message << endl;
}
void Logger::logFatal (string message) {
	_logWriter << DEFAULT_FATAL_LOG_IDENTIFIER << message << endl;
}

Logger::~Logger() {
	this->_logWriter.close();
}