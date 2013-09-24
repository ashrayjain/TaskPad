#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include<string>
#include<list>
#include "task.h"

using namespace std;

enum STATUS{
	ERROR, SUCCESS, INTERMEDIATE, SUCCESS_CWI
};

class Messenger
{
	private:
		string _errorMsg;
		STATUS _status;
		list<Task> _resultList;
		int _index;

		bool isValidIndex (int index);
	public:
		// constructor
		Messenger();
		Messenger(string errorMsg, STATUS status, list<Task> result);

		//getter methods
		
		string getErrorMsg();
		STATUS getStatus();
		list<Task> getList();
		int getIndex();

		//setter functions

		bool setErrorMsg(string errorMsg);
		bool setStatus(STATUS status);
		bool setList(list<Task> result);
		bool setInt(int index);
};

#endif