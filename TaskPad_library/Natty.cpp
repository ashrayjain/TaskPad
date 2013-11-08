#include "Natty.h"

natty* natty::_natty = NULL;

natty::natty() {
	jvmOptions[0].optionString = "-Djava.class.path=./Natty;./Natty/java;./Natty/java/lib;./Natty/java/lib/natty-0.8.jar;./Natty/java/lib/ical4j-1.0.2.jar;./Natty/java/lib/antlr-runtime-3.2.jar;./Natty/java/lib/antlr-2.7.7.jar";
	memset(&vmArgs, 0, sizeof(vmArgs));
	vmArgs.version = JNI_VERSION_1_6;
	vmArgs.nOptions = 1;
	vmArgs.options = jvmOptions; 
	hVM = LoadLibrary(L"./Natty/bin/client/jvm.dll");
	if (hVM == NULL)
		throw("Java Virtual Machine DLL file missing!");
	typedef jint (CALLBACK *fpCJV)(JavaVM**, void**, JavaVMInitArgs*);
	fpCJV CreateJavaVM = (fpCJV)::GetProcAddress(hVM, "JNI_CreateJavaVM");

	jvmCreated = CreateJavaVM(&jVM, (void**)&jEnv, &vmArgs);
	
	if (jvmCreated == JNI_ERR)
		throw("JVM could not be loaded!");
	else {
		parseClass = jEnv->FindClass("DateParser");
		if(parseClass == 0)
			throw("Java Class not found!");
	}
}

natty::~natty() {
	FreeLibrary(hVM);
	jVM->DestroyJavaVM();
}

std::string natty::parseDateTime(std::string commandStr) {
	if(parseClass != 0) {
		jmethodID mid = jEnv->GetStaticMethodID(parseClass,"parse","(Ljava/lang/String;)Ljava/lang/String;");
		if(mid != 0){
			jstring data = (jstring)jEnv->CallStaticObjectMethod(parseClass,mid,jEnv->NewStringUTF(commandStr.c_str()));
			if(data != NULL){
				const char *str= jEnv->GetStringUTFChars(data,0);
				std::string result = std::string(str);
				if(result != "")
					commandStr = result;
				jEnv->ReleaseStringUTFChars(data, str);
			}
		}
	}
	return commandStr;
}

natty& natty::getNatty() {
	if(_natty == NULL)
	{
		static natty theOne;
		_natty = &theOne;
	}
	return *_natty;
}