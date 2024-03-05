// TriangleMeshPointMinDist.cpp

#include "stdafx.h"
#include "TriangleMeshPointMinDist.h"
#include "TriangleMesh.h"
#include "VPoint.h"
#include "TrianglePointMinDist.h"

TriangleMeshPointMinDist::TriangleMeshPointMinDist ( const TriangleMesh & newmesh )
: mesh (newmesh) , cubes (newmesh) 
{

}

vreal TriangleMeshPointMinDist::MinDist ( const VPoint & p , VPoint & pointonmesh , VPoint &cubeCenter) const
{
	LongVec relatedindices , notincludedindices;
	//cubes.SetRelatedIndiceListForPointWithAtLeastOneItem 
	//	( p , relatedindices , notincludedindices );
	cubes.GetRelatedIndicesForMinDist (p,relatedindices,cubeCenter);

	TrianglePointMinDist service;
	VPoint pointontri;
	vreal dist = 1.0 / EPSILON;

	for ( int i=0; i < relatedindices.Size(); i++ )
	{
		int currindex = relatedindices[i];
		const VTriangleE * currtri = mesh[currindex];
		vreal currdist = service.MinDist(*currtri , p , pointontri );
		if ( currdist < dist )
		{
			if ( fabs (currdist) > 1.e-3 || currtri->GetArea() > 1.e-5 )
			{	
				dist = currdist;
				pointonmesh = pointontri;
			}
		}
	}
	return dist;
}

vreal TriangleMeshPointMinDist::MinDistWithAllTriangle ( const VPoint & p , VPoint & pointonmesh , VPoint &cubeCenter) const
{	
	TrianglePointMinDist service;
	VPoint pointontri;
	vreal dist = 1.0 / EPSILON;

	for ( int i=0; i < mesh.Size(); i++ )
	{
		const VTriangleE * currtri = mesh[i];
		vreal currdist = service.MinDist(*currtri , p , pointontri );
		if ( currdist < dist )
		{
			if ( fabs (currdist) > 1.e-3 || currtri->GetArea() > 1.e-5 )
			{	
				dist = currdist;
				pointonmesh = pointontri;
			}
		}
	}
	return dist;
}
