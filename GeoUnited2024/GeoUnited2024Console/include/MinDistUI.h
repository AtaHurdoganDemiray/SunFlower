// MinDistUI.h

#ifndef __MinDistUI_h__
#define __MinDistUI_h__

#include "VPoint.h"
struct VTriangleE;

struct MinDistInfo
{
public:
	VPoint p1,p2;
	vreal distance;

};

class MinDistUI
{
public:
	MinDistInfo MinDist ( const VPoint & p , const VTriangleE & tri ) const;

};

#endif