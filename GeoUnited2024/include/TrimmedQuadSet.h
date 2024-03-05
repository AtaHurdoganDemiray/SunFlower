// TrimmedQuadSet.h

#ifndef __TrimmedQuadSet_h__
#define __TrimmedQuadSet_h__

//#include <vector>
#include "LongVec.h"
#include "gmQuad2d.h"

class gmQuadSet;
class PreTrimmedQuadSet;
class TrimLoop2dSet;
class TriangleMesh2d;
class UntrimmedSurface;
struct VTriangle2dE;
struct VPoint2d;
class PointSet2d;
class TrimQuad;

class TrimmedQuadSet
{
public:
	friend class DigitisedSurface;
	TrimmedQuadSet ( const gmQuadSet & _quads , TrimLoop2dSet & _trimloops );
	~TrimmedQuadSet();
	void SplitQuadWithExtraPoints ( int qno , int extrapointsno 
	, const PreTrimmedQuadSet & prequads , TriangleMesh2d & trimesh );
	TriangleMesh2d * TrimTriMesh ( const TriangleMesh2d &source );
	void RecordExtraTriangles ( int quadno , TriangleMesh2d * extratri );
	void TrimQuadSet ( const PreTrimmedQuadSet & prequads , bool * isquadvalid );
	void TrimSingleQuad ( int qno , const PreTrimmedQuadSet & prequads , bool * isquadvalid );
	void MarkIfTrimmed ( int qno , bool * isquadvalid );
	bool IsPrevQuadNeighbor ( const gmQuad2d & currquad , bool isstartquad );
	void TrimSingleTri ( const VTriangle2dE & tri , TriangleMesh2d & remainingtri );
	bool TrimSingleTri ( const VTriangle2dE & tri );
	void ForceMeshSplitting ( const TriangleMesh2d & source , TriangleMesh2d & result ) const;
	void TrimPoints ( const PointSet2d & firstset , PointSet2d & trimmedset ) const;
	bool IsPointTrimmed ( const VPoint2d & p ) const;
	void MakeTriMesh ( TrimQuad & tq , TriangleMesh2d & result ) const;
	
protected:
	const gmQuadSet & quads;
	TrimLoop2dSet & trimloops;
	//tLongVec extratrianglesinfo;
	std::vector <TriangleMesh2d *>extratriangles;
	//bool * isquadvalid;
	UntrimmedSurface * surf;
	bool isprevquadsplitted;
	bool isprevquadtrimmed;
	int prevquadno;
};

#endif