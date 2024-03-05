// VArc2d.cpp

#include "stdafx.h"
#include "VArc2d.h"
#include "LinearEquations.h"
#include "PolarCalc2d.h"

VArc2d::VArc2d()
: center (0.0,0.0),radius(1.0),startangle(0.0),endangle(360.0)
{
	
}

VArc2d::VArc2d ( const VPoint2d & _center , const vreal & _radius , const vreal & _startangle , const vreal & _endangle )
:center(_center),radius(_radius),startangle(_startangle),endangle(_endangle)
{

}

VArc2d::VArc2d (const VPoint2d &_center , double _radius , const VPoint2d &p0 , const VPoint2d &p1 , bool isCCW)
: center (_center) , radius (_radius)
{
	VPoint2d dp0 = p0 - center;
	VPoint2d dp1 = p1 - center;
	double a0 = PolarCalc2d::GetPolarAngle (dp0.x() , dp0.y());
	double a1 = PolarCalc2d::GetPolarAngle (dp1.x() , dp1.y());

	if (fabs (a1) < 1.e-8)
		a1 = 360.0;
	if (isCCW)
	{
		startangle = a0;
		endangle = a1;
	}
	else
	{
		startangle = a1;
		endangle = a0;
	}
}

VArc2d::VArc2d ( const VArc2d & Foo )
{
	(*this) = Foo;
}

VArc2d & VArc2d::operator = ( const VArc2d & Foo )
{
	center = Foo.center;
	radius = Foo.radius;
	startangle = Foo.startangle;
	endangle = Foo.endangle;
	return (*this);
}

const VPoint2d & VArc2d::GetCenter() const
{
	return center;
}

const vreal & VArc2d::GetRadius() const
{
	return radius;
}

const vreal & VArc2d::GetStartAngle() const
{
	return startangle;
}

const vreal & VArc2d::GetEndAngle() const
{
	return endangle;
}

bool VArc2d::operator == ( const VArc2d & comp ) const
{
	if ( center == comp.center
		&& radius == comp.radius
		&& startangle == comp.startangle
		&& endangle == comp.endangle )
		return true;
	else
		return false;
}

bool VArc2d::operator != ( const VArc2d & comp ) const
{
	return !( (*this) == comp );
}

bool VArc2d::IsFull()const
{
	if (fabs (endangle - startangle) < EPSILON || fabs(fabs (endangle - startangle)-360.0) < EPSILON)
		return true;
	else 
		return false;
}

bool VArc2d::Calculate (const vreal x1
						,const vreal y1
						,const vreal x2
						,const vreal y2
						,const vreal x3
						,const vreal y3
						, vreal &solutionx
						, vreal &solutiony)
{
	vreal a1 , b1 , c1 , a2 , b2 , c2;
	a1 = 2 * ( x2 - x1 );
	b1 = 2 * ( y2 - y1 );
	c1 = x1 * x1 + y1 * y1 - ( x2 * x2 + y2 * y2 );

	a2 = 2 * ( x3 - x1 );
	b2 = 2 * ( y3 - y1 );
	c2 = x1 * x1 + y1 * y1 - ( x3 * x3 + y3 * y3 );
	
	if ( !LinearEquations::SolveLinear2Unknown ( a1 , b1 , c1 , a2 , b2 , c2 , solutionx , solutiony ))
	{
		a1 = 2 * ( x1 - x2 );
		b1 = 2 * ( y1 - y2 );
		c1 = x2 * x2 + y2 * y2 - ( x1 * x1 + y1 * y1 );

		a2 = 2 * ( x3 - x2 );
		b2 = 2 * ( y3 - y2 );
		c2 = x2 * x2 + y2 * y2 - ( x3 * x3 + y3 * y3 );

		if ( !LinearEquations::SolveLinear2Unknown 
			( a1 , b1 , c1 , a2 , b2 , c2 , solutionx , solutiony ))
		{
			a1 = 2 * ( x2 - x3 );
			b1 = 2 * ( y2 - y3 );
			c1 = x3 * x3 + y3 * y3 - ( x2 * x2 + y2 * y2 );

			a2 = 2 * ( x1 - x3 );
			b2 = 2 * ( y1 - y3 );
			c2 = x3 * x3 + y3 * y3 - ( x1 * x1 + y1 * y1 );
		
			if ( !LinearEquations::SolveLinear2Unknown 
			( a1 , b1 , c1 , a2 , b2 , c2 , solutionx , solutiony ))
				return false;
		}
	}
	
	return true;
}

bool VArc2d::IsAngleInArc (double angle)const
{
	// if full arc return true
	if (fabs (fabs (endangle - startangle) - 360) < EPSILON)
		return true;

	PolarCalc2d::CastAngleTo360presc6(angle); // Make angle between [0-360]
	if (angle < 0.0)
		angle += 360.0;	// Make angle positive	
	bool isout;
	double smallangle , bigangle;
	if (startangle < endangle)
	{
		smallangle = startangle;
		bigangle = endangle;
		isout = false;
	}
	else
	{
		smallangle = endangle;
		bigangle = startangle;
		isout = true;
	}
	if ( angle < smallangle || angle > bigangle)
		return isout;
	else
		return !isout;
}

VPoint2d VArc2d::GetPoint (vreal angle) const
{
	double rada = angle * DEGREE_TO_RAD;
	double x = center.x() + radius * cos (rada);
	double y = center.y() + radius * sin (rada);
	return VPoint2d (x,y);
}

VPoint2d VArc2d::GetStartPoint() const
{
	return GetPoint(startangle);
}

VPoint2d VArc2d::GetEndPoint() const
{
	return GetPoint(endangle);
}
