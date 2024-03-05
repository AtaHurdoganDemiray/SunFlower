// RectangleSpace.h

#ifndef __RectangleSpace_h__
#define __RectangleSpace_h__

#include "SpaceConverter.h"
#include "CoordinateSystem.h"

class RectangleSpace : public SpaceConverter
{
public:
	// Rectangle's lower left point is the origin of m_cs
	RectangleSpace (const CoordinateSystem &cs , double xlen , double ylen , double zlen);
	const CoordinateSystem &GetCs()const;
	void SetCs (const CoordinateSystem &cs);
	double GetXLen()const;
	void SetXLen (double xlen);
	double GetYLen ()const;
	void SetYLen (double ylen);
	double GetZLen ()const;
	void SetZLen (double zlen);
	// point x and y values are scaled such that 0 is at origin 1 is at m_xlen for u and m_ylen for v , z is not scaled
	virtual VPoint Convert (const VPoint &p);
	virtual VPoint ConvertThrowIfOut(const VPoint &p , double tol);
	bool IsPointIn (const VPoint &p , double tol)const;
protected:
	CoordinateSystem m_cs;
	double m_xlen , m_ylen , m_zlen;
};

#endif