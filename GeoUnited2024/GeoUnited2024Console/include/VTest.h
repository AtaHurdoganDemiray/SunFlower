// VTest.h

#ifndef __VTest_h__
#define __VTest_h__

class VTest
{
public:
	virtual void StartTest(const VStr &testName) = 0;
	static void Register(const VStr &uniqueTestName, VTest *tester);
	static std::map<VStr, VTest *> m_allTests;
	static void Trigger(const VStr &testName);
};

#endif