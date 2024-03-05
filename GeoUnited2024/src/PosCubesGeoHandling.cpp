// PosCubesGeoHandling.cpp

#include "stdafx.h"
#include "PosCubes.h"
//#include "PosCubesCore.h"
#include "LongVec.h"
#include "VPoint.h"
#include "VLine.h"
#include "VTriangleE.h"
//#include "SolidPosCube.h"

//PosCubesGeoHandling::PosCubesGeoHandling ( PosCubesCore &newcore )
//: mcore ( newcore )
//{
//
//}

//#############################################################################

//PosCubesGeoHandling::~PosCubesGeoHandling()
//{
//
//}

//#############################################################################

void PosCubes::GetRelatedCubesForLine 
( const VLine &myline , LongVec &relcubes ) const
{
	long relmin[3];
	long relmax[3];
	long relcoord[3];
	long cubeindex;

	bool ck = GetRelMinMaxFor2Points 
		( relmin , relmax , myline.GetP1() , myline.GetP2() );

	if ( ck == false )
		return;

	for ( long i1 = relmin[0]; i1 < relmax[0] + 1; i1++ )
		for ( long i2 = relmin[1]; i2 < relmax[1] + 1; i2++ )
			for ( long i3 = relmin[2]; i3 < relmax[2] + 1; i3++ )
			{
				relcoord[0] = i1;
				relcoord[1] = i2;
				relcoord[2] = i3;
				cubeindex = GetCubeIndexForRelCoord ( relcoord );
			
				//if ( FineCheckIsLineInCube ( cubeindex , myline ) )
				relcubes.AddLong ( cubeindex );//recording cube index
			}// for all cubes in [relmin , relmax]
}

//#############################################################################

void PosCubes::GetRelatedCubesForTri ( const VTriangleE &mytri , LongVec &relcubes ) const
{
	long relmin[3];
	long relmax[3];
	long relcoord[3];
	long cubeindex;

	bool ck = GetRelMinMaxFor2Points 
		( relmin , relmax , mytri.GetMin() , mytri.GetMax() );

	if ( ck == false )
		return;

	for ( long i1 = relmin[0]; i1 < relmax[0] + 1; i1++ )
		for ( long i2 = relmin[1]; i2 < relmax[1] + 1; i2++ )
			for ( long i3 = relmin[2]; i3 < relmax[2] + 1; i3++ )
			{
				relcoord[0] = i1;
				relcoord[1] = i2;
				relcoord[2] = i3;
				cubeindex = GetCubeIndexForRelCoord ( relcoord );
			
				//if ( FineCheckIsTriInCube ( cubeindex , mytri ) )
				relcubes.AddLong ( cubeindex );//recording cube index
			}// for all cubes in [relmin , relmax]
}

//#############################################################################

bool PosCubes::FineCheckIsLineInCube  
( const long &cubeindex , const VLine &myline ) const
{
	return true;
	//SolidPosCube mysolid ( mcore , cubeindex );
	//bool ck = mysolid.IsLineRelated ( myline );
	//return ck;
}

//#############################################################################

bool PosCubes::FineCheckIsTriInCube 
( const long &cubeindex , const VTriangleE &mytri ) const
{
	return true;
	//SolidPosCube mysolid ( mcore , cubeindex );
	//bool ck = mysolid.IsTriRelated ( mytri );
	//return ck;
}

//#############################################################################

void PosCubes::RecordPointToRelCubes 
( const VPoint &mypoint , const long &index )
{
	long relcoord[3];

	bool ck = GetRelCoordForPoint ( relcoord , mypoint );
		
	if ( ck == false )
		return;

	long cubeindex = GetCubeIndexForRelCoord ( relcoord );
	RecordIndexToCube ( index , cubeindex );

}

//#############################################################################

void PosCubes::RecordLineToRelCubes 
							( const VLine &myline , const long &index )
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

void PosCubes::RecordTriToRelCubes 
							( const VTriangleE &mytri , const long &index )
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

void PosCubes::SetRelatedIndiceListForPoint 
(  const VPoint &mypoint , LongVec &relatedindices
				, const LongVec &not_included_indices ) const
{
	SetAllFlag ( false );

	long relcoord[3];
	long relcubeindex;

	GetRelCoordForPoint ( relcoord , mypoint );
	relcubeindex = GetCubeIndexForRelCoord ( relcoord );
	
	RecordCubeContentToList 
		( relatedindices , relcubeindex , not_included_indices );

}

//#############################################################################

void PosCubes::SetRelatedIndiceListForBox 
(  const VPoint &boxmin , const VPoint &boxmax 
 , LongVec &relatedindices , const LongVec &not_included_indices ) const
{
	SetAllFlag ( false );
	
	long relmin[3] , relmax[3];

	GetRelMinMaxFor2Points ( relmin , relmax , boxmin , boxmax );
	
	SetRelatedIndiceListForRelBox 
		( relmin , relmax , relatedindices , not_included_indices );
	
}

