// SurfacePolygonTrimmer.cpp

#include "stdafx.h"
#include "SurfacePolygonTrimmer.h"
#include "SequentialLineSet2d.h"
#include "VRefTriangleMesh.h"
#include "VPolygon.h"
#include "UntrimmedSurface.h"
#include "dSurface.h"
#include "VLine2dE.h"
#include "SerializeVPolygon.h"
#include "LoopLineInt.h"

SurfacePolygonTrimmer::SurfacePolygonTrimmer (dSurface &surf
												,SequentialLineSet2d *outerloop
											  ,const std::vector<SequentialLineSet2d *> &innerLoopVec)
											  :m_surf(surf)
												,m_outerLoop (outerloop)
											  ,m_innerLoopVec (innerLoopVec)
{

}

VRefTriangleMesh * SurfacePolygonTrimmer::Trim()
{
	VRefTriangleMesh *res = new VRefTriangleMesh (m_outerLoop->Size()+1);
	/*if (ShouldIReverseOuterLoop (m_outerLoop))
	{
		m_outerLoop->Reverse();
	}*/
	
	int mysizef = m_outerLoop->GetPointSize();
	std::vector<int> polyref;
	for (int i=0; i<mysizef; ++i)
		polyref.push_back (i);
	
	VPolygon mypolygon (m_outerLoop->GetPointSet().GetVec()
						, polyref);
	std::vector<VPoint2d>tmpvec1;
	std::vector<int>tmpvec2;
	VPolygon mypolygon2(tmpvec1,tmpvec2);
	//mypolygon.RemoveCollinear (mypolygon2);
	int mysize = (int)mypolygon.m_pointPool->size();	
	
	for (int i=0;i<mysize;++i)
	{
		const VPoint2d &p =  mypolygon.m_pointPool->operator [] (i);
		VPoint m = m_surf.GetSurface().GetPoint (p.x(),p.y());
		VPoint n (-1);
		res->surfpack.InsertPoint (p,m,n);
	}


	SerializeVPolygon ser;
	ser.WritePolygon (*mypolygon.m_pointPool,mypolygon.m_polygonPoints,( IDS_CSerTxt));

	std::vector <VPolygon> triangles;
	mypolygon.Triangulate (triangles);

	std::vector<VPolygon>::const_iterator i = triangles.begin();
	std::vector<VPolygon>::const_iterator e = triangles.end();
	for (i;i!=e;++i)
	{
		const std::vector<int> &cv = i->m_polygonPoints;
		RefTriangle r (cv[0],cv[1],cv[2]);
		res->AddTriangle (r);
	}

	return res;
}

const vreal LOOP_SMALL_DIST = 0.05;

bool SurfacePolygonTrimmer::ShouldIReverseOuterLoop(const SequentialLineSet2d *sl) const
{
	VLine2d l = sl->operator [] (0);
	VLine2dE tmp (l.GetP1(),l.GetP2());
	VPoint2d innerp = l.GetMidPoint () + tmp.N() * LOOP_SMALL_DIST;
	if (LoopLineInt::IsPointInLoop (innerp , sl , m_surf.cubesOuterLoop) )
		return false;
	else
		return true;
}

bool SurfacePolygonTrimmer::ShouldIReverseInnerLoop(const SequentialLineSet2d *sl,int i) const
{
	VLine2d l = sl->operator [] (0);
	VLine2dE tmp (l.GetP1(),l.GetP2());
	VPoint2d innerp = l.GetMidPoint() - tmp.N() * LOOP_SMALL_DIST;
	if (LoopLineInt::IsPointInLoop (innerp , sl , m_surf.cubesInnerLoopVec[i]))
		return false;
	else
		return true;
}
