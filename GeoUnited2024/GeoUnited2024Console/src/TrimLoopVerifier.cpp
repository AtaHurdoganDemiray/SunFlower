// TrimLoopVerifier.cpp

#include "stdafx.h"
#include "TrimLoopVerifier.h"
#include "SequentialLineSet2d.h"
#include "VLine2dE.h"

bool TrimLoopVerifier::IsCCW(const SequentialLineSet2d &l)
{
	int minref = l.GetMinRef();
	minref = minref % (l.GetPointSize()-1);
	VLine2dE myline (l[minref]);
	VPoint2d absOut (-1.0,-1.0);
	VPoint2d myvec (absOut - myline.GetP1());
	vreal dotp = myline.N()%myvec;
	if (dotp > 0.0)
		return false;
	else
		return true;
}

bool TrimLoopVerifier::IsClosed(const SequentialLineSet2d &l,const vreal tol)
{
	return l.Start()->Compare(*l.End(),tol);
}

bool TrimLoopVerifier::IsInUnitSquare(const SequentialLineSet2d &l,const vreal tol)
{
	const VPoint2d mi = l.GetMin();
	const VPoint2d ma = l.GetMax();
	if (mi.x()<-tol || mi.y()<-tol)
		return false;
	if (ma.x()>1.0+tol || ma.y() >1.0+tol)
		return false;
	return true;
}

void TrimLoopVerifier::RemoveDuplicatePoints (const SequentialLineSet2d &sour
											  ,SequentialLineSet2d &dest
											  ,const vreal tol)
{
	int mysize = sour.GetPointSize();
	if (mysize < 2)
	{
		dest = sour;	
		return;
	}
	const VPoint2d *pp = sour.GetPoint (0);
	dest.InsertPoint (*pp);
	for (int i=1;i<mysize;++i)
	{
		const VPoint2d *cp = sour.GetPoint (i);
		if (!cp->Compare (*pp,tol))
			dest.InsertPoint (*cp);
		pp = cp;
	}
}
