/*
 * =====================================================================================
 *
 *		Filename:  Messenger.h
 *
 *      Version:  1.0
 *
 *      Author:  Thyaegsh Manikandan (A0100124J)
 *		Organization:  NUS, SoC
 *
 * =====================================================================================

 ** Gist of File Contents:
  *
  * This file defines the Messenger Class, its functions and attributes
  * A messenger object is used as a way to return and exchange different values from
  * functions. This way a more sophisticated value can be returned allowing for 
  * greater information transfer from the function to its caller.
  *
 */

#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include <string>
#include <list>
#include "task.h"
#include "Enum.h"

class Messenger {
	public:

		// constructor
		Messenger(
			TP::COMMAND_TYPE commandType=TP::UNDEFINED, 
			TP::STATUS status=TP::SUCCESS, 
			std::list<Task> resultList= std::list<Task>(), 
			Task resultTask=Task(), 
			int index =-1, 
			std::string errorMsg="");

		//getter methods
		
		std::string			getErrorMsg()		const;
		TP::STATUS				getStatus()			const;
		std::list<Task>		getList()			const;
		int					getIndex()			const;
		TP::COMMAND_TYPE		getCommandType()	const;
		Task				getTask()			const;

		// state resetter
		void resetMessenger();

		//setter functions
		void setErrorMsg	(const std::string&			errorMsg);
		void setStatus		(const TP::STATUS&				status);
		void setList		(const std::list<Task>&		result);
		void setInt			(const int&					index);
		void setCommandType	(const TP::COMMAND_TYPE&		commandType);
		void setTask		(const Task&				task);

		const static int DEFAULT_INDEX_VALUE;
		const static TP::STATUS DEFAULT_STATUS_VALUE;
		const static TP::COMMAND_TYPE DEFAULT_COMMAND_TYPE_VALUE;
		const static Task DEFAULT_TASK_VALUE;
		const static std::list<Task> DEFAULT_TASK_LIST_VALUE;
		const static std::string DEFAULT_ERROR_MESSAGE_VALUE;

	private:
		std::string			_errorMsg;
		TP::STATUS				_status;
		std::list<Task>		_resultList;
		Task				_resultTask;
		int					_index;
		TP::COMMAND_TYPE		_commandType;
};

#endif