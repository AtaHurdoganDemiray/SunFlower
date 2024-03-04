// SurfaceTrimmerDelaunay.h

#ifndef __SurfaceTrimmerDelaunay_h__
#define __SurfaceTrimmerDelaunay_h__

#include "LineIntersection2d.h"

class SurfPointsPack;
struct VPoint2d;
class VRefTriangleMesh;
class PointSet2d;
class IgesTrimLoop2d;
class dSurface;
class UntrimmedSurface;
class SequentialLineSet2d;

class SurfaceTrimmerDelaunay
{
public:
	SurfaceTrimmerDelaunay (UntrimmedSurface &surf
							,const SequentialLineSet2d *outerLoop
							,const std::vector <SequentialLineSet2d *> &innerLoopVec
							,dSurface &dsurf);
	VRefTriangleMesh * TriangulateSurf ();
	void TrimPointsAndDelaunayTriangulate (const UntrimmedSurface & surf);
	void MakeTri (const PointSet2d &parset,const UntrimmedSurface &surf);
	void AddAllTrimLoopsToSurfPack (const UntrimmedSurface &surf);
	void AddTrimLoopToSurfPack (const SequentialLineSet2d &sl 
								, const UntrimmedSurface &surf);
	void PrepareParPoints (int uno,int vno,PointSet2d &parset);
	void AddTrimLoopToSurfPackSimple (const SequentialLineSet2d &sl
									,const UntrimmedSurface &surf);
	VRefTriangleMesh *m_tri;
	//VRefTriangleMesh *m_trimmedTri;
	
private:
	UntrimmedSurface &m_surf;
	const SequentialLineSet2d *m_outerLoop;
	const std::vector<SequentialLineSet2d *> &m_innerLoopVec;
	dSurface &m_dsurf;
};

#endif