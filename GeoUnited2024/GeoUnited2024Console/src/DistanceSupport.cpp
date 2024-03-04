// DistanceSupport.cpp

#include "stdafx.h"
#include "DistanceSupport.h"
#include "BasicLineFunctions2d.h"

DistanceSupport::DistanceSupport ( const PointSet2d &basepointset )
: baseset ( basepointset )
{

}

vreal DistanceSupport::GetMinDistance2BetweenLineAndPoint2d ( const long &linep1 , const long &linep2 , const long &mypoint )const
{
	BasicLineFunctions2d calc;

	VPoint2d lp1 = *( baseset[linep1] );
	VPoint2d lp2 = *( baseset[linep2] );
	VPoint2d mypoint2d = *( baseset[mypoint] );
	VLine2d myline2d ( lp1 , lp2 );

	return ( calc.GetDist2BetwLineAndPoint2d ( myline2d , mypoint2d ) );
}
