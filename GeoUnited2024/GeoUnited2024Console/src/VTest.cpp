// VTest.cpp

#include "stdafx.h"
#include "VTest.h"

std::map<VStr,VTest *> VTest::m_allTests = std::map<VStr, VTest *>();

void VTest::Register(const VStr &uniqueTestName, VTest * tester)
{
	m_allTests[uniqueTestName] = tester;
}

void VTest::Trigger(const VStr &testName)
{
	std::map<VStr, VTest *>::iterator ti = m_allTests.begin();
	std::map<VStr, VTest *>::iterator te = m_allTests.end();
	for (ti; ti != te; ++ti)
	{
		if (ti->first == testName)
			ti->second->StartTest(ti->first);
	}
}