//#############################################################################

void PosCubes::SetRelatedIndiceListForRelBox
( const long relmin[] , const long relmax[] 
 , LongVec &relatedindices , const LongVec &notincludedindices ) const
{	
	long relcoord[3];
	long cubeindex;

	for ( long i1 = relmin[0]; i1 < relmax[0] + 1; i1++ )
		for ( long i2 = relmin[1]; i2 < relmax[1] + 1; i2++ )
			for ( long i3 = relmin[2]; i3 < relmax[2] + 1; i3++ )	
		{
			relcoord[0] = i1;
			relcoord[1] = i2;
			relcoord[2] = i3;

			cubeindex = GetCubeIndexForRelCoord ( relcoord );
			
			RecordCubeContentToList 
				( relatedindices , cubeindex , notincludedindices );	
		
		}// for all cubes in [relmin , relmax]

}

//#############################################################################

void PosCubes::SetRelatedIndiceListForLine
( const VLine &myline , LongVec &relatedindices , const LongVec &notincludedindices ) const
{
	SetAllFlag ( false );

	LongVec relatedcubes;
	GetRelatedCubesForLine ( myline , relatedcubes );
	long cubeindex;

	for ( long i=0; i < relatedcubes.Size(); i++ )
	{
		cubeindex = relatedcubes[i];
		RecordCubeContentToList 
			( relatedindices , cubeindex , notincludedindices );
	}

}

//#############################################################################

void PosCubes::SetRelatedIndiceListForTri
( const VTriangleE &mytri , LongVec &relatedindices , const LongVec &notincludedindices ) const
{
	SetAllFlag ( false );

	LongVec relatedcubes;
	GetRelatedCubesForTri ( mytri , relatedcubes );
	long cubeindex;

	for ( long i=0; i < relatedcubes.Size(); i++ )
	{
		cubeindex = relatedcubes[i];
		RecordCubeContentToList 
			( relatedindices , cubeindex , notincludedindices );
	}

}

//#############################################################################
/*
bool PosCubesGeoHandling::SetRelatedIndiceListForPointWithAtLeastOneItem 
(  const VPoint &mypoint , LongVec &relatedindices
				, const LongVec &not_included_indices ) const
{
	SetRelatedIndiceListForPoint ( mypoint , relatedindices , not_included_indices );

	if ( relatedindices.Size() > 0 )
		return true; // rel list is not empty

	long relmin[3] , relmax[3];
	mcore.GetRelCoordForPoint ( relmin , mypoint );
	relmax[0] = relmin[0];
	relmax[1] = relmin[1];
	relmax[2] = relmin[2];

	bool loop = true;
	bool returnval = false;
	bool done = false;

	while ( loop )
	{
		mcore.OffsetRelBox ( relmin , relmax );

		SetRelatedIndiceListForRelBox ( relmin , relmax , relatedindices , not_included_indices );
		
		if ( done )
		{
			returnval = true; // rel list is not empty
			loop = false;
		}
		// one more offset is done after items added to list
		if ( relatedindices.Size() > 0 )
			done = true;

		if ( mcore.IsRelBoxMax( relmin , relmax) )
		{
			loop = false;// all set have been checked , no items in the set!
			returnval = false;
		}
	}

	return returnval; 

}

//#############################################################################

bool PosCubesGeoHandling::SetRelatedIndiceListForBoxWithAtLeastOneItem  
(  const VPoint &boxmin , const VPoint &boxmax 
 , LongVec &relatedindices , const LongVec &not_included_indices ) const
{
	SetRelatedIndiceListForBox ( boxmin , boxmax , relatedindices , not_included_indices );

	if ( relatedindices.Size() > 0 )
		return true; // rel list is not empty

	long relmin[3] , relmax[3];
	mcore.GetRelMinMaxFor2Points ( relmin , relmax , boxmin , boxmax );

	bool loop = true;
	bool returnval = false;
	bool done = false;

	while ( loop )
	{
		mcore.OffsetRelBox ( relmin , relmax );

		SetRelatedIndiceListForRelBox ( relmin , relmax , relatedindices , not_included_indices );
		
		if ( done )
		{
			returnval = true; // rel list is not empty
			loop = false;
		}
		// one more offset is done after items added to list
		if ( relatedindices.Size() > 0 )
			done = true;

		if ( mcore.IsRelBoxMax( relmin , relmax) )
		{
			loop = false;// all set have been checked , no items in the set!
			returnval = false;
		}
	}

	return returnval;

}

//#############################################################################

bool PosCubesGeoHandling::SetRelatedIndiceListForLineWithAtLeastOneItem
( const VLine &myline , LongVec &relatedindices , const LongVec &notincludedindices ) const
{
	SetRelatedIndiceListForLine ( myline , relatedindices , notincludedindices );

	if ( relatedindices.Size() > 0 )
		return true; // rel list is not empty

	long relmin[3] , relmax[3];
	mcore.GetRelMinMaxFor2Points ( relmin , relmax , myline.GetP1() , myline.GetP2() );

	bool loop = true;
	bool returnval = false;
	bool done = false;

	while ( loop )
	{
		mcore.OffsetRelBox ( relmin , relmax );

		SetRelatedIndiceListForRelBox ( relmin , relmax , relatedindices , notincludedindices );
		
		if ( done )
		{
			returnval = true; // rel list is not empty
			loop = false;
		}
		// one more offset is done after items added to list
		if ( relatedindices.Size() > 0 )
			done = true;

		if ( mcore.IsRelBoxMax( relmin , relmax) )
		{
			loop = false;// all set have been checked , no items in the set!
			returnval = false;
		}
	}

	return returnval;

}

//#############################################################################

bool PosCubesGeoHandling::SetRelatedIndiceListForTriWithAtLeastOneItem
( const VTriangleE &mytri , LongVec &relatedindices , const LongVec &notincludedindices ) const
{
	SetRelatedIndiceListForTri ( mytri , relatedindices , notincludedindices );

	if ( relatedindices.Size() > 0 )
		return true; // rel list is not empty

	long relmin[3] , relmax[3];
	mcore.GetRelMinMaxFor2Points ( relmin , relmax , mytri.GetMin() , mytri.GetMax() );

	bool loop = true;
	bool returnval = false;
	bool done = false;

	while ( loop )
	{
		mcore.OffsetRelBox ( relmin , relmax );

		SetRelatedIndiceListForRelBox ( relmin , relmax , relatedindices , notincludedindices );
		
		if ( done )
		{
			returnval = true; // rel list is not empty
			loop = false;
		}
		// one more offset is done after items added to list
		if ( relatedindices.Size() > 0 )
			done = true;

		if ( mcore.IsRelBoxMax( relmin , relmax) )
		{
			loop = false;// all set have been checked , no items in the set!
			returnval = false;
		}
	}

	return returnval;

}
//#############################################################################
*/

