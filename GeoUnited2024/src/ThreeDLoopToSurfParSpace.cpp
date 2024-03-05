// ThreeDLoopToSurfParSpace.cpp

#include "stdafx.h"
#include "ThreeDLoopToSurfParSpace.h"
#include "VRefTriangleMesh.h"
#include "SequentialLineSet.h"
#include "PointSet.h"
#include "TriangleMeshPointSetMinDist.h"
#include "Triangles.h"
#include "MultiTriangles.h"
#include "sPoints.h"
#include "MultiPoints.h"

void ThreeDLoopToSurfParSpace::Convert (const VRefTriangleMesh &surf
										,const SequentialLineSet &loop
										,const vreal tolstep
										,PointSet &pset)
{
	// making multi triangles from the surface
	std::vector<const Triangles *> trivec;
	trivec.push_back (&surf);
	MultiTriangles mt (trivec);

	PointSet densp (loop.GetPointSize());
	DigitiseLoop (loop,tolstep,densp);
	sPoints sp (densp);
	
	std::vector<const Points *> pvec;
	pvec.push_back (&sp);	
	MultiPoints mp (pvec);
	TriangleMeshPointSetMinDist calc (mt,mp);
	std::vector<vreal> allmindist;
	std::vector<int> triind;
	std::vector<VPoint>wp;
	calc.MinDistAllTrianglesAndAllPoints (allmindist,triind,wp);	

	int mysize = (int)triind.size();
	const RefTriangleMesh &rm = surf.GetRefTriangleMesh();
	for (int j=0;j<mysize;++j)
	{
		int myind (triind[j]);
		VTriangleE currtri = mt[myind];
		VTriangle2dE currtri2d (*surf.surfpack.parampoints[rm[myind]->ref0]
								,*surf.surfpack.parampoints[rm[myind]->ref1]
								,*surf.surfpack.parampoints[rm[myind]->ref2]);
		vreal u,v;
		VPoint witp = wp [j];
		currtri.GetUV (witp,u,v);
		VPoint2d param = currtri2d.GetCorner1 () + u * currtri2d.GetEdge1() + v * currtri2d.GetEdge2();
		VPoint uv (param.x(),param.y(),0.0);
		pset.InsertPoint (uv);
	}

}

void ThreeDLoopToSurfParSpace::DigitiseLoop(const SequentialLineSet &poly 
											, const vreal mystep
											, PointSet &densp)
{
	// Digitise loop so that it is formed of many points by a step
	vreal oneoverstep = 1. / mystep;
	int mysize0 = poly.Size();
	for (int k=0;k<mysize0;++k)
	{
		VLine l = poly[k];
		vreal ll = l.GetLength();
		int pcount = (int) (ll * oneoverstep);
		for (int w = 0; w <= pcount; ++w)
		{
			vreal myt = w * mystep / ll;
			densp.InsertPoint (l.GetP(myt));
		}
	}
	densp.InsertPoint (poly[mysize0-1].GetP2());
}
