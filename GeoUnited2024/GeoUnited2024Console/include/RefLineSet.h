// RefLineSet.h

#ifndef __RefLineSet_h__
#define __RefLineSet_h__
#include "SurfPointsPack.h"

class RefLineSet
{
public:
	RefLineSet (int initialcapacity);
	void AddLine (const RefLine &newline);
	const VLine operator [] (int i)const;
	int Size()const;
	
	SurfPointsPack surfpack;	
	std::vector <RefLine> m_reflines;
};

#endif