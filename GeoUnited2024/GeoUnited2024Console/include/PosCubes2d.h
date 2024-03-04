// PosCubes2d.h

#ifndef __PosCubes2d_h__
#define __PosCubes2d_h__

#include "LongVec.h"
#include "VPoint2d.h"

struct VLine2d;
class PointSet2d;
class LineSet2d;
class TriangleMesh2d;
struct VTriangle2dE;
class SequentialLineSet2d;
class gmQuadSet;
class PosCubes2dCore;
struct PosCubes2dGeoHandling;
struct gmQuad2d;
class VRefTriangleMesh;

class PosCubes2d
{
public:
	friend struct NearestNeighborsPointCollector;
	PosCubes2d ( const PointSet2d & mypointset );
	PosCubes2d ( const LineSet2d &mylineset );
	PosCubes2d ( const TriangleMesh2d &newtrimesh );
	PosCubes2d ( const VRefTriangleMesh &refmesh );
	PosCubes2d ( const SequentialLineSet2d & seqlines );
	PosCubes2d ( const gmQuadSet & qset );
	PosCubes2d( const VPoint2d & overallmin , const VPoint2d & overallmax  );

	~PosCubes2d();
	
	void AddItem ( const VPoint2d & newitem , long index );
	void AddItem ( const VLine2d & newitem , long index );
	void AddItem ( const VTriangle2dE & newitem , long index );
	void AddItem ( const gmQuad2d & newitem , long index );
	
private:

	void FillCubes ( const PointSet2d &mypointset );
	void FillCubes ( const LineSet2d &mylineset );
	void FillCubes ( const TriangleMesh2d &newtrimesh );
	void FillCubes ( const VRefTriangleMesh &refmesh );
	void FillCubes ( const SequentialLineSet2d &seqlines );
	void FillCubes ( const gmQuadSet &qset );

	void DecideCubes ( const VPoint2d &min , const VPoint2d &max
							  , const long &setcount , long &nox , long &noy);
private:
	void PosCubes2dCore ( const VPoint2d &newmin , const VPoint2d &newmax 					
						, const long &newnox , const long &newnoy 
						, const long &newsetcount );
	void PosCubes2dCoreDelete();
	void CreateCubes ();
	void CalcCubeLength ();
	bool GetRelCoordForPoint ( long relcoord[] , const VPoint2d &mypoint )const;
	bool GetRelMinMaxFor2Points ( long relmin[] , long relmax[] 
								, const VPoint2d &p1 , const VPoint2d &p2 )const;
	long GetCubeIndexForRelCoord ( long relcoord[] )const;
	bool IsRelCoordValid ( long relcoord[] )const;
	bool IsRelBoxMax ( long boxmin[] , long boxmax[] )const;
	void SetCubeNo ();
	void OffsetRelBox ( long relmin[] , long relmax[] )const;
	void LimitRelCoordToMargins ( long relcoord[] )const;
	//void CreateFlags ();
	//void SetAllFlag ( bool newval )const;
	void RecordIndexToCube ( const long &index , const long &cubeindex );
	void RecordCubeContentToList ( LongVec &relatedindices 
		, const long &cubeindex , const LongVec & not_included_indices )const;
	bool IsNotIncludedIndice ( const long &indice 
								, const LongVec & not_included_indices ) const;
	void GetRelCoordForCubeIndex ( long relcoord[2] , const long &cubeindex ) const;
	void AddItem ();
	vreal cubelengthx , cubelengthy;
	VPoint2d min;
	VPoint2d max;

private:
	LongVec * cubes;
	long cubeno;
	long nox;
	long noy;
	//mutable std::vector<bool> flag;
	long setcount;
private:
	void RecordPointToRelCubes ( const VPoint2d &mypoint , const long &index );
	void RecordLineToRelCubes ( const VLine2d &myline , const long &index );
	void RecordTriToRelCubes ( const VTriangle2dE &mytri , const long &index );
	void RecordQuadToRelCubes ( const gmQuad2d & quad , const long & index );

	void GetRelatedCubesForLine ( const VLine2d &myline , LongVec &relcubes ) const;
	void GetRelatedCubesForTri ( const VTriangle2dE &mytri , LongVec &relcubes ) const;
	void GetRelatedCubesForQuad ( const gmQuad2d & quad , LongVec & relcubes ) const;
	bool FineCheckIsLineInCube ( const long &cubeindex , const VLine2d &myline ) const;
	bool FineCheckIsTriInCube ( const long &cubeindex , const VTriangle2dE &mytri ) const;
public:
	void SetRelatedIndiceListForPoint (  const VPoint2d &mypoint 
		, LongVec &relatedindices , const LongVec &not_included_indices ) const;
	
	void SetRelatedIndiceListForBox (  const VPoint2d &boxmin , const VPoint2d &boxmax 
		, LongVec &relatedindices , const LongVec &not_included_indices ) const;

	void SetRelatedIndiceListForRelBox ( const long relmin[] , const long relmax[] 
		, LongVec &relatedindices , const LongVec &notincludedindices ) const;

	void SetRelatedIndiceListForLine ( const VLine2d &myline 
		, LongVec &relatedindices , const LongVec &notincludedindices ) const;

	void SetRelatedIndiceListForTri ( const VTriangle2dE &mytri 
		, LongVec &relatedindices , const LongVec &notincludedindices ) const;

	bool SetRelatedIndiceListForPointWithAtLeastOneItem
		(  const VPoint2d &mypoint , LongVec &relatedindices 
		, const LongVec &not_included_indices ) const;
	
	bool SetRelatedIndiceListForBoxWithAtLeastOneItem
		(  const VPoint2d &boxmin , const VPoint2d &boxmax 
		, LongVec &relatedindices , const LongVec &not_included_indices ) const;

	bool SetRelatedIndiceListForLineWithAtLeastOneItem
		( const VLine2d &myline , LongVec &relatedindices , const LongVec &notincludedindices ) const;

	bool SetRelatedIndiceListForTriWithAtLeastOneItem
		( const VTriangle2dE &mytri , LongVec &relatedindices , const LongVec &notincludedindices ) const;

};


#endif //__PosCubes2d_h__
