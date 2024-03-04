// CylindiricalSpace.h

#ifndef __CylindiricalSpace_h__
#define __CylindiricalSpace_h__

#include "SpaceConverter.h"
#include "CoordinateSystem.h"

class CylindiricalSpace : public SpaceConverter
{
public:
	CylindiricalSpace (const CoordinateSystem &cs , double r , double h);
	const CoordinateSystem & GetCs()const;
	void SetCs (const CoordinateSystem &cs);
	double GetR()const;
	void SetR (double r);
	double GetH()const;
	void SetH (double h);
	// point's x value is angle in degrees relative to cs x axis , y value is radius , z is height 
	virtual VPoint Convert (const VPoint &p);
	virtual VPoint ConvertThrowIfOut(const VPoint &p , double tol);
	bool IsPointOn (const VPoint &p , double tol)const;
protected:
	CoordinateSystem m_cs;
	double m_r;
	double m_h;
};

#endif