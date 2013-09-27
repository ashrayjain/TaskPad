#include "Storage.h"
#include "Task.h"
#include "Enum.h"
#include <list>

using namespace TP;

Storage::Storage(list<Task>&)
{
}

bool Storage::save(list<Task>& taskList)
{
	string singleEntry = "";
	if(taskList.empty())
	{
		// EMPTY THE WHOLE FILE!!
		return true;
	}

	list<Task>::iterator it = taskList.begin();

	while(it != taskList.end())
	{
		switch(it->getTaskType())
		{
			case DEADLINE:
			case TIMED:
			case FLOATING:
				break;
		} 
		return false;


		it++;
	}
}
bool Storage::save(const Command&)
{
	return false;
}

Storage::~Storage()
{
	this->_fileReader.close();
	this->_fileWriter.close();
}