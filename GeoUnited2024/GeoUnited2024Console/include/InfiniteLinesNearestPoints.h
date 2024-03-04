// InfiniteLinesNearestPoints.h

#ifndef __InfiniteLinesNearestPoints_h__
#define __InfiniteLinesNearestPoints_h__

#include "VPoint.h"
#include "VLine.h"

struct InfiniteLinesNearestPoints
{
public:

	enum info { LINESPARALLEL , REGULARCASE };
	//InfiniteLinesNearestPoints(){};
	//~InfiniteLinesNearestPoints(){};

	info Calc ( const VPoint &p1 , const VPoint &v1 
				, const VPoint &p2 , const VPoint &v2 
				, vreal &int_t , vreal &int_s )
	{
		info returnvalue = REGULARCASE;

		vreal myA = -2 * (v1 % v2);
		vreal myB = 2 * (v1 % v1);
		vreal myC = 2 * ( ( p1 % v1 ) - ( v1 % p2 ) );
		vreal myD = 2 * ( v2 % v2 );
		vreal myE = myA;//-2 * ( v1 % v2 );
		vreal myF = 2 * ( ( p2 % v2 ) - ( p1 % v2 ) );

		vreal mydivisor = myE * myA - myB * myD;

		if ( fabs(mydivisor) < EPSILON )
		{	
			// lines are paralel
			returnvalue = LINESPARALLEL;
			return returnvalue;		
		}

		int_t = ( myD * myC - myA * myF ) / mydivisor;
		int_s = (myB * myF - myE * myC) / mydivisor;

		return returnvalue;
	}

};

#endif//__InfiniteLinesNearestPoints_h__ 