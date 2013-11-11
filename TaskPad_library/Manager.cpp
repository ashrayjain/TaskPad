/*
 * =====================================================================================
 *
 *		Filename:  Manager.cpp
 *
 *      Version:  2.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file implements all of the functions defined in Manager.h file
  * 
 */

#include <ctime>
#include <cassert>
#include "Manager.h"
#include "Storage.h"
#include "Messenger.h"
#include "Executor.h"
#include "Interpreter.h"
#include "Command.h"
#include "Task.h"
#include "Enum.h"
#include "Logger.h"
#include "Datastore.h"

using namespace TP;
using namespace std;

const string Manager::CLASS_NAME = "Manager";

const string Manager::MESSAGE_INDEX_OUT_OF_RANGE						= "Given index is out of range!";
const string Manager::MESSAGE_ERROR_UNEXPECTED_COMMAND_TYPE_WITH_INDEX	= "Unexpected Command with index!";
const string Manager::MESSAGE_DATE_LIMIT_REACHED						= "We have reached the end of time!";
const string Manager::MESSAGE_INDEX_NOT_GIVEN							= "Please provide an index!";

const string Manager::FIND_TODAY_TASKS_COMMAND	= "find from `today midnight` to `today 23:59` undone";

const int	 Manager::LOWER_END_OF_TIME									= 70;
const int	 Manager::UPPER_END_OF_TIME									= 200;
const int	 Manager::DAY_UNIT_OF_TIME									= 1;
const int	 Manager::WEEK_UNIT_OF_TIME									= 7;
const int	 Manager::MONTH_UNIT_OF_TIME								= 1;

//@Manikandan Thyagesh A0100124J
Manager::Manager() {
	_taskDS					= new Datastore;
	_logger					= Logger::getLogger();
	_storage				= new Storage(_taskDS);
	_executor				= new Executor(*_taskDS);
	_interpreter			= new Interpreter;
	_response				= Messenger();
	_cmd					= NULL;
	_lastSuccessfulFindCmd	= NULL;
}

Messenger Manager::refreshList() {
	_logger->log(CLASS_NAME, "entering refreshList()");

	_executor->executeCommand(_lastSuccessfulFindCmd,_response);
	return _response;
}

list<Task> Manager::getCurrentReminders	() {
	_logger->log(CLASS_NAME, "entering getCurrentReminders()");

	return _executor->getCurrentReminders();
}

void Manager::syncTaskList (const list<Task>& taskList) {
	_response.setList(taskList);
}

void Manager::syncTask (const Task& task) {
	_response.setTask(task);
}

void Manager::resetStatus() {
	_logger->log(CLASS_NAME, "entering resetStatus()", NOTICELOG);

	removePreviousCommand();
	removeLastSuccessfulFindCommand();
	_currentPeriod = pair<tm,tm>();
	_response.resetMessenger();
	tm todayTm = getTodayTm();
	tm nextDayTm = getNextDayTm(todayTm);
	setCurrPeriod(todayTm,nextDayTm);
}

Messenger Manager::processCommand(const string& newCommand) {
	_logger->log(CLASS_NAME, "processing Command");

	switch(_response.getStatus()) {
		case INTERMEDIATE:
			/* empty and falls through*/
		case ERR_INTER:
			handleIntermediateScenarioCommands(newCommand);
			break;
		default:
			removePreviousCommand();
			handleNormalScenarioCommands(newCommand);
			break;
	}
	saveChanges();

	_logger -> log(CLASS_NAME, "leaving processCommand() \n");
	return _response;
}

/*
 * Saves the changed Task by calling the API of the storage class
 */

void Manager::saveChanges()
{
	if(isNotSuccessfulCommand()) return;

	_logger->log(CLASS_NAME,"saving changes");

	switch(_cmd->getCommandType()) {
		case ADD:
			/* empty and falls through*/
		case MOD:
			/* empty and falls through*/
		case DEL:
			try {
				_storage->save(_response.getTask(), _cmd->getCommandType());
			}
			catch(BaseException be) {
				setResponseToError(ERR,be.what());
			}
			break;
		case FIND:
				updateLastSuccessfulFindCommand();
			break;
		default:
			break;
	}
}

