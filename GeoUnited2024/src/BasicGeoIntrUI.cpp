// BasicGeoIntrUI.cpp
#include "stdafx.h"
#include "BasicGeoIntrUI.h"
#include "VLine.h"


BasicIntrInfo::BasicIntrInfo ()
: intrpoints ( 5 )
{

}

BasicGeoIntrUI::BasicGeoIntrUI()
{

}

BasicGeoIntrUI::~BasicGeoIntrUI()
{

}

LineIntersection::intersectiontype BasicGeoIntrUI::FindIntr 
		( const VLine &l1 , const VLine &l2 , BasicIntrInfo &info )
{
	VPoint intpoint,parallelint;
	LineIntersection calc;
	
	LineIntersection::intersectiontype detailedinfo =  
		calc.LinesIntersect ( l1.GetP1() , l1.GetV() , l2.GetP1() , l2.GetV() , intpoint,parallelint );
	
	if ( detailedinfo == LineIntersection::LINESINTERSECT )
	{
		info.isintr = true;
		info.specialcase = false;
		info.intrpoints.InsertPoint ( intpoint.GetArray() );
		return detailedinfo;
	}
	if ( detailedinfo == LineIntersection::NOINTERSECTION )
	{
		info.isintr = false;
		info.specialcase = false;
		return detailedinfo;
	}
	if ( detailedinfo == LineIntersection::INFINITELINESINTERSECT )
	{
		info.isintr = false;
		info.specialcase = false;
		return detailedinfo;
	}
	if ( detailedinfo == LineIntersection::LINESPARALLEL_APART )
	{
		info.isintr = false;
		info.specialcase = true;
		return detailedinfo;
	}
	if ( detailedinfo == LineIntersection::LINESPARALLEL_SAMELINE_SEPERATE )
	{
		info.isintr = false;
		info.specialcase = true;
		return detailedinfo;
	}
	if ( detailedinfo == LineIntersection::LINESPARALLEL_INTERSECTING )
	{
		info.isintr = true;
		info.specialcase = true;
		info.intrpoints.InsertPoint ( intpoint.GetArray() );
		info.intrpoints.InsertPoint ( parallelint.GetArray() );
		return detailedinfo;
	}
	
	return detailedinfo;
}
