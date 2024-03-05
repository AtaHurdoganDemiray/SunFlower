// PosCubesCore.cpp

#include "stdafx.h"
#include "PosCubes.h"
#include "VLine.h"
#include "VTriangleE.h"

void PosCubes::PosCubesCore ( const VPoint &newmin , const VPoint &newmax 
							, const long &newnox , const long &newnoy , const long &newnoz
							, const long &newsetcount )
{
	min = newmin;
	max = newmax;
	nox = newnox;
	noy = newnoy;
	noz = newnoz;
	setcount = newsetcount;

	CreateCubes ();
	CalcCubeLength ();
	CreateFlags ();
}

//#############################################################################

void PosCubes::PosCubesCoreDelete()
{
	delete [] cubes;
}

//#############################################################################

void PosCubes::CreateCubes ( )
{
	cubeno = nox * noy * noz;

	cubes = new LongVec [cubeno];

	return;
}

//#############################################################################

void PosCubes::CalcCubeLength ()
{
	VPoint diff = max - min;
	cubelengthx = diff.x() / nox;
	cubelengthy = diff.y() / noy;
	cubelengthz = diff.z() / noz;
	return;
}

//#############################################################################

void PosCubes::CreateFlags ()
{
	for ( long i=0; i < setcount; i++ )
	{
		flag.push_back ( false );
	}
}

//#############################################################################

void PosCubes::SetAllFlag ( bool newval )const
{
	for ( long i = 0; i < setcount; i++ )
	{
		flag[i] = newval;
	}
}

//#############################################################################

bool PosCubes::GetRelCoordForPoint ( long relcoord[3] , const VPoint &mypoint )const
{
	VPoint relpoint = mypoint - min;
	relcoord[0] = (long) ( relpoint.x() / cubelengthx );
	relcoord[1] = (long) ( relpoint.y() / cubelengthy );
	relcoord[2] = (long) ( relpoint.z() / cubelengthz );
	//bool ck = IsRelCoordValid ( relcoord );
	LimitRelCoordToMargins ( relcoord );
	
	return true;
}

//#############################################################################

bool PosCubes::GetRelMinMaxFor2Points ( long relmin[3] , long relmax[3] 
											 , const VPoint &p1 , const VPoint &p2 )const
{
	VPoint boxmin = p1;
	VPoint boxmax = p1;
	
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

bool PosCubes::IsRelCoordValid ( long relcoord[] )const
{
	if ( relcoord[0] < 0 || relcoord[0] >= nox 
		|| relcoord[1] < 0 || relcoord[1] >= noy 
		|| relcoord[2] < 0 || relcoord[2] >= noz )
		return false;

	return true;
}

//#############################################################################

bool PosCubes::IsRelBoxMax ( long boxmin[] , long boxmax[]  )const
{
	if ( boxmin[0] == 0 && boxmin[1] == 0 && boxmin[2] == 0 
		&& boxmax[0] == nox && boxmax[1] == noy && boxmax[2] == noz )
		return true;

	return false;
}

//#############################################################################

long PosCubes::GetCubeIndexForRelCoord ( long relcoord[] )const
{
	if ( IsRelCoordValid ( relcoord ) == false )
		return -1;

	long cubno = ( relcoord[0] * noy * noz + relcoord[1] * noz + relcoord[2] );
	//long cubno = ( relcoord[0] * noy + relcoord[1]);
	return cubno;

}

//#############################################################################

void PosCubes::GetRelCoordForCubeIndex ( long relcoord[] , const long &cubeindex ) const
{
	relcoord[0] = (long) (cubeindex / (noy*noz));
	relcoord[1] = (long) ((cubeindex - relcoord[0]*noy*noz) / noz);
	relcoord[2] = cubeindex - ( relcoord[0]*noy*noz + relcoord[1]*noz );	
}

//#############################################################################

void PosCubes::SetCubeNo ()
{
	cubeno = nox * noy * noz;
}

//#############################################################################

void PosCubes::OffsetRelBox ( long relmin[] , long relmax[] )const
{
	if ( relmin[0] > 0 )
		relmin[0] -= 1;

	if ( relmin[1] > 0 )
		relmin[1] -= 1;

	if ( relmin[2] > 0 )
		relmin[2] -= 1;

	if ( relmax[0] < nox-1 )
		relmax[0] += 1;
	
	if ( relmax[1] < noy-1 )
		relmax[1] += 1;

	if ( relmax[2] < noz-1 )
		relmax[2] += 1;

}

//#############################################################################

void PosCubes::LimitRelCoordToMargins ( long relcoord[] )const
{
	if ( relcoord[0] < 0 )
		relcoord[0] = 0;

	if ( relcoord[1] < 0 )
		relcoord[1] = 0;

	if ( relcoord[2] < 0 )
		relcoord[2] = 0;
	
	if ( relcoord[0] > nox-1 )
		relcoord[0] = nox-1;

	if ( relcoord[1] > noy-1 )
		relcoord[1] = noy-1;

	if ( relcoord[2] > noz-1 )
		relcoord[2] = noz-1;

}

//#############################################################################

void PosCubes::RecordIndexToCube ( const long &index , const long &cubeindex )
{
	cubes[cubeindex].AddLong(index);
}

//#############################################################################

void PosCubes::RecordCubeContentToList ( LongVec &relatedindices 
					, const long &cubeindex , const LongVec & not_included_indices )const 
{
	long relatedindex;

	for ( long i = 0; i < cubes[cubeindex].Size(); i++ )	
	{
		relatedindex = cubes[cubeindex][i];

		if ( relatedindex > setcount || relatedindex < 0 
			|| IsNotIncludedIndice ( relatedindex , not_included_indices) )
				continue;

		if ( flag[relatedindex] == false )
		{
			relatedindices.AddLong ( relatedindex );
			flag[relatedindex] = true;
		}
	}

}

//#############################################################################

bool PosCubes::IsNotIncludedIndice ( const long &indice 
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

void PosCubes::AddItem ()
{
	setcount++;
	flag.push_back ( false );
}

//#############################################################################

VPoint PosCubes::GetCenterPointOfCube ( const long &cubeindex ) const
{
	long relcoord[3];
	GetRelCoordForCubeIndex(relcoord,cubeindex);
	VPoint delta = max-min;
	VPoint deltaDivided (delta.x()/nox,delta.y()/noy,delta.z()/noz);	
	vreal myx = min.x() + deltaDivided.x() * ((vreal)(relcoord[0]) + 0.5);	
	vreal myy = min.y() + deltaDivided.y() * ((vreal)(relcoord[1]) + 0.5);	
	vreal myz = min.z() + deltaDivided.z() * ((vreal)(relcoord[2]) + 0.5);	
	VPoint result(myx,myy,myz);
	return result;
}

//#############################################################################

bool PosCubes::IsCubeEmpty (const long &cubeindex)const
{
	if ( cubes[cubeindex].Size() == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//#############################################################################

int PosCubes::GetCubeCount()const
{
	return cubeno;
}

const LongVec & PosCubes::GetCube (int i)const
{
	return cubes[i];
}
