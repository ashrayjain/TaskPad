#ifndef _ENUM_H
#define _ENUM_H

#include <string>
#include <exception>

namespace TP{

	static const int COMMAND_TYPE_COUNT		= 8;
	static const int PRIORITY_COUNT			= 3;
	static const int TASK_STATE_COUNT		= 3;
	static const int TASK_TYPE_COUNT		= 3;
	static const int LOG_TYPE_COUNT			= 5;
    enum COMMAND_TYPE	{ ADD, MOD, DEL, FIND, INDEX, UNDO, REDO, SYNC, UNDEFINED};
    enum PRIORITY		{ HIGH, MEDIUM, LOW, NON_HIGH };
    enum TASK_STATE		{ UNDONE, OVERDUE, DONE };
    enum TASK_TYPE		{ TIMED, DEADLINE, FLOATING };
    enum CMD_PROPERTY	{ UNCHANGED = false, EDITED = true };
    enum STATUS			{
        ERR, ERR_INTER, SUCCESS, INTERMEDIATE, SUCCESS_INDEXED_COMMAND, DISPLAY
    };
    enum PERIOD_TYPE	{ DAY, WEEK, MONTH};
    enum LIST_OP		{ ADD_ELEMENT, REMOVE_ELEMENT };
	enum LOG_TYPE		{ INFOLOG, NOTICELOG, WARNINGLOG, ERRORLOG, FATALLOG };

	//String representations of the Enums above
	static const std::string COMMAND_TYPE_STRING[COMMAND_TYPE_COUNT] =	
                    { "ADD","MOD","DEL","FIND","UNDO","REDO","SYNC","UNDEFINED"	};
	static const std::string PRIORITY_STRING[PRIORITY_COUNT]		=		{"HIGH","MEDIUM","LOW"};
	static const std::string TASK_STATE_STRING[TASK_STATE_COUNT]	=		{"UNDONE","OVERDUE","DONE"};
	static const std::string TASK_TYPE_STRING[TASK_TYPE_COUNT]		=		{"TIMED","DEADLINE","FLOATING"};
	static const std::string LOG_TYPE_STRING[LOG_TYPE_COUNT]		=		{"INFOLOG","NOTICELOG","WARNINGLOG","ERRORLOG","FATALLOG"};

	//exceptions for the classes
	class BaseException : public std::exception {
	protected:
		std::string _errorStr;
	public:
		virtual const char* what() const throw() {
			return _errorStr.c_str();
		}
		
	};

	// for datastore and task

	class InvalidIndexException : public TP::BaseException {
	public:
		InvalidIndexException (std::string errorStr)	{ _errorStr = errorStr; }
	};

	class EmptyStackException : public TP::BaseException {
	public:
		EmptyStackException (std::string errorStr)	{ _errorStr = errorStr; }
	};

	// for natty:

	class FileNotFoundException : public TP::BaseException {
	public:
		FileNotFoundException (std::string errorStr)	{ _errorStr = errorStr; }
	};

	//For interpreter:

	class DuplicateKeyWordException : public TP::BaseException {
	public:
		DuplicateKeyWordException (std::string errorStr)	{ _errorStr = errorStr; }
	};

	class TimeFormatException : public TP::BaseException {
	public:
		TimeFormatException (std::string errorStr)	{ _errorStr = errorStr; }
	};

	class GeneralCommandFormatException : public TP::BaseException {
	public:
		GeneralCommandFormatException (std::string errorStr)	{ _errorStr = errorStr; }
	};

	//for logger:

	class FatalLogException : public TP::BaseException {
		public:
			FatalLogException (std::string errorStr)	{ _errorStr = errorStr; }
	};

	class ClassNotFoundException : public TP::BaseException {
		public:
			ClassNotFoundException (std::string errorStr)	{ _errorStr = errorStr; }
	};

	class DllNotFoundException : public TP::BaseException {
		public:
			DllNotFoundException (std::string errorStr)	{ _errorStr = errorStr; }
	};

	class StackUnderflowException : public TP::BaseException {
		public:
			StackUnderflowException (std::string errorStr)	{ _errorStr = errorStr; }
	};

	class JvmLoadException : public TP::BaseException {
		public:
			JvmLoadException (std::string errorStr)	{ _errorStr = errorStr; }
	};
}
#endif
