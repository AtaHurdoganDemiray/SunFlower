// SweptCircleLineColl.h

#ifndef __SweptCircleLineColl_h__
#define __SweptCircleLineColl_h__

#include "VPoint2d.h"

class VInterval;

class SweptCircleLineColl
{
public:
	SweptCircleLineColl (const vreal r , const VPoint2d &a , const VPoint2d &b);
	VInterval Coll (const VPoint2d &p0 , const VPoint2d &v);
	static bool IsColliding (const VInterval &ck);
private:
	vreal m_r;
	vreal m_r2;
	VPoint2d A;
	vreal B,C,D,E,F,G,H,I;
	VPoint2d m_a,m_b,vn1;
	void ReplaceIntervalIf (VInterval &i , const vreal aroot);
};

#endif