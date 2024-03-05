// PlaneSurfacePointGenerator.h

#ifndef __PlaneSurfacePointGenerator_h__
#define __PlaneSurfacePointGenerator_h__

#include "SurfacePointGenerator.h"
class PlaneSurface;
#include "CoordinateSystem.h"
class SequentialLineSet;

class PlaneSurfacePointGenerator : public SurfacePointGenerator
{
public:
	PlaneSurfacePointGenerator(const PlaneSurface &surf,IgesEntitySet &entset);
	virtual VPoint GetPoint(const vreal & u, const vreal & v) const;
	virtual VPoint GetNormal(const vreal & u, const vreal & v, const VPoint & surfpoint) const;
	virtual void GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const;
	virtual void GetUDefIntervals(DoubleVec & defint) const;
	virtual void GetVDefIntervals(DoubleVec & defint) const;
	virtual VRefTriangleMesh * Digitise(const vreal &tol) const;

private:
	/*mutable*/ const PlaneSurface& m_surf;
	IgesEntitySet &m_entset;
	CoordinateSystem m_cs;
public:
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	void SetMinMax(const SequentialLineSet &outerLoop);
	virtual IgesEntity * GetIgesEntity();
};

#endif