/**
 * Deletes the previously created command and,
 * returns the memory to the system
 */
void Manager::removePreviousCommand() {
	if(_cmd != NULL) {
		delete _cmd;
		_cmd = NULL;
	}
	return;
}

void Manager::updateLastSuccessfulFindCommand	() {
	 removeLastSuccessfulFindCommand();

	 _lastSuccessfulFindCmd	= new Command_Find;
	*(_lastSuccessfulFindCmd) = *(_cmd);

}

void Manager::removeLastSuccessfulFindCommand() {
	if(_lastSuccessfulFindCmd != NULL) {
		delete _lastSuccessfulFindCmd;
		_lastSuccessfulFindCmd = NULL;
	}
	return;
}

/**
 * Handles the 3 types of commands that can be given in a normal scenario
 * They are:
 * - An index (for displaying the details of a task)
 * - A command with an index (non-ambiguous mod/delete commands)
 * - A generic command (an ambiguous command of any type)
 *
*/
void Manager::handleNormalScenarioCommands(string newCommand) {
	_logger->log(CLASS_NAME,"handling normal scenario command");

	if (!interpretCommand(newCommand)) return;

	if(isShowCommand()) {
		_logger->log(CLASS_NAME,"index given by user",NOTICELOG);
		storeIndexFromCommandToClassAttribute();
		handleShowCommand();
	}
	else if (isCommandWithIndexGiven()) {
		_logger->log(CLASS_NAME,"command with index given by user",NOTICELOG);
		storeIndexFromCommandToClassAttribute();
		handleCommandWithIndex();
	}
	else  {
		_logger->log(CLASS_NAME,"generic command given by user",NOTICELOG);
		handleGenericCommand();
	}
	return;
}

bool Manager::interpretCommand(string newCommand) {
	_cmd = _interpreter->interpretCommand(newCommand,_response);
	return hasNoInterpretationError();
}

/**
 * This handles the scenario where an ambiguous mod/del was done and 
 * the user was given a list of similar tasks to choose the actual one he was talking about
 * Accepts:
 * - Only index
 */
void Manager::handleIntermediateScenarioCommands(string newCommand) {
	Command* backupCommand = _cmd;
	interpretCommand(newCommand);

	if(isIndexGiven()) {
		removePreviousCommand();
		_cmd = backupCommand;
		_response.setStatus(INTERMEDIATE);
		_response.setCommandType(_cmd->getCommandType());
		handleIntermediateIndexCommand();
	}
	else {
		removePreviousCommand();
		_cmd = backupCommand;
		_response.setStatus(INTERMEDIATE);
		_response.setCommandType(_cmd->getCommandType());
		setResponseToError(ERR_INTER, MESSAGE_INDEX_NOT_GIVEN);
	}
	return;
}

void Manager::handleShowCommand() {
	if(isIndexWithinRange()) {
		_response.setInt(_index);
		_response.setStatus(DISPLAY);
	}
	else {
		setResponseToError(ERR, MESSAGE_INDEX_OUT_OF_RANGE);
	}
}

void Manager::handleIntermediateIndexCommand() {
	if(isIndexWithinRange()) {
		insertActualIndexIntoCommand();
		_executor->executeCommand(_cmd,_response);
	}
	else {
		setResponseToError(ERR_INTER, MESSAGE_INDEX_OUT_OF_RANGE);
	}
}

void Manager::handleCommandWithIndex()
{
	if(isIndexWithinRange()) {
		insertActualIndexIntoCommand();
		_executor->executeCommand(_cmd,_response);

		if(hasNoError()) {
			editTaskListInResponse();
			_response.setStatus(SUCCESS_INDEXED_COMMAND);
		}
	}
	else {
		setResponseToError(ERR, MESSAGE_INDEX_OUT_OF_RANGE);
	}
}

