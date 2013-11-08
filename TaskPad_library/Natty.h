#pragma once

#include "./Natty/jni.h"
#include <Windows.h>
#include <string>
#include <cstring>

class natty {
private:
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