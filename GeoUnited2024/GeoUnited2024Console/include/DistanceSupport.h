// DistanceSupport.h

#ifndef __DistanceSupport_h__
#define __DistanceSupport_h__

#include "PointSet2d.h"

class DistanceSupport
{
public:
	DistanceSupport ( const PointSet2d &basepointset );
	vreal GetMinDistance2BetweenLineAndPoint2d ( const long &linep1 , const long &linep2 , const long &mypoint ) const;

private:
	const PointSet2d &baseset;

};

#endif