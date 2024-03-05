// VChronometer.cpp

#include "stdafx.h"
#include "VChronometer.h"

VChronometer::VChronometer()
{
	m_start = clock();
	//m_start = std::chrono::system_clock::now();
}

void VChronometer::ResetStart()
{
	m_start = clock();
}

vreal VChronometer::GetElapsedSeconds()const
{
	return  ((vreal)(m_start - clock())) / CLOCKS_PER_SEC;
}

vreal VChronometer::GetElapsedSeconds(clock_t t0 , clock_t t1)const
{
	return  ((vreal)(t1-t0)) / CLOCKS_PER_SEC;
}

clock_t VChronometer::GetStart()const
{
	return m_start;
}

clock_t VChronometer::Clock()const
{
	return clock();
}