#ifndef _STORAGE_H_
#define _STORAGE_H_

#include<fstream>
#include<list>

//forward definitions
class Task;

using namespace std;

class Storage
{
	private:
		ofstream _fileWriter;
		ifstream _fileReader;
	public:
		Storage(list<Task>&);

		bool save(const list<Task>&);
		bool save(const Command&);

		~Storage();
};
#endif