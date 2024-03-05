// LinePointMinDist.cpp

#include "stdafx.h"
#include "LinePointMinDist.h"
#include "VLine.h"
#include "VPoint.h"

vreal LinePointMinDist::MinDist ( const VLine & line , const VPoint & p , VPoint & pointontri ) const
{
	vreal dist;

	vreal d;
	bool chk;
	VPoint poe;

	// Checking edges
	chk = PointVSEdge ( line.GetP1() , line.GetV() , p , d , poe );

	dist = 1.0 / EPSILON;
	bool done = false;

	if ( chk )
	{
		if ( dist > d )
		{
			dist = d;
			pointontri = poe;
			done = true;			
		}
	}
	
	if ( done )
		return dist;

	// Checking Vertices
	vreal dv[2];
	dv[0] = PointVSVertex ( line.GetP1() , p );
	dv[1] = PointVSVertex ( line.GetP2() , p );

	dist = 1.0 / EPSILON;
	for ( int i = 0; i < 2; i++ )
		if ( dist > dv[i] )
		{
			dist = dv[i];
			if (i == 0)
				pointontri = line.GetP1();
			else
				pointontri = line.GetP2();
		}

	return dist;
}

bool LinePointMinDist::PointVSEdge
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

vreal LinePointMinDist::PointVSVertex ( const VPoint & vp , const VPoint & p ) const
{
	VPoint minv = p - vp;
	return (minv.GetLength());
}

double LinePointMinDist::InfiniteLinePointMinDist (const VPoint & lp , const VPoint & lv , const VPoint & p) const
{
	VPoint diff = p - lp;
	vreal s = diff % lv;
	vreal lengthlv2 = lv.GetLength2();
	s /= lengthlv2;
	VPoint pointonedge = lp + (lv * s);
	VPoint minv = p - pointonedge;
	double d = minv.GetLength();
	return d;
}

double LinePointMinDist::InfiniteLinePointMinDist (const VPoint & lp , const VPoint & lv , const VPoint & p , vreal &s) const
{
	VPoint diff = p - lp;
	s = diff % lv;
	vreal lengthlv2 = lv.GetLength2();
	s /= lengthlv2;
	VPoint pointonedge = lp + (lv * s);
	VPoint minv = p - pointonedge;
	double d = minv.GetLength();
	return d;
}