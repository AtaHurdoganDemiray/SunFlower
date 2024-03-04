// TrianglePointMinDist.cpp

#include "stdafx.h"
#include "TrianglePointMinDist.h"
#include "VTriangleE.h"
#include "VPoint.h"

vreal TrianglePointMinDist::MinDist ( const VTriangleE & tri , const VPoint & p , VPoint & pointontri ) const
{
	vreal dist=1./EPSILON;
	if (tri.IsCorrupted())
		return dist;

	// Checking plane of tri
	bool ck = PointVSTriPlane ( tri , p , dist , pointontri );
	
	if ( ck )
		return dist;
	
	vreal d[3];
	bool chk[3];
	VPoint poi[3];

	// Checking edges
	chk[0] = PointVSEdge ( tri.GetCorner1() , tri.GetEdge1() , p , d[0] , poi[0] );
	chk[1] = PointVSEdge ( tri.GetCorner2() , tri.GetEdge2() , p , d[1] , poi[1] );
	chk[2] = PointVSEdge ( tri.GetCorner3() , tri.GetEdge3() , p , d[2] , poi[2] );

	dist = 1.0 / EPSILON;

	int i;
	for ( i=0; i<3; i++ )
	{
		if ( chk[i] )
		{
			if ( dist > d[i] )
			{
				dist = d[i];
				pointontri = poi[i];		
			}
		}
	}
	
	// Checking Vertices
	d[0] = PointVSVertex ( tri.GetCorner1() , p );
	poi[0]=tri.GetCorner1();
	
	d[1] = PointVSVertex ( tri.GetCorner2() , p );
	poi[1] = tri.GetCorner2();

	d[2] = PointVSVertex ( tri.GetCorner3() , p );
	poi[2]=tri.GetCorner3();
	
	for ( i = 0; i < 3; i++ )
		if ( dist > d[i] )
		{
			dist = d[i];
			pointontri = poi[i]; 
		}

	return dist;
}

bool TrianglePointMinDist::PointVSTriPlane 
( const VTriangleE & tri , const VPoint & p , vreal & d , VPoint & pointonplane ) const
{
	d = ( tri.GetCorner1() - p ) % tri.GetNormal();
	pointonplane = p + ( tri.GetNormal() * d );
	
	if (d<0.0)
		d*=-1.0;//d should always be returned positive

	return ( tri.IsInTriangle ( pointonplane ) );
}

bool TrianglePointMinDist::PointVSEdge
( const VPoint & lp , const VPoint & lv , const VPoint & p , vreal & d , VPoint & pointonedge ) const
{
	VPoint diff = p - lp;
	vreal s = diff % lv;
	
	if ( s < 0.0 )
		return false;

	vreal lengthlv2 = lv.GetLength2();
	s /= lengthlv2;

	if ( s > 1.0 )
		return false;
	
	pointonedge = lp + (lv * s);
	
	VPoint minv = p - pointonedge;
	d = minv.GetLength();

	return true;
}

vreal TrianglePointMinDist::PointVSVertex ( const VPoint & vp , const VPoint & p ) const
{
	VPoint minv = p - vp;
	return (minv.GetLength());
}

