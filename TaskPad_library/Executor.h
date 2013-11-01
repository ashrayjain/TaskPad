/*
 * =====================================================================================
 *
 *       Filename:  Executor.h
 *
 *        Version:  1.0
 *        Created:  09/25/13 00:22:00
 *
 *         Author:  ASHRAY JAIN (A0105199B), ashrayj11@gmail.com
 *   Organization:  NUS, SoC
 *
 * =====================================================================================
 */

#include <string>
#include <cstring>
#include <chrono>
#include <list>
#include <unordered_map>
#include <stack>
#include <queue>
#include "Task.h"
#include "Messenger.h"
#include "Command.h"


class Executor
{
public:
	Executor (std::list<Task>* data) { _data = data; rebuildHashes(); }
	~Executor ()					 { clearRedoStack(); clearUndoStack(); }

	void		executeCommand		(Command* cmd, Messenger &response);
	list<Task>	getCurrentReminders	();

protected:
	std::list<Task>*									_data;
	std::unordered_map<unsigned long long, Task*>		_indexHash;
	std::unordered_map<std::string, std::list<Task*>>	_hashTagsHash;
	std::unordered_map<std::time_t, std::list<Task*>>	_remindTimesHash;
	std::stack<std::pair<Command*, Task>>				_undoStack;
	std::stack<std::pair<Command*, Task>>				_redoStack;
	Task												_interimTask;

	// Class constants
	static const unsigned		EMPTY_LIST_SIZE;
	static const unsigned		SINGLE_RESULT_LIST_SIZE;
	static const std::string	NAME_NOT_FOUND_ERROR;
	static const std::string	INVALID_INDEX_ERROR;
	static const std::string	UNDOSTACK_EMPTY_MSG;
	static const std::string	REDOSTACK_EMPTY_MSG;
	static const std::string	MODIFY_SAME_NAME_ERROR;
	static const std::string	INVALID_FROMDATE_ERROR;
	static const std::string	INVALID_TODATE_ERROR;
	static const std::string	INVALID_FROMDATE_TODATE_ERROR;
	static const std::string	NAME_NOT_SPECIFIED_ERROR;

	void	rebuildHashes();
	void	rebuildIndexHash();
	void	rebuildHashTagsHash();
	void	rebuildRemindTimesHash();

	void getTasksFromTaskPtrList	(list<Task> &taskResults, list<Task*> &results);

	// Functions for ADD COMMAND
	void executeAdd						(Command_Add* cmd,  Messenger &response);
	bool validAddCmd					(Command_Add* cmd, Messenger &response);
	void formTaskFromAddCmd				(Command_Add* cmd, Task &newTask);
	void setDefaultRemindTimes			(Task &task);
	void setDefaultRemindTimesPriorityH	(Task &task);
	void setDefaultRemindTimesPriorityM	(Task &task);
	void setDefaultRemindTimesPriorityL	(Task &task);
	void handleHashTagPtrs				(Task &newTask, const list<string> &hashTagsList);
	void handleExistingHashTag			(list<list<Task*>::iterator> &newHashTagPtrs, Task &newTask, list<Task*> &hashTag);
	void handleNewHashTag				(list<list<Task*>::iterator> &newHashTagPtrs, Task &newTask, list<string>::const_iterator &hashTag);
	void handleRemindTimesPtrs			(Task &newTask, const list<time_t> &remindTimesList);
	void handleExistingRemindTime		(list<list<Task*>::iterator> &newRemindTimesPtrs, Task &newTask, list<Task*> &remindTime);
	void handleNewRemindTime			(list<list<Task*>::iterator> &newRemindTimesPtrs, Task &newTask, list<time_t>::const_iterator &remindTime);

	// Functions for DELETE COMMAND
	void executeDel							(Command_Del* cmd,  Messenger &response);
	void deleteTaskByIndex					(const unsigned long long &index, Messenger &reponse);	
	void deleteTaskByName					(const string &name, Messenger &reponse, const bool &exactFlag);
	void deleteByExactName					(const string &name, Messenger &response);
	void deleteByApproxName					(const string &name, Messenger &response);
	void selectAppropriateDeleteResponse	(const list<Task> &matchingResults, const list<Task> &caseInsensitiveResults,
												const string &name, Messenger &response);
	void deleteTask							(list<Task>::iterator &i);
	void deleteHashTags						(Task &task);
	void deleteRemindTimes					(Task &task);

