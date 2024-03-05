// PosCubes2dGeoHandling.cpp

#include "stdafx.h"
#include "PosCubes2d.h"
#include "LongVec.h"
#include "VPoint2d.h"
#include "VLine2d.h"
#include "VTriangle2dE.h"
#include "gmQuad2d.h"
//#include "SolidPosCube2d.h"

void PosCubes2d::GetRelatedCubesForLine 
( const VLine2d &myline , LongVec &relcubes ) const
{
	long relmin[2];
	long relmax[2];
	long relcoord[2];
	long cubeindex;

	bool ck = GetRelMinMaxFor2Points 
		( relmin , relmax , myline.GetP1() , myline.GetP2() );

	if ( ck == false )
		return;

	for ( long i1 = relmin[0]; i1 < relmax[0] + 1; i1++ )
	{
		for ( long i2 = relmin[1]; i2 < relmax[1] + 1; i2++ )
		{
			relcoord[0] = i1;
			relcoord[1] = i2;
			cubeindex = GetCubeIndexForRelCoord ( relcoord );
			
			//if ( FineCheckIsLineInCube ( cubeindex , myline ) )
				relcubes.AddLong ( cubeindex );//recording cube index
		}// for all cubes in [relmin , relmax]
	}

	//relcubes.RemoveDuplicate();		
}

//#############################################################################

void PosCubes2d::GetRelatedCubesForTri ( const VTriangle2dE &mytri , LongVec &relcubes ) const
{
	long relmin[2];
	long relmax[2];
	long relcoord[2];
	long cubeindex;

	bool ck = GetRelMinMaxFor2Points 
		( relmin , relmax , mytri.GetMin() , mytri.GetMax() );

	if ( ck == false )
		return;

	for ( long i1 = relmin[0]; i1 < relmax[0] + 1; i1++ )
	{
		for ( long i2 = relmin[1]; i2 < relmax[1] + 1; i2++ )
		{
			relcoord[0] = i1;
			relcoord[1] = i2;
			cubeindex = GetCubeIndexForRelCoord ( relcoord );
			
			//if ( FineCheckIsTriInCube ( cubeindex , mytri ) )
				relcubes.AddLong ( cubeindex );//recording cube index
		}// for all cubes in [relmin , relmax]
	}
	//relcubes.RemoveDuplicate();

}

//#############################################################################

void PosCubes2d::GetRelatedCubesForQuad 
( const gmQuad2d & quad , LongVec &relcubes ) const
{
	long relmin[2];
	long relmax[2];
	long relcoord[2];
	long cubeindex;

	bool ck = GetRelMinMaxFor2Points 
		( relmin , relmax ,  quad.GetCor1() , quad.GetCor3() );

	if ( ck == false )
		return;

	for ( long i1 = relmin[0]; i1 < relmax[0] + 1; i1++ )
	{
		for ( long i2 = relmin[1]; i2 < relmax[1] + 1; i2++ )
		{
			relcoord[0] = i1;
			relcoord[1] = i2;
			cubeindex = GetCubeIndexForRelCoord ( relcoord );
			
			//if ( FineCheckIsQuadInCube ( cubeindex , quad ) )
				relcubes.AddLong ( cubeindex );//recording cube index
		}// for all cubes in [relmin , relmax]
	}
	//relcubes.RemoveDuplicate();

}

//#############################################################################

bool PosCubes2d::FineCheckIsLineInCube  
( const long &cubeindex , const VLine2d &myline ) const
{
	//SolidPosCube2d mysolid ( *this , cubeindex );
	//bool ck = mysolid.IsLineRelated ( myline );
	return true;
}

//#############################################################################

bool PosCubes2d::FineCheckIsTriInCube 
( const long &cubeindex , const VTriangle2dE &mytri ) const
{
	//SolidPosCube2d mysolid ( mcore , cubeindex );
	//bool ck = mysolid.IsTriRelated ( mytri );
	return true;
}

//#############################################################################

void PosCubes2d::RecordPointToRelCubes 
( const VPoint2d &mypoint , const long &index )
{
	long relcoord[2];

	bool ck = GetRelCoordForPoint ( relcoord , mypoint );
		
	if ( ck == false )
		return;

	long cubeindex = GetCubeIndexForRelCoord ( relcoord );
	RecordIndexToCube ( index , cubeindex );

}