void Manager::handleGenericCommand()
{
	if(hasNoInterpretationError()) {
		_executor->executeCommand(_cmd,_response);
	}
}

void Manager::editTaskListInResponse()
{
	assert(isDeleteCommand() || isModifyCommand());

	list<Task> tempList = _response.getList();
	list<Task>::iterator lit = tempList.begin();
	advance (lit,(_index - 1));

	if(isDeleteCommand()) {
		tempList.erase(lit);
	}
	else if(isModifyCommand()) {
		(*lit) = _response.getTask();
	}
	_response.setList(tempList);
	return;
}

bool Manager::isIndexGiven() {
	if(isShowCommand()) {
		storeIndexFromCommandToClassAttribute();
		return true;
	}

	return false;
}

bool Manager::isCommandWithIndexGiven() {
	bool isModifyCommandWithIndex = false, isDeleteCommandWithIndex = false;

	switch (_cmd->getCommandType()) {
		case MOD:
			isModifyCommandWithIndex = isIndexedModifyCommand();
			break;
		case DEL:
			isDeleteCommandWithIndex = isIndexedDeleteCommand();
			break;
		default:
			break;
	}

	if(isModifyCommandWithIndex || isDeleteCommandWithIndex) {
		return true;
	}
	else {
		return false;
	}
}

bool Manager::isIndexedModifyCommand() {
	Command_Mod tempCommand = *((Command_Mod *) _cmd);
	return tempCommand.getFlagIndex();
}

bool Manager::isIndexedDeleteCommand() {
	Command_Del tempCommand = *((Command_Del *) _cmd);
	return tempCommand.getFlagIndex();
}

bool Manager::isIndexWithinRange() {
	int sizeOfCurrentList = _response.getList().size();
	return (sizeOfCurrentList >= _index && _index > 0);
}

void Manager::insertActualIndexIntoCommand() {
	assert(_cmd != NULL);
	assert(isModifyCommand() || isDeleteCommand());

	unsigned long long actualIndex = getActualIndexOfTask();

	switch(_cmd->getCommandType()) {
		case MOD:
			insertActualIndexIntoModifyCommand(actualIndex);
			break;
		case DEL:
			insertActualIndexIntoDeleteCommand(actualIndex);
			break;
	}
}

void Manager::insertActualIndexIntoDeleteCommand(unsigned long long& actualIndex) {
	Command_Del* tempCommand = (Command_Del *) _cmd;
	tempCommand->setCreatedTime(actualIndex);
	return;
}

void Manager::insertActualIndexIntoModifyCommand(unsigned long long& actualIndex) {
	Command_Mod* tempCommand = (Command_Mod *) _cmd;
	tempCommand->setCreatedTime(actualIndex);
	return;
}

Task Manager::getAffectedTask() const {
	list<Task> tempList = _response.getList();
	list<Task>::iterator it = tempList.begin();
	advance(it,(_index-1));

	return (*it);
}

unsigned long long Manager::getActualIndexOfTask() const {
	Task chosenTask = getAffectedTask();
	unsigned long long createdTime = chosenTask.getIndex();
	return createdTime;
}

void Manager::storeIndexFromCommandToClassAttribute() {
	assert(isModifyCommand() || isDeleteCommand() || isShowCommand());

	switch (_cmd->getCommandType()) {
	case MOD:
			storeIndexFromModCommandToClassAttribute();
			break;
		case DEL:
			storeIndexFromDelCommandToClassAttribute();
			break;
		case SHOW:
			storeIndexFromShowCommandToClassAttribute();
			break;
	}
	return;
}

void Manager::storeIndexFromModCommandToClassAttribute() {
	Command_Mod* cmdTemp = (Command_Mod*) _cmd;
	_index = cmdTemp->getIndex();
}

