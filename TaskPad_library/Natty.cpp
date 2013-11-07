#include "Natty.h"

natty* natty::_natty = NULL;

natty::natty() {
	_options[0].optionString = "-Djava.class.path=./Natty;./Natty/java;./Natty/java/lib;./Natty/java/lib/natty-0.8.jar;./Natty/java/lib/ical4j-1.0.2.jar;./Natty/java/lib/antlr-runtime-3.2.jar;./Natty/java/lib/antlr-2.7.7.jar";
	memset(&_vm_args, 0, sizeof(_vm_args));
	_vm_args.version = JNI_VERSION_1_6;
	_vm_args.nOptions = 1;
	_vm_args.options = _options; 
	HINSTANCE hVM = LoadLibrary(L"./Natty/bin/client/jvm.dll");
	if (hVM == NULL) {
		DWORD dwe = GetLastError();
		throw("Java Virtual Machine DLL file missing!");
	}
	typedef jint (CALLBACK *fpCJV)(JavaVM**, void**, JavaVMInitArgs*);
	fpCJV CreateJavaVM = (fpCJV)::GetProcAddress(hVM, "JNI_CreateJavaVM");

	_status = CreateJavaVM(&_jvm, (void**)&_env, &_vm_args);
	
	_cls = _env->FindClass("DateParser");
	if(_cls == 0)
		throw("Java Class not found!");
}

natty::~natty() {
	_jvm->DestroyJavaVM();
}

std::string natty::parseDateTime(std::string commandStr) {
	if(_cls != 0) {
		jmethodID mid = _env->GetStaticMethodID(_cls,"parse","(Ljava/lang/String;)Ljava/lang/String;");
		if(mid != 0){
			jstring data = (jstring)_env->CallStaticObjectMethod(_cls,mid,_env->NewStringUTF(commandStr.c_str()));
			const char *str= _env->GetStringUTFChars(data,0);
			std::string result = std::string(str);
			if(result != "")
				commandStr = result;
			_env->ReleaseStringUTFChars(data, str);
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