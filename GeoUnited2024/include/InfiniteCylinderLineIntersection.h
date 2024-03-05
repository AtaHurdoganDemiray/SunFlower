// InfiniteCylinderLineIntersection.h

#ifndef __InfiniteCylinderLineIntersection_h__
#define __InfiniteCylinderLineIntersection_h__

#include "VLine.h"

class InfiniteCylinderLineIntersection
{
public:
	static int Int(const VPoint &cylcenter, const VPoint &cylax, vreal cylrad , const VLine &l , std::vector<vreal> &intt);
};

#endif