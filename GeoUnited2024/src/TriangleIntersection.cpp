// TriangleIntersection.cpp

#include "stdafx.h"
#include "TriangleIntersection.h"
#include "VLine.h"
#include "VTriangleE.h"
#include "VPlane.h"
#include "InfiniteLinesNearestPoints.h"
#include "VInterval.h"

TriangleIntersection::IntType TriangleIntersection::Int (const VTriangleE &t1 
														, const VTriangleE &t2 
														, bool &intrsct
														, VLine &intL)
{
	VPlane p1 (t1.GetCorner1() , t1.GetNormal());
	VPlane p2 (t2.GetCorner1() , t2.GetNormal());

	VPlane::IntType itype = p1.Int (p2 , intL);
	if (itype == VPlane::COINCIDENT)
		return COINCIDENT;

	if (itype == VPlane::NOINT)
		return NOINT;

	VPoint p = intL.GetP1();
	VVector v = intL.GetV();
	vreal tmp;
	vreal t1s1=0.0,t1s2=0.0,t2s1=0.0,t2s2=0.0;
	vreal *ptr = &t1s1;
	InfiniteLinesNearestPoints calc;
	InfiniteLinesNearestPoints::info ck;
	int counter = 0;
	ck = calc.Calc (p,v,t1.GetCorner1(),t1.GetEdge1(),*ptr,tmp);
	if (ck != InfiniteLinesNearestPoints::LINESPARALLEL && tmp <= 1. && tmp >= 0.)
	{
		ptr = &t1s2;		
		counter++;
	}
	ck = calc.Calc (p,v,t1.GetCorner2(),t1.GetEdge2(),*ptr,tmp);
	if (ck != InfiniteLinesNearestPoints::LINESPARALLEL && tmp <= 1. && tmp >= 0.)
	{
		ptr = &t1s2;
		counter++;
	}
	ck = calc.Calc (p,v,t1.GetCorner3(),t1.GetEdge3(),*ptr,tmp);
	if (ck != InfiniteLinesNearestPoints::LINESPARALLEL && tmp <= 1. && tmp >= 0.)
	{
		counter++;
	}
	//second triangle
	ptr = &t2s1;
	ck = calc.Calc (p,v,t2.GetCorner1(),t2.GetEdge1(),*ptr,tmp);
	if (ck != InfiniteLinesNearestPoints::LINESPARALLEL && tmp <= 1. && tmp >= 0.)
	{
		ptr = &t2s2;
		counter++;
	}
	ck = calc.Calc (p,v,t2.GetCorner2(),t2.GetEdge2(),*ptr,tmp);
	if (ck != InfiniteLinesNearestPoints::LINESPARALLEL && tmp <= 1. && tmp >= 0.)
	{
		ptr = &t2s2;
		counter++;
	}
	ck = calc.Calc (p,v,t2.GetCorner3(),t2.GetEdge3(),*ptr,tmp);
	if (ck != InfiniteLinesNearestPoints::LINESPARALLEL && tmp <= 1. && tmp >= 0.)
	{
		counter++;
	}
	if (counter == 4)
	{
		VInterval t1i,t2i;
		t1i.SetT0(t1s1);
		t1i.SetT1(t1s2);
		t2i.SetT0(t2s1);
		t2i.SetT1(t2s2);
		
		std::vector<VInterval> res = t1i.Intersect (t2i);
		if (res.size() == 1)
		{
			VInterval ti = res.at(0);
			intrsct	= true;
			VPoint i1 = intL.GetP (ti.GetT0());
			VPoint i2 = intL.GetP (ti.GetT1());
			VLine myres (i1,i2);
			intL = myres;
		}
		else
		{
			intrsct = false;
		}
	}
	else
	{
		intrsct = false;
	}
	return REGULAR;
}

bool TriangleIntersection::Int (const VTriangleE &t 
								, const VPlane &p		
								, VLine &intL )
{
	if ( fabs(1.-fabs (t.GetNormal() % p.normal)) < 1.e-9  )
		return false;		// plane and triangle seems to be parallel
	// a quick check and reject 
	//double dotpro1 = (t.GetCorner1() - p.pointonplane) % p.normal;
	//double dotpro2 = (t.GetCorner2() - p.pointonplane) % p.normal;
	//double dotpro3 = (t.GetCorner3() - p.pointonplane) % p.normal;
	//int onPlaneCount = 0;
	//if (fabs(dotpro1) < EPSILON)
	//	++onPlaneCount;
	//if (fabs(dotpro2) < EPSILON)
	//	++onPlaneCount;
	//if (fabs(dotpro3) < EPSILON)
	//	++onPlaneCount;
	////if (onPlaneCount == 1)// plane touch the triangle on one corner , this is not a solid intersection
	////	return false;
	//if (onPlaneCount == 0)// No point is on plane
	//	if ( (dotpro1 * dotpro2) * (dotpro1 * dotpro3) > EPSILON)  // all points are on the same side of plane so no intersection
	//		return false;
	
	int result = 0;
	VPoint lp[3];
	VLine l(t.GetCorner1(),t.GetCorner2());
	vreal s;
	VPlane::IntType ck = p.Int (l,s);
	if (ck == VPlane::REGULAR)
	{
		if (s<(1.0+EPSILON) && s>(-EPSILON))
		{
			lp[result] = l.GetP (s);
			result ++;
		}
	}
	l.SetP2(t.GetCorner3());
	ck = p.Int (l,s);
	if (ck == VPlane::REGULAR)
	{
		if (s<(1.0+EPSILON) && s>(-EPSILON))
		{
			lp[result] = l.GetP (s);
			result ++;
		}
	}
	l.SetP1(t.GetCorner2());
	ck = p.Int (l,s);
	if (ck == VPlane::REGULAR)
	{
		if (s<(1.0+EPSILON) && s>(-EPSILON))
		{
			lp[result] = l.GetP (s);
			result ++;
		}
	}
	intL.SetP1(lp[0]);
	intL.SetP2(lp[1]);
	if (result <= 1)// == 2 or == 3 because plane may cut tri just at corner
		return false;
	else if (result == 2)
		return true;
	else
	{
		// Make sure the points are not coincident
		if ( (intL.GetP1()-intL.GetP2()).GetLength2() <  (intL.GetP1()-lp[2]).GetLength2() )
			intL.SetP2 (lp[2]);
		return true;
	}
}

