/*
 * =====================================================================================
 *
 *       Filename:  Executor.cpp
 *
 *        Version:  1.0
 *        Created:  09/25/13 00:22:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Executor.h"
/*
using namespace TP;

const unsigned			Executor::EMPTY_LIST_SIZE				= 0;
const unsigned			Executor::SINGLE_RESULT_LIST_SIZE		= 1;
const std::string		Executor::NAME_NOT_FOUND_ERROR			= "No results for name: ";
const std::string		Executor::INVALID_INDEX_ERROR			= " is not a valid index!";
const std::string		Executor::UNDOSTACK_EMPTY_MSG			= "Nothing to Undo!";
const std::string		Executor::REDOSTACK_EMPTY_MSG			= "Nothing to Redo!";
const std::string		Executor::MODIFY_SAME_NAME_ERROR		= "New name is the same as the Existing name!";
const std::string		Executor::INVALID_FROMDATE_ERROR		= "Invalid 'From' Attribute!";
const std::string		Executor::INVALID_TODATE_ERROR			= "Invalid 'To' Attribute!";
const std::string		Executor::INVALID_FROMDATE_TODATE_ERROR	= "Invalid 'From' and 'To' Attributes!";
const std::string		Executor::NAME_NOT_SPECIFIED_ERROR		= "No Name specified!";
const unsigned			Executor::RT_MIN_H_ARR[]				= {5, 15, 30, 60};
const unsigned			Executor::RT_MIN_M_ARR[]				= {15, 30};
const unsigned			Executor::RT_MIN_L_ARR[]				= {60};

void Executor::rebuildHashes() {
	rebuildIndexHash();
	rebuildHashTagsHash();
	rebuildRemindTimesHash();
}

void Executor::rebuildIndexHash() {
	for (std::list<Task>::iterator i = _data->begin(); i != _data->end(); i++)
		_indexHash[i->getIndex()] = &(*i);
}

void Executor::rebuildHashTagsHash() {
	for (std::list<Task>::iterator i = _data->begin(); i != _data->end(); i++)
		if(i->getFlagTags())
			handleHashTagPtrs(*i, i->getTags());
}

void Executor::rebuildRemindTimesHash() {
	for (std::list<Task>::iterator i = _data->begin(); i != _data->end(); i++)
		if(i->getFlagRemindTimes())
			handleRemindTimesPtrs(*i, i->getRemindTimes());
}
*/
list<Task> Executor::getCurrentReminders() {
	time_t now = time(NULL);
	struct tm* timeNow = localtime(&now);
	timeNow->tm_sec = 0;
	now = mktime(timeNow);
	/*
	list<Task> taskResults;
	unordered_map<time_t, list<Task*>>::iterator result = _remindTimesHash.find(now);
	if (result != _remindTimesHash.end())
		getTasksFromTaskPtrList(taskResults, result->second);
	return taskResults;
	*/
	return _ds.getTasksWithRemindTimes(now);
}
/*
void Executor::getTasksFromTaskPtrList(list<Task> &taskResults, list<Task*> &results) {
	for(list<Task*>::iterator i = results.begin(); i != results.end(); i++)
		taskResults.push_back(Task(**i));
}
*/
void Executor::executeCommand(Command* cmd, Messenger &response) {
	Executor_Base* executor;
	switch (cmd->getCommandType()) {
	case COMMAND_TYPE::ADD:	executor = new Executor_Add();
							executor->executeCommand(cmd, response, _ds);
							if (isCmdSuccessful(response))
								_ds.stackCmdForUndo(cmd, response);
							break;
	case COMMAND_TYPE::DEL:	executor = new Executor_Del();
							executor->executeCommand(cmd, response, _ds);
							if (isCmdSuccessful(response))
								_ds.stackCmdForUndo(cmd, response);
							break;
	case COMMAND_TYPE::MOD:	executor = new Executor_Mod();
							executor->executeCommand(cmd, response, _ds);
							if (isCmdSuccessful(response))
								_ds.stackCmdForUndo(cmd, response);
							break;
	case COMMAND_TYPE::FIND:executor = new Executor_Find();
							executor->executeCommand(cmd, response, _ds);
							break;
	case COMMAND_TYPE::UNDO:executor = new Executor_Undo();
							executor->executeCommand(cmd, response, _ds);
							executeCommandWithoutUndoRedo(
								dynamic_cast<Executor_Undo*>(executor)->getUndoCommandToExecute(), 
								response);
							break;
	case COMMAND_TYPE::REDO:executor = new Executor_Redo();
							executor->executeCommand(cmd, response, _ds);
							executeCommandWithoutUndoRedo(
								dynamic_cast<Executor_Redo*>(executor)->getRedoCommandToExecute(), 
								response);
							break;
	default:
		break;
	}
	delete executor;
}

void Executor::executeCommandWithoutUndoRedo(Command* cmd, Messenger &response) {
	Executor_Base* executor;
	switch (cmd->getCommandType()) {
	case COMMAND_TYPE::ADD:	executor = new Executor_Add();
							break;
	case COMMAND_TYPE::DEL:	executor = new Executor_Del();
							break;
	case COMMAND_TYPE::MOD:	executor = new Executor_Mod();
							break;
	}
	executor->executeCommand(cmd, response, _ds);
	delete executor;
}

bool Executor::isCmdSuccessful(const Messenger &response) const {
	return response.getStatus() == TP::STATUS::SUCCESS;
}

