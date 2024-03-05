// PlaneEntityPointGenerator.h

#ifndef __PlaneEntityPointGenerator_h__
#define __PlaneEntityPointGenerator_h__
#include "SurfacePointGenerator.h"
#include "CoordinateSystem.h"

class PlaneEntity;
class IgesEntitySet;
class SequentialLineSet;

class PlaneEntityPointGenerator:public SurfacePointGenerator
{
public:
	PlaneEntityPointGenerator ( const PlaneEntity &plnent , const IgesEntitySet &entset);
	virtual ~PlaneEntityPointGenerator();
	virtual VPoint GetPoint ( const vreal & u , const vreal & v ) const;
	virtual VPoint GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const;
	virtual void GetPointAndNormal (const vreal &u , const vreal &v , VPoint &m , VPoint &n)const;
	virtual void GetUDefIntervals ( DoubleVec & defint ) const;
	virtual void GetVDefIntervals ( DoubleVec & defint ) const;
	virtual VRefTriangleMesh * Digitise (const vreal &tol) const;
	void ResetRectangle (const vreal &_u0 , const vreal & _u1 , const vreal &_v0 , const vreal & _v1);
	const CoordinateSystem & GetPlaneCS ()const;
	CoordinateSystem & GetPlaneCS ();
	VPoint2d GetNormalizedPlaneCoord (const VPoint &spaceCoord)const;
private:
	/*mutable*/ const PlaneEntity& m_plnent;
	CoordinateSystem m_cs;
	vreal m_DNormed;
	VPoint m_normal;
	VPoint m_pointonplane;
	VPoint m_origin;
	VPoint m_minInCs;
	VPoint m_maxInCs;
	VPoint m_deltaMnMx;
public:
	void SetMinMax(const SequentialLineSet &outerLoop);
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	virtual IgesEntity * GetIgesEntity();
};

#endif