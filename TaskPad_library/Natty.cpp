#include "Natty.h"

natty* natty::_natty = NULL;
//char* natty::JVM_OPTIONS = "-Djava.class.path=./Natty;.Natty/natty;./Natty/natty/natty-0.8.jar;./Natty/natty/ical4j-1.0.2.jar;./Natty/natty/antlr-runtime-3.2.jar;./Natty/natty/antlr-2.7.7.jar"; 
char* natty::JVM_OPTIONS = "-Djava.class.path=./Natty;.Natty/natty;./Natty/natty/natty.jar"; 
LPCWSTR natty::DLL_LOCATION = L"./Natty/bin/client/jvm.dll"; 
char* natty::CLASS_TO_LOAD = "DateParser";
char* natty::SIGNATURE = "(Ljava/lang/String;)Ljava/lang/String;";
char* natty::METHOD_TO_CALL = "parse";
char* natty::DLL_NOT_FOUND = "Java Virtual Machine DLL file missing!";
char* natty::JVM_LOAD_ERROR = "JVM could not be loaded!";
char* natty::CLASS_NOT_FOUND = "Java Class not found!";
char* natty::JNI_CREATEVM = "JNI_CreateJavaVM";

natty::natty() {
	jvmOptions[0].optionString = JVM_OPTIONS;
	memset(&vmArgs, 0, sizeof(vmArgs));
	vmArgs.version = JNI_VERSION_1_6;
	vmArgs.nOptions = 1;
	vmArgs.options = jvmOptions;
	hVM = LoadLibrary(DLL_LOCATION);
	if (hVM == NULL)
		throw(DLL_NOT_FOUND);
	typedef jint (CALLBACK *fpCJV)(JavaVM**, void**, JavaVMInitArgs*);
	fpCJV CreateJavaVM = (fpCJV)::GetProcAddress(hVM, JNI_CREATEVM);

	jvmCreated = CreateJavaVM(&jVM, (void**)&jEnv, &vmArgs);
	
	if (jvmCreated == JNI_ERR)
		throw(JVM_LOAD_ERROR);
	else {
		parseClass = jEnv->FindClass(CLASS_TO_LOAD);
		if(parseClass == 0)
			throw(CLASS_NOT_FOUND);
	}
}

natty::~natty() {
	FreeLibrary(hVM);
	jVM->DestroyJavaVM();
}

std::string natty::parseDateTime(std::string commandStr) {
	if(parseClass != 0) {
		jmethodID mid = jEnv->GetStaticMethodID(parseClass, METHOD_TO_CALL, SIGNATURE);
		if(mid != 0){
			jstring data = (jstring)jEnv->CallStaticObjectMethod(parseClass, mid, jEnv->NewStringUTF(commandStr.c_str()));
			if(data != NULL){
				const char *str= jEnv->GetStringUTFChars(data, 0);
				std::string result = std::string(str);
				if(result != std::string())
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