//#############################################################################

void PosCubes2d::RecordLineToRelCubes 
							( const VLine2d &myline , const long &index )
{
	long cubeindex;
	LongVec relatedcubes;
	this->GetRelatedCubesForLine ( myline , relatedcubes );

	for ( long i=0; i < relatedcubes.Size(); i++ )
	{
		cubeindex = relatedcubes[i];
		RecordIndexToCube ( index , cubeindex );
	}

}

//#############################################################################

void PosCubes2d::RecordTriToRelCubes 
							( const VTriangle2dE &mytri , const long &index )
{
	long cubeindex;
	LongVec relatedcubes;
	this->GetRelatedCubesForTri ( mytri , relatedcubes );

	for ( long i=0; i < relatedcubes.Size(); i++ )
	{
		cubeindex = relatedcubes[i];
		RecordIndexToCube ( index , cubeindex );
	}
}

//#############################################################################

void PosCubes2d::RecordQuadToRelCubes ( const gmQuad2d &quad , const long & index )
{
	long cubeindex;
	LongVec relatedcubes;
	this->GetRelatedCubesForQuad ( quad , relatedcubes );

	for ( long i=0; i < relatedcubes.Size(); i++ )
	{
		cubeindex = relatedcubes[i];
		RecordIndexToCube ( index , cubeindex );
	}

}

//#############################################################################

void PosCubes2d::SetRelatedIndiceListForPoint 
(  const VPoint2d &mypoint , LongVec &relatedindices
				, const LongVec &not_included_indices ) const
{
	long relcoord[2];
	long relcubeindex;

	GetRelCoordForPoint ( relcoord , mypoint );
	relcubeindex = GetCubeIndexForRelCoord ( relcoord );
	
	RecordCubeContentToList 
		( relatedindices , relcubeindex , not_included_indices );
	relatedindices.RemoveDuplicate();
}

//#############################################################################

void PosCubes2d::SetRelatedIndiceListForBox 
(  const VPoint2d &boxmin , const VPoint2d &boxmax 
 , LongVec &relatedindices , const LongVec &not_included_indices ) const
{
	long relmin[2] , relmax[2];

	GetRelMinMaxFor2Points ( relmin , relmax , boxmin , boxmax );
	
	SetRelatedIndiceListForRelBox 
		( relmin , relmax , relatedindices , not_included_indices );
	
}

//#############################################################################

void PosCubes2d::SetRelatedIndiceListForRelBox
( const long relmin[] , const long relmax[] 
 , LongVec &relatedindices , const LongVec &notincludedindices ) const
{	
	long relcoord[2];
	long cubeindex;

	for ( long i1 = relmin[0]; i1 < relmax[0] + 1; i1++ )
		for ( long i2 = relmin[1]; i2 < relmax[1] + 1; i2++ )
		{
			relcoord[0] = i1;
			relcoord[1] = i2;
			cubeindex = GetCubeIndexForRelCoord ( relcoord );
			
			RecordCubeContentToList 
				( relatedindices , cubeindex , notincludedindices );	
		
		}// for all cubes in [relmin , relmax]
		relatedindices.RemoveDuplicate();
}

//#############################################################################

void PosCubes2d::SetRelatedIndiceListForLine
( const VLine2d &myline , LongVec &relatedindices , const LongVec &notincludedindices ) const
{
	LongVec relatedcubes;
	GetRelatedCubesForLine ( myline , relatedcubes );
	long cubeindex;

	for ( long i=0; i < relatedcubes.Size(); i++ )
	{
		cubeindex = relatedcubes[i];
		RecordCubeContentToList 
			( relatedindices , cubeindex , notincludedindices );
	}
	relatedindices.RemoveDuplicate();
}

//#############################################################################

void PosCubes2d::SetRelatedIndiceListForTri
( const VTriangle2dE &mytri , LongVec &relatedindices , const LongVec &notincludedindices ) const
{
	LongVec relatedcubes;
	GetRelatedCubesForTri ( mytri , relatedcubes );
	long cubeindex;

	for ( long i=0; i < relatedcubes.Size(); i++ )
	{
		cubeindex = relatedcubes[i];
		RecordCubeContentToList 
			( relatedindices , cubeindex , notincludedindices );
	}
	relatedindices.RemoveDuplicate();
}

