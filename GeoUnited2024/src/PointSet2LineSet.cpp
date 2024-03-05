// PointSet2LineSet.cpp

#include "stdafx.h"
#include "PointSet2LineSet.h"
#include "PointSet.h"
#include "PointSet2d.h"
#include "LineSet.h"
#include "LineSet2d.h"

PointSet2LineSet::PointSet2LineSet()
{

}

PointSet2LineSet::~PointSet2LineSet()
{

}

void PointSet2LineSet::FillLineSetByPointSet ( const PointSet &pointset , LineSet &lineset ) const
{
	VPoint *myp1 , *myp2;
	for ( long i = 0; i < pointset.Size() - 1; i++ )
	{
		myp1 = pointset[i];
		myp2 = pointset[i+1];
		lineset.AddLine ( *myp1 , *myp2 );
	}
	myp1 = myp2;
	myp2 = pointset[0];
	lineset.AddLine ( *myp1 , *myp2 );
}

void PointSet2LineSet::FillLineSetByPointSet ( const PointSet2d &pointset , LineSet2d &lineset ) const
{
	VPoint2d *myp1 , *myp2;
	for ( long i = 0; i < pointset.Size() - 1; i++ )
	{
		myp1 = pointset[i];
		myp2 = pointset[i+1];
		lineset.AddLine ( *myp1 , *myp2 );
	}
	myp1 = myp2;
	myp2 = pointset[0];
	lineset.AddLine ( *myp1 , *myp2 );
}
