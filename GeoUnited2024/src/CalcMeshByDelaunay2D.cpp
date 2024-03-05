// CalcMeshByDelaunay2D.cpp

#include "stdafx.h"
#include "CalcMeshByDelaunay2D.h"
#include "PointSet2d.h"
#include "PointSet.h"
#include "TriangleMesh2d.h"
#include "TriangleMesh.h"
#include "MgcDelaunay2D.h"
#include "gmTriangleUtility.h"

CalcMeshByDelaunay2D::CalcMeshByDelaunay2D 
( const PointSet2d &paramp , const PointSet &modelp , const PointSet &normalp )
: parampoints ( paramp ) , modelpoints ( modelp ) , normals ( normalp )
{

}

//#############################################################################

void CalcMeshByDelaunay2D::CalcMesh ( TriangleMesh2d &paramtri , TriangleMesh &modeltri ) const
{
	int pointno = (int)(parampoints.Size());
	VPoint2d * pointarray = new VPoint2d [pointno];	

	for ( long j=0; j<parampoints.Size(); j++)
	{
		pointarray[j] = *(parampoints[j]);	
	}
	
	Delaunay2D mydel ( pointno , pointarray );
	
	int formedtrino = mydel.GetTriangleQuantity();
	
	for ( int w=0; w<formedtrino; w++)
	{
		Delaunay2D::Triangle &temp = mydel.GetTriangle (w);
		
		int ref1 = temp.m_aiVertex[0];
		int ref2 = temp.m_aiVertex[1];
		int ref3 = temp.m_aiVertex[2];

		//VPoint cor1 ( pointarray[ref1].x() , pointarray[ref1].y() , 0.0 );
		//VPoint cor2 ( pointarray[ref2].x() , pointarray[ref2].y() , 0.0 );
		//VPoint cor3 ( pointarray[ref3].x() , pointarray[ref3].y() , 0.0 );
		
		gmTriangleUtility tool;

		tool.AddTriangle ( *(modelpoints[ref1]) , *(modelpoints[ref2]) , *(modelpoints[ref3]) 
		, *(normals[ref1]) , *(normals[ref2]) , *(normals[ref3])	
		, *(parampoints[ref1]) , *(parampoints[ref2]) , *(parampoints[ref3]) 
		, modeltri , paramtri );	
	
	}

}

//#############################################################################
