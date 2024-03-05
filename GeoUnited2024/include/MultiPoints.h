// MultiPoints.h

#ifndef __MultiPoints_h__
#define __MultiPoints_h__

#include "MultiGV.h"

class MultiPoints:public MultiGV <VPoint,Points>
{
public:
	MultiPoints (const std::vector<const Points *> & pointsVec);
	MultiPoints (int totalSize , const std::vector<const Points *> &pointsVec);
	MultiPoints (const MultiPoints &toCopy);
	MultiPoints & operator = (const MultiPoints &toCopy);
	
};

#endif