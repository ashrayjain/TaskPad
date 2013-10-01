#ifndef _STORAGE_H_
#define _STORAGE_H_

#include<fstream>
#include<list>

//forward definitions
class Task;
class Command;
class DeadlineTask;
class TimedTask;
class FloatingTask;

using namespace std;

class Storage
{
	private:
		ofstream _fileWriter;
		ifstream _fileReader;

		static const string _fileName;

		void saveDeadlineTask(Task* tempTask);
		void saveTimedTask(Task* tempTask);
		void saveFloatingTask(Task* tempTask);
	public:
		Storage(list<Task>&);

		bool save(list<Task>&);
		bool save(const Command&);

		~Storage();
};
#endif