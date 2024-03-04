// DebugHelper.h

#ifndef __DebugHelper_h__
#define __DebugHelper_h__

#include "VPoint.h"

bool CheckPoint ( const VPoint &newpoint )
{
	if ( fabs ( newpoint.x() -  ( -7.08 ) ) < 0.08
		&& fabs ( newpoint.y() - ( -1.29 ) ) < 0.08
		&& fabs ( newpoint.z() - ( 6.94 ) ) < 0.08 )
		return true;

	return false;
}

bool CheckPwithP ( const VPoint&p1 , const VPoint&p2 )
{
	if ( fabs ( p1.x() - p2.x() ) < 0.01
		&& fabs ( p1.y() - p2.y() ) < 0.01
		&& fabs ( p1.z() - p2.z() ) < 0.01 )
		return true;

	return false;
}

bool CheckPwithP ( const VPoint2d&p1 , const VPoint2d&p2 )
{
	if ( fabs ( p1.x() - p2.x() ) < 0.01
		&& fabs ( p1.y() - p2.y() ) < 0.01 )
		return true;

	return false;
}

bool CheckTri ( const VPoint &cor1 , const VPoint &cor2 , const VPoint &cor3 )
{
	VPoint p1 ( 113.0165 , 25.0536 , 57.5 );
	VPoint p2 ( 105.4130 , 18.1867 , 57.5 );
	VPoint p3 ( 113.0165 , 25.0536 , 55.0 );

	int coinc = 0;

	if ( CheckPwithP ( cor1 , p1 ) || CheckPwithP ( cor1 , p2 ) || CheckPwithP ( cor1 , p3 ) )
		coinc++;
	if ( CheckPwithP ( cor2 , p1 ) || CheckPwithP ( cor2 , p2 ) || CheckPwithP ( cor2 , p3 ) )
		coinc++;
	if ( CheckPwithP ( cor3 , p1 ) || CheckPwithP ( cor3 , p2 ) || CheckPwithP ( cor3 , p3 ) )
		coinc++;

	if ( coinc == 3 )
		return true;

	return false;
}

bool CheckQuad ( const VPoint &cor1 , const VPoint &cor2 , const VPoint &cor3 , const VPoint &cor4 )
{
	VPoint p1 ( -89.5961 , 17.1579 , 57.9310 );
	VPoint p2 ( -83.9610 , 24.2303 , 60.0 );
	VPoint p3 ( -83.9610 , 24.2303 , 57.9310 );
	VPoint p4 ( -89.5961 , 17.1579 , 60.0 );

	int coinc = 0;

	if ( CheckPwithP ( cor1 , p1 ) || CheckPwithP ( cor1 , p2 ) || CheckPwithP ( cor1 , p3 ) || CheckPwithP ( cor1 , p4 ) )
		coinc++;
	if ( CheckPwithP ( cor2 , p1 ) || CheckPwithP ( cor2 , p2 ) || CheckPwithP ( cor2 , p3 ) || CheckPwithP ( cor2 , p4 ) )
		coinc++;
	if ( CheckPwithP ( cor3 , p1 ) || CheckPwithP ( cor3 , p2 ) || CheckPwithP ( cor3 , p3 ) || CheckPwithP ( cor3 , p4 ) )
		coinc++;
	if ( CheckPwithP ( cor4 , p1 ) || CheckPwithP ( cor4 , p2 ) || CheckPwithP ( cor4 , p3 ) || CheckPwithP ( cor4 , p4 ) )
		coinc++;
	
	if ( coinc == 4 )
		return true;

	return false;
}

#endif //__DebugHelper_h__