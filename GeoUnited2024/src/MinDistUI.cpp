// MinDistUI.cpp

#include "stdafx.h"
#include "MinDistUI.h"
#include "VTriangleE.h"
#include "TrianglePointMinDist.h"

MinDistInfo MinDistUI::MinDist ( const VPoint & p , const VTriangleE & tri ) const
{
	MinDistInfo a;
	a.p1 = p;
	TrianglePointMinDist service;
	a.distance = service.MinDist ( tri , p , a.p2 );
	return (a);
}



