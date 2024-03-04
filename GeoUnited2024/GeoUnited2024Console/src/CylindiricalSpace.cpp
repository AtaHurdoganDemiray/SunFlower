// CylindiricalSpace.cpp

#include "stdafx.h"
#include "CylindiricalSpace.h"
#include "PolarCalc2d.h"

CylindiricalSpace::CylindiricalSpace(const CoordinateSystem &cs , double r , double h)
: m_cs (cs) , m_r (r) , m_h (h)
{

}

const CoordinateSystem & CylindiricalSpace::GetCs()const
{
	return m_cs;
}

double CylindiricalSpace::GetR()const
{
	return m_r;
}

void CylindiricalSpace::SetR(double r)
{
	m_r = r;
}

double CylindiricalSpace::GetH()const
{
	return m_h;
}

void CylindiricalSpace::SetH(double h)
{
	m_h = h;
}

VPoint CylindiricalSpace::Convert(const VPoint &p)
{
	VPoint tp = m_cs.AbsoluteCoordToRelativeCoord(p);
	double myr = sqrt (tp.x() * tp.x() + tp.y() * tp.y());
	double myangle = PolarCalc2d::GetPolarAngle(tp.x() , tp.y());
	tp.x (myangle);
	tp.y (myr);
	return tp;
}

VPoint CylindiricalSpace::ConvertThrowIfOut(const VPoint &p, double tol)
{
	VPoint tp = m_cs.AbsoluteCoordToRelativeCoord(p);
	if (tp.z() < -tol || tp.z() > m_h + tol)
		throw VException (( IDS_NotOnCylinder),1);

	double myr = sqrt (tp.x() * tp.x() + tp.y() * tp.y());
	if (fabs (myr - m_r) > tol)
		throw VException (( IDS_NotOnCylinder),1);
	
	double myangle = PolarCalc2d::GetPolarAngle(tp.x() , tp.y());
	tp.x(myangle);
	tp.y(myr);
	return tp;
}
