// LineSetPointMinDist.cpp

#include "stdafx.h"
#include "LineSetPointMinDist.h"
#include "LineSet.h"
#include "VPoint.h"
#include "LinePointMinDist.h"
#include "SequentialLineSet.h"

LineSetPointMinDist::LineSetPointMinDist ( const LineSet & newset )
: set (newset) , cubes (newset) 
{

}

vreal LineSetPointMinDist::MinDist ( const VPoint & p , VPoint & pointonset , int *seti) const
{
	LongVec relatedindices , notincludedindices;
	//cubes.SetRelatedIndiceListForPointWithAtLeastOneItem 
	//	( p , relatedindices , notincludedindices );
	VPoint cubeCenter;
	cubes.GetRelatedIndicesForMinDist (p,relatedindices,cubeCenter);

	LinePointMinDist service;
	VPoint pointonline;
	vreal dist = 1.0 / EPSILON;
	int ni = -1;

	for ( int i=0; i < relatedindices.Size(); i++ )
	{
		int currindex = relatedindices[i];
		const VLine * currline = set[currindex];
		vreal currdist = service.MinDist(*currline , p , pointonline );
		if ( currdist < dist )
		{
			dist = currdist;
			pointonset = pointonline;
			ni = currindex;
		}
	}
	if (seti != 0)
		*seti = ni;
	return dist;
}


SequentialLineSetPointMinDist::SequentialLineSetPointMinDist ( const SequentialLineSet & newset )
: set (newset) , cubes (newset) 
{

}

vreal SequentialLineSetPointMinDist::MinDist ( const VPoint & p , VPoint & pointonset , int *seti) const
{
	LongVec relatedindices , notincludedindices;
	//cubes.SetRelatedIndiceListForPointWithAtLeastOneItem 
	//	( p , relatedindices , notincludedindices );
	VPoint cubeCenter;
	cubes.GetRelatedIndicesForMinDist (p,relatedindices,cubeCenter);

	LinePointMinDist service;
	VPoint pointonline;
	vreal dist = 1.0 / EPSILON;
	int ni = -1;

	for ( int i=0; i < relatedindices.Size(); i++ )
	{
		int currindex = relatedindices[i];
		VLine currline = set[currindex];
		vreal currdist = service.MinDist(currline , p , pointonline );
		if ( currdist < dist )
		{
			dist = currdist;
			pointonset = pointonline;
			ni = currindex;
		}
	}
	if (seti != 0)
		*seti = ni;
	return dist;
}
