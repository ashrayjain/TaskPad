#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include<string>
#include<list>
#include "task.h"
#include "Enum.h"

using namespace std;
using namespace TP;

class Messenger
{
	public:

		// constructor
		Messenger(COMMAND_TYPE commandType=UNDEFINED, STATUS status=SUCCESS, list<Task> result= list<Task>(), int index =-1, string errorMsg="");

		//getter methods
		
		string			getErrorMsg()		const;
		STATUS			getStatus()			const;
		list<Task>		getList()			const;
		int				getIndex()			const;
		COMMAND_TYPE	getCommandType()	const;

		// state resetter
		void resetMessenger();

		//setter functions
		void setErrorMsg	(const string&			errorMsg);
		void setStatus		(const STATUS&			status);
		void setList		(const list<Task>&		result);
		void setInt			(const int&				index);
		void setCommandType	(const COMMAND_TYPE&	commandType);

	private:
		string			_errorMsg;
		STATUS			_status;
		list<Task>		_resultList;
		int				_index;
		COMMAND_TYPE	_commandType;
};

#endif