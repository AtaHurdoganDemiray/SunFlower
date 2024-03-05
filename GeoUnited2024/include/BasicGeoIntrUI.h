// BasicGeoIntrUI.h

#ifndef __BasicGeoIntrUI_h__
#define __BasicGeoIntrUI_h__

#include "PointSet.h"
#include "LineIntersection.h"
struct VLine;

struct BasicIntrInfo
{
public:
	BasicIntrInfo();

	bool isintr;
	bool specialcase;
	PointSet intrpoints;
};

class BasicGeoIntrUI
{
public:
	BasicGeoIntrUI();
	~BasicGeoIntrUI();

	LineIntersection::intersectiontype FindIntr 
		( const VLine &l1 , const VLine &l2 , BasicIntrInfo &info );

};

#endif //__BasicGeoIntrUI_h__