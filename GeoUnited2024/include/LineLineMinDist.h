// LineLineMinDist.h

#ifndef __LineLineMinDist_h__
#define __LineLineMinDist_h__
#include "InfiniteLinesNearestPoints.h"

struct VLine;
struct VPoint;

class LineLineMinDist
{
public:
	void Calc (const VLine &l1 
				, const VLine &l2
				, vreal &mindist
				, VLine &minline);
	void ProjectPointToLine (const VPoint &p 
							  , const VLine &l
							  , vreal &mindist
							  , vreal &t);
	static InfiniteLinesNearestPoints calc;
};

#endif
