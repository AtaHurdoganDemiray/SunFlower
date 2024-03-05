// RefTriangleSplitter.cpp

#include "stdafx.h"
#include "RefTriangleSplitter.h"
#include "RefTriangle.h"
#include "VTriangle2dE.h"
#include "RefTriangleMesh.h"

bool sf (const RefTriangleSplitter::SS &ss1 
		, const RefTriangleSplitter::SS &ss2)
{
	if (ss1.d < ss2.d)
		return true;
	else
		return false;
}

void RefTriangleSplitter::Split (const std::vector<VPoint2d> &points
								 , std::list<int> &indvec
								 , std::list<int> &shared
								 , const RefTriangle &parent
								 , RefTriangleMesh &result)
{
	std::list <RefTriangle> split;
	split.push_back (parent);
	while (split.size())
	{
		RefTriangle &rt = split.front();
		VTriangle2dE currTri (points[rt.ref0] , points[rt.ref1] , points[rt.ref2] );
		VPoint2d cg = currTri.GetCenterOfMass();
		intitr bi = indvec.begin();
		intitr be = indvec.end();
		std::vector<SS> sortVec;
		for (bi;bi!=be;)
		{
			if (currTri.GetCorner1().Compare(points[*bi],1.e-7)
				||currTri.GetCorner2().Compare(points[*bi],1.e-7)
				||currTri.GetCorner3().Compare(points[*bi],1.e-7))
			{
				bi = indvec.erase(bi);
			}
			else
			{
				SS tmp;
				tmp.i = bi;
				tmp.d = (points[*bi] - cg).GetLength2();
				sortVec.push_back(tmp);
				++bi;
			}
		}
		std::sort (sortVec.begin(),sortVec.end(),sf);
		ssitr sb = sortVec.begin();
		ssitr se = sortVec.end();
		bool intri = false;
		for (sb;sb!=se;++sb)
		{
			const VPoint2d &myp = points[*(sb->i)];
			if (currTri.IsInTriangle(myp))//(currTri.IsInTriangleTol (myp,1.e-7)!=0 )
			{
				intri = true;
				// Split triangle and
				// add new triangles to split list
				VPoint2d dif = myp - currTri.GetCorner1();
				vreal l2 = dif.GetLength2();
				vreal dp1 = dif ^ currTri.GetEdge1();
				if (dp1 * dp1 / l2 > 1.e-7)
				{
					RefTriangle newtri (rt.ref0,rt.ref1,*(sb->i));
					split.push_back (newtri);
				}
				else
					shared.push_back (*(sb->i));

				vreal dp2 = dif ^ currTri.GetEdge2();
				if (dp2 * dp2 / l2 > 1.e-7)
				{
					RefTriangle newtri (rt.ref0 , rt.ref2 , *(sb->i));
					split.push_back (newtri);
				}
				else
					shared.push_back (*(sb->i));

				VPoint2d dif2 = myp - currTri.GetCorner3 ();
				vreal dp3 = dif2 ^ currTri.GetEdge3();
				if (dp3 *dp3  / dif2.GetLength2() > 1.e-7)
				{
					RefTriangle newtri (rt.ref1,rt.ref2,*(sb->i));
					split.push_back (newtri);
				}
				else
					shared.push_back (*(sb->i));

				// pop old triangle from split list
				split.pop_front ();
				// pop point from ind list
				indvec.erase (sb->i);
				break;
			}
		}
		if (intri == false)
		{
			// no point is in triangle , it can not be splitted so record
			result.AddTriangle(rt);
			// pop triangle from split list
			split.pop_front();
		}		
	}
}
