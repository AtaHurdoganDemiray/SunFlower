// gmQuadSet.cpp

#include "stdafx.h"
#include "gmQuadSet.h"
#include "BasicDefinitions.h"
//#include "BSplineSurfacePointGenerator.h"
//#include "DynamicPosCubes2d.h"
//#include "DebugHelper.h"




gmQuadSet::gmQuadSet( SurfPointsPack &surfpackref )
: surfpack ( surfpackref )
{
	//p_poscubes = NULL;
	FillSet();	
}

gmQuadSet::gmQuadSet ( const gmQuadSet & Foo , bool isquadvalid[] )
:surfpack ( Foo.surfpack )
{
	for ( int i=0; i < Foo.Size(); i++ )
	{
		if ( isquadvalid[i] )
		{
			const CornerRef * cref = Foo[i];
			AddItem ( cref->cor1ref , cref->cor2ref , cref->cor3ref , cref->cor4ref );
		}
	}
}
/*
gmQuadSet::gmQuadSet( gmPointSet2d &baseparams , DynamicPosCubes2d &poscubes 
					 , gmPointSet &basepoints , gmPointSet &normals )
: m_baseparams ( baseparams ) 
, m_basepoints ( basepoints ) , m_normals ( normals )
{
	p_poscubes = &poscubes;	
}
*/
/*
void gmQuadSet::InsertPointsToSets ( const VPoint2d params[4] , const VPoint cors[4] 
									, const VPoint normals[4] )
{
	LongVec relatedindices , notincludedindices;
	long corref[4];	

	for ( int j=0; j<4; j++ )
	{
		// Check if this point already exist in the param set
		long coincindex = -1;
		p_poscubes->SetRelatedIndiceListForPoint 
			( params[j] , relatedindices , notincludedindices );

		for ( long i=0; i < relatedindices.Size(); i++ )
		{
			long currIndex = relatedindices[i];
			VPoint2d *currPar = m_baseparams[currIndex];
			if ( params[j].IsCoincident ( (*currPar) ) )
			{
				coincindex = currIndex;
				break;
			}
		}

		if ( coincindex == -1 )
		{
			// point doesnt exist so add it to set
			corref[j] = m_baseparams.Size();
			p_poscubes->AddPoint2d ( params[j] );
			m_basepoints.InsertPoint ( cors[j].GetArray() );
			m_normals.InsertPoint ( normals[j].GetArray() );
		}
		else
		{
			// point exist so no need to add it
			corref[j] = coincindex;
		}

	}// for 4 points
	
	// add quad to set
	AddItem ( corref[0] , corref[1] , corref[2] , corref[3] );

}
*/
gmQuadSet::~gmQuadSet()
{
	for (unsigned long i=0; i < allcornerref.size(); i++)
		delete ( allcornerref[i] );
}

long gmQuadSet::Size() const 
{
	return (int)(allcornerref.size());
}

void gmQuadSet::AddItem ( const long &cor1ref 
						 , const long &cor2ref 
						 , const long &cor3ref 
						 , const long &cor4ref )//CornerRef *newitem )
{
	CornerRef * newitem = new CornerRef ( cor1ref , cor2ref , cor3ref , cor4ref );
	allcornerref.push_back ( newitem );
}

const CornerRef * gmQuadSet::operator [] ( long index ) const
{
	if ( index < 0 || index > (Size()-1) )
		throw(VException (( IDS_GmQInvInd) , 0 ));

	return ( allcornerref[index] );
}

gmQuad2d gmQuadSet::GetQuad2d ( long quadno ) const
{
	if ( quadno < 0 || quadno >= Size() )
		throw ( VException ( ( IDS_QuaSetExc) , 0 ) );

	CornerRef * mref = allcornerref[quadno];
	
	VPoint2d * cor1 = surfpack.parampoints[mref->cor1ref];
	VPoint2d * cor2 = surfpack.parampoints[mref->cor2ref];
	VPoint2d * cor3 = surfpack.parampoints[mref->cor3ref];
	VPoint2d * cor4 = surfpack.parampoints[mref->cor4ref];

	gmQuad2d aquad ( *cor1 , *cor2 , *cor3 , *cor4 );
	
	return aquad;
}

void gmQuadSet::FillSet()
{
	//long mysize = surfpack.parampoints.Size();

	//for ( long i=0; i < mysize; i++ )
	//{
	//	// form a corner ref for the quad and record it
	//	long cor1ref = i;
	//	long cor2ref = i + 1;
	//	long cor3ref = i + NURBS_SURFACE_TESSELATION_POINTS_PER_DIR+1;
	//	long cor4ref = i + NURBS_SURFACE_TESSELATION_POINTS_PER_DIR;

	//	if ( cor3ref < mysize 
	//		&& (i+1)%(NURBS_SURFACE_TESSELATION_POINTS_PER_DIR) != 0 )
	//		AddItem ( cor1ref , cor2ref , cor3ref , cor4ref );

	//	//for debug
	//	/*gmPoint * cor1 = m_basepoints[cor1ref];
	//	gmPoint * cor2 = m_basepoints[cor2ref];
	//	gmPoint * cor3 = m_basepoints[cor3ref];
	//	gmPoint * cor4 = m_basepoints[cor4ref];

	//	if ( CheckQuad ( *cor1 , *cor2 , *cor3 , *cor4 ) )
	//		int stp = 1;*/

	//}

}

void gmQuadSet::FillSet( const int &uno , const int &vno )
{
	int unext , vnext;

	for ( int u = 0; u < uno-1; u++ )
		for ( int v = 0; v < vno-1; v++ )
		{
			unext = u + 1;
			vnext = v + 1;
			long indexcor1 = GetIndexForPointSet ( uno , vno , u , v );
			long indexcor2 = GetIndexForPointSet ( uno , vno , u , vnext );
			long indexcor3 = GetIndexForPointSet ( uno , vno , unext , vnext );
			long indexcor4 = GetIndexForPointSet ( uno , vno , unext , v );
			AddItem ( indexcor1 , indexcor2 , indexcor3 , indexcor4 );
		}
}

long gmQuadSet::GetIndexForPointSet ( const int &uno , const int &vno , const int &u , const int &v )
{
	if ( u < 0 || u >= uno || v < 0 || v >= vno )
		return -1;

	long index = u * vno + v;
	return index;
}

VPoint2d gmQuadSet::GetMin2d () const
{
	return (surfpack.parampoints.GetMin());
}

VPoint2d gmQuadSet::GetMax2d () const
{
	return (surfpack.parampoints.GetMax());
}

VPoint gmQuadSet::GetMin () const
{
	return (surfpack.modelpoints.GetMin() );
}

VPoint gmQuadSet::GetMax () const
{
	return (surfpack.modelpoints.GetMax() );
}

