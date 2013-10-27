/*
 * =====================================================================================
 *
 *		Filename:  Manager.cpp
 *
 *      Version:  1.0
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

using namespace TP;

const string Manager::MESSAGE_INDEX_OUT_OF_RANGE = "Given index is out of range!";
const string Manager::MESSAGE_ERROR_UNEXPECTED_COMMAND_TYPE_WITH_INDEX = "Unexpected Command with index!!";

Manager::Manager() {
	this->_logger		= Logger::getLogger();
	this->_storage		= new Storage(_tasks);
	this->_executor		= new Executor(&_tasks);
	this->_interpreter	= new Interpreter;
	this->_response		= Messenger();
	this->_cmd			= NULL;
	this->_lastSuccessfulFindCmd = NULL;
}

Messenger Manager::refreshList()
{
	this->_executor->executeCommand(this->_lastSuccessfulFindCmd,this->_response);
	return this->_response;
}

list<Task> Manager::getCurrentReminders	()
{
	return this->_executor->getCurrentReminders();
}

void Manager::syncTaskList (const list<Task>& taskList)
{
	this->_response.setList(taskList);
}

void Manager::syncTask (const Task& task)
{
	this->_response.setTask(task);
}

void Manager::resetStatus() {
	if(this->_cmd != NULL && this->_cmd->getCommandType() != FIND)
		delete this->_cmd;
	this->_cmd = NULL;
	this->_currentPeriod = pair<tm,tm>();
	this->_response.resetMessenger();
	std::tm todayTm = getTodayTm();
	this->setCurrPeriod(todayTm,todayTm);
}

Manager::~Manager() {
	this->_storage->save(this->_tasks);
	delete this->_interpreter;
	delete this->_executor;
	delete this->_storage;
	delete this->_lastSuccessfulFindCmd;
	this->removePreviousCommand();
	this->_response.resetMessenger();
}

Messenger Manager::processCommand(const string& newCommand) {
	_logger->log("Manager", "processing Command");
	switch(this->_response.getStatus()) {
		case INTERMEDIATE:
			/* empty and falls through*/
		case ERROR_INTERMEDIATE:
			handleIntermediateScenarioCommands(newCommand);
			break;
		default:
			removePreviousCommand();
			handleNormalScenarioCommands(newCommand);
			break;
	}
	this->saveChanges();

	_logger -> log("Mananger", "leaving processCommand() \n");
	return this->_response;
}

/*
 * Saves the changed Task by calling the API of the storage class
 * Only saves if the command was of type MOD, DEL or ADD
 */

void Manager::saveChanges()
{
	if(this->isSuccessfulCommand()){
		switch(this->_cmd->getCommandType())
		{
			case MOD:
				/* empty and falls through*/
			case DEL:
				/* empty and falls through*/
			case ADD:
				_logger->log("Manager","saving changes");
				this->_storage->save(this->_response.getTask(),this->_response.getCommandType());
				break;
			case FIND:
				if(this->_lastSuccessfulFindCmd != NULL)
					delete this->_lastSuccessfulFindCmd;//delete last time cmd_Find
				this->_lastSuccessfulFindCmd = this->_cmd;
				break;
			default:
				break;
		}
	}
}

/**
 * Deletes the previously created command and,
 * returns the memory to the system
 */
