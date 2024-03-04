// BasicGeoIntr2dUI.cpp
#include "stdafx.h"
#include "BasicGeoIntr2dUI.h"
#include "VLine2d.h"


BasicIntrInfo2d::BasicIntrInfo2d ()
: intrpoints ( 5 )
{

}

BasicGeoIntr2dUI::BasicGeoIntr2dUI()
{

}

BasicGeoIntr2dUI::~BasicGeoIntr2dUI()
{

}

LineIntersection2d::intersectiontype2d BasicGeoIntr2dUI::FindIntr 
( const VLine2d &l1 , const VLine2d &l2 , BasicIntrInfo2d &info )
{
	LineIntersection2d calc;
	VPoint2d intpoint,parallelint;
	
	LineIntersection2d::intersectiontype2d detailedinfo = 

		calc.LinesIntersect2d ( l1.GetP1() , l1.GetV() , l2.GetP1() , l2.GetV() , intpoint , parallelint ); 
	
	if ( detailedinfo == LineIntersection2d::LINESINTERSECT )
	{
		info.intrpoints.InsertPoint ( intpoint.GetArray() );
		info.isintr = true;
		info.specialcase = false;
		return detailedinfo;
	}
	if ( detailedinfo == LineIntersection2d::INFINITELINESINTERSECT )
	{
		info.isintr = false;
		info.specialcase = false;
		return detailedinfo;
	}
	if ( detailedinfo == LineIntersection2d::LINESPARALLEL_APART )
	{
		info.isintr = false;
		info.specialcase = true;
		return detailedinfo;
	}
	//if ( detailedinfo == LineIntersection2d::LINESPARALLEL_SAMELINE_NOINTERSECTION )
	//{
	//	info.isintr = false;
	//	info.specialcase = true;
	//	return detailedinfo;
	//}
	if ( detailedinfo == LineIntersection2d::LINESPARALLEL_INTERSECTING )
	{
		info.isintr = true;
		info.specialcase = true;
		info.intrpoints.InsertPoint ( intpoint.GetArray() );
		info.intrpoints.InsertPoint ( parallelint.GetArray() );
		return detailedinfo;
	}

	return detailedinfo;

}
