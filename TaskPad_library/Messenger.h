#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include<string>
#include<list>
#include "task.h"

using namespace std;

enum STATUS{
	ERROR, ERROR_INTERMEDIATE, SUCCESS, INTERMEDIATE, SUCCESS_INDEXED_COMMAND, DISPLAY
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
		Messenger(STATUS status=SUCCESS, list<Task> result= list<Task>(), int index =-1, string errorMsg="");

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