void PosCubes::GetRelatedIndicesForMinDist ( const VPoint &mypoint , LongVec &relatedindices , VPoint &cubeCenter , int offsetDepth)const
{
	long nearestCubeIndex =GetNearestCube ( mypoint , cubeCenter );
	long relcoord[3];
	GetRelCoordForCubeIndex (relcoord,nearestCubeIndex); 
	long relmin[3] = {relcoord[0],relcoord[1],relcoord[2]};
	long relmax[3] = {relcoord[0],relcoord[1],relcoord[2]};
	
	for (int k=0;k<offsetDepth;k++)
	{
		OffsetRelBox(relmin,relmax);
	}
	
	long relc[3];
	long cubeindex;
	LongVec not_included_indices;

	SetAllFlag(false);
	for ( long i1 = relmin[0]; i1 < relmax[0] + 1; i1++ )
		for ( long i2 = relmin[1]; i2 < relmax[1] + 1; i2++ )
			for ( long i3 = relmin[2]; i3 < relmax[2] + 1; i3++ )
			{
				relc[0] = i1;
				relc[1] = i2;
				relc[2] = i3;
				cubeindex = GetCubeIndexForRelCoord ( relc );	
				RecordCubeContentToList (relatedindices,cubeindex,not_included_indices);
			}
}

//#############################################################################

long PosCubes::GetNearestCube (const VPoint &mypoint,VPoint &cubeCenter) const
{
	//if (mypoint>=mcore.min && mypoint<=mcore.max)
	{
		long relcoord[3];
		GetRelCoordForPoint(relcoord,mypoint);
		long myindex=GetCubeIndexForRelCoord(relcoord);
		if(IsCubeEmpty(myindex) == false)
			return myindex;
	}

	long relcoord[3];
	long cubeindex;
	vreal minDist2 = 1./EPSILON;
	long nearestIndex=-1;
	for ( long i1 = 0; i1 < nox; i1++ )
		for ( long i2 = 0; i2 < noy; i2++ )
			for ( long i3 = 0; i3 < noz; i3++ )
			{
				relcoord[0] = i1;
				relcoord[1] = i2;
				relcoord[2] = i3;
				cubeindex = GetCubeIndexForRelCoord ( relcoord );
				if (IsCubeEmpty(cubeindex) == false)
				{
					VPoint currCenter = GetCenterPointOfCube(cubeindex);
					VPoint diff=currCenter-mypoint;
					vreal currDist2 = diff.GetLength2();
					if ( currDist2<minDist2)
					{
						//if ( IsCubeEmpty(cubeindex) == false )
						//{
						nearestIndex=cubeindex;
						minDist2=currDist2;
						cubeCenter=currCenter;
						//}
					}
				}
			}

	return nearestIndex;
}

//#############################################################################
