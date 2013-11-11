#include "TaskLoader.h"
#include "Enum.h"

using namespace TP;
using namespace std;

const string TaskLoader::MESSAGE_FILE_OPENING_ERROR = "A file used for handling changes could not be opened to load!";
const string TaskLoader::MESSAGE_FILE_CLOSING_ERROR = "A file used by the program could not be closed!";
const string TaskLoader::MESSAGE_RESTART_ADVICE		= "Please restart the program to prevent unintentional loss of data";

void TaskLoader::openFile(const std::string& fileName,
						  std::ios_base::openmode oMode) 
{
	try {
		this->_fileReader.open(fileName,oMode);
	}
	catch (std::ifstream::failure e) {
		throw FileHandlingException(MESSAGE_FILE_OPENING_ERROR + MESSAGE_RESTART_ADVICE);
	}
}

void TaskLoader::closeFile() {
	try {
		_fileReader.close(); 
	}
	catch (std::ifstream::failure e) {
		throw FileHandlingException(MESSAGE_FILE_CLOSING_ERROR + MESSAGE_RESTART_ADVICE);
	}
}