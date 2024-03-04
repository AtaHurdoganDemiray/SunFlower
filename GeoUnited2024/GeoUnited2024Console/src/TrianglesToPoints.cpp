// TrianglesToPoints.cpp

#include "stdafx.h"
#include "TrianglesToPoints.h"
#include "Triangles.h"
#include "VTriangleE.h"

void TrianglesToPoints::TriToPoiWithStep(const vreal &step
										 , const Triangles &tri
										 , std::vector<VPoint> &pvec
										 , std::vector<VPoint> &nvec)
{
	int mysize = tri.Size();
	for (int i=0;i<mysize;i++)
	{
		VTriangleE currtri = tri.operator [](i);
		currtri.Digitise (step,pvec,nvec);
	}
}
