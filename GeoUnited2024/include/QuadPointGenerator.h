// QuadPointGenerator.h

#ifndef __QuadPointGenerator_h__
#define __QuadPointGenerator_h__

#include "VPoint.h"

class QuadPointGenerator 
{
public:
	virtual int QuadSize() const= 0;
	virtual void Get4Corners (int i,VPoint *corners) const= 0;
	virtual void Get4Normals (int i,VPoint *normals) const= 0;
	virtual int GetBoxCornersAndNormals (int i , VPoint *corners , VPoint *normals) const = 0;
};

#endif