// PosCubes2dCore.cpp

#include "stdafx.h"
#include "PosCubes2d.h"
#include "VLine2d.h"
#include "VTriangle2dE.h"

void PosCubes2d::PosCubes2dCore ( const VPoint2d &newmin , const VPoint2d &newmax 
								, const long &newnox , const long &newnoy
								, const long &newsetcount )
{
	min = newmin;
	max = newmax;
	nox = newnox;
	noy = newnoy;
	setcount = newsetcount;

	CreateCubes ();

	CalcCubeLength ();
	
	//CreateFlags ();
}

//#############################################################################

void PosCubes2d::PosCubes2dCoreDelete()
{
	delete [] cubes;
}

//#############################################################################

void PosCubes2d::CreateCubes ( )
{
	cubeno = nox * noy ;

	cubes = new LongVec [cubeno];

	return;
}

//#############################################################################

void PosCubes2d::CalcCubeLength ()
{
	VPoint2d diff = max - min;
	
	cubelengthx = diff.x() / nox;
	cubelengthy = diff.y() / noy;
	return;

}

//#############################################################################

//void PosCubes2d::CreateFlags ()
//{
//	for ( long i=0; i < setcount; i++ )
//	{
//		flag.push_back ( false );
//	}
//}

//#############################################################################

//void PosCubes2d::SetAllFlag ( bool newval )const
//{
//	for ( long i = 0; i < setcount; i++ )
//	{
//		flag[i] = newval;
//	}
//}

//#############################################################################

bool PosCubes2d::GetRelCoordForPoint ( long relcoord[3] , const VPoint2d &mypoint )const
{
	VPoint2d relpoint = mypoint - min;
	relcoord[0] = (long) ( relpoint.x() / cubelengthx );
	relcoord[1] = (long) ( relpoint.y() / cubelengthy );
	
	//bool ck = IsRelCoordValid ( relcoord );
	LimitRelCoordToMargins ( relcoord );
	
	return true;
}

//#############################################################################

bool PosCubes2d::GetRelMinMaxFor2Points ( long relmin[3] , long relmax[3] 
											 , const VPoint2d &p1 , const VPoint2d &p2 )const
{
	VPoint2d boxmin = p1;
	VPoint2d boxmax = p1;
	
	boxmin.ReplaceMin ( p2 );
	boxmax.ReplaceMax ( p2 );

	bool ckmin = GetRelCoordForPoint ( relmin , boxmin );

	if ( ckmin == false )
		return false;

	bool ckmax = GetRelCoordForPoint ( relmax , boxmax );

	if ( ckmax == false )
		return false;

	return true;
}

//#############################################################################

bool PosCubes2d::IsRelCoordValid ( long relcoord[] )const
{
	if ( relcoord[0] < 0 || relcoord[0] >= nox 
		|| relcoord[1] < 0 || relcoord[1] >= noy )
		return false;

	return true;
}

//#############################################################################

bool PosCubes2d::IsRelBoxMax ( long boxmin[] , long boxmax[]  )const
{
	if ( boxmin[0] == 0 && boxmin[1] == 0 
		&& boxmax[0] == nox && boxmax[1] == noy )
		return true;

	return false;
}

//#############################################################################

long PosCubes2d::GetCubeIndexForRelCoord ( long relcoord[] )const
{
	if ( IsRelCoordValid ( relcoord ) == false )
		return -1;

	long cubno = ( relcoord[0] * noy + relcoord[1]);
	return cubno;

}

//#############################################################################

void PosCubes2d::GetRelCoordForCubeIndex ( long relcoord[] , const long &cubeindex ) const
{
	relcoord[0] = (long) (cubeindex / noy);
	relcoord[1] = cubeindex - relcoord[0]*noy;
}

//#############################################################################

void PosCubes2d::SetCubeNo ()
{
	cubeno = nox * noy;
}

//#############################################################################

void PosCubes2d::OffsetRelBox ( long relmin[] , long relmax[] )const
{
	if ( relmin[0] > 0 )
		relmin[0] -= 1;

	if ( relmin[1] > 0 )
		relmin[1] -= 1;


	if ( relmax[0] < nox-1 )
		relmax[0] += 1;
	
	if ( relmax[1] < noy-1 )
		relmax[1] += 1;
	
}

//#############################################################################

void PosCubes2d::LimitRelCoordToMargins ( long relcoord[] )const
{
	if ( relcoord[0] < 0 )
		relcoord[0] = 0;

	if ( relcoord[1] < 0 )
		relcoord[1] = 0;

	if ( relcoord[0] > nox-1 )
		relcoord[0] = nox-1;

	if ( relcoord[1] > noy-1 )
		relcoord[1] = noy-1;

}

//#############################################################################

void PosCubes2d::RecordIndexToCube ( const long &index , const long &cubeindex )
{
	cubes[cubeindex].AddLong(index);
}

//#############################################################################

void PosCubes2d::RecordCubeContentToList ( LongVec &relatedindices 
					, const long &cubeindex , const LongVec & not_included_indices ) const
{
	long relatedindex;

	for ( long i = 0; i < cubes[cubeindex].Size(); i++ )	
	{
		relatedindex = cubes[cubeindex][i];

	//	if ( relatedindex > setcount || relatedindex < 0 
	//		|| IsNotIncludedIndice ( relatedindex , not_included_indices) )
	//			continue;

		//if ( flag[relatedindex] == false )
		//{
			relatedindices.AddLong ( relatedindex );
			//flag[relatedindex] = true;
		//}
	}

}

//#############################################################################

bool PosCubes2d::IsNotIncludedIndice ( const long &indice 
										  , const LongVec & not_included_indices ) const
{
	for ( int i=0; i<  not_included_indices.Size(); i++ )
	{
		if ( indice == not_included_indices[i] )
			return true;
	}
	
	return false;

}

//#############################################################################

void PosCubes2d::AddItem ()
{
	setcount++;
	//flag.push_back ( false );
}

//#############################################################################