	// Functions for MODIFY COMMAND
	void			executeMod						(Command_Mod* cmd,  Messenger &response);
	void			modifyByIndex					(Command_Mod* cmd, Messenger &response);
	void			modifyByName					(Command_Mod* cmd, Messenger &response);
	void			modifyByExactName				(Command_Mod* cmd, Messenger &response);
	void			modifyByApproxName				(Command_Mod* cmd, Messenger &response);
	void			selectAppropriateModifyResponse	(const list<Task> &matchingResults, const list<Task> &caseInsensitiveResults, 
														Command_Mod* cmd, Messenger &response);
	void			modifyTaskTo					(Task &oldTask, Command_Mod* cmd);
	bool			isModCmdValid					(Command_Mod* cmd, const Task& task, Messenger &response);
	void			handleHashTagsModify			(Task &oldTask, const list<string> &newTags);
	void			handleRemindTimesModify			(Task &oldTask, const list<time_t> &newRemindTimes);
	void			handleAddRemoveParticipants		(Task &task, list<string> &participants, TP::LIST_OP op);
	void			handleAddRemoveRemindTimes		(Task &task, list<time_t> &remindTimes, TP::LIST_OP op);
	list<string>	getTagsListDifference			(const list<string> &taskTags, const list<string> &tagsToRemove) const;

	// Functions for FIND COMMAND
	void			executeFind					(Command_Find* cmd, Messenger &response);
	void			formTaskFromFindCmd			(Command_Find* cmd, Task &newTask);
	void			findByIndex					(const unsigned long long index, Messenger &response);
	void			findGeneral					(Command_Find* cmd, Messenger &response);
	void			runSearch					(const Task &taskToCompare, list<Task> &results, string substringName, set<Task*> &customData, bool customDataSet);
	void			findByTags					(Command_Find* cmd, Messenger &response);
	void			getCustomDataRangeByTags	(set<Task*> &customDataRange, list<string> &tags);
	void			getCustomDataRangeByRT		(set<Task*> &customDataRange, list<time_t> &remindTimes);
	void			filterResponseListByType	(Messenger &response, list<TP::TASK_TYPE> &types);
	void			runSearchWithTask			(const Task &taskToCompare,	list<Task> &results);
	void			runSearchWithTask			(const Task &taskToCompare, list<Task> &results, string substringName);
	void			runSearchWithTask			(const Task &taskToCompare,	list<Task> &results, set<Task*> &customData);
	void			runSearchWithTask			(const Task &taskToCompare, list<Task> &results, string substringName, set<Task*> &customData);
	bool			taskMatch					(const Task& lhs, const Task& rhs);
	bool			participantsMatchFound		(const list<string> &rhsParticipants, const list<string> &lhsParticipants) const;
	bool			invalidDateChk				(const Task &lhs, const Task &rhs) const;
	bool			chkDateBound				(const time_t &fromTime, const time_t &toTime, const Task &lhs) const;
	bool			chkFromDateBound			(const time_t &fromTime, const Task &lhs) const;
	bool			chkToDateBound				(const time_t &toTime, const Task &lhs) const;
	std::list<Task> getSortListByPriority		(std::list<Task> &taskList);
	
	// Custom Comparator for sorting Tasks by Priority
	static bool sortTaskByPriorityComparator	(const Task first, const Task second);

	// Functions for UNDO and REDO COMMAND
	void		executeCommandWithoutUndoRedo	(Command* cmd, Messenger &response);
	void		executeUndo						(Command_Undo* cmd, Messenger &response);
	void		executeRedo						(Command_Redo* cmd, Messenger &response);
	Command*	updateDelCmdForUndoStack		(Command_Del* cmd, Task &task);
	Command*	getTransposeCommand				(Command* cmd, Task &task);
	Command*	getTransposeCommand				(Command_Add* cmd, Task &task);
	Command*	getTransposeCommand				(Command_Del* cmd, Task &task);
	Command*	getTransposeCommand				(Command_Mod* cmd, Task &task);
	void		getCmdForSubtractingCmdFromTask	(Command_Mod* subtractCmd, Command_Mod* cmd, Task &task);
	void		formAddCmdFromTask				(Task &task, Command_Add* cmd);
	bool		isCmdSuccessful					(const Messenger &response) const;
	void		stackForUndo					(Command* cmd, Messenger &response);
	void		clearRedoStack					();
	void		clearUndoStack					();

	// Utility functions
	string			getLowerStr		(string str);
	list<string>	getLowerStrList	(list<string> strList);

	// Setters for Messenger to return
	void setOpSuccessTask			(const Task &retTask, Messenger &response);
	void setOpSuccessTaskList		(const list<Task>& results, Messenger &response);
	void setOpIntermediateTaskList	(const list<Task> &results, Messenger &response);
	void setIndexNotFound			(const unsigned long long &index, Messenger &response);
	void setNameNotFound			(const string &name, Messenger &response);
	void setErrorWithErrMsg			(Messenger &response, const string errMsg);
};