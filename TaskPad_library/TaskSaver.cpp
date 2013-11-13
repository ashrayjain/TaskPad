#include "TaskSaver.h"
#include "Enum.h"

using namespace std;
using namespace TP;
// @author A0100124J

const string TaskSaver::MESSAGE_FILE_OPENING_ERROR	= "A file used for handling changes could not be opened for saving!";
const string TaskSaver::MESSAGE_FILE_CLOSING_ERROR	= "A file used by the program could not be closed!";
const string TaskSaver::MESSAGE_RESTART_ADVICE		= "Please restart the program to prevent unintentional loss of data";

void TaskSaver::openFile(const std::string& fileName,
						  std::ios_base::openmode oMode) 
{
	try {
		this->_fileWriter.open(fileName,oMode);
	}
	catch (std::ifstream::failure e) {
		throw FileHandlingException(MESSAGE_FILE_OPENING_ERROR + MESSAGE_RESTART_ADVICE);
	}
}

void TaskSaver::closeFile() {
	try {
		_fileWriter.close(); 
	}
	catch (std::ifstream::failure e) {
		throw FileHandlingException(MESSAGE_FILE_CLOSING_ERROR + MESSAGE_RESTART_ADVICE);
	}
}