void Manager::storeIndexFromDelCommandToClassAttribute() {
	Command_Del* cmdTemp = (Command_Del*) _cmd;
	_index = cmdTemp->getIndex();
}
void Manager::storeIndexFromShowCommandToClassAttribute() {
	Command_Show* cmdTemp = (Command_Show*) _cmd;
	_index = cmdTemp->getIndex();
}

string Manager::createFindCommand(tm startTm, tm endTm) {
	string startTmStr = getStrFromTm(startTm);
	string endTmStr = getStrFromTm(endTm);

	return "find from `" + startTmStr + " midnight` to `" + endTmStr + " 11:59pm`";
}

string Manager::getStrFromTm(tm timeInfo) {
	char todayCharArray [80];

	strftime (todayCharArray,80,"%d/%m/%Y",&timeInfo);

	//convert the char array to a string
	string today(todayCharArray);

	return today;
}

tm Manager::getTodayTm() {
	time_t rawtime;
	struct tm * timeInfo;

	//get the time right now, convert it to a string in the format DD/MM/YYYY
	time (&rawtime);
	timeInfo = localtime (&rawtime);

	return *timeInfo;
}

bool Manager::hasInterpretationError() {
	return !hasNoInterpretationError();
}

bool Manager::hasNoInterpretationError() {
	 return hasNoError();
}

bool Manager::hasNoError() {
	if(_response.getStatus() == ERR || _response.getStatus() == ERR_INTER) {
		return false;
	}
	else {
		return true;
	}
}

bool Manager::isNotSuccessfulCommand() {
	if(_response.getStatus() == SUCCESS || _response.getStatus() == SUCCESS_INDEXED_COMMAND) {
		return false;
	}
	return true;
}

/*********************************************************/
/************* NAVIGATION RELATED METHODS ****************/
/*********************************************************/

Messenger Manager::getTodayTasks() {
	tm todayTm = getTodayTm();
	tm nextDayTm = getNextDayTm(todayTm);
	setCurrPeriod(todayTm,nextDayTm);

	return processCommand(FIND_TODAY_TASKS_COMMAND);
}

Messenger Manager::getNextPeriodTasks(PERIOD_TYPE pType) {
	tm currTm = _currentPeriod.second;
	tm nextTm = getNextTm(pType, currTm);

	bool canAdvanceToNextPeriod = (mktime(&nextTm) != mktime(&currTm));

	if(canAdvanceToNextPeriod) {
		setCurrPeriod(currTm,nextTm);
		string command = createFindCommand(currTm,nextTm);
		return processCommand(command);
	}
	else {
		setResponseToError(ERR,MESSAGE_DATE_LIMIT_REACHED);
		return _response;
	}
}

Messenger Manager::getPrevPeriodTasks(PERIOD_TYPE pType) {
	tm currTm = _currentPeriod.first;
	tm prevTm = getPrevTm(pType, currTm);
	bool canAdvanceToPrevPeriod = mktime(&prevTm) != mktime(&currTm);

	if(canAdvanceToPrevPeriod) {
		setCurrPeriod(prevTm, currTm);
		string command = createFindCommand(prevTm, currTm);
		return processCommand(command);
	}
	else {
		setResponseToError(ERR,MESSAGE_DATE_LIMIT_REACHED);
		return _response;
	}
}

void Manager::setResponseToError(const STATUS& status, const string& message) {
	_response.setStatus(ERR);
	_response.setErrorMsg(message);
	return;
}

pair<tm,tm> Manager::getCurrentPeriod() {
	return _currentPeriod;
}

tm Manager::getNextTm(const PERIOD_TYPE& pType, tm currTm) {
	tm nextTm;
	switch(pType) {
		case DAY:
			nextTm = getNextDayTm(currTm);
			break;
		case WEEK:
			nextTm = getNextWeekTm(currTm);
			break;
		case MONTH:
			nextTm = getNextMonthTm(currTm);
			break;
	}

	return nextTm;
}