void Manager::removePreviousCommand() {
	if(this->_cmd != NULL && this->_cmd->getCommandType() != FIND) {
		delete this->_cmd;
		this->_cmd = NULL;
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
	_logger->log("Manager","handling normal scenario command");
	if(isIndexGiven(newCommand)) {
		_logger->log("Manager","index given by user",NOTICELOG);
		this->handleIndexCommand();
	}
	else if (isCommandWithIndexGiven(newCommand)) {
		_logger->log("Manager","command with index given by user",NOTICELOG);
		this->storeIndexFromCommandToClassAttribute();
		this->handleCommandWithIndex();
	}
	else  {
	// a generic command and has already been interpreted by isCommandWithIndexGiven() above
		_logger->log("Manager","generic command given by user",NOTICELOG);
		this->handleGenericCommand();
	}
	return;
}

/**
 * This handles the scenario where an ambiguous mod/del was done and 
 * the user was given a list of similar tasks to choose the actual one he was talking about
 * Accepts:
 * - Only index
 */
void Manager::handleIntermediateScenarioCommands(string newCommand) {
	if(isIndexGiven(newCommand)) {
		if(isIndexWithinRange()) {
			this->insertCreatedTimeIntoCommand();
			this->_executor->executeCommand(this->_cmd,this->_response);
		}
		else {
			this->_response.setErrorMsg(MESSAGE_INDEX_OUT_OF_RANGE);
			this->_response.setStatus(ERROR_INTERMEDIATE);
		}
	}
	else {
		this->_response.setStatus(ERROR_INTERMEDIATE);
	}
	return;
}

void Manager::handleIndexCommand() {
	if(isIndexWithinRange()) {
		this->_response.setInt(this->_index);
		this->_response.setStatus(DISPLAY);
	}
	else {
		this->_response.setErrorMsg(MESSAGE_INDEX_OUT_OF_RANGE);
		this->_response.setStatus(ERROR);
	}
}

void Manager::handleCommandWithIndex()
{
	if(this->isIndexWithinRange()) {
		this->insertCreatedTimeIntoCommand();
		this->_executor->executeCommand(this->_cmd,this->_response);
		this->editTaskListInResponse();
		this->_response.setStatus(SUCCESS_INDEXED_COMMAND);
	}
	else {
		this->_response.setErrorMsg(MESSAGE_INDEX_OUT_OF_RANGE);
		this->_response.setStatus(ERROR);
	}
}

void Manager::handleGenericCommand()
{
	if(hasNoInterpretationError()) {
		this->_executor->executeCommand(this->_cmd,this->_response);
	}
}

void Manager::editTaskListInResponse()
{
	assert(isDeleteCommand() || isModifyCommand());

	list<Task> tempList = this->_response.getList();
	list<Task>::iterator lit = tempList.begin();
	advance (lit,(_index - 1));

	if(isDeleteCommand())
	{
		tempList.erase(lit);
	}
	else if(isModifyCommand())
	{
		(*lit) = this->_response.getTask();
	}
	this->_response.setList(tempList);
	return;
}

bool Manager::isDeleteCommand()
{
	return (this->_cmd->getCommandType() == DEL);
}

bool Manager::isModifyCommand()
{
	return (this->_cmd->getCommandType() == MOD);
}

bool Manager::isIndexGiven(string newCommand) {
	this->_index = this->_interpreter->interpretIndex(newCommand,this->_response);

	if(this->hasNoInterpretationError()) {
		return true;
	}
	return false;
}

bool Manager::isCommandWithIndexGiven(string newCommand) {
	//tempStorage is used to ensure that the list is not lost in the interpreter by mistake
	list<Task> tempStorage = this->_response.getList();
	this->_cmd = this->_interpreter->interpretCommand(newCommand,this->_response);
	this->_response.setList(tempStorage);

	if (hasNoInterpretationError()) {
		bool isModifyCommandWithIndex = false, isDeleteCommandWithIndex = false;

		switch (this->_cmd->getCommandType()) {
			case MOD:
				isModifyCommandWithIndex = this->isIndexedModifyCommand();
				break;
			case DEL:
				isDeleteCommandWithIndex = this->isIndexedDeleteCommand();
				break;
			default:
				break;
		}

		if(isModifyCommandWithIndex || isDeleteCommandWithIndex) {
			return true;
		}
	}
	//else
	return false;
}

bool Manager::isIndexedModifyCommand() {
	Command_Mod tempCommand = *((Command_Mod *) this->_cmd);
	return tempCommand.getFlagIndex();
}

bool Manager::isIndexedDeleteCommand() {
	Command_Del tempCommand = *((Command_Del *) this->_cmd);
	return tempCommand.getFlagIndex();
}

bool Manager::isIndexWithinRange() {
	int sizeOfCurrentList = this->_response.getList().size();
	return (sizeOfCurrentList >= this->_index && this->_index > 0);
}

void Manager::insertCreatedTimeIntoCommand() {
	assert(_cmd != NULL);
	assert(isModifyCommand() || isDeleteCommand());

	switch(this->_cmd->getCommandType()) {
		case MOD:
			this->insertCreatedTimeIntoModifyCommand();
			break;
		case DEL:
			this->insertCreatedTimeIntoDeleteCommand();
			break;
	}
}

void Manager::insertCreatedTimeIntoDeleteCommand() {
	Task chosenTask = getPointerToChosenTask();
	unsigned long long createdTime = this->getCreatedTimeOfTask(chosenTask);

	Command_Del* tempCommand = (Command_Del *) this->_cmd;
	tempCommand->setCreatedTime(createdTime);
	return;
}

void Manager::insertCreatedTimeIntoModifyCommand() {
	Task chosenTask = this->getPointerToChosenTask();
	unsigned long long createdTime = this->getCreatedTimeOfTask(chosenTask);

	Command_Mod* tempCommand = (Command_Mod *) this->_cmd;
	tempCommand->setCreatedTime(createdTime);
	return;
}

Task Manager::getPointerToChosenTask() const {
	list<Task> tempList = this->_response.getList();
	list<Task>::iterator it = tempList.begin();
	advance(it,(this->_index-1));

	return (*it);
}

unsigned long long Manager::getCreatedTimeOfTask(Task task) const {
	unsigned long long createdTime = task.getIndex();
	return createdTime;
}

void Manager::storeIndexFromCommandToClassAttribute() {
	assert(isModifyCommand() || isDeleteCommand());

	switch (_cmd->getCommandType())
	{
		case MOD:
			{
				Command_Mod* cmdTemp = (Command_Mod*) _cmd;
				this->_index = cmdTemp->getIndex();
				break;
			}
		case DEL:
			{
				Command_Del* cmdTemp = (Command_Del*) _cmd;
				this->_index = cmdTemp->getIndex();
				break;
			}
	}
	return;
}

std::string Manager::createFindCommand(std::tm startTm, std::tm endTm)
{
	std::string startTmStr = getStrFromTm(startTm);
	std::string endTmStr = getStrFromTm(endTm);

	return "find from " + startTmStr + " to " + endTmStr + " undone";
}

std::string Manager::getStrFromTm(std::tm timeInfo)
{
	char todayCharArray [80];

	strftime (todayCharArray,80,"`%d/%m/%y`",&timeInfo);

	//convert the char array to a string
	string today(todayCharArray);

	return today;
}

std::tm Manager::getTodayTm()
{
	time_t rawtime;
	struct tm * timeInfo;

	//get the time right now, convert it to a string in the format DD/MM/YYYY
	time (&rawtime);
	timeInfo = localtime (&rawtime);

	return *timeInfo;
}

Messenger Manager::getTodayTasks() {
	std::tm todayTm = getTodayTm();

	std::tm end_of_todayTm = todayTm;
	end_of_todayTm.tm_hour = 23;
	end_of_todayTm.tm_min = 59;
	char endOfTodayCharArray [80];
	strftime (endOfTodayCharArray, 80, "`%d/%m/%y %H:%M`",&end_of_todayTm);

	std::string today = this->getStrFromTm(todayTm);
	std::string end_of_today = endOfTodayCharArray;

	this->setCurrPeriod(todayTm,todayTm);

	return this->processCommand("find from "+ today + " to "+ end_of_today + " undone");
	//return this->processCommand("find undone");
}

bool Manager::hasInterpretationError() {
	return !this->hasNoInterpretationError();
}

bool Manager::hasNoInterpretationError() {
	if(this->hasNoError())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Manager::hasNoError()
{
	if(this->_response.getStatus() == ERROR || this->_response.getStatus() == ERROR_INTERMEDIATE) {
		return false;
	}
	else
	{
		return true;
	}
}

bool Manager::isSuccessfulCommand()
{
	if(this->_response.getStatus() == SUCCESS || this->_response.getStatus() == SUCCESS_INDEXED_COMMAND)
	{
		return true;
	}
	return false;
}

/*********************************************************/
/************* NAVIGATION RELATED METHODS ****************/
/*********************************************************/

Messenger Manager::getNextPeriodTasks(PERIOD_TYPE pType)
{
	std::tm& currTm = _currentPeriod.second;
	std::tm nextTm;

	switch(pType)
	{
		case DAY:
			nextTm = this->getNextDayTm(currTm);
			break;
		case WEEK:
			nextTm = this->getNextWeekTm(currTm);
			break;
		case MONTH:
			nextTm = this->getNextMonthTm(currTm);
			break;
	}

	this->setCurrPeriod(currTm,nextTm);
	std::string command = this->createFindCommand(currTm,nextTm);

	return this->processCommand(command);
}
Messenger Manager::getPrevPeriodTasks(PERIOD_TYPE pType)
{
	std::tm& currTm = this->_currentPeriod.first;
	std::tm prevTm;

	switch(pType)
	{
		case DAY:
			prevTm = this->getPrevDayTm(currTm);
			break;
		case WEEK:
			prevTm = this->getPrevWeekTm(currTm);
			break;
		case MONTH:
			prevTm = this->getPrevMonthTm(currTm);
			break;
	}

	this->setCurrPeriod(prevTm, currTm);

	std::string command = createFindCommand(prevTm, currTm);

	return this->processCommand(command);
}

pair<tm,tm> Manager::getCurrentPeriod(){
	return _currentPeriod;
}

std::tm Manager::getNextDayTm(std::tm currTm)
{
	std::time_t intermediateResult;
	currTm.tm_mday += 1;
	intermediateResult = mktime(&currTm);
	
	return *localtime(&intermediateResult);
}

std::tm Manager::getNextWeekTm(std::tm currTm)
{
	std::time_t intermediateResult;
	currTm.tm_mday += 7;
	intermediateResult = mktime(&currTm);
	
	return *localtime(&intermediateResult);
}

std::tm Manager::getNextMonthTm(std::tm currTm)
{
	std::time_t intermediateResult;
	currTm.tm_mon += 1;
	intermediateResult = mktime(&currTm);
	
	return *localtime(&intermediateResult);
}

std::tm Manager::getPrevDayTm(std::tm currTm)
{
	std::time_t intermediateResult;
	currTm.tm_mday -= 1;
	intermediateResult = mktime(&currTm);
	
	return *localtime(&intermediateResult);
}

std::tm Manager::getPrevWeekTm(std::tm currTm)
{
	std::time_t intermediateResult;
	currTm.tm_mday -= 7;
	intermediateResult = mktime(&currTm);
	
	return *localtime(&intermediateResult);
}

std::tm Manager::getPrevMonthTm(std::tm currTm)
{
	std::time_t intermediateResult;
	currTm.tm_mon -= 1;
	intermediateResult = mktime(&currTm);
	
	return *localtime(&intermediateResult);
}

void Manager::setCurrPeriod(std::tm startTm, std::tm endTm)
{
	this->_currentPeriod = pair<tm,tm>(startTm,endTm);
}