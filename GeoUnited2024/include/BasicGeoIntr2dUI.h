// BasicGeoIntr2dUI.h

#ifndef __BasicGeoIntr2dUI_h__
#define __BasicGeoIntr2dUI_h__

#include "PointSet2d.h"
#include "LineIntersection2d.h"

struct VLine2d;

struct BasicIntrInfo2d
{
public:
	BasicIntrInfo2d();

	bool isintr;
	bool specialcase;
	PointSet2d intrpoints;
};

class BasicGeoIntr2dUI
{
public:
	BasicGeoIntr2dUI();
	~BasicGeoIntr2dUI();

	LineIntersection2d::intersectiontype2d FindIntr 
		( const VLine2d &l1 , const VLine2d &l2 , BasicIntrInfo2d &info );

};

#endif //__BasicGeoIntr2dUI_h__