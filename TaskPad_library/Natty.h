#pragma once

#include "./Natty/jni.h"
#include <Windows.h>
#include <string>
#include <cstring>

class natty {
private:
	JavaVMOption _options[1];
	JNIEnv *_env;
	JavaVM *_jvm;
	JavaVMInitArgs _vm_args;
	long _status;
	jclass _cls;
	static natty *_natty;
	natty();
	~natty();
public:
	std::string parseDateTime(std::string commandStr);
	static natty& getNatty();
};