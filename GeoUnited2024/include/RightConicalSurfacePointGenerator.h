// RightConicalSurfacePointGenerator.h

#ifndef __RightConicalSurfacePointGenerator_h__
#define __RightConicalSurfacePointGenerator_h__

#include "SurfacePointGenerator.h"

class RightCircularConicalSurface;
class IgesEntitySet;
#include "CoordinateSystem.h"
class SequentialLineSet;

class RightConicalSurfacePointGenerator : public SurfacePointGenerator
{
public:
	RightConicalSurfacePointGenerator(const RightCircularConicalSurface &surf,IgesEntitySet &entset);
	virtual VPoint GetPoint(const vreal & u, const vreal & v) const;
	virtual VPoint GetNormal(const vreal & u, const vreal & v, const VPoint & surfpoint) const;
	virtual void GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const;
	virtual void GetUDefIntervals(DoubleVec & defint) const;
	virtual void GetVDefIntervals(DoubleVec & defint) const;
	virtual VRefTriangleMesh * Digitise(const vreal &tol) const;
private:
	/*mutable*/ const RightCircularConicalSurface& m_surf;
	IgesEntitySet &m_entset;
	CoordinateSystem m_cs;
	vreal m_radius;
	vreal m_angleInDegrees;
	vreal m_H;
	vreal m_mnr;
	vreal m_angleInRad;
public:
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	void DecideH(const SequentialLineSet &outerloop);
	virtual IgesEntity * GetIgesEntity();
};

#endif