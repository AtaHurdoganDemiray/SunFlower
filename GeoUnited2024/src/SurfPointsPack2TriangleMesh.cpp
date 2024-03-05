/******************************************************************************
*               File: SurfPointsPack2TriangleMesh.cpp											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  04.03.2004 15:51:43 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by ModuleWorks GmbH                                  *
******************************************************************************/

#include "stdafx.h"
#include "SurfPointsPack2TriangleMesh.h"
#include "SurfPointsPack.h"
#include "TriangleMesh2d.h"
#include "TriangleMesh.h"
#include "NearestNeighbors.h"
#include "NeighborsData.h"
#include "NearestNeighborsLoop.h"
#include "gmTriangleUtility.h"

SurfPointsPack2TriangleMesh::SurfPointsPack2TriangleMesh ( const SurfPointsPack &surfpack )
:msurfpack ( surfpack ) , poscubes ( surfpack.parampoints ) , anglesupport ( surfpack.parampoints ) , distancesupport ( surfpack.parampoints )
{
	long pointno = msurfpack.parampoints.Size();
	if ( pointno > 0 )
		history = new NeighborsData [pointno];
}

//#############################################################################

SurfPointsPack2TriangleMesh::~SurfPointsPack2TriangleMesh()
{
	delete [] history;
}

//#############################################################################

void SurfPointsPack2TriangleMesh::CalculateMesh ( TriangleMesh2d &paramtri , TriangleMesh &modeltri ) const
{
	for ( long i = 0; i < msurfpack.parampoints.Size(); i++ )
	{
		LongList triangles;
		
		if ( i == 165 )
			int stp = 1;
		
		GetTrianglesAroundPoint ( i , triangles );
		RecordTriangles ( i , triangles , paramtri , modeltri );
	}

}

//#############################################################################

void SurfPointsPack2TriangleMesh::GetTrianglesAroundPoint ( const long &originpoint , LongList &triangles) const
{
	NearestNeighbors nn ( msurfpack.parampoints , poscubes , originpoint );
	bool closed = nn.FillData();
 	
	NearestNeighborsLoop nloop ( nn , anglesupport , distancesupport );
	bool success = nloop.GetTrianglesAroundPoint ( history , triangles );

	if ( closed == true && success == false )
	{
		int stp=1;
		/*nn.ForceData();
		triangles.RemoveAll();
		NearestNeighborsLoop forcedloop ( nn , anglesupport );
		success = forcedloop.GetTrianglesAroundPoint ( history , triangles );*/
	}
}

//#############################################################################

void SurfPointsPack2TriangleMesh::RecordTriangles ( const long &originindex , const LongList &triangles 
, TriangleMesh2d &paramtri , TriangleMesh &modeltri ) const
{
	VPoint &cor1 = *(msurfpack.modelpoints[originindex]);
	VPoint2d &parcor1 = *(msurfpack.parampoints[originindex]);
	VPoint &normal1 = *(msurfpack.normals[originindex]);
	
	gmTriangleUtility formtri;

	for ( long i=0; i<(triangles.Size()/2); i++ )
	{
		long p2index = triangles[i*2];
		long p3index = triangles[i*2+1];

		VPoint &cor2 = *(msurfpack.modelpoints[p2index]);
		VPoint &cor3 = *(msurfpack.modelpoints[p3index]);

		VPoint2d &parcor2 = *(msurfpack.parampoints[p2index]);
		VPoint2d &parcor3 = *(msurfpack.parampoints[p3index]);

		VPoint &normal2 = *(msurfpack.normals[p2index]);
		VPoint &normal3 = *(msurfpack.normals[p3index]);

		formtri.AddTriangle ( cor1 , cor2 , cor3 , normal1 , normal2 , normal3 
			, parcor1 , parcor2 , parcor3 , modeltri , paramtri );
	}	
}

//#############################################################################
