// PosCubes.h

#ifndef __PosCubes_h__
#define __PosCubes_h__

#include "LongVec.h"
#include "VPoint.h"

struct VLine;
class PointSet;
class LineSet;
class TriangleMesh;
class Points;
class Lines;
class Triangles;

struct VTriangleE;
class PosCubesCore;
struct PosCubesGeoHandling;
class MultiTriangles;
class MultiLines;
class MultiPoints;
class SequentialLineSet;
class Triangles;

class PosCubes
{
public:
	friend struct NearestNeighborsPointCollector;
	friend class PosCubesVisualiser;
	PosCubes ( const PointSet & mypointset );
	PosCubes ( const LineSet &mylineset );
	PosCubes ( const SequentialLineSet &mylineset );
	PosCubes ( const TriangleMesh &newtrimesh );
	PosCubes ( const MultiTriangles &newtriangles );
	PosCubes ( const MultiLines &newlines );
	PosCubes ( const MultiPoints &newpoints );
	PosCubes ( const VPoint & overallmin , const VPoint & overallmax  );
	PosCubes (const MultiTriangles &tri , const vreal &xyGridLength);
	PosCubes (const Triangles &tri);
	virtual ~PosCubes();
	
	void AddItem ( const VPoint & newitem , long index );
	void AddItem ( const VLine & newitem , long index );
	void AddItem ( const VTriangleE & newitem , long index );
	
	void SetRelatedIndiceListForPoint ( const VPoint &mypoint 
					, LongVec &relatedindices , const LongVec &not_included_indices )const;	
	
	void SetRelatedIndiceListForBox ( const VPoint &boxmin , const VPoint &boxmax 
					, LongVec &relatedindices , const LongVec &not_included_indices )const;
	
	void SetRelatedIndiceListForLine ( const VLine &myline 
					, LongVec &relatedindices , const LongVec &notincludedindices ) const;
	
	void SetRelatedIndiceListForTri  ( const VTriangleE &mytri
										, LongVec &relatedindices
										, const LongVec &not_included_indices ) const;

	void GetRelatedIndicesForMinDist ( const VPoint &mypoint 
									, LongVec &relatedindices 
									, VPoint &cubeCenter
									, int offsetDepth =1)const;
	//const PosCubesCore & GetCore ()const;
private:

	void FillCubes ( const PointSet &mypointset );
	void FillCubes ( const LineSet &mylineset );
	void FillCubes ( const SequentialLineSet &mylineset );
	void FillCubes ( const TriangleMesh &newtrimesh );
	void FillCubes ( const Triangles &tri);
	void FillCubes ( const MultiTriangles &newtriangles);
	void FillCubes (const MultiLines &newlines);
	void FillCubes (const MultiPoints &newpoints);
	void DecideCubes ( const VPoint &min , const VPoint &max
							  , const long &setcount , long &nox , long &noy , long &noz);
	void DecideCubesXY ( const VPoint &min , const VPoint &max , const vreal &xystep
						, long &nox , long &noy);
	void GetAllIndices ( LongVec &relatedindices ) const;

private:
	friend class PosCubesVisualiser;

	void PosCubesCore ( const VPoint &newmin , const VPoint &newmax 					
					, const long &newnox , const long &newnoy , const long &newnoz
					, const long &newsetcount );
	void PosCubesCoreDelete();
	void CreateCubes ();
	void CalcCubeLength ();
	bool GetRelCoordForPoint ( long relcoord[] , const VPoint &mypoint )const;
	bool GetRelMinMaxFor2Points ( long relmin[] , long relmax[] 
								, const VPoint &p1 , const VPoint &p2 )const;
	long GetCubeIndexForRelCoord ( long relcoord[] )const;
	bool IsRelCoordValid ( long relcoord[] )const;
	bool IsRelBoxMax ( long boxmin[] , long boxmax[] )const;
	void SetCubeNo ();
	void OffsetRelBox ( long relmin[] , long relmax[] )const;
	void LimitRelCoordToMargins ( long relcoord[] )const;
	void CreateFlags ();
	void SetAllFlag ( bool newval )const;
	void RecordIndexToCube ( const long &index , const long &cubeindex );
	void RecordCubeContentToList ( LongVec &relatedindices 
		, const long &cubeindex , const LongVec & not_included_indices )const;
	bool IsNotIncludedIndice ( const long &indice 
								, const LongVec & not_included_indices ) const;
	void GetRelCoordForCubeIndex ( long relcoord[] , const long &cubeindex ) const;
	void AddItem ();
public:
	VPoint GetCenterPointOfCube ( const long &cubeindex ) const;
	bool IsCubeEmpty(const long &cubeindex) const;
	int GetCubeCount()const;
	const LongVec & GetCube(int i)const; 
private:	
	VPoint min;
	VPoint max;
	vreal cubelengthx , cubelengthy , cubelengthz;
	long nox;
	long noy;
	long noz;
	LongVec * cubes;
	long cubeno;
	mutable std::vector<bool> flag;
	long setcount;

private:
	void RecordPointToRelCubes ( const VPoint &mypoint , const long &index );
	void RecordLineToRelCubes ( const VLine &myline , const long &index );
	void RecordTriToRelCubes ( const VTriangleE &mytri , const long &index );

	void GetRelatedCubesForLine ( const VLine &myline , LongVec &relcubes ) const;
	void GetRelatedCubesForTri ( const VTriangleE &mytri , LongVec &relcubes ) const;

	void SetRelatedIndiceListForRelBox ( const long relmin[] , const long relmax[] 
		, LongVec &relatedindices , const LongVec &notincludedindices ) const;
	bool FineCheckIsLineInCube ( const long &cubeindex , const VLine &myline ) const;
	bool FineCheckIsTriInCube ( const long &cubeindex , const VTriangleE &mytri ) const;
	long GetNearestCube (const VPoint &mypoint,VPoint &cubeCenter) const;
};

#endif //__PosCubes_h__
