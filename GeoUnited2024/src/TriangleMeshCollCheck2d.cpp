/******************************************************************************
*               File: TriangleMeshCollCheck2d.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  01.02.2004 05:47:03 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by positive GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "TriangleMeshCollCheck2d.h"
#include "VTriangle2d.h"
#include "Tri2TriCollCheck2d.h"

TriangleMeshCollCheck2d::TriangleMeshCollCheck2d ( const TriangleMesh2d &newtrimesh2d )
: m_trimesh2d ( newtrimesh2d ) //, poscubes ( newtrimesh2d )
{

}

//#############################################################################

TriangleMeshCollCheck2d::~TriangleMeshCollCheck2d()
{

}

//#############################################################################

bool TriangleMeshCollCheck2d::CheckColl( const long &triindice , const LongList &notincludedindices , PosCubes2d &poscubes ) const
{
	VTriangle2d * p_tri1 = m_trimesh2d[triindice];
	Tri2TriCollCheck2d mychecker;
	
	VPoint2d currmin = p_tri1->GetMin();
	VPoint2d currmax = p_tri1->GetMax();
	LongList relindices; 
	poscubes.SetRelatedIndiceListForBox 
				( currmin , currmax , relindices , notincludedindices );
	
	for ( long i=0; i<relindices.Size(); i++ )
	{
		long currindice = relindices[i];
		
		if ( currindice == triindice )
			continue;

		VTriangle2d * p_tri2 = m_trimesh2d[currindice];										
		bool ck = mychecker.CheckKoll ( *p_tri1 , *p_tri2 );
		
		if (ck)
			return true;// Triangles collide
	}
	
	return false;// No Coll
}

//#############################################################################

bool TriangleMeshCollCheck2d::CheckColl
( const VPoint2d &cor1 , const VPoint2d &cor2 , const VPoint2d &cor3 
	, DynamicPosCubes2d &poscubes ) const
{
	VTriangle2d tri1( cor1, cor2 , cor3 );
	Tri2TriCollCheck2d mychecker;
	
	VPoint2d currmin = cor1;
	VPoint2d currmax = cor2;
	currmin.ReplaceMin ( cor2 );
	currmax.ReplaceMax ( cor2 );
	currmin.ReplaceMin ( cor3 );
	currmax.ReplaceMax ( cor3 );

	LongList notincludedindices;

	LongList relindices;
	poscubes.SetRelatedIndiceListForBox 
	( currmin , currmax , relindices , notincludedindices );
	
	for ( long i=0; i<relindices.Size(); i++ )
	{
		long currindice = relindices[i];
		
		//if ( currindice == triindice )
		//	continue;

		VTriangle2d * p_tri2 = m_trimesh2d[currindice];										
		bool ck = mychecker.CheckKoll ( tri1 , *p_tri2 );
		
		if (ck)
			return true;// Triangles collide
	}
	
	return false;// No Coll
}

//#############################################################################

bool TriangleMeshCollCheck2d::CheckCollToLine ( const VLine2d &newline 
											   , DynamicPosCubes2d &poscubes ) const
{
	Tri2TriCollCheck2d mychecker;
	
	VPoint2d currmin = newline.GetP1();
	VPoint2d currmax = newline.GetP1();
	currmin.ReplaceMin ( newline.GetP2() );
	currmax.ReplaceMax ( newline.GetP2() );

	LongList notincludedindices;
	LongList relindices;

	poscubes.SetRelatedIndiceListForLine(newline,relindices,notincludedindices); 
	
	for ( long i=0; i<relindices.Size(); i++ )
	{
		long currindice = relindices[i];

		VTriangle2d * p_tri2 = m_trimesh2d[currindice];										
		bool ck = mychecker.DoesAnyEdgeIntersectToLine ( *p_tri2 , newline );
		
		if (ck)
			return true;// Triangles collide
	}
	
	return false;// No Coll
}

//#############################################################################

