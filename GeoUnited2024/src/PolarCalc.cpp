// PolarCalc.cpp

#include "stdafx.h"
#include "PolarCalc.h"
#include "VPoint.h"
#include "CoordinateSystem.h"
#include "PolarCalc2d.h"


vreal PolarCalc::GetAngleBetween (const VPoint &centerp,const VPoint &p1,const VPoint &p2,const VPoint &aroundVec)
{
	VPoint v1=p1-centerp;
	VPoint v2=p2-centerp;
	v1.NormThis();
	v2.NormThis();
	VPoint crossp_vec=v1^v2;
	vreal crossp=crossp_vec.GetLength();
	vreal dotp=v1%v2;
	if (crossp<EPSILON)
	{
		if(dotp<0.0)
			return 180.0;
		else
			return 0.0;
	}
	crossp_vec.NormThis();
	vreal check=aroundVec%crossp_vec;
	if(check<0.0)
	{
		crossp_vec*=-1.;
	}	
	VPoint upVec=crossp_vec^v1;
	CoordinateSystem cs(centerp,v1,upVec,crossp_vec);
	VPoint tP2=cs.AbsoluteCoordToRelativeCoord(p2);
	vreal myAngle= PolarCalc2d::GetPolarAngle(tP2.x(),tP2.y());
	return myAngle;
}

vreal PolarCalc::GetAngleBetween(const VPoint &v1 , const VPoint &v2)
{
	VPoint v1n = v1.Unit();
	double x = v1n % v2;
	double y = sqrt (v2.GetLength2() - x*x);
	vreal angle = PolarCalc2d::GetPolarAngle(x,y);
	return angle;
}
