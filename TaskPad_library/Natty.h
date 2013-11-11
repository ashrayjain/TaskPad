#ifndef TASKPAD_NATTY_H
#define TASKPAD_NATTY_H

#include "./Natty/jni.h"
#include <Windows.h>
#include <string>
#include <cstring>

class natty {
private:
	static char* JVM_OPTIONS;
	static LPCWSTR DLL_LOCATION;
	static char* CLASS_TO_LOAD;
	static char* SIGNATURE;
	static char* METHOD_TO_CALL;
	static char* DLL_NOT_FOUND;
	static char* JVM_LOAD_ERROR;
	static char* CLASS_NOT_FOUND;
	static char* JNI_CREATEVM;
	JavaVMOption jvmOptions[1];
	JNIEnv* jEnv;
	JavaVM* jVM;
	JavaVMInitArgs vmArgs;
	long jvmCreated;
	jclass parseClass;
	HINSTANCE hVM;
	static natty* _natty;
	natty();
	~natty();
public:
	std::string parseDateTime(std::string commandStr);
	static natty& getNatty();
};
#endif