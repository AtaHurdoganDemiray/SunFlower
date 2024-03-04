// BSplineSurfacePointGenerator2.h
#if !USENN
#ifndef __BSplineSurfacePointGenerator2_h__
#define __BSplineSurfacePointGenerator2_h__
#include "BSplineControlPoints.h"
#include "BSplineSurfacePointGenerator.h"
#include "DoubleVec.h"

class BSplineSurface;
class BSplineControlPoints;
struct DoubleVec;
struct VPoint;
class BSplineCurve;
class BSplineCurvePointGenerator;
class PointSet2d;

class BSplineSurfacePointGenerator2:public BSplineSurfacePointGenerator
{
public:
	BSplineSurfacePointGenerator2 (const BSplineSurface &surf);
	virtual ~BSplineSurfacePointGenerator2();
	void NetDivide (int uDivideNo , int vDivideNo , PointSet &ps , PointSet &vs , PointSet2d &pars);
	
private:
	std::vector<BSplineCurve *> m_preCurveV;
	std::vector<BSplineCurvePointGenerator *> m_preGenV;
	std::vector<BSplineCurve *> m_preCurveU;
	std::vector<BSplineCurvePointGenerator *> m_preGenU;
	std::vector<BSplineCurve *> m_curveU;
	std::vector<BSplineCurvePointGenerator *> m_genU;
	std::vector<BSplineCurve *> m_curveV;
	std::vector<BSplineCurvePointGenerator *> m_genV;	
	DoubleVec m_vParam;
	DoubleVec m_uParam;
	BSplineCurve * MakeUCurve (const vreal &v);	
	BSplineCurve * MakeVCurve (const vreal &u);	

	//void DivideSpans (const DoubleVec &spans , DoubleVec &divided , int divideNo);
	void PreCalculateU (int vDivideNo);
	void PreCalculateV (int uDivideNo);
	void DivideByOldWay (int uDivideNo , int vDivideNo , PointSet &ps , PointSet &vs , PointSet2d &pars);

};
#endif
#endif