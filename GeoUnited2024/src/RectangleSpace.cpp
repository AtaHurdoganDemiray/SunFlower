// RectangleSpace.cpp

#include "stdafx.h"
#include "RectangleSpace.h"

RectangleSpace::RectangleSpace(const CoordinateSystem &cs , double xlen , double ylen , double zlen)
:m_cs (cs) , m_xlen (xlen) , m_ylen (ylen) , m_zlen (zlen)
{

}

const CoordinateSystem & RectangleSpace::GetCs()const
{
	return m_cs;
}

void RectangleSpace::SetCs(const CoordinateSystem &cs)
{
	m_cs =cs;
}

double RectangleSpace::GetXLen() const
{
	return m_xlen;
}

void RectangleSpace::SetXLen(double xlen)
{
	m_xlen = xlen;
}

double RectangleSpace::GetYLen() const
{
	return m_ylen;
}

void RectangleSpace::SetYLen(double ylen)
{
	m_ylen = ylen;
}

double RectangleSpace::GetZLen()const
{
	return m_zlen;
}

void RectangleSpace::SetZLen(double zlen)
{
	m_zlen = zlen;
}

VPoint RectangleSpace::Convert(const VPoint &p)
{
	VPoint tp = m_cs.AbsoluteCoordToRelativeCoord(p);
	double u = tp.x() / m_xlen;
	double v = tp.y() / m_ylen;
	tp.x(u);
	tp.y(v);
	return tp;
}

VPoint RectangleSpace::ConvertThrowIfOut(const VPoint &p , double tol)
{
	VPoint tp = m_cs.AbsoluteCoordToRelativeCoord(p);
	if (tp.z() < -tol || tp.z() > m_zlen + tol)
		throw VException (( IDS_OutRec),1);
	if (tp.x() < -tol || tp.x() > m_xlen + tol)
		throw VException (( IDS_OutRec),1);
	if (tp.y() < -tol || tp.y() > m_ylen + tol)
		throw VException (( IDS_OutRec),1);
	double u = tp.x() / m_xlen;
	double v = tp.y() / m_ylen;
	tp.x(u);
	tp.y(v);
	return tp;
}
bool RectangleSpace::IsPointIn(const VPoint &p, double tol) const
{
	VPoint tp = m_cs.AbsoluteCoordToRelativeCoord(p);
	if (fabs (tp.z()) > -tol && tp.z() < m_zlen + tol)
	{
		if (tp.x() > -tol && tp.x() < m_xlen + tol)
		{
			if (tp.y() > -tol && tp.y() < m_ylen + tol)
				return true;
		}
	}
	return false;
}
