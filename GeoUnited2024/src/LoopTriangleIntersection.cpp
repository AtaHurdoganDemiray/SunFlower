// LoopTriangleIntersection.cpp

#include "stdafx.h"
#include "LoopTriangleIntersection.h"
#include "VLine2d.h"
#include "SequentialLineSet2d.h"
#include "PosCubes2d.h"
#include "LineSet2d.h"

bool LoopTriangleIntersection::IsIntersecting (const VLine2d &l
											   ,const SequentialLineSet2d &loop
											   ,const PosCubes2d &cubes
											   ,LineSet2d &intLines)
{
	bool ck = false;
	LongVec relind,nii;
	cubes.SetRelatedIndiceListForLine (l,relind,nii);
	int mysize = relind.Size();
	for (int i=0;i<mysize;i++)
	{
		int currind = relind[i];
		VLine2d currl = loop[currind];
		VPoint2d currlnormal = currl.GetNormal();
		VPoint2d lnormal = l.GetNormal();
		vreal s1 = (l.GetP1() - currl.GetP1()) % currlnormal;
		vreal s2 = (l.GetP2() - currl.GetP1()) % currlnormal;
		vreal t1 = (currl.GetP1() - l.GetP1()) % lnormal;
		vreal t2 = (currl.GetP2() - l.GetP1()) % lnormal;
		if (s1 * s2 < 0.0 && t1 * t2 < 0.0)
		{
			ck = true;
			intLines.AddLine (currl);
		}

	}
	return ck;
}

void LoopTriangleIntersection::SplitTriangle(RefTriangle &tri
											 , VRefTriangleMesh &mesh
											 , VLine2d &l
											 , std::vector<RefTriangle> &splitRes) 
{
	
}