/*
// Add functions

void Executor::executeAdd (Command_Add* cmd, Messenger &response) {
	if(validAddCmd(cmd, response)) {
		Task newTask = formTaskFromAddCmd(cmd);
		//
		_data->push_back(newTask);
		_indexHash[newTask.getIndex()] = &(_data->back());
		handleHashTagPtrs(_data->back(), _data->back().getTags());
		handleRemindTimesPtrs(_data->back(), _data->back().getRemindTimes());
		//
		setOpSuccessTask(newTask, response);
	}
}

bool Executor::validAddCmd(Command_Add* cmd, Messenger &response) {
	if (cmd->getFlagFrom() && cmd->getFlagTo() && cmd->getFromDate() > cmd->getToDate()) {
		setErrorWithErrMsg(response, INVALID_FROMDATE_TODATE_ERROR);
		return false;
	}
	if (!cmd->getFlagName()) {
		setErrorWithErrMsg(response, NAME_NOT_SPECIFIED_ERROR);
		return false;
	}
	return true;
}

Task Executor::formTaskFromAddCmd(Command_Add* cmd) {
	Task newTask;
	if(cmd->getFlagCreatedTime()) {
		newTask = Task(cmd->getCreatedTime());
		newTask.setName(cmd->getName());
	}
	else
		newTask = Task(cmd->getName());

	if(cmd->getFlagLocation())
		newTask.setLocation(cmd->getLocation());
	if(cmd->getFlagNote())
		newTask.setNote(cmd->getNote());
	if(cmd->getFlagParticipants())
		newTask.setParticipants(cmd->getParticipants());
	if(cmd->getFlagPriority())
		newTask.setPriority(cmd->getPriority());
	if(cmd->getFlagFrom())
		newTask.setFromDate(cmd->getFromDate());
	if(cmd->getFlagTo())
		newTask.setToDate(cmd->getToDate());
	if(cmd->getFlagDue())
		newTask.setDueDate(cmd->getDueDate());
	if(cmd->getFlagTags())
		newTask.setTags(cmd->getTags());

	if(cmd->getFlagRemindTimes())
		newTask.setRemindTimes(cmd->getRemindTimes());
	else if (newTask.getFlagFromDate())
		setDefaultRemindTimes(newTask);

	return newTask;
}

void Executor::setDefaultRemindTimes(Task &task) {
	switch(task.getPriority()) {
	case TP::PRIORITY::HIGH:	setDefaultRemindTimesPriorityH(task); break;
	case TP::PRIORITY::MEDIUM:	setDefaultRemindTimesPriorityM(task); break;
	case TP::PRIORITY::LOW:		setDefaultRemindTimesPriorityL(task); break;
	}
}

void Executor::setDefaultRemindTimesPriorityH(Task &task) {
	int n = sizeof(RT_MIN_H_ARR) / sizeof(RT_MIN_H_ARR[0]);
	task.setRemindTimes(getRemindTimesFromMinutesBefore(RT_MIN_H_ARR, n, task.getFromDate()));
}

void Executor::setDefaultRemindTimesPriorityM(Task &task) {
	int n = sizeof(RT_MIN_M_ARR) / sizeof(RT_MIN_M_ARR[0]);
	task.setRemindTimes(getRemindTimesFromMinutesBefore(RT_MIN_M_ARR, n, task.getFromDate()));
}

void Executor::setDefaultRemindTimesPriorityL(Task &task) {
	int n = sizeof(RT_MIN_L_ARR) / sizeof(RT_MIN_L_ARR[0]);
	task.setRemindTimes(getRemindTimesFromMinutesBefore(RT_MIN_L_ARR, n, task.getFromDate()));
}
list<time_t> Executor::getRemindTimesFromMinutesBefore(const unsigned minutesBeforeList[], const int listSize, const time_t &deadline) const{
	list<time_t> remindTimesList;
	struct tm * deadlineTime = localtime(&deadline);
	for(int i = 0; i < listSize; i++) {
		deadlineTime->tm_min -= minutesBeforeList[i];
		remindTimesList.push_back(mktime(deadlineTime));
		deadlineTime->tm_min += minutesBeforeList[i];
		mktime(deadlineTime);
	}
	return remindTimesList;
}

void Executor::handleHashTagPtrs(Task &newTask, const list<string> &hashTagsList) {
	list<list<Task*>::iterator> newHashTagPtrs;
	for (list<string>::const_iterator i = hashTagsList.begin(); i != hashTagsList.end(); i++) {
		std::unordered_map<std::string, list<Task*>>::iterator foundHashTag = _hashTagsHash.find(*i);
		if (foundHashTag != _hashTagsHash.end())
			handleExistingHashTag(newHashTagPtrs, newTask, foundHashTag->second);
		else
			handleNewHashTag(newHashTagPtrs, newTask, i);
	}
	newTask.setHashTagPtrs(newHashTagPtrs);
}

void Executor::handleExistingHashTag(list<list<Task*>::iterator> &newHashTagPtrs, Task &newTask, list<Task*> &hashTag) {
	hashTag.push_back(&newTask);
	newHashTagPtrs.push_back(--hashTag.end());
}

void Executor::handleNewHashTag(list<list<Task*>::iterator> &newHashTagPtrs, Task &newTask, list<string>::const_iterator &hashTag) {
	_hashTagsHash[*hashTag] = list<Task*>(1, &newTask);
	newHashTagPtrs.push_back(--(_hashTagsHash[*hashTag].end()));
}

void Executor::handleRemindTimesPtrs(Task &newTask, const list<time_t> &remindTimesList) {
	list<list<Task*>::iterator> newRemindTimesPtrs;
	for (list<time_t>::const_iterator i = remindTimesList.begin(); i != remindTimesList.end(); i++) {
		std::unordered_map<std::time_t, list<Task*>>::iterator foundRemindTime = _remindTimesHash.find(*i);
		if (foundRemindTime != _remindTimesHash.end())
			handleExistingRemindTime(newRemindTimesPtrs, newTask, foundRemindTime->second);
		else
			handleNewRemindTime(newRemindTimesPtrs, newTask, i);
	}
	newTask.setRemindTimesPtrs(newRemindTimesPtrs);
}

void Executor::handleExistingRemindTime(list<list<Task*>::iterator> &newRemindTimesPtrs, Task &newTask, list<Task*> &remindTime) {
	remindTime.push_back(&newTask);
	newRemindTimesPtrs.push_back(--remindTime.end());
}

void Executor::handleNewRemindTime(list<list<Task*>::iterator> &newRemindTimesPtrs, Task &newTask, list<time_t>::const_iterator &remindTime) {
	_remindTimesHash[*remindTime] = list<Task*>(1, &newTask);
	newRemindTimesPtrs.push_back(--(_remindTimesHash[*remindTime].end()));
}

// Delete Functions

void Executor::executeDel (Command_Del* cmd, Messenger &response) {
	if (cmd->getFlagCreatedTime())
		deleteTaskByIndex(cmd->getCreatedTime(), response);
	else
		deleteTaskByName(cmd->getName(), response, cmd->getFlagExact());
}

void Executor::deleteTaskByIndex(const unsigned long long &index, Messenger &response) {
	bool indexFound = false;
	for(list<Task>::iterator i = _data->begin(); i != _data->end() && !indexFound; ++i)
		if (i->getIndex() == index) {
			setOpSuccessTask(*i, response);
			deleteTask(i);
			indexFound = true;
			break;
		}

	if (!indexFound)
		setIndexNotFound(index, response);
}

void Executor::deleteTaskByName(const string &name, Messenger &response, const bool &exactFlag) {
	if (exactFlag)
		deleteByExactName(name, response);
	else
		deleteByApproxName(name, response);
}

void Executor::deleteByExactName(const string &name, Messenger &response) {
	bool nameFound = false;
	for(list<Task>::iterator i = _data->begin(); i != _data->end() && !nameFound; ++i)
		if (i->getName() == name) {
			setOpSuccessTask(*i, response);
			deleteTask(i);			
			nameFound = true;
			break;
		}

	if (!nameFound)
		setNameNotFound(name, response);
}

void Executor::deleteByApproxName(const string &name, Messenger &response) {
	list<Task> matchingResults;
	list<Task> caseInsensitiveResults;
	string lowerName = getLowerStr(name);
	for(list<Task>::iterator i = _data->begin(); i != _data->end(); ++i) {
		string currName = getLowerStr(i->getName());
		if (currName.find(name) != string::npos)
			matchingResults.push_back(Task(*i));
		else if (currName.find(lowerName) != string::npos)
			caseInsensitiveResults.push_back(Task(*i));
	}
	selectAppropriateDeleteResponse(matchingResults, caseInsensitiveResults, name, response);
}

void Executor::selectAppropriateDeleteResponse(const list<Task> &matchingResults, const list<Task> &caseInsensitiveResults, const string &name, Messenger &response) {
	if (matchingResults.size() == EMPTY_LIST_SIZE) {
		if (caseInsensitiveResults.size() == EMPTY_LIST_SIZE)
			setNameNotFound(name, response);
		else if (caseInsensitiveResults.size() == SINGLE_RESULT_LIST_SIZE)
			deleteTaskByIndex(caseInsensitiveResults.front().getIndex(), response);
		else
			setOpIntermediateTaskList(caseInsensitiveResults, response);
	}
	else if (matchingResults.size() == SINGLE_RESULT_LIST_SIZE)
		deleteTaskByIndex(matchingResults.front().getIndex(), response);
	else
		setOpIntermediateTaskList(matchingResults, response);
}

void Executor::deleteTask(list<Task>::iterator &i) {
	_indexHash.erase(i->getIndex());
	deleteHashTags(*i);
	deleteRemindTimes(*i);
	_data->erase(i);
}

void Executor::deleteHashTags(Task &task) {
	list<string> tags = task.getTags();
	list<list<Task*>::iterator> tagPtrs = task.getHashTagPtrs();
	list<string>::iterator k = tags.begin();
	for (list<list<Task*>::iterator>::iterator j = tagPtrs.begin(); j != tagPtrs.end(); j++, k++)
		_hashTagsHash[*k].erase(*j);
}

void Executor::deleteRemindTimes(Task &task) {
	list<time_t> remindTimes = task.getRemindTimes();
	list<list<Task*>::iterator> remindTimePtrs = task.getRemindTimesPtrs();
	list<time_t>::iterator k = remindTimes.begin();
	for (list<list<Task*>::iterator>::iterator j = remindTimePtrs.begin(); j != remindTimePtrs.end(); j++, k++)
		_remindTimesHash[*k].erase(*j);
}

// Modify Functions

void Executor::executeMod (Command_Mod* cmd, Messenger &response) {
	if(cmd->getFlagCreatedTime())
		modifyByIndex(cmd, response);
	else
		modifyByName(cmd, response);
}

void Executor::modifyByIndex(Command_Mod* cmd, Messenger &response) {
	unordered_map<unsigned long long, Task*>::iterator result = _indexHash.find(cmd->getCreatedTime());
	if (result != _indexHash.end()) {
		if (isModCmdValid(cmd, *(result->second), response)) {
			_interimTask = *(result->second);
			modifyTaskTo(*(result->second), cmd);
			setOpSuccessTask(*(result->second), response);
		}
	}
	else
		setIndexNotFound(cmd->getCreatedTime(), response);
}

void Executor::modifyByName(Command_Mod* cmd, Messenger &response) {
	if (cmd->getFlagExact())
		modifyByExactName(cmd, response);
	else
		modifyByApproxName(cmd, response);
}

void Executor::modifyByExactName(Command_Mod* cmd, Messenger &response) {
	bool nameFound = false;
	for(list<Task>::iterator i = _data->begin(); i != _data->end() && !nameFound; ++i)
		if (i->getName() == cmd->getName()) {
			if (isModCmdValid(cmd, *i, response)) {
				_interimTask = *i;
				modifyTaskTo(*i, cmd);
				setOpSuccessTask(Task(*i), response);
			}
			nameFound = true;
		}

	if (!nameFound)
		setNameNotFound(cmd->getName(), response);
}

void Executor::modifyByApproxName(Command_Mod* cmd, Messenger &response) {
	list<Task> matchingResults;
	list<Task> caseInsensitiveResults;
	string lowerName = getLowerStr(cmd->getName());
	for(list<Task>::iterator i = _data->begin(); i != _data->end(); ++i) {
		string currName = getLowerStr(i->getName());
		if (currName.find(cmd->getName()) != string::npos)
			matchingResults.push_back(Task(*i));
		else if (currName.find(lowerName) != string::npos)
			caseInsensitiveResults.push_back(Task(*i));
	}
	selectAppropriateModifyResponse(matchingResults, caseInsensitiveResults, cmd, response);
}

void Executor::selectAppropriateModifyResponse(const list<Task> &matchingResults, const list<Task> &caseInsensitiveResults, Command_Mod* cmd, Messenger &response) {
	if (matchingResults.size() == EMPTY_LIST_SIZE) {
		if (caseInsensitiveResults.size() == EMPTY_LIST_SIZE)
			setNameNotFound(cmd->getName(), response);
		else if (caseInsensitiveResults.size() == SINGLE_RESULT_LIST_SIZE) {
			cmd->setCreatedTime(caseInsensitiveResults.front().getIndex());
			modifyByIndex(cmd, response);
		}
		else
			setOpIntermediateTaskList(caseInsensitiveResults, response);
	}
	else if(matchingResults.size() == SINGLE_RESULT_LIST_SIZE){
		cmd->setCreatedTime(matchingResults.front().getIndex());
		modifyByIndex(cmd, response);
	}
	else
		setOpIntermediateTaskList(matchingResults, response);
}

void Executor::modifyTaskTo(Task &oldTask, Command_Mod* cmd) {
	if(cmd->getFlagOptName()) 
		oldTask.setName(cmd->getOptName());//need to set exception handler in case the new name is the name of an existing task.
	if(cmd->getFlagLocation())
		oldTask.setLocation(cmd->getLocation());
	if(cmd->getFlagNote())
		oldTask.setNote(cmd->getNote());
	if(cmd->getFlagParticipants())
		oldTask.setParticipants(cmd->getParticipants());
	if(cmd->getFlagAddParticipants())
		handleAddRemoveParticipants(oldTask, cmd->getAddParticipants(), TP::LIST_OP::ADD_ELEMENT);
	if(cmd->getFlagRemoveParticipants())
		handleAddRemoveParticipants(oldTask, cmd->getRemoveParticipants(), TP::LIST_OP::REMOVE_ELEMENT);
	if(cmd->getFlagRemoveAllParticipants())
		oldTask.setParticipants(Task::DEFAULT_PARTICIPANTS);
	if(cmd->getFlagTags())
		handleHashTagsModify(oldTask, cmd->getTags());
	if(cmd->getFlagAddTags()) {
		list<string> tags = oldTask.getTags();
		tags.splice(tags.end(), cmd->getAddTags());
		handleHashTagsModify(oldTask, tags);
	}
	if(cmd->getFlagRemoveTags())
		handleHashTagsModify(oldTask, getTagsListDifference(oldTask.getTags(), cmd->getRemoveTags()));
	if(cmd->getFlagRemoveAllTags())
		handleHashTagsModify(oldTask, Task::DEFAULT_TAGS);
	if(cmd->getFlagRemindTimes())
		handleRemindTimesModify(oldTask, cmd->getRemindTimes());
	if(cmd->getFlagAddRemindTimes())
		handleAddRemoveRemindTimes(oldTask, cmd->getAddRemindTimes(), TP::LIST_OP::ADD_ELEMENT);
	if(cmd->getFlagRemoveRemindTimes())
		handleAddRemoveRemindTimes(oldTask, cmd->getRemoveRemindTimes(), TP::LIST_OP::REMOVE_ELEMENT);
	if(cmd->getFlagRemoveAllRemindTimes())
		handleRemindTimesModify(oldTask, Task::DEFAULT_REMINDTIMES);
	if(cmd->getFlagPriority())
		oldTask.setPriority(cmd->getPriority());
	if(cmd->getFlagFrom())
		oldTask.setFromDate(cmd->getFromDate());
	if(cmd->getFlagTo())
		oldTask.setToDate(cmd->getToDate());
	if(cmd->getFlagDue())
		oldTask.setDueDate(cmd->getDueDate());
	if(cmd->getFlagTaskState())
		oldTask.setState(cmd->getTaskState());
	if(cmd->getFlagRemoveDue())
		oldTask.setDueDate(Task::DEFAULT_FROMDATE);
	if(cmd->getFlagRemoveFrom())
		oldTask.setFromDate(Task::DEFAULT_FROMDATE);
	if(cmd->getFlagRemoveTo())
		oldTask.setToDate(Task::DEFAULT_TODATE);
}

bool Executor::isModCmdValid(Command_Mod* cmd, const Task& task, Messenger &response) {
	if (cmd->getFlagOptName() && cmd->getOptName() == task.getName()) {
		setErrorWithErrMsg(response, MODIFY_SAME_NAME_ERROR);
		return false;
	}
	if (!(cmd->getFlagFrom() && cmd->getFlagTo())) {
		if (cmd->getFlagFrom() && task.getFlagToDate() && cmd->getFromDate() > task.getToDate()) {
			setErrorWithErrMsg(response, INVALID_FROMDATE_ERROR);
			return false;
		}
		if (cmd->getFlagTo() && task.getFlagFromDate() && cmd->getToDate() < task.getFromDate()) {
			setErrorWithErrMsg(response, INVALID_TODATE_ERROR);
			return false;
		}
	}
	else if(cmd->getFromDate() > cmd->getToDate()) {
		setErrorWithErrMsg(response, INVALID_FROMDATE_TODATE_ERROR);
		return false;
	}
	return true;
}

void Executor::handleHashTagsModify(Task &oldTask, const list<string> &newTags) {
	deleteHashTags(oldTask);
	oldTask.setTags(newTags);
	handleHashTagPtrs(oldTask, newTags);
}

void Executor::handleRemindTimesModify(Task &oldTask, const list<time_t> &newRemindTimes) {
	deleteRemindTimes(oldTask);
	oldTask.setRemindTimes(newRemindTimes);
	handleRemindTimesPtrs(oldTask, newRemindTimes);
}

void Executor::handleAddRemoveParticipants(Task &task, list<string> &participants, TP::LIST_OP op) {
	for(list<string>::iterator i = participants.begin(); i != participants.end(); i++)
		task.setParticipants(*i, op);
}

void Executor::handleAddRemoveRemindTimes(Task &task, list<time_t> &remindTimes, TP::LIST_OP op) {
	for(list<time_t>::iterator i = remindTimes.begin(); i != remindTimes.end(); i++)
		task.setRemindTimes(*i, op);
	list<time_t> newRemindTimes = task.getRemindTimes();
	handleRemindTimesModify(task, newRemindTimes);
}

list<string> Executor::getTagsListDifference(const list<string> &taskTags, const list<string> &tagsToRemove) const {
	list<string> newTagsList;
	for(list<string>::const_iterator i = taskTags.begin(); i != taskTags.end(); i++)
		if(find(tagsToRemove.begin(), tagsToRemove.end(), *i) == tagsToRemove.end())
			newTagsList.push_back(*i);
	return newTagsList;
}

// Find functions

void Executor::executeFind (Command_Find* cmd, Messenger &response) {
	if(cmd->getFlagIndex())
		findByIndex(cmd->getIndex(), response);
	else
		findGeneral(cmd, response);
	if(cmd->getFlagTaskType())
		filterResponseListByType(response, list<TP::TASK_TYPE>(1, cmd->getTaskType()));
	response.setList(getSortListByPriority(response.getList()));
}

void Executor::formTaskFromFindCmd(Command_Find* cmd, Task &newTask) {
	if(cmd->getFlagExact() && cmd->getFlagOptName())
		newTask.setName(cmd->getOptName());
	if(cmd->getFlagLocation())
		newTask.setLocation(cmd->getLocation());
	if(cmd->getFlagNote())
		newTask.setNote(cmd->getNote());
	if(cmd->getFlagRemindTimes())
		newTask.setRemindTimes(cmd->getRemindTimes());
	if(cmd->getFlagParticipants())
		newTask.setParticipants(cmd->getParticipants());
	if(cmd->getFlagPriority())
		newTask.setPriority(cmd->getPriority());
	if(cmd->getFlagFrom())
		newTask.setFromDate(cmd->getFromDate());
	if(cmd->getFlagTo())
		newTask.setToDate(cmd->getToDate());
	if(cmd->getFlagTaskState())
		newTask.setState(cmd->getTaskState());
}

void Executor::findByIndex(const unsigned long long index, Messenger &response) {
	unordered_map<unsigned long long, Task*>::iterator result = _indexHash.find(index);
	if (result != _indexHash.end())
		setOpSuccessTask(*(result->second), response);
	else
		setIndexNotFound(index, response);
}

void Executor::findGeneral(Command_Find* cmd, Messenger &response) {
	Task taskToCompare;
	list<Task> results;
	set<Task*> customDataRange;
	formTaskFromFindCmd(cmd, taskToCompare);
	if (cmd->getFlagTags())
		getCustomDataRangeByTags(customDataRange, cmd->getTags());
	if (cmd->getFlagRemindTimes())
		getCustomDataRangeByRT(customDataRange, cmd->getRemindTimes());

	runSearch(taskToCompare,
		results, 
		getLowerStr(cmd->getOptName()),
		customDataRange, 
		cmd->getFlagTags() || cmd->getFlagRemindTimes());		

	setOpSuccessTaskList(results, response);
}

void Executor::runSearch(const Task &taskToCompare, list<Task> &results, string substringName, set<Task*> &customData, bool customDataSet) {
	if (customDataSet) {
		if (substringName != string())
			runSearchWithTask(taskToCompare, results, substringName, customData);
		else
			runSearchWithTask(taskToCompare, results, customData);
	}
	else {
		if (substringName != string())
			runSearchWithTask(taskToCompare, results, substringName);
		else
			runSearchWithTask(taskToCompare, results);
	}
}

/*
void Executor::findByTags(Command_Find* cmd, Messenger &response) {
	Task taskToCompare;
	list<Task*> customDataRange;
	list<Task> results;
	formTaskFromFindCmd(cmd, taskToCompare);
	getCustomDataRangeByTags(customDataRange, cmd->getTags());
	runSearchWithTaskOnData(taskToCompare, results, customDataRange);
	setOpSuccessTaskList(results, response);
}

void Executor::findByRemindTimes(Command_Find* cmd, Messenger &response) {
	Task taskToCompare;
	list<Task*> customDataRange;
	list<Task> results;
	formTaskFromFindCmd(cmd, taskToCompare);
	getCustomDataRangeByTags(customDataRange, cmd->getTags());
	runSearchWithTaskOnData(taskToCompare, results, customDataRange);
	setOpSuccessTaskList(results, response);
}
*//*
void Executor::getCustomDataRangeByTags(set<Task*> &customDataRange, list<string> &tags) {
	for(list<string>::iterator i = tags.begin(); i != tags.end(); ++i)
		customDataRange.insert(_hashTagsHash[*i].begin(), _hashTagsHash[*i].end());
}

void Executor::getCustomDataRangeByRT(set<Task*> &customDataRange, list<time_t> &remindTimes) {
	for(list<time_t>::iterator i = remindTimes.begin(); i != remindTimes.end(); ++i)
		customDataRange.insert(_remindTimesHash[*i].begin(), _remindTimesHash[*i].end());
}

void Executor::filterResponseListByType(Messenger &response, list<TP::TASK_TYPE> &types) {
	list<Task> newResults, oldResults = response.getList();
	for (list<Task>::iterator i = oldResults.begin(); i != oldResults.end(); ++i)
		if (find(types.begin(), types.end(), i->getTaskType()) != types.end())
			newResults.push_back(*i);
	setOpSuccessTaskList(newResults, response);
}

void Executor::runSearchWithTask(const Task &taskToCompare, list<Task> &results) {
	for(list<Task>::iterator i = _data->begin(); i != _data->end(); ++i)
		if (taskMatch(*i, taskToCompare))
			results.push_back(Task(*i));
}

void Executor::runSearchWithTask(const Task &taskToCompare, list<Task> &results, string substringName) {
	for(list<Task>::iterator i = _data->begin(); i != _data->end(); ++i)
		if (getLowerStr(i->getName()).find(substringName) != string::npos && taskMatch(*i, taskToCompare))
			results.push_back(Task(*i));
}

void Executor::runSearchWithTask(const Task &taskToCompare, list<Task> &results, set<Task*> &customData) {
	for(set<Task*>::iterator i = customData.begin(); i != customData.end(); ++i)
		if (taskMatch(**i, taskToCompare))
			results.push_back(Task(**i));
}

void Executor::runSearchWithTask(const Task &taskToCompare, list<Task> &results, string substringName, set<Task*> &customData) {
	for(set<Task*>::iterator i = customData.begin(); i != customData.end(); ++i)
		if (getLowerStr((*i)->getName()).find(substringName) != string::npos && taskMatch(**i, taskToCompare))
			results.push_back(Task(**i));
}

bool Executor::taskMatch(const Task& lhs, const Task& rhs) {
	if (rhs.getFlagName() && strcmpi(rhs.getName().c_str(), lhs.getName().c_str()))
		return false;
	else if (rhs.getFlagLocation() && (!lhs.getFlagLocation() || rhs.getLocation() != lhs.getLocation()))
		return false;
	else if (rhs.getFlagParticipants() && (!lhs.getFlagParticipants() || !participantsMatchFound(getLowerStrList(rhs.getParticipants()), getLowerStrList(lhs.getParticipants()))))
		return false;
	else if (rhs.getFlagNote() && (!lhs.getFlagNote() || rhs.getNote() != lhs.getNote()))
		return false;
	else if (invalidDateChk(lhs, rhs))
		return false;
	else if (rhs.getFlagPriority() && rhs.getPriority() != lhs.getPriority())
		return false;
	else if (rhs.getFlagState() && rhs.getState() != lhs.getState())
		return false;
	return true;
} 

bool Executor::participantsMatchFound(const list<string> &rhsParticipants, const list<string> &lhsParticipants) const {
	for(list<string>::const_iterator i = lhsParticipants.begin(); i != lhsParticipants.end(); i++)
		for(list<string>::const_iterator j = rhsParticipants.begin(); j != rhsParticipants.end(); j++)
			if(i->find(*j) != string::npos)
				return true;
	return false;
}

bool Executor::invalidDateChk(const Task &lhs, const Task &rhs) const {
	bool retVal = true;
	if (!rhs.getFlagFromDate() && !rhs.getFlagToDate())
		retVal = false;
	else if (rhs.getFlagFromDate() && rhs.getFlagToDate()) {
		if (chkDateBound(rhs.getFromDate(), rhs.getToDate(), lhs))
			retVal = false;
	}
	else if (rhs.getFlagFromDate()) {
		if (chkFromDateBound(rhs.getFromDate(), lhs))
			retVal = false;
	}
	else if (chkToDateBound(rhs.getToDate(), lhs))
		retVal = false;
	return retVal;
}

bool Executor::chkDateBound(const time_t &fromTime, const time_t &toTime, const Task &lhs) const {
	return !((!lhs.getFlagToDate() && !lhs.getFlagFromDate()) || 
		(lhs.getFlagToDate() && fromTime > lhs.getToDate()) || 
		(lhs.getFlagFromDate() && toTime < lhs.getFromDate()));
}

bool Executor::chkFromDateBound(const time_t &fromTime, const Task &lhs) const {
	bool retVal = true;
	if (lhs.getFlagToDate()) {
		if (lhs.getToDate() < fromTime)
			retVal = false;
	}
	else if (!lhs.getFlagFromDate())
		retVal = false;
	return retVal;
}

bool Executor::chkToDateBound(const time_t &toTime, const Task &lhs) const {
	bool retVal = true;
	if (lhs.getFlagFromDate()) {
		if (lhs.getFromDate() < toTime)
			retVal = false;
	}
	else if (!lhs.getFlagToDate())
		retVal = false;
	return retVal;
}

list<Task> Executor::getSortListByPriority(list<Task> &taskList) {
	taskList.sort(sortTaskByPriorityComparator);
	return taskList;
}

bool Executor::sortTaskByPriorityComparator(const Task first, const Task second) {
	return (first.getPriority() < second.getPriority());
}

// Undo and Redo functions

void Executor::executeUndo(Command_Undo* cmd, Messenger &response) {
	if (_undoStack.empty())
		setErrorWithErrMsg(response, UNDOSTACK_EMPTY_MSG);
	else {
		Command* undoCmd = getTransposeCommand(_undoStack.top().first, _undoStack.top().second);
		executeCommandWithoutUndoRedo(undoCmd, response);
		delete undoCmd;
		//if (_undoStack.top().first->getCommandType() == TP::COMMAND_TYPE::DEL)
		//	dynamic_cast<Command_Del*>(_undoStack.top().first)->setCreatedTime(response.getTask().getIndex());
		_redoStack.push(_undoStack.top());
		_undoStack.pop();
	}
}

void Executor::executeRedo(Command_Redo* cmd, Messenger &response) {
	Command* redoCmd;
	if (_redoStack.empty())
		setErrorWithErrMsg(response, REDOSTACK_EMPTY_MSG);
	else {
		executeCommandWithoutUndoRedo(_redoStack.top().first, response);		
		_undoStack.push(_redoStack.top());
		_redoStack.pop();
	}
}

Command* Executor::updateDelCmdForUndoStack(Command_Del* cmd, Task &task) {
	cmd->setCreatedTime(task.getIndex());
	return cmd;
}

Command* Executor::getTransposeCommand(Command* cmd, Task &task) {
	switch (cmd->getCommandType()) {
	case COMMAND_TYPE::ADD:
		return getTransposeCommand(dynamic_cast<Command_Add*>(cmd), task);
		break;
	case COMMAND_TYPE::DEL:
		return getTransposeCommand(dynamic_cast<Command_Del*>(cmd), task);
		break;
	case COMMAND_TYPE::MOD:
		return getTransposeCommand(dynamic_cast<Command_Mod*>(cmd), task);
		break;
	default:
		break;
	}
	return cmd;
}

Command* Executor::getTransposeCommand(Command_Add* cmd, Task &task) {
	Command_Del* transposeCmd = new Command_Del();
	transposeCmd->setCreatedTime(_data->back().getIndex());
	return transposeCmd;
}

Command* Executor::getTransposeCommand(Command_Del* cmd, Task &task) {
	Command_Add* transposeCmd = new Command_Add();
	formAddCmdFromTask(task, transposeCmd);
	return transposeCmd;
}

void Executor::formAddCmdFromTask(Task &task, Command_Add* cmd) {
	cmd->setCreatedTime(task.getIndex());
	if(task.getFlagName())
		cmd->setName(task.getName());
	if(task.getFlagLocation())
		cmd->setLocation(task.getLocation());
	if(task.getFlagNote())
		cmd->setNote(task.getNote());
	if(task.getFlagRemindTimes())
		cmd->setRemindTimes(task.getRemindTimes());
	if(task.getFlagParticipants())
		cmd->setParticipants(task.getParticipants());
	if(task.getFlagPriority())
		cmd->setPriority(task.getPriority());
	if(task.getFlagDueDate())
		cmd->setDueDate(task.getDueDate());
	if(task.getFlagFromDate())
		cmd->setFromDate(task.getFromDate());
	if(task.getFlagToDate())
		cmd->setToDate(task.getToDate());
	if(task.getFlagTags())
		cmd->setTags(task.getTags());
}

Command* Executor::getTransposeCommand(Command_Mod* cmd, Task &task) {
	Command_Mod* transposeCmd = new Command_Mod();
	getCmdForSubtractingCmdFromTask(transposeCmd, cmd, task);
	return transposeCmd;
}

void Executor::getCmdForSubtractingCmdFromTask(Command_Mod* subtractCmd, Command_Mod* cmd, Task &task) {
	subtractCmd->setCreatedTime(task.getIndex());
	if(cmd->getFlagOptName()) 
		subtractCmd->setOptName(task.getName());
	if(cmd->getFlagLocation())
		subtractCmd->setLocation(task.getLocation());
	if(cmd->getFlagNote())
		subtractCmd->setNote(task.getNote());
	if(cmd->getFlagRemindTimes() || 
		cmd->getFlagAddRemindTimes() || 
		cmd->getFlagRemoveRemindTimes() || 
		cmd->getFlagRemoveAllRemindTimes())
		subtractCmd->setRemindTimes(task.getRemindTimes());
	if(cmd->getFlagParticipants() || 
		cmd->getFlagAddParticipants() || 
		cmd->getFlagRemoveParticipants() || 
		cmd->getFlagRemoveAllParticipants())
		subtractCmd->setParticipants(task.getParticipants());
	if(cmd->getFlagTags() || 
		cmd->getFlagAddTags() ||
		cmd->getFlagRemoveTags() || 
		cmd->getFlagRemoveAllTags())
		subtractCmd->setTags(task.getTags());
	if(cmd->getFlagPriority())
		subtractCmd->setPriority(task.getPriority());
	if(cmd->getFlagDue() || cmd->getFlagRemoveDue())
		subtractCmd->setDueDate(task.getDueDate());
	if(cmd->getFlagFrom() || cmd->getFlagRemoveFrom())
		subtractCmd->setFromDate(task.getFromDate());
	if(cmd->getFlagTo() || cmd->getFlagRemoveTo())
		subtractCmd->setToDate(task.getToDate());
	if(cmd->getFlagTaskState())
		subtractCmd->setTaskState(task.getState());
}

bool Executor::isCmdSuccessful(const Messenger &response) const {
	return response.getStatus() == TP::STATUS::SUCCESS;
}

void Executor::stackForUndo(Command* cmd, Messenger &response) {
	switch(cmd->getCommandType()) {
	case TP::COMMAND_TYPE::MOD: stackModCmdForUndo(cmd, response); break;
	case TP::COMMAND_TYPE::ADD: stackAddCmdForUndo(cmd, response); break;
	case TP::COMMAND_TYPE::DEL: stackDelCmdForUndo(cmd, response); break;
	}
	clearRedoStack();
}

void Executor::stackModCmdForUndo(Command* cmd, Messenger &response) {
	Command* newCmd = new Command_Mod();
	*newCmd = *cmd;
	_undoStack.push(pair<Command*, Task>(newCmd, _interimTask));
}

void Executor::stackAddCmdForUndo(Command* cmd, Messenger &response) {
	Command* newCmd = new Command_Add();
	*newCmd = *cmd;
	dynamic_cast<Command_Add*>(newCmd)->setCreatedTime(response.getTask().getIndex());
	_undoStack.push(pair<Command*, Task>(newCmd, response.getTask()));
}

void Executor::stackDelCmdForUndo(Command* cmd, Messenger &response) {
	Command* newCmd = new Command_Del();
	*newCmd = *cmd;
	_undoStack.push(pair<Command*, Task>(newCmd, response.getTask()));
}

void Executor::clearRedoStack() {
	while(!_redoStack.empty()) {
		delete _redoStack.top().first;
		_redoStack.pop();
	}
}

void Executor::clearUndoStack() {
	while(!_undoStack.empty()) {
		delete _undoStack.top().first;
		_undoStack.pop();
	}
}

// Utility functions

string Executor::getLowerStr(string str) {
	string lowerStr = str;
	transform(str.begin(), str.end(), lowerStr.begin(), ::tolower);
	return lowerStr;
}

list<string> Executor::getLowerStrList(list<string> strList) {
	list<string> lowerStrList;
	for(list<string>::const_iterator i = strList.begin(); i != strList.end(); i++)
		lowerStrList.push_back(getLowerStr(*i));
	return lowerStrList;
}

// Status setting functions

void Executor::setOpSuccessTask(const Task &retTask, Messenger &response) {
	response.setStatus(TP::STATUS::SUCCESS);
	response.setTask(Task(retTask));
}

void Executor::setOpSuccessTaskList(const list<Task>& results, Messenger &response) {
	response.setStatus(TP::STATUS::SUCCESS);
	response.setList(results);
}

void Executor::setOpIntermediateTaskList(const list<Task>& results, Messenger &response) {
	response.setStatus(TP::STATUS::INTERMEDIATE);
	response.setList(results);
}

void Executor::setIndexNotFound(const unsigned long long &index, Messenger &response) {
	response.setStatus(TP::STATUS::ERROR);
	response.setErrorMsg(std::to_string(index) + INVALID_INDEX_ERROR);
}

void Executor::setNameNotFound(const string &name, Messenger &response) {
	response.setStatus(TP::STATUS::ERROR);
	response.setErrorMsg(NAME_NOT_FOUND_ERROR + name);
}

void Executor::setErrorWithErrMsg(Messenger &response, const string errMsg) {
	response.setStatus(TP::STATUS::ERROR);
	response.setErrorMsg(errMsg);
}
*/

