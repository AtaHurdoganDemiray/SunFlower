// VRand.h

#ifndef __VRand_h__
#define __VRand_h__

class VRand
{
public:
	// Range min and max may be generated
	VRand (int rangeMin , int rangeMax , int seed = 1);
	int GetRandom ();
private:
	int m_rangeMin;
	int m_rangeMax;
	int m_delta;
};

#endif