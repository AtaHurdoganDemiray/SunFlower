// RightCylindricalSurfacePointGenerator.h

#ifndef __RightCylindricalSurfacePointGenerator_h__
#define __RightCylindricalSurfacePointGenerator_h__

#include "SurfacePointGenerator.h"
class RightCircularCylindricalSurface;
class IgesEntitySet;
#include "CoordinateSystem.h"
class SequentialLineSet;

class RightCylindricalSurfacePointGenerator : public SurfacePointGenerator
{
public:
	RightCylindricalSurfacePointGenerator(const RightCircularCylindricalSurface &e , IgesEntitySet &entset);
	virtual VPoint GetPoint(const vreal & u, const vreal & v) const;
	virtual VPoint GetNormal(const vreal & u, const vreal & v, const VPoint & surfpoint) const;
	virtual void GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const;
	virtual void GetUDefIntervals(DoubleVec & defint) const;
	virtual void GetVDefIntervals(DoubleVec & defint) const;
	virtual VRefTriangleMesh * Digitise(const vreal &tol) const;
private:
	/*mutable*/ const RightCircularCylindricalSurface& m_surf;
	IgesEntitySet &m_entset;
	CoordinateSystem m_cs;
	vreal m_radius;
	vreal m_H;
public:
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	void SetH(vreal H);
	vreal GetH()const;
	void DecideH(const SequentialLineSet &outerLoop);
	virtual IgesEntity * GetIgesEntity();
};

#endif