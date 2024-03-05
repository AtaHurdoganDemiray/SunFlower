// LineIntersection2d.h
#ifndef __LineIntersection2d_h__
#define __LineIntersection2d_h__

#include "VPoint2d.h"
#include "BasicLineFunctions2d.h"


class LineIntersection2d : public BasicLineFunctions2d
{
public:
	enum info { GENERALCASE , LINESPARALLEL };
	enum intersectiontype2d 
					{ INFINITELINESINTERSECT
					, LINESINTERSECT  
					, LINESPARALLEL_APART
					//, LINESPARALLEL_SAMELINE_NOINTERSECTION 
					, LINESPARALLEL_INTERSECTING };
	
	struct LineIntersectionData2d
	{
		intersectiontype2d intinfo;
		int intno;
		VPoint2d intpoint[2];
		vreal sofline1[2];
		vreal sofline2[2];
	};

	static info CoreCalc2d ( const VPoint2d &p1 , const VVector2d &v1 
							, const VPoint2d &p2 , const VVector2d &v2 
							, vreal &int_t , vreal &int_s )
	{
		info returnvalue;
		vreal mydivisor = v1.x()*v2.y() - v2.x()*v1.y();

		if ( fabs ( mydivisor ) < EPSILON )
		{
			returnvalue = LINESPARALLEL;
			return returnvalue;
		}
		
		int_t = (-v2.y()*p2.x() + v2.y()*p1.x() + v2.x()*p2.y() - v2.x()*p1.y())
									/ (-mydivisor);
		
		int_s = ( v1.y()*p2.x() - v1.y()*p1.x() - v1.x()*p2.y() + v1.x()*p1.y())
									/ mydivisor ;
		returnvalue = GENERALCASE;
		return returnvalue;

	}

	static intersectiontype2d LinesIntersect2d ( const VPoint2d &p1 , const VVector2d &v1 
							, const VPoint2d &p2 , const VVector2d &v2 
							, VPoint2d &intpoint , VPoint2d &parallelint )
	{
		intersectiontype2d returnvalue;
		vreal int_t , int_s;

		info ck = CoreCalc2d ( p1 , v1 , p2 , v2 , int_t , int_s );

		if ( ck == LineIntersection2d::LINESPARALLEL )
		{
			vreal sp[4];
			returnvalue = HandleParallelLines2d ( p1 , v1 , p2 , v2 , intpoint , parallelint , sp );
			return returnvalue;
		}

		intpoint = GetPointOnLine2d ( p1 , v1 , int_t );

		if ( int_t <= 1.0+EPSILON /*ONEMINUSDISTANCETOL*/ && int_t >=0.0-EPSILON /*ZEROPLUSDISTANCETOL*/ 
			&& int_s <=1.0+EPSILON /*ONEMINUSDISTANCETOL*/ && int_s >=0.0-EPSILON /*ZEROPLUSDISTANCETOL*/ )
		{
			returnvalue = LineIntersection2d::LINESINTERSECT;
			return returnvalue;
		}

		returnvalue = LineIntersection2d::INFINITELINESINTERSECT;
		return returnvalue;
	}

	static intersectiontype2d LinesIntersect2d ( const VPoint2d &p1 , const VVector2d &v1 
							, const VPoint2d &p2 , const VVector2d &v2 
							, VPoint2d &intpoint , VPoint2d & parallelint , vreal &t , vreal &s )
	{
		intersectiontype2d returnvalue;
		
		info ck = CoreCalc2d ( p1 , v1 , p2 , v2 , t , s );

		if ( ck == LineIntersection2d::LINESPARALLEL )
		{
			vreal sp[4];
			returnvalue = HandleParallelLines2d ( p1 , v1 , p2 , v2 , intpoint , parallelint , sp );
			return returnvalue;
		}

		intpoint = GetPointOnLine2d ( p1 , v1 , t );

		if ( t <= 1.0+EPSILON /*ONEMINUSDISTANCETOL*/ && t >=0.0-EPSILON /*ZEROPLUSDISTANCETOL*/ 
			&& s <=1.0+EPSILON /*ONEMINUSDISTANCETOL*/ && s >=0.0-EPSILON /*ZEROPLUSDISTANCETOL*/ )
		{
			returnvalue = LineIntersection2d::LINESINTERSECT;
			return returnvalue;
		}

		returnvalue = LineIntersection2d::INFINITELINESINTERSECT;
		return returnvalue;
	}
		
