#include "stdafx.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include "CppUnitTest.h"
#include "../TaskPad_library/Logger.h"
#include "../TaskPad_library/Enum.h"


//using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace TP;

namespace UnitTest
{		
	TEST_CLASS(test_Logger)
	{
	public:
		
		TEST_METHOD(Logger_getLogger)
		{
			Logger* log1 = Logger::getLogger();
			Logger* log2 = Logger::getLogger();

			Microsoft::VisualStudio::CppUnitTestFramework::Assert:: IsTrue(log1 == log2);
		}

		TEST_METHOD(Logger_log)
		{
			stringstream tempStream;
			std::string curTime;

			Logger* logger = Logger::getLogger();
			curTime = getCurTime();
			tempStream <<  "Logging Started at: " << curTime << endl;
			Microsoft::VisualStudio::CppUnitTestFramework::Assert:: IsTrue(compareStreams(tempStream));

			logger->log("tClass0","Info Msg");
			tempStream << "\t\t\t" << "tClass0: Info Msg"<<endl;
			Microsoft::VisualStudio::CppUnitTestFramework::Assert:: IsTrue(compareStreams(tempStream));

			logger->log("tClass1","Notice Msg",NOTICELOG);
			tempStream << "\t\t" << "tClass1: Notice Msg" << endl;
			Microsoft::VisualStudio::CppUnitTestFramework::Assert:: IsTrue(compareStreams(tempStream));

			logger->log("tClass2","Warning Msg",WARNINGLOG);
			tempStream << "\t" << "tClass2: Warning Msg" << endl;
			Microsoft::VisualStudio::CppUnitTestFramework::Assert:: IsTrue(compareStreams(tempStream));

			logger->log("tClass3","Error Msg",ERRORLOG);
			tempStream << "tClass3: Error Msg" << endl;
			Microsoft::VisualStudio::CppUnitTestFramework::Assert:: IsTrue(compareStreams(tempStream));
			
			auto fnptr = [&] {logger->log("tClass4","Fatal Msg",FATALLOG);};
			tempStream << "FATALLOG: tClass4: Fatal Msg" << endl;
			Microsoft::VisualStudio::CppUnitTestFramework::Assert::ExpectException<FatalLogException>(fnptr);
			Microsoft::VisualStudio::CppUnitTestFramework::Assert:: IsTrue(compareStreams(tempStream));

			bool threwException = false;
			std::string exceptionString;
			try
			{
				logger->log("tClass4","Fatal Msg",FATALLOG);
			} catch(FatalLogException fle)
			{
				threwException = true;
				exceptionString = fle.what();
			}

			Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(threwException);
			Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(exceptionString.c_str(), "Fatal log occurred!");

			tempStream << "FATALLOG: tClass4: Fatal Msg" << endl;
			Microsoft::VisualStudio::CppUnitTestFramework::Assert:: IsTrue(compareStreams(tempStream));

		}

		string getCurTime()
		{
			char logTime[40];
			time_t rawTime = time(0);
			tm curTimeTm;
			localtime_s(&curTimeTm, &rawTime);
			strftime(logTime, 40, "%d-%m-%Y",&curTimeTm);

			return logTime;
		}

		bool compareStreams(stringstream& sTemp)
		{
			const std::string LOG_FILE_NAME = "TaskPadLog.txt";
			ifstream logFile(LOG_FILE_NAME);
			stringstream fileBuf;

			fileBuf << logFile.rdbuf();

			// debugging code
			ofstream logFileTemp(LOG_FILE_NAME + "_temp.txt");
			logFileTemp << sTemp.str();
			logFileTemp.close();

			logFile.close();

			return (fileBuf.str() == sTemp.str());
		}
	};
}