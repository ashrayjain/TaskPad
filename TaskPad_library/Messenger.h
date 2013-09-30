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
		Messenger(STATUS status=SUCCESS, list<Task> result= list<Task>(), int index =-1, string errorMsg="");

		//getter methods
		
		string getErrorMsg() const;
		STATUS getStatus() const;
		list<Task> getList() const;
		int getIndex() const;
		void resetMessenger();

		//setter functions

		bool setErrorMsg(const string& errorMsg);
		bool setStatus(const STATUS& status);
		bool setList(const list<Task>& result);
		bool setInt(const int& index);
	
	private:
		string _errorMsg;
		STATUS _status;
		list<Task> _resultList;
		int _index;

		bool isValidIndex (const int& index) const;
};

#endif