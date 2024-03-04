// BoundingBox2TriMesh.cpp

#include "stdafx.h"
#include "BoundingBox2TriMesh.h"
#include "BoundingBox.h"
#include "TriangleMesh.h"
#include "VPoint.h"

BoundingBox2TriMesh::BoundingBox2TriMesh ( const BoundingBox &newbox )
: mbox ( newbox )
{

}

//#############################################################################

void BoundingBox2TriMesh::FillTriMesh ( TriangleMesh &trimesh ) const
{
	VPoint min = mbox.GetMin();
	VPoint max = mbox.GetMax();

	VVector diff = max - min;
	VVector vx ( diff.x() , 0.0 , 0.0 );
	VVector vy ( 0.0 , diff.y() , 0.0 );
	VVector vz ( 0.0 , 0.0 , diff.z() );

	// bottom xy
	VPoint t1 , t2 , t3;
	t1 = min;
	t2 = max - vz;
	t3 = t2 - vx;
	trimesh.AddTriangle ( t1 , t3 , t2 );

	t3 = t2 - vy;
	trimesh.AddTriangle ( t1 , t2 , t3 );

	// front xz
	t2 = t3 + vz;
	trimesh.AddTriangle ( t1 , t3 , t2 );

	t3 = t2 - vx;
	trimesh.AddTriangle ( t1 , t2 , t3 );
	
	// top xy
	trimesh.AddTriangle ( t3 , t2 , max );

	t2 = t3 + vy;
	trimesh.AddTriangle ( t3 , max , t2 );
	
	// left yz
	trimesh.AddTriangle ( min , t3 , t2 );
	
	t1 = min + vy;
	trimesh.AddTriangle ( t2 , t1 , min );

	// back xz
	trimesh.AddTriangle ( t1 , t2 , max );

	t3 = max - vz;
	trimesh.AddTriangle ( max , t3 , t1 );

	// right yz
	t2 = min + vx;
	trimesh.AddTriangle ( t2 , t3 , max );

	t1 = max - vy;
	trimesh.AddTriangle ( max , t1 , t2 );

}

//#############################################################################
