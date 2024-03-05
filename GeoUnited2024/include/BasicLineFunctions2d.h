// BasicLineFunctions2d.h

#ifndef __BasicLineFunctions2d_h__
#define __BasicLineFunctions2d_h__

#include "VPoint2d.h"
#include "VLine2d.h"

const vreal DISTANCETOL = 1.0e-6;
const vreal DISTANCETOL2 = 5.0e-12;
const vreal ONEMINUSDISTANCETOL = (1.0 - 1.0e-6);
const vreal ZEROPLUSDISTANCETOL = (0.0 + 1.0e-6);

class BasicLineFunctions2d
{
public:
	enum ispointonline2dinfo { POINT_ON_INFINITELINE
							, POINT_ON_LINE
							, POINT_NOT_ON_INFINITELINE };

	//BasicLineFunctions2d();
	//virtual ~BasicLineFunctions2d();
	
	static inline bool IsPointOnLine2d 	
	( const VPoint2d &linep , const VVector2d &linev , const VPoint2d &mypoint 								
	, BasicLineFunctions2d::ispointonline2dinfo &info )
	{
		VVector2d diff = mypoint - linep;
		VVector2d normedv = linev.Unit();
			
		VVector2d perptoline ( -normedv.y() , normedv.x() );
			
		if ( fabs(perptoline%diff) > DISTANCETOL ) 
		{
			info = POINT_NOT_ON_INFINITELINE;// point is not on infinite line
			return false;
		}

		vreal factor = normedv % diff;

		if ( factor<=1.0 && factor>=0.0 )
		{
			info = POINT_ON_LINE;
			return true;
		}

		info = POINT_ON_INFINITELINE;
		return false;	
	};

	static inline bool IsPointOnLine2d 	
	( const VPoint2d &linep , const VVector2d &linev , const VPoint2d &mypoint 								
	, vreal & s , BasicLineFunctions2d::ispointonline2dinfo &info )
	{
		VVector2d diff = mypoint - linep;
		vreal dotp = diff % linev;
		s = dotp / linev.GetLength2();
		
		info=BasicLineFunctions2d::POINT_NOT_ON_INFINITELINE;		
		// s^2 * linev.GetLength2() = diff.linev * s
		vreal l2 = dotp * s;
		vreal h2 = diff.GetLength2() - l2;	
		if ( fabs ( h2 ) > DISTANCETOL2 )
			return false;

		info=BasicLineFunctions2d::POINT_ON_INFINITELINE;			
		if ( s < 0.0 || s > 1.0 )
			return false;
		
		info=BasicLineFunctions2d::POINT_ON_LINE;		
		return true;
	};

	static inline bool IsPointOnLine2d 	
	( const VPoint2d &linep , const VVector2d &linev , const VPoint2d &mypoint 								
	, vreal & s , BasicLineFunctions2d::ispointonline2dinfo &info , const vreal & offsettol )
	{
		VVector2d diff = mypoint - linep;
		vreal dotp = diff % linev;
		s = dotp / linev.GetLength2();
		
		info=BasicLineFunctions2d::POINT_NOT_ON_INFINITELINE;		
		// s^2 * linev.GetLength2() = diff.linev * s
		vreal l2 = dotp * s;
		vreal h2 = diff.GetLength2() - l2;	
		if ( fabs ( h2 ) > (DISTANCETOL+offsettol)*(DISTANCETOL+offsettol) )
			return false;

		info=BasicLineFunctions2d::POINT_ON_INFINITELINE;			
		
		// s domain tol: 
		// vlength = 2 , tolreal = 1 , tol_in_s = 1/2 = 0.5
		// tol_in_s = tolreal / vlength
		// tol_in_s ^ 2 = tolreal^2 / vlength^2
		vreal tol_in_s2 = offsettol*offsettol / linev.GetLength2();

		if ( s < -tol_in_s2 || s > 1.0+tol_in_s2 )
			return false;
		
		info=BasicLineFunctions2d::POINT_ON_LINE;		
		return true;
	};


	static inline VPoint2d BasicLineFunctions2d::GetPointOnLine2d 
	( const VPoint2d &bp , const VVector2d &lv , const vreal &intparam ) 
	{
		VPoint2d temp = lv * intparam;
		temp += bp;
		return temp;
	};

	static inline vreal BasicLineFunctions2d::GetDist2BetwLineAndPoint2d 
		( const VLine2d &myline , const VPoint2d &mypoint )
	{
		vreal dist2;

		// dist to linepoint 1
		VVector2d diff1 = mypoint - myline.GetP1();
		vreal mag2diff1 = diff1.GetLength2();
		dist2 = mag2diff1;
		//touchp = 1;
		vreal temp;

		// infinite line point dist
		VVector2d linevec = myline.GetV();
		vreal linemag = linevec.GetLength();
		VVector2d linevecunit = linevec / linemag;
		temp = diff1 % linevecunit;
		
		if ( temp >= 0.0 && temp <= linemag )
		{
			dist2 = ( mag2diff1 - temp * temp );
			//touchp = 0;
			return dist2;
		}

		// dist to linepoint 2
		VVector2d diff2 = mypoint - myline.GetP2();
		temp = diff2.GetLength2();

		if ( temp < dist2 )
		{	
			dist2 = temp;
			//touchp = 2;
		}

		return dist2;		
	};

	static inline bool IsPointOnInfiniteLine2d 
	( const VPoint2d &linep1 , const VPoint2d &linep2 , const VPoint2d &mypoint )
	{
		VVector2d vec1 = mypoint - linep1;
		VVector2d vec2 = mypoint - linep2;
		vreal checkmg = vec1 ^ vec2;
		if ( fabs(checkmg) < EPSILON )
			return true;

		return false;
	};
	
	static inline bool IsPointOnInfiniteLine2d 
	( const VPoint2d &linep1 , const VPoint2d &edgevec , const VPoint2d &mypoint , const vreal & offsettol )
	{
		VVector2d vec1 = edgevec;
		vec1.NormThis();
		VVector2d vec2 = mypoint - linep1;
		vreal checkmg = vec1 ^ vec2;
		if ( fabs(checkmg) < offsettol )
			return true;

		return false;
	};
};

#endif//__BasicLineFunctions_h__