#include "Manager.h"
#include <string>
 using namespace std;

 Manager::Manager()
 {
	 this->_executor = new Executor;
	 this->_interpreter = new Interpreter;
	 this->_storage = new Storage;
 }