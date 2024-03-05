// sPoints.h

#ifndef __sPoints_h__
#define __sPoints_h__
#include "Points.h"
struct VPoint;
class PointSet;

class sPoints:public Points
{
public:
	sPoints (const PointSet &ps);
private:
	const PointSet &m_ps;
public:
	virtual const VPoint & operator [] (int i)const;
	virtual int Size()const;
	virtual const VPoint GetMin()const;
	virtual const VPoint GetMax()const;
};

#endif