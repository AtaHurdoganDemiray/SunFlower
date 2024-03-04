// ToroidalSurfacePointGenerator.h

#ifndef __ToroidalSurfacePointGenerator_h__
#define __ToroidalSurfacePointGenerator_h__

#include "SurfacePointGenerator.h"
#include "CoordinateSystem.h"

class ToroidalSurfacePointGenerator : public SurfacePointGenerator
{
public:
	ToroidalSurfacePointGenerator(const ToroidalSurface &surf , IgesEntitySet &entset);
	virtual VPoint GetPoint(const vreal & u, const vreal & v) const;
	virtual VPoint GetNormal(const vreal & u, const vreal & v, const VPoint & surfpoint) const;
	virtual void GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const;
	virtual void GetUDefIntervals(DoubleVec & defint) const;
	virtual void GetVDefIntervals(DoubleVec & defint) const;
	virtual VRefTriangleMesh * Digitise(const vreal &tol) const;
private:
	/*mutable*/ const ToroidalSurface& m_surf;
	IgesEntitySet &m_entset;
	CoordinateSystem m_cs;
public:
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	virtual IgesEntity * GetIgesEntity();
};

#endif