tm Manager::getNextDayTm(tm currTm) {
	time_t intermediateResult;
	currTm.tm_mday += DAY_UNIT_OF_TIME;

	intermediateResult = mktime(&currTm);

	if(currTm.tm_year == UPPER_END_OF_TIME) {
		currTm.tm_mday -= DAY_UNIT_OF_TIME;
		intermediateResult = mktime(&currTm);
	}

	return *localtime(&intermediateResult);
}

tm Manager::getNextWeekTm(tm currTm) {
	time_t intermediateResult;
	currTm.tm_mday += WEEK_UNIT_OF_TIME;
	intermediateResult = mktime(&currTm);

	if(currTm.tm_year == UPPER_END_OF_TIME) {
		currTm.tm_mday -= WEEK_UNIT_OF_TIME;
		intermediateResult = mktime(&currTm);
	}
	
	return *localtime(&intermediateResult);
}

tm Manager::getNextMonthTm(tm currTm) {
	time_t intermediateResult;
	currTm.tm_mon += MONTH_UNIT_OF_TIME;
	intermediateResult = mktime(&currTm);
	
	if(currTm.tm_year == UPPER_END_OF_TIME) {
		currTm.tm_mon -= MONTH_UNIT_OF_TIME;
		intermediateResult = mktime(&currTm);
	}

	return *localtime(&intermediateResult);
}

tm Manager::getPrevTm(const PERIOD_TYPE& pType, tm currTm) {
	tm prevTm;
	switch(pType) {
		case DAY:
			prevTm = getPrevDayTm(currTm);
			break;
		case WEEK:
			prevTm = getPrevWeekTm(currTm);
			break;
		case MONTH:
			prevTm = getPrevMonthTm(currTm);
			break;
	}
	return prevTm;
}

tm Manager::getPrevDayTm(tm currTm) {
	time_t intermediateResult;
	currTm.tm_mday -= DAY_UNIT_OF_TIME;

	intermediateResult = mktime(&currTm);

	if(currTm.tm_year == LOWER_END_OF_TIME) {
		currTm.tm_mday += DAY_UNIT_OF_TIME;
		intermediateResult = mktime(&currTm);
	}

	return *localtime(&intermediateResult);
}

tm Manager::getPrevWeekTm(tm currTm) {
	time_t intermediateResult;
	currTm.tm_mday -= WEEK_UNIT_OF_TIME;
	intermediateResult = mktime(&currTm);

	if(currTm.tm_year == LOWER_END_OF_TIME) {
		currTm.tm_mday += WEEK_UNIT_OF_TIME;
		intermediateResult = mktime(&currTm);
	}
	
	return *localtime(&intermediateResult);
}

tm Manager::getPrevMonthTm(tm currTm) {
	time_t intermediateResult;
	currTm.tm_mon -= MONTH_UNIT_OF_TIME;
	intermediateResult = mktime(&currTm);

	if(currTm.tm_year == LOWER_END_OF_TIME) {
		currTm.tm_mon += MONTH_UNIT_OF_TIME;
		intermediateResult = mktime(&currTm);
	}
	
	return *localtime(&intermediateResult);
}

void Manager::setCurrPeriod(tm startTm, tm endTm) {
	_currentPeriod = pair<tm,tm>(startTm,endTm);
}

//validators
bool Manager::isDeleteCommand() {
	return (_cmd->getCommandType() == DEL);
}

bool Manager::isModifyCommand() {
	return (_cmd->getCommandType() == MOD);
}

bool Manager::isShowCommand() {
	return _response.getCommandType() == SHOW;
}

Manager::~Manager() {
	_storage->save(_taskDS);

	if(_taskDS != NULL) {
		delete _taskDS;
		_taskDS = NULL;
	}

	if(_interpreter != NULL) {
		delete _interpreter;
		_interpreter = NULL;
	}

	if(_executor != NULL) {
		delete _executor;
		_executor = NULL;
	}

	if(_storage != NULL) {
		delete _storage;
		_storage = NULL;
	}

	removePreviousCommand				();
	removeLastSuccessfulFindCommand		();
	_response.resetMessenger			();
}
