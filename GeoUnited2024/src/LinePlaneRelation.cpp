// LinePlaneRelation.cpp

#include "stdafx.h"
#include "LinePlaneRelation.h"
#include "VLine.h"
#include "VPlane.h"

LinePlaneRelation::Type LinePlaneRelation::Clasify(const VLine &l, const VPlane &p , VLine &subpos , VLine &subneg)
{
	Type res;
	VVector diff1 = l.GetP1() - p.pointonplane;
	VVector diff2 = l.GetP2() - p.pointonplane;
	vreal dp1 = diff1 % p.normal;
	vreal dp2 = diff2 % p.normal;
	if (fabs(dp1) < EPSILON && fabs(dp2) < EPSILON)
	{
		res = COINCIDENT;
		return res;
	}
	if (dp1 >= 0.0 && dp2 >= 0.0)
	{
		res = POSITIVE;
		return res;
	}
	if (dp1 <= 0.0 && dp2 <= 0.0)
	{
		res = NEGATIVE;
		return res;
	}
	res = INTERSECTING;
	vreal t;
	VPlane::IntType ck = p.Int (l,t);
	VPoint planeInt = l.GetP (t);
	if (dp1 < 0)
	{
		subneg.SetP1 (l.GetP1());
		subneg.SetP2 (planeInt);
		subpos.SetP1 (planeInt);
		subpos.SetP2 (l.GetP2());
	}
	else if (dp2 < 0)
	{
		subneg.SetP1 (l.GetP2());
		subneg.SetP2 (planeInt);
		subpos.SetP1 (planeInt);
		subpos.SetP2 (l.GetP1());
	}
	return res;
}
