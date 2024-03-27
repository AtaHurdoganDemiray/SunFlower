// SweptCircleLineColl.cpp

#include "stdafx.h"
#include "SweptCircleLineColl.h"
#include "VInterval.h"
#include "QdrEqSolver.h"


SweptCircleLineColl::SweptCircleLineColl(const vreal r
										 , const VPoint2d &a
										 , const VPoint2d &b)
										 :m_r(r),m_a(a),m_b(b)
{
	m_r2 = m_r*m_r;
	vn1 = b - a;
	H = vn1 % vn1;
}

VInterval SweptCircleLineColl::Coll(const VPoint2d &p0, const VPoint2d &v)
{
	VInterval i(-2.0,-1.0);
	vreal roots[2];
	if (v.GetLength2() < 1.e-12)
		LogFileWriter::WriteUnknownExceptionLog (_T("SweptCircleLinColl::Coll warning .Line length too small warning big wrong result  chance"));//return i;		// degenerate line , return no collision

	A = p0 - m_a;
	D = A%A;
	vreal myav = A % v;
	E = 2. * myav;
	F = -2. * (A % vn1);
	G = v % v;
	//H = vn1 % vn1;
	vreal tmp = v % vn1;
	I = -2. * tmp;

	// Check coll at t=0
	int ck1 = QdrEqSolver<vreal>::FindRoots (G ,E,D-m_r2,roots);
	if (ck1)
	{
		VInterval i1 (roots[0],roots[1]);
		std::vector<VInterval> clp = i1.ClipedTo_0_1();
		if (clp.size())
		{
			i.SetT0 (0.0);
		}
	}
	// Check coll at t =1
	int ck2 = QdrEqSolver<vreal>::FindRoots (G,E+I,D+F+H-m_r2,roots);
	if (ck2)
	{
		VInterval i2 (roots[0],roots[1]);
		std::vector<VInterval> clp2 = i2.ClipedTo_0_1();
		if (clp2.size())
		{
			i.SetT1 (1.0);
		}
	}
	// Check if all gauge
	if (i.GetT0() == 0.0 && i.GetT1() == 1.0)
		return i;
	// check line vertex hits
	int ck3 = QdrEqSolver<vreal>::FindRoots (H,F,D-m_r2,roots);
	if (ck3)
	{
		for (int w=0;w<ck3;++w)
			ReplaceIntervalIf (i,roots[w]);
	}

	int ck4 = QdrEqSolver<vreal>::FindRoots (H,(F+I),(E+G+D-m_r2),roots);
	if (ck4)
	{
		for (int w=0;w<ck4;++w)
			ReplaceIntervalIf (i,roots[w]);
	}

	// check line tangent hits
	B = -myav / G;
	C = tmp / G;
	vreal mytanIntA = I*C + G*C*C + H;
	vreal mytanIntB = E*C + F + 2.*G*B*C + I*B;
	vreal mytanIntC = D + E*B + G*B*B - m_r2;
	bool regularCase = true;
	if (fabs(mytanIntA) < EPSILON && fabs(mytanIntB) < EPSILON)
	{
		regularCase = false;
	}
	if (regularCase)
	{
		int ck5 = QdrEqSolver<vreal>::FindRoots(mytanIntA, mytanIntB, mytanIntC, roots);
		if (ck5)
		{
			vreal mys1 = B + C * roots[0];
			if (mys1 <= 1.0 && mys1 >= 0.0)
			{
				ReplaceIntervalIf(i, roots[0]);
			}
			if (ck5 == 2)
			{
				vreal mys2 = B + C * roots[1];
				if (mys2 <= 1.0 && mys2 >= 0.0)
				{
					ReplaceIntervalIf(i, roots[1]);
				}
			}
		}
	}
	else
	{
		// this happens when line is parallel to tool sweep line
		// there are  possibilities
		//  1) line is far from tool sweep line  distance > toolRadius
		int stp = 1;
	}
	return i;
}

void SweptCircleLineColl::ReplaceIntervalIf (VInterval &i , const vreal aroot)
{
	if (aroot>= 0.0 && aroot<=1.0)
	{
		if (fabs(i.GetT0 ()) > aroot)
			i.SetT0(aroot);
		else if (i.GetT1() < aroot)
			i.SetT1 (aroot);
	}
}

bool SweptCircleLineColl::IsColliding(const VInterval &ck)
{
	if (fabs (ck.GetT0()+2) > EPSILON && fabs(ck.GetT1()+1) > EPSILON && ck.GetLength() > EPSILON)
	{
		return true;
	}
	else
	{
		return false;
	}
}
