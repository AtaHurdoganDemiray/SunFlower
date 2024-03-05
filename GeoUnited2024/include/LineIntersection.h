// LineIntersection.h
#ifndef __LineIntersection_h__
#define __LineIntersection_h__

#include "VPoint.h"
#include "InfiniteLinesNearestPoints.h"
#include "BasicLineFunctions.h"


class LineIntersection : public BasicLineFunctions , public InfiniteLinesNearestPoints
{
public:
	enum intersectiontype 
					{ NOINTERSECTION 
					, INFINITELINESINTERSECT
					, LINESINTERSECT 
					, LINESPARALLEL_APART 
					, LINESPARALLEL_SAMELINE_SEPERATE 
					, LINESPARALLEL_INTERSECTING };
	
	intersectiontype LinesIntersect 
		( const VPoint &p1 , const VPoint &v1 , const VPoint &p2 , const VPoint &v2 
		, VPoint &intpoint , VPoint & parallelint , double coinTol = EPSILON)
	{
		
		if (v1.Compare(VPoint(0.0), coinTol))
		{
			LogFileWriter::WriteUnknownExceptionLog("LineIntersection::LinesIntersect "," a degenerate line comes , 0 length , no intersection returned");
			return NOINTERSECTION;
		}
		if (v2.Compare(VPoint(0.0), coinTol))
		{
			LogFileWriter::WriteUnknownExceptionLog("LineIntersection::LinesIntersect ", " a degenerate line comes , 0 length , no intersection returned");
			return NOINTERSECTION;
		}

		intersectiontype returnvalue;
		vreal int_t , int_s;
		info myintinfo = 
			Calc ( p1 , v1 , p2 , v2 , int_t , int_s );

		if ( myintinfo == LINESPARALLEL )
		{	
			returnvalue = ParallelLinesIntersect ( p1 , v1 , p2 , v2 , intpoint , parallelint );
			
			return	returnvalue;
		}

		if ( int_t <= 1.0+EPSILON && int_t >= -EPSILON
			&&	int_s <= 1.0+EPSILON && int_s >= -EPSILON )
		{
			// Check if the nearest points are coincident.
			VPoint nrstp1 = GetPointOnLine ( p1 , v1 , int_t );
			VPoint nrstp2 = GetPointOnLine ( p2 , v2 , int_s );

			if ( nrstp1.Compare ( nrstp2 , coinTol ) )
			{
				intpoint = nrstp1;
				returnvalue = LINESINTERSECT;
			}
			else
			{
				returnvalue = NOINTERSECTION;
			}
			return returnvalue;
		}
			
		returnvalue = NOINTERSECTION;
		return returnvalue;
	}
			

	intersectiontype ParallelLinesIntersect ( const VPoint &p1 , const VPoint &v1 
									, const VPoint &p2 , const VPoint &v2 
									, VPoint & intstart , VPoint & intend )
	{
		intersectiontype returnvalue = LINESPARALLEL_INTERSECTING;
		
		ispointonlineinfo myinfo;
		
		bool ck = IsPointOnLine ( p1 , v1 , p2 , myinfo );
		bool intr = false;
		if ( ck )
		{
			intr = true;
			intstart = p2;
		}

		VPoint tmp = p2 + v2;
		ck = IsPointOnLine ( p1 , v1 , tmp , myinfo );
		if ( ck )
		{
			if ( intr )
				intend = tmp;
			else
			{
				intr = true;
				intstart = tmp;
			}
		}
				
		ck = IsPointOnLine ( p2 , v2 , p1 , myinfo );
		if ( ck )
		{
			if ( intr )
				intend = p1;
			else
			{
				intr = true;
				intstart = p1;
			}
		}

		tmp = p1 + v1;
		ck = IsPointOnLine ( p2 , v2 , tmp , myinfo );
		if ( ck )
		{
			if ( intr )
				intend = tmp;
			else
			{
				//throw (VException("line intersection exception!", 0));
				LogFileWriter::WriteUnknownExceptionLog("LineIntersection::LinesIntersect ", " a degenerate line comes , 0 length , no intersection returned");
				return NOINTERSECTION;
			}
		}

		if ( intr )
			return returnvalue;

		if ( myinfo == POINT_ON_INFINITELINE )
			returnvalue = LINESPARALLEL_SAMELINE_SEPERATE;
		else if ( myinfo == POINT_NOT_ON_INFINITELINE )
			returnvalue = LINESPARALLEL_APART;
		
		return returnvalue;

		//intersectiontype returnvalue = LINESPARALLEL_INTERSECTING;

		//VPoint p12 = p1 + v1;

	}
	
	void ParallelIntersectingLinesInt ( VPoint & intstart , VPoint & intend ) const
	{
		

	}
private:

};

#endif // __LineIntersection_h__