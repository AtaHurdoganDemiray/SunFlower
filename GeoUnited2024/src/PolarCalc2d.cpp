// PolarCalc2d.cpp
#include "stdafx.h"
#include "PolarCalc2d.h"
#include "BasicDefinitions.h"
#include "VPoint2d.h"
#include "VInterval.h"

vreal PolarCalc2d::GetPolarAngle ( const VPoint2d &originp , const VPoint2d &secondp )
{
	VPoint2d difference = secondp - originp;
	vreal polarangle = GetPolarAngle ( difference.x() , difference.y() );
	return polarangle;
};

vreal PolarCalc2d::GetPolarDistance ( const VPoint2d &originp , const VPoint2d &secondp )
{
	VPoint2d difference = secondp - originp;

	return ( difference.GetLength() );

};

vreal PolarCalc2d::GetPolarDistance2 ( const VPoint2d &originp , const VPoint2d &secondp )
{
	VPoint2d difference = secondp - originp;

	return ( difference.GetLength2() );
};

vreal PolarCalc2d::GetCCWAngle ( const vreal &startpolarangle , const vreal &endpolarangle )
{
	vreal difference = endpolarangle - startpolarangle;
	vreal ccwangle;

	if ( difference < -EPSILON )
		ccwangle = 360.0 + difference;
	else
		ccwangle = difference;

	return ccwangle;
};

vreal PolarCalc2d::GetCWAngle ( const vreal &startpolarangle , const vreal &endpolarangle )
{
	vreal difference = startpolarangle - endpolarangle;
	vreal cwangle;

	if ( difference < -EPSILON )
		cwangle = 360.0 + difference;
	else
		cwangle = difference;

	return cwangle;
};

vreal PolarCalc2d::GetPolarAngle ( const vreal &xval , const vreal &yval )
{
	if ( fabs(xval) < EPSILON )
	{
		if (fabs(yval) < EPSILON)
			throw VException (( IDS_PolGetExcXAndYI),1);
		if ( yval > 0.0 )
			return 90.0;
		else
			return (270.0);
	}
	if( fabs(yval) < EPSILON )
	{
		//if (fabs(xval) < EPSILON)
		//	throw VException (("PolarCalc2d::GetPolarAngle exception , x and y is 0"),1);
		if( xval > 0.0 )
			return 0.0;
		else
			return 180.0;
	}

	vreal radianangle = atan2 (fabs(yval) , fabs(xval));
	vreal angleindegrees = radianangle * RAD_TO_DEGREE;
	
	vreal polarangle(0.0);
	
	if ( xval >= 0.0 && yval >= 0.0 )
		polarangle = angleindegrees;
	else if ( xval >= 0.0 && yval <= 0.0 )
		polarangle = 360.0 - angleindegrees;
	else if ( xval <= 0.0 && yval >= 0.0 )
		polarangle = 180.0 - angleindegrees;
	else if ( xval <= 0.0 && yval <= 0.0 )
		polarangle = 180.0 + angleindegrees;

	return polarangle;
};

vreal PolarCalc2d::Inverse ( const vreal &polarangle )
{
	vreal returnvalue;

	if ( polarangle < 180.0 )
		returnvalue = polarangle + 180.0;
	else
		returnvalue = polarangle - 180.0;
	
	return returnvalue;
};

PolarCalc2d::BetweenAnglesInfo PolarCalc2d::IsAngleBetween2Angles ( const vreal &startangle , const vreal &endangle , const vreal &checkangle )
{
	BetweenAnglesInfo betweenanglesinfo = OUTSIDE;

	vreal ccwangle = GetCCWAngle ( startangle , endangle );
	vreal checkccw = GetCCWAngle ( startangle , checkangle );
	
	if ( ccwangle < 180.0 )
	{	
		if ( checkccw < ccwangle && checkccw > 0 )
			betweenanglesinfo = BETWEEN;
		else
			betweenanglesinfo = OUTSIDE;
		
		if ( fabs(checkccw-ccwangle) < EPSILON )
			betweenanglesinfo = ONEND;
		if ( fabs(checkccw) < EPSILON )
			betweenanglesinfo = ONSTART;	
	}
	else
	{
		if ( checkccw < ccwangle && checkccw > 0 )
			betweenanglesinfo = OUTSIDE;
		else
			betweenanglesinfo = BETWEEN;
		
		if ( fabs(checkccw-ccwangle) < EPSILON )
			betweenanglesinfo = ONEND;
		if ( fabs(checkccw) < EPSILON )
			betweenanglesinfo = ONSTART;
	
	}
	
	return betweenanglesinfo;

};

