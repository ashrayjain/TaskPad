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

using namespace TP;

const string Manager::MESSAGE_INDEX_OUT_OF_RANGE = "Given index is out of range!";
const string Manager::MESSAGE_ERROR_UNEXPECTED_COMMAND_TYPE_WITH_INDEX = "Unexpected Command with index!!";

Manager::Manager() {
	this->_storage = new Storage(_tasks);
	this->_executor = new Executor(&_tasks);
	this->_interpreter = new Interpreter;
	this->_response = Messenger();
	this->_cmd = NULL;
}

Messenger Manager::processCommand(const string& newCommand) {
	switch(this->_response.getStatus()) {
		case INTERMEDIATE:
		case ERROR_INTERMEDIATE:
			handleIntermediateScenarioCommands(newCommand);
			break;
		default:
			removePreviousCommand();
			handleNormalScenarioCommands(newCommand);
			break;
	}
	this->saveChanges();
	return this->_response;
}

void Manager::saveChanges()
{
	if(this->isSuccessfulCommand()){
		switch(this->_cmd->getCommandType())
		{
			case MOD:
			case DEL:
				this->_storage->save(this->_tasks);
				break;
			case ADD:
				this->_storage->save(this->_response.getTask());
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
	if(this->_cmd != NULL) {
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
	if(isIndexGiven(newCommand)) {
		this->handleIndexCommand();
	}
	else if (isCommandWithIndexGiven(newCommand)) {
		this->storeIndexFromCommandToClassAttribute();
		this->handleCommandWithIndex();
	}
	else  {
	// a generic command and has already been interpreted by isCommandWithIndexGiven() above
		this->handleGenericCommand();
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
	return (sizeOfCurrentList >= this->_index);
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
	if(this->_response.getStatus() == SUCCESS)
	{
		return true;
	}
	return false;
}

std::string Manager::createFindCommand(std::tm startTm, std::tm endTm)
{
	std::string startTmStr = getStrFromTm(startTm);
	std::string endTmStr = getStrFromTm(endTm);

	return "find from `" + startTmStr + "` to `" + endTmStr + "`";
}

std::string Manager::getStrFromTm(std::tm timeInfo)
{
	char todayCharArray [80];

	strftime (todayCharArray,80,"`%d/%m/%Y`",&timeInfo);

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
	std::string today = this->getStrFromTm(todayTm);

	this->setCurrTm(todayTm);

	return this->processCommand("find from "+today + " to "+today);
	//return this->processCommand("find undone");
}

Messenger Manager::getNextPeriodTasks(PERIOD_TYPE pType)
{
	std::tm nextTm;

	switch(pType)
	{
		case DAY:
			nextTm = this->getNextDayTm(_currTm);
			break;
		case WEEK:
			nextTm = this->getNextWeekTm(_currTm);
			break;
		case MONTH:
			nextTm = this->getNextMonthTm(_currTm);
			break;
	}

	this->setCurrTm(nextTm);

	std::string command = this->createFindCommand(_currTm,nextTm);
	
	return this->processCommand(command);
}
Messenger Manager::getPrevPeriodTasks(PERIOD_TYPE pType)
{
	std::tm currTm = this->_currTm;
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

	this->setCurrTm(prevTm);
	std::string command = createFindCommand(prevTm, currTm);

	return this->processCommand(command);
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

void Manager::setCurrTm(std::tm newTm)
{
	this->_currTm = newTm;
}

void Manager::resetStatus() {
	delete this->_cmd;
	this->_cmd = NULL;
	this->_response.resetMessenger();
	this->setCurrTm(getTodayTm());
}

Manager::~Manager() {
	//this->_storage->save(this->_tasks);
	delete this->_interpreter;
	delete this->_executor;
	delete this->_storage;
	this->removePreviousCommand();
	this->_response.resetMessenger();
}