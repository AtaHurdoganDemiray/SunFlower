// PolarCalc.h

#ifndef __PolarCalc_h__
#define __PolarCalc_h__


struct VPoint;

class PolarCalc
{
public:
	static vreal GetAngleBetween (const VPoint &centerp,const VPoint &p1,const VPoint &p2,const VPoint &aroundVec);
	static vreal GetAngleBetween(const VPoint &v1 , const VPoint &v2);

};

#endif