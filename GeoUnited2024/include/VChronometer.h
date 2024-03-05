// VChronometer.h

#ifndef __VChronometer_h__
#define __VChronometer_h__

//#include <chrono>
//#include <VTime>

class VChronometer
{
public:
	VChronometer();
	void ResetStart();
	vreal GetElapsedSeconds()const;
	clock_t Clock()const;
	clock_t GetStart()const;
	vreal GetElapsedSeconds (clock_t t0 , clock_t t1)const;
private:
	clock_t m_start;
	//std::chrono::time_point<std::chrono::system_clock> m_start;
};

#endif