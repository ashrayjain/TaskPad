#include "Natty.h"

natty::natty() {
	_options[0].optionString = "-Djava.class.path=../Natty;../Natty/java;../Natty/java/lib;../Natty/java/lib/natty-0.7.jar;../Natty/java/lib/ical4j-1.0.2.jar;../Natty/java/lib/antlr-runtime-3.2.jar;../Natty/java/lib/antlr-2.7.7.jar";
	memset(&_vm_args, 0, sizeof(_vm_args));
	_vm_args.version = JNI_VERSION_1_2;
	_vm_args.nOptions = 1;
	_vm_args.options = _options; 
	// this should contain the path to the JVM DLL file
	HINSTANCE hVM = LoadLibrary(L"C:\\Program Files (x86)\\Java\\jre7\\bin\\client\\jvm.dll");//../jre/bin/client/jvm.dll");
	if (hVM == NULL) {
		DWORD dwe = GetLastError();
		throw("Cannot find Java Virtual Machine DLL file");
	}
	typedef jint (CALLBACK *fpCJV)(JavaVM**, void**, JavaVMInitArgs*);
	fpCJV CreateJavaVM = (fpCJV)::GetProcAddress(hVM, "JNI_CreateJavaVM");

	_status = CreateJavaVM(&_jvm, (void**)&_env, &_vm_args);

	if (_status != JNI_ERR) {
		_cls = _env->FindClass("DateParser");
		if(_cls == 0)
			throw("Cannot find Java Class");
	}else{
		throw("Cannot start Java Virtual Machine");
	}
}
