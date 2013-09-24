#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include<string>
#include<list>
#include "task.h"

using namespace std;

enum STATUS{
	SUCCESS, INTERMEDIATE, ERROR
};

class Messenger
{
	private:
		string _errorMsg;
		STATUS _status;
		list<Task> _resultList;
	public:
		// constructor
		Messenger(string errorMsg, STATUS status, list<Task> result);

		//getter methods
		
		string getErrorMsg();
		STATUS getStatus();
		list<Task> getIntermediateList();
		list<Task> getResultList();
		list<Task> getList();

		//setter functions

		bool setErrorMsg(string errorMsg);
		bool setStatus(STATUS status);
		bool setList(list<Task> result);
};

#endif