//#############################################################################

bool PosCubes2d::SetRelatedIndiceListForPointWithAtLeastOneItem 
(  const VPoint2d &mypoint , LongVec &relatedindices
				, const LongVec &not_included_indices ) const
{
	SetRelatedIndiceListForPoint ( mypoint , relatedindices , not_included_indices );

	if ( relatedindices.Size() > 0 )
		return true; // rel list is not empty

	long relmin[2] , relmax[2];
	GetRelCoordForPoint ( relmin , mypoint );
	relmax[0] = relmin[0];
	relmax[1] = relmin[1];

	bool loop = true;

	while ( loop )
	{
		OffsetRelBox ( relmin , relmax );

		SetRelatedIndiceListForRelBox ( relmin , relmax , relatedindices , not_included_indices );
		
		if ( relatedindices.Size() > 0 )
			return true; // rel list is not empty

		if ( IsRelBoxMax( relmin , relmax) )
			loop = false;// all set have been checked , no items in the set!
		
	}

	return false; // no items recorded

}

//#############################################################################

bool PosCubes2d::SetRelatedIndiceListForBoxWithAtLeastOneItem  
(  const VPoint2d &boxmin , const VPoint2d &boxmax 
 , LongVec &relatedindices , const LongVec &not_included_indices ) const
{
	SetRelatedIndiceListForBox ( boxmin , boxmax , relatedindices , not_included_indices );

	if ( relatedindices.Size() > 0 )
		return true; // rel list is not empty

	long relmin[2] , relmax[2];
	GetRelMinMaxFor2Points ( relmin , relmax , boxmin , boxmax );

	bool loop = true;

	while ( loop )
	{
		OffsetRelBox ( relmin , relmax );

		SetRelatedIndiceListForRelBox ( relmin , relmax , relatedindices , not_included_indices );
		
		if ( relatedindices.Size() > 0 )
			return true; // rel list is not empty

		if ( IsRelBoxMax( relmin , relmax) )
			loop = false;// all set have been checked , no items in the set!
		
	}

	return false; // no items recorded

}

//#############################################################################

bool PosCubes2d::SetRelatedIndiceListForLineWithAtLeastOneItem
( const VLine2d &myline , LongVec &relatedindices , const LongVec &notincludedindices ) const
{
	SetRelatedIndiceListForLine ( myline , relatedindices , notincludedindices );

	if ( relatedindices.Size() > 0 )
		return true; // rel list is not empty

	long relmin[2] , relmax[2];
	GetRelMinMaxFor2Points ( relmin , relmax , myline.GetP1() , myline.GetP2() );

	bool loop = true;

	while ( loop )
	{
		OffsetRelBox ( relmin , relmax );

		SetRelatedIndiceListForRelBox ( relmin , relmax , relatedindices , notincludedindices );
		
		if ( relatedindices.Size() > 0 )
			return true; // rel list is not empty

		if ( IsRelBoxMax( relmin , relmax) )
			loop = false;// all set have been checked , no items in the set!
		
	}

	return false; // no items recorded

}

//#############################################################################

bool PosCubes2d::SetRelatedIndiceListForTriWithAtLeastOneItem
( const VTriangle2dE &mytri , LongVec &relatedindices , const LongVec &notincludedindices ) const
{
	SetRelatedIndiceListForTri ( mytri , relatedindices , notincludedindices );

	if ( relatedindices.Size() > 0 )
		return true; // rel list is not empty

	long relmin[2] , relmax[2];
	GetRelMinMaxFor2Points ( relmin , relmax , mytri.GetMin() , mytri.GetMax() );

	bool loop = true;

	while ( loop )
	{
		OffsetRelBox ( relmin , relmax );

		SetRelatedIndiceListForRelBox ( relmin , relmax , relatedindices , notincludedindices );
		
		if ( relatedindices.Size() > 0 )
			return true; // rel list is not empty

		if ( IsRelBoxMax( relmin , relmax) )
			loop = false;// all set have been checked , no items in the set!
		
	}

	return false; // no items recorded

}
//#############################################################################