bool TriangleIntersection::Int (const VTriangleE &t , const VLine &l , VPoint &intp)
{
	VPlane pln = t.GetPlane();
	vreal intt;
	VPlane::IntType itype = pln.Int (l,intt);
	if (itype == REGULAR)
	{
		intp = l.GetP(intt);
		if (t.IsInTriangle(intp))
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

bool TriangleIntersection::IsTriCollidingToBox(const VTriangleE &tri , const VPoint &boxmin, const VPoint &boxmax)
{
	VPoint boxmin2(boxmin);
	boxmin2 -= VPoint(EPSILON*100.);
	VPoint boxmax2(boxmax);
	boxmax2 += VPoint(EPSILON*100.);

	const VPoint &c0 = tri.GetCorner1();
	const VPoint &c1 = tri.GetCorner2();
	const VPoint &c2 = tri.GetCorner3();
	// try to prove not colliding
	if (c0.x() < boxmin2.x() && c1.x() < boxmin2.x() && c2.x() < boxmin2.x())
		return false;
	if (c0.y() < boxmin2.y() && c1.y() < boxmin2.y() && c2.y() < boxmin2.y())
		return false;
	if (c0.z() < boxmin2.z() && c1.z() < boxmin2.z() && c2.z() < boxmin2.z())
		return false;

	if (c0.x() > boxmax2.x() && c1.x() > boxmax2.x() && c2.x() > boxmax2.x())
		return false;
	if (c0.y() > boxmax2.y() && c1.y() > boxmax2.y() && c2.y() > boxmax2.y())
		return false;
	if (c0.z() > boxmax2.z() && c1.z() > boxmax2.z() && c2.z() > boxmax2.z())
		return false;

	// try to prove colliding
	if (c0.x() >= boxmin2.x() && c0.x() <= boxmax2.x() && c0.y() >= boxmin2.y() && c0.y() <= boxmax2.y() && c0.z() >= boxmin2.z() && c0.z() <= boxmax2.z())
		return true;

	if (c1.x() >= boxmin2.x() && c1.x() <= boxmax2.x() && c1.y() >= boxmin2.y() && c1.y() <= boxmax2.y() && c1.z() >= boxmin2.z() && c1.z() <= boxmax2.z())
		return true;

	if (c2.x() >= boxmin2.x() && c2.x() <= boxmax2.x() && c2.y() >= boxmin2.y() && c2.y() <= boxmax2.y() && c2.z() >= boxmin2.z() && c2.z() <= boxmax2.z())
		return true;

	std::vector<VTriangleE> boxtri;
	TriangulateBox(boxmin, boxmax, boxtri);
	int boxtrisize = (int)boxtri.size();
	bool bflg;
	VLine intl;
	for (int i = 0; i < boxtrisize; ++i)
	{
		const VTriangleE &curboxtri = boxtri.at(i);
		IntType ck = Int(tri, curboxtri, bflg, intl);
		if (ck != NOINT)
		{
			if (ck == COINCIDENT)
				return true;
			if (ck == REGULAR && bflg)
				return true;
		}
	}
	return false;
}

void TriangleIntersection::TriangulateBox(const VPoint &mn, const VPoint &mx, std::vector<VTriangleE> &tri)
{

	VPoint p[8] = { VPoint(mn)
		, VPoint(mx.x(), mn.y(), mn.z())
		, VPoint(mx.x(), mx.y(), mn.z())
		, VPoint(mn.x(), mx.y(), mn.z())
		, VPoint(mn.x(), mn.y(), mx.z())
		, VPoint(mx.x(), mn.y(), mx.z())
		, VPoint(mx)
		, VPoint(mn.x(), mx.y(), mx.z())
	};
	//base 0 1 2 3
	VTriangleE base1(p[0], p[1], p[2]);
	VTriangleE base2(p[2], p[3], p[0]);
	// top 4 5 6 7
	VTriangleE top1(p[4], p[5], p[6]);
	VTriangleE top2(p[6], p[7], p[4]);
	// front 0 1 5 4
	VTriangleE front1(p[0], p[1], p[5]);
	VTriangleE front2(p[5], p[4], p[0]);
	// back 2 3 7 6
	VTriangleE back1(p[2], p[3], p[7]);
	VTriangleE back2(p[7], p[6], p[2]);
	// left 3 0 4 7
	VTriangleE left1(p[3], p[0], p[4]);
	VTriangleE left2(p[4], p[7], p[3]);
	// right 1 2 6 5
	VTriangleE right1(p[1], p[2], p[6]);
	VTriangleE right2(p[6], p[5], p[1]);

	tri.push_back(base1);
	tri.push_back(base2);
	tri.push_back(top1);
	tri.push_back(top2);
	tri.push_back(front1);
	tri.push_back(front2);
	tri.push_back(back1);
	tri.push_back(back2);
	tri.push_back(left1);
	tri.push_back(left2);
	tri.push_back(right1);
	tri.push_back(right2);
}