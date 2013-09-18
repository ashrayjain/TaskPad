#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(ClassNameHere)
	{
	public:
		
		TEST_METHOD(TestMethodNameHere)
		{
			Assert::AreEqual(1,1);//test
			Assert::AreEqual(1,1);//test
		}

	};
}