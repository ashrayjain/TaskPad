/*
 * =====================================================================================
 *
 *       Filename:  Datastore.cpp
 *
 *        Version:  1.0
 *        Created:  10/25/13 00:22:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include "Datastore.h"

using namespace std;

void Datastore::clearRedoStack() {
	while(!_redoStack.empty()) {
		delete _redoStack.top().first;
		_redoStack.pop();
	}
}

void Datastore::clearUndoStack() {
	while(!_undoStack.empty()) {
		delete _undoStack.top().first;
		_undoStack.pop();
	}
}

void Datastore::addTask(const Task &newTask) {
	_ds->addTask(newTask);
	_indexHash[newTask.getIndex()] = &(_ds->back());
	handleHashTagPtrs(_ds->back(), _ds->back().getTags());
	handleRemindTimesPtrs(_ds->back(), _ds->back().getRemindTimes());
}

void Datastore::handleHashTagPtrs(Task &newTask, const list<string> &hashTagsList) {
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

void Datastore::handleExistingHashTag(list<list<Task*>::iterator> &newHashTagPtrs, Task &newTask, list<Task*> &hashTag) {
	hashTag.push_back(&newTask);
	newHashTagPtrs.push_back(--hashTag.end());
}

void Datastore::handleNewHashTag(list<list<Task*>::iterator> &newHashTagPtrs, Task &newTask, list<string>::const_iterator &hashTag) {
	_hashTagsHash[*hashTag] = list<Task*>(1, &newTask);
	newHashTagPtrs.push_back(--(_hashTagsHash[*hashTag].end()));
}

void Datastore::handleRemindTimesPtrs(Task &newTask, const list<time_t> &remindTimesList) {
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

void Datastore::handleExistingRemindTime(list<list<Task*>::iterator> &newRemindTimesPtrs, Task &newTask, list<Task*> &remindTime) {
	remindTime.push_back(&newTask);
	newRemindTimesPtrs.push_back(--remindTime.end());
}

void Datastore::handleNewRemindTime(list<list<Task*>::iterator> &newRemindTimesPtrs, Task &newTask, list<time_t>::const_iterator &remindTime) {
	_remindTimesHash[*remindTime] = list<Task*>(1, &newTask);
	newRemindTimesPtrs.push_back(--(_remindTimesHash[*remindTime].end()));
}

void Datastore::deleteTask(const unsigned &pos) {
	Datastore_Base::iterator i = _ds->begin();
	advance(i, pos);
	_indexHash.erase(i->getIndex());
	deleteHashTags(*i);
	deleteRemindTimes(*i);
	_ds->deleteTask(i);
}

void Datastore::deleteHashTags(Task &task) {
	list<string> tags = task.getTags();
	list<list<Task*>::iterator> tagPtrs = task.getHashTagPtrs();
	list<string>::iterator k = tags.begin();
	for (list<list<Task*>::iterator>::iterator j = tagPtrs.begin(); j != tagPtrs.end(); j++, k++)
		_hashTagsHash[*k].erase(*j);
}

void Datastore::deleteRemindTimes(Task &task) {
	list<time_t> remindTimes = task.getRemindTimes();
	list<list<Task*>::iterator> remindTimePtrs = task.getRemindTimesPtrs();
	list<time_t>::iterator k = remindTimes.begin();
	for (list<list<Task*>::iterator>::iterator j = remindTimePtrs.begin(); j != remindTimePtrs.end(); j++, k++)
		_remindTimesHash[*k].erase(*j);
}

Task Datastore::indexHashSearch(unsigned long long indexToSearch) {
	unordered_map<unsigned long long, Task*>::iterator result = _indexHash.find(indexToSearch);
	if (result != _indexHash.end())
		throw exception("Invalid Index");	
	return *(result->second);
}

bool Datastore::isIndexPresent(unsigned long long indexToSearch) {
	return _indexHash.count(indexToSearch);
}

list<Task>	Datastore::getTasksWithHash(string hash) {
	list<Task> retList;
	for(list<Task*>::iterator i = _hashTagsHash[hash].begin(); i != _hashTagsHash[hash].end(); ++i)
		retList.push_back(**i);
	return retList;
}

list<Task> Datastore::getTasksWithRemindTimes(time_t remindTime) {
	list<Task> retList;
	for(list<Task*>::iterator i = _remindTimesHash[remindTime].begin(); i != _remindTimesHash[remindTime].end(); ++i)
		retList.push_back(**i);
	return retList;
}

Task Datastore::modifyTask(const unsigned &pos, Command_Mod* cmd) {
	Datastore_Base::iterator i = _ds->begin();
	advance(i, pos);
	_interimTask = Task(*i);
	modifyTaskWithPtr(*i, cmd);
	return Task(*i);
}

Task Datastore::modifyTaskWithIndex(const unsigned long long index, Command_Mod* cmd) {
	unordered_map<unsigned long long, Task*>::iterator result = _indexHash.find(index);
	_interimTask = Task(*(result->second));
	modifyTaskWithPtr(*(result->second), cmd);
	return Task(*(result->second));
}


void Datastore::modifyTaskWithPtr(Task &oldTask, Command_Mod* cmd) {
	if(cmd->getFlagOptName()) 
		oldTask.setName(cmd->getOptName());
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

void Datastore::handleHashTagsModify(Task &oldTask, const list<string> &newTags) {
	deleteHashTags(oldTask);
	oldTask.setTags(newTags);
	handleHashTagPtrs(oldTask, newTags);
}

void Datastore::handleRemindTimesModify(Task &oldTask, const list<time_t> &newRemindTimes) {
	deleteRemindTimes(oldTask);
	oldTask.setRemindTimes(newRemindTimes);
	handleRemindTimesPtrs(oldTask, newRemindTimes);
}

void Datastore::handleAddRemoveParticipants(Task &task, list<string> &participants, TP::LIST_OP op) {
	for(list<string>::iterator i = participants.begin(); i != participants.end(); i++)
		task.setParticipants(*i, op);
}

void Datastore::handleAddRemoveRemindTimes(Task &task, list<time_t> &remindTimes, TP::LIST_OP op) {
	for(list<time_t>::iterator i = remindTimes.begin(); i != remindTimes.end(); i++)
		task.setRemindTimes(*i, op);
	list<time_t> newRemindTimes = task.getRemindTimes();
	handleRemindTimesModify(task, newRemindTimes);
}

list<string> Datastore::getTagsListDifference(const list<string> &taskTags, const list<string> &tagsToRemove) const {
	list<string> newTagsList;
	for(list<string>::const_iterator i = taskTags.begin(); i != taskTags.end(); i++)
		if(find(tagsToRemove.begin(), tagsToRemove.end(), *i) == tagsToRemove.end())
			newTagsList.push_back(*i);
	return newTagsList;
}

void Datastore::stackCmdForUndo(Command* cmd, Messenger &response) {
	switch(cmd->getCommandType()) {
	case TP::COMMAND_TYPE::MOD: stackModCmdForUndo(cmd, response); break;
	case TP::COMMAND_TYPE::ADD: stackAddCmdForUndo(cmd, response); break;
	case TP::COMMAND_TYPE::DEL: stackDelCmdForUndo(cmd, response); break;
	}
	clearRedoStack();
}

void Datastore::stackModCmdForUndo(Command* cmd, Messenger &response) {
	Command* newCmd = new Command_Mod();
	*newCmd = *cmd;
	_undoStack.push(pair<Command*, Task>(newCmd, _interimTask));
}

void Datastore::stackAddCmdForUndo(Command* cmd, Messenger &response) {
	Command* newCmd = new Command_Add();
	*newCmd = *cmd;
	dynamic_cast<Command_Add*>(newCmd)->setCreatedTime(response.getTask().getIndex());
	_undoStack.push(pair<Command*, Task>(newCmd, response.getTask()));
}

void Datastore::stackDelCmdForUndo(Command* cmd, Messenger &response) {
	Command* newCmd = new Command_Del();
	*newCmd = *cmd;
	_undoStack.push(pair<Command*, Task>(newCmd, response.getTask()));
}
void Datastore::popTopRedoStackToUndoStack() {
	if (!_redoStack.empty()) {
		_undoStack.push(_redoStack.top());
		_redoStack.pop(); 
	}
	else
		throw exception("Nothing to pop");
}

void Datastore::popTopUndoStackToRedoStack() {
	if (!_undoStack.empty()) {
		_redoStack.push(_undoStack.top());
		_undoStack.pop(); 
	}
	else
		throw exception("Nothing to pop");
}