	static intersectiontype2d HandleParallelLines2d 
	( const VPoint2d &p1 , const VVector2d &v1 
									, const VPoint2d &p2 , const VVector2d &v2 
									, VPoint2d & intstart , VPoint2d & intend 
									, vreal s[] )
	{
		bool ck;
		ispointonline2dinfo info;
		vreal sl;
		intersectiontype2d returnvalue = LineIntersection2d::LINESPARALLEL_INTERSECTING;
		
		bool intr = false;
		
		ck = IsPointOnLine2d ( p1 , v1 , p2 , sl , info);
		
		if ( info == BasicLineFunctions2d::POINT_NOT_ON_INFINITELINE )
		{
			returnvalue = LineIntersection2d::LINESPARALLEL_APART;
			return returnvalue;
		}
		else
		{
			returnvalue = LineIntersection2d::LINESPARALLEL_INTERSECTING;
			return returnvalue;
		}

		//s[2] = -1.0;	
		//if ( ck )
		//{
		//	intr = true;
		//	intstart = p2;
		//	s[0]=sl;
		//	s[1]=0.0;
		//}
		//		
		//VPoint2d tmp = p2 + v2;
		//ck = IsPointOnLine2d ( p1 , v1 , tmp , sl , info);
		//if ( ck )
		//{
		//	if ( intr )
		//	{
		//		intend = tmp;
		//		s[2]=sl;
		//		s[3]=1.0;
		//	}
		//	else
		//	{
		//		intr = true;
		//		intstart = tmp;
		//		s[0]=sl;
		//		s[1]=1.0;
		//	}
		//}

		//ck = IsPointOnLine2d ( p2 , v2 , p1 , sl , info);
		//
		//if ( ck )
		//{
		//	if ( intr )
		//	{
		//		intend = p1;
		//		s[2]=0.0;
		//		s[3]=sl;
		//	}
		//	else
		//	{
		//		intr = true;
		//		intstart = p1;
		//		s[0]=0.0;
		//		s[1]=sl;
		//	}
		//}

		//tmp = p1 + v1;
		//ck = IsPointOnLine2d ( p2 , v2 , tmp , sl , info );
		//if ( ck )
		//{
		//	if ( intr )
		//	{
		//		intend = tmp;
		//		s[2]=1.0;
		//		s[3]=sl;
		//	}
		//	else
		//	{
		//		intstart = tmp;
		//		s[0] = 1.0;
		//		s[1] = sl;
		//		//throw ( VException ( "line2d line2d intersection exception" , 0 ) );
		//		intend = tmp;
		//		s[2]=1.0;
		//		s[3]=sl;
		//	}
		//}

		//if ( intr )
		//{
		//	if ( s[2] == -1.0 )
		//	{
		//		intend = intstart;
		//		s[2] = s[0];
		//		s[3] = s[1];
		//		//throw ( VException ( "parallel line int exception" , 0 ));
		//	}
		//	return returnvalue;
		//}

		//returnvalue = LINESPARALLEL_SAMELINE_NOINTERSECTION;
		//return returnvalue;
	}

	static LineIntersectionData2d CalcInt ( const VLine2d & l1 , const VLine2d & l2 )
	{
		LineIntersectionData2d result;
		vreal t,s;
		intersectiontype2d inttype;
		
		VVector2d v1 = l1.GetV();
		VVector2d v2 = l2.GetV();	

		info ck = CoreCalc2d ( l1.GetP1() , v1 , l2.GetP1() , v2 , t , s );

		if ( ck == LineIntersection2d::LINESPARALLEL )
		{
			vreal sp[4];
			VPoint2d intpoint , parallelint;
			inttype = HandleParallelLines2d ( l1.GetP1() , v1 , l2.GetP1() , v2 , intpoint , parallelint , sp );
			result.intinfo = inttype;
			if ( inttype == LineIntersection2d::LINESPARALLEL_INTERSECTING )
			{
				result.intno = 2;
				result.intpoint[0] = intpoint;
				result.intpoint[1] = parallelint;
				result.sofline1[0] = sp[0];
				result.sofline1[1] = sp[2];
				result.sofline2[0] = sp[1];
				result.sofline2[1] = sp[3];
			}
			else
				result.intno = 0;

			return result;
		}

		VPoint2d intpoint = GetPointOnLine2d ( l1.GetP1() , v1 , t );

		if ( t <= 1.0+EPSILON /*ONEMINUSDISTANCETOL*/ && t >=0.0-EPSILON /*ZEROPLUSDISTANCETOL*/ 
			&& s <=1.0+EPSILON /*ONEMINUSDISTANCETOL*/ && s >=0.0-EPSILON /*ZEROPLUSDISTANCETOL*/ )
		{
			result.intinfo = LineIntersection2d::LINESINTERSECT;
			result.intno = 1;
			result.intpoint[0] = intpoint;	
			result.sofline1[0] = t;
			result.sofline2[0] = s;
			return result;
		}
		else
		{
			result.intinfo = LineIntersection2d::INFINITELINESINTERSECT;
			result.intno = 0;		
			return result;
		}
	};

private:

};

#endif // __LineIntersection2d_h__