double vp_asin (double val)
{
	if (val > 1.)
	{
		if (val-1.0 < 1.e-8)
			val = 1.;
		else
			throw VException (( IDS_VpAsiExcValIsGr),1);
	}
	else if (val < -1.0)
	{
		if (1.-val < 1.e-8)
			val = -1.;
		else
			throw VException (( IDS_VpAsiExcValIsSm),1);
	}
	return asin (val);
}

double vp_acos (double val)
{
	if (val > 1.)
	{
		if (val-1.0 < 1.e-8)
			val = 1.;
		else
			throw VException (( IDS_VpAcoExcValIsGr),1);
	}
	else if (val < -1.0)
	{
		if (1.+val < -1.e-8)
			val = -1.;
		else
			throw VException (( IDS_VpAcoExcValIsSm),1);
	}
	return acos (val);
}

double PolarCalc2d::GetAbsShortestAngleBtw (double angle1 , double angle2)
{
	double da1 = fabs (angle2 - angle1);
	if (da1 > 360)
	{
		int da1i = (int) (da1 * 1.e5);
		da1i = da1i % 36000000;
		da1 = da1i * 1.e-5;
	}
	if (da1 > 180)
	{
		da1 = 360 - da1;
	}
	return da1;
}

void PolarCalc2d::CastAngleTo360presc6 (double &angle)
{
	if (fabs(angle) > 360)
	{
		int anglei = (int) (angle * 1.e6);
		int anglemod360 = anglei % 360000000;
		angle = anglemod360 * 1.e-6;
	}
}

int PolarCalc2d::GetQuadrantFor (vreal angle)
{
	if (angle >= 0.0 && angle < 90.0)
		return 0;
	else if (angle >= 90.0 && angle < 180.0)
		return 1;
	else if (angle >= 180.0 && angle < 270.0)
		return 2;
	else if (angle >= 270.0 && angle <= 360.0)
		return 3;
	else
		throw VException ("PolarCalc2d::GetQuadrantFor exception , angle is out of [0,360]",1);
}
	
VInterval PolarCalc2d::GetQuadrantAngles (int qno)
{
	if (qno == 0)
	{
		return VInterval (0.0,90.0);
	}
	if (qno == 1)
	{
		return VInterval (90.0,180.0);
	}
	if ( qno == 2)
	{
		return VInterval (180.0,270.0);
	}
	if ( qno == 3)
	{
		return VInterval (270.0,360.0);
	}
	throw VException ("PolarCalc2d::GetQuadrantAngles exception , qno out of range [0,3]",1);
}

void PolarCalc2d::UnwrapArcToLineInterval (vreal startAngle , vreal sweepAngle , std::list <VSignedInterval> &unwrappedArcs)
{
	if (startAngle < 0.0 || startAngle > 360.0)
		throw VException ("PolarCalc2d::UnwrapArcToLineInterval exception , start angle must be between 0.0 and 360.0",1);
	if (sweepAngle > 360.0 || sweepAngle < -360.0)
		throw VException ("PolarCalc2d::UnwrapArcToLineInterval exception , sweep angle must be between -360 and 360.0",1);
		
	vreal endAngle = startAngle + sweepAngle;

	if (sweepAngle > 0.0)
	{
		if (endAngle > 360.0)
		{
			VInterval i1 (startAngle , 360.0); 
			VInterval i2 (0.0, endAngle - 360.0);
			int signF = 1;
			VSignedInterval si1 (i1,signF);
			unwrappedArcs.push_back (si1);
			VSignedInterval si2 (i2,signF);
			unwrappedArcs.push_back (si2);
		}
		else
		{
			VInterval i1 (startAngle , endAngle);
			int signF = 1;
			VSignedInterval si1 (i1,signF);
			unwrappedArcs.push_back (si1);
		}
	}
	else
	{
		if (endAngle < 0)
		{
			VInterval i1 ( 0.0 , startAngle);
			//VInterval i1 (startAngle , 0.0);
			VInterval i2 (360.0 + endAngle , 360);
			//VInterval i2 (360.0 , 360.0 + endAngle);
			int signF =  -1;
			VSignedInterval si1 (i1,signF);
			VSignedInterval si2 (i2,signF);
			unwrappedArcs.push_back (si1);
			unwrappedArcs.push_back (si2);
		}
		else
		{
			VInterval i1 (endAngle , startAngle);
			//VInterval i1 (startAngle , endAngle);

			int signF = -1;
			VSignedInterval si1 (i1,signF);
			unwrappedArcs.push_back (si1);
		}
	}
}