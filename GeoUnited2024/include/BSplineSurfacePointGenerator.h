// BSplineSurfacePointGenerator.h

#ifndef __BSplineSurfacePointGenerator_h__
#define __BSplineSurfacePointGenerator_h__

#define TEST_GEN2 0


#include "BSplineKnots.h"
#include "BSplineControlPoints.h"
#include "BSplineWeights.h"
#include "SurfacePointGenerator.h"
#include "NurbsSurfEval.h"

const int NURBS_SURFACE_TESSELATION_POINTS_PER_DIR = 17;

class BSplineSurface;

class BSplineSurfacePointGenerator : public SurfacePointGenerator
{
public:
	BSplineSurfacePointGenerator ( const BSplineSurface & surf );
	virtual VPoint GetPoint ( const vreal & u , const vreal & v ) const;	
	virtual VPoint GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const;
	virtual void GetPointAndNormal (const vreal &u , const vreal &v , VPoint &m , VPoint &n)const;
	virtual void GetUDefIntervals ( DoubleVec & defint ) const;
	virtual void GetVDefIntervals ( DoubleVec & defint ) const;
	virtual VRefTriangleMesh * Digitise(const vreal &tol)const;
	NurbsSurfEval & GetNE();
protected:
	/*mutable*/ const BSplineSurface& m_surf;

#if TEST_GEN2
	VPoint TryGen2 (const vreal &u , const vreal &v)const;
#endif
#if USENN
	mutable NurbsSurfEval m_NE;
#else
	BSplineKnots knots_u , knots_v;
	BSplineControlPoints controlpoints;
	BSplineWeights weights;
#endif
public:
	//virtual bool Unproject(const VPoint &mp, vreal &u, vreal &v , vreal tol);
	virtual IgesEntity * GetIgesEntity();
};

#endif