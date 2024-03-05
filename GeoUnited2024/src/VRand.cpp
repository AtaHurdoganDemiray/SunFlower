// VRand.cpp

#include "stdafx.h"
#include "VRand.h"

VRand::VRand(int rangeMin,int rangeMax,int seed)
:m_rangeMin (rangeMin),m_rangeMax (rangeMax)
{
	m_delta = m_rangeMax - m_rangeMin + 1;
	srand(seed);
}

int VRand::GetRandom()
{
	int v = rand();
	int res = v % m_delta;
	res += m_rangeMin;
	return res;
}
