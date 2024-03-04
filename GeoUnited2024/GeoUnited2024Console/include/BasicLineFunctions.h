// BasicLineFunctions.h

#ifndef __BasicLineFunctions_h__
#define __BasicLineFunctions_h__

#include "VPoint.h"
#include "VLine.h"

class BasicLineFunctions
{
public:
	enum ispointonlineinfo { POINT_ON_INFINITELINE
							, POINT_ON_LINE
							, POINT_NOT_ON_INFINITELINE };

	BasicLineFunctions(){};
	virtual ~BasicLineFunctions(){};
	
	bool BasicLineFunctions::IsPointOnLine 	
	( const VPoint &linep , const VVector &linev , const VPoint &mypoint 
		, BasicLineFunctions::ispointonlineinfo &info )
	{
		VVector diff = mypoint - linep;
		//VVector normedv = linev.Unit();
			
		VVector perptoline = diff ^ linev;
			
		if ( perptoline.GetLength2() > 1.0e-16 ) 
		{
			info = POINT_NOT_ON_INFINITELINE;// point is not on infinite line
			return false;
		}

		vreal factor = (linev % diff)/linev.GetLength2();

		if ( factor<=1.0 && factor>=0.0 )
		{
			info = POINT_ON_LINE;
			return true;
		}

		info = POINT_ON_INFINITELINE;
		return false;	
	}

	VPoint BasicLineFunctions::GetPointOnLine 
	( const VPoint &bp , const VVector &lv , const vreal &intparam ) const
	{
		VPoint temp = lv * intparam;
		temp +=	bp;
		return temp;
	}

	vreal BasicLineFunctions::GetDist2BetwLineAndPoint 
		( const VLine &myline , const VPoint &mypoint )
	{
		vreal dist2;
		
		// dist to linepoint 1
		VPoint diff1 = mypoint - myline.GetP1();
		vreal mag2diff1 = diff1.GetLength2();
		dist2 = mag2diff1;
		vreal temp;

		// infinite line point dist
		VVector linevec = myline.GetV();
		vreal linemag = linevec.GetLength();
		VVector linevecunit = linevec / linemag;
		temp = diff1 % linevecunit;
		
		if ( temp > 0.0 && temp < linemag )
		{
			dist2 = ( mag2diff1 - temp * temp );
			return dist2;
		}

		// dist to linepoint 2
		VVector diff2 = mypoint - myline.GetP2();
		temp = diff2.GetLength2();

		if ( temp < dist2 )
		{	dist2 = temp;;
		}

		return dist2;
	}

};

#endif//__BasicLineFunctions_h__