// BSplineCurvePointGenerator.h

#ifndef __BSplineCurvePointGenerator_h__
#define __BSplineCurvePointGenerator_h__

#include "BSplineKnots.h"
#include "BSplineControlPoints.h"
#include "BSplineWeights.h"
#include "CurvePointGenerator.h"
//#include "BSplineCurve.h"
#include "NurbsEval.h"

const int NURBS_CURVE_TESSELATION_POINTS_PER_DIR = 50;

class BSplineCurve;
class DeBoorEvaluation;

class BSplineCurvePointGenerator : public CurvePointGenerator
{
public:
	BSplineCurvePointGenerator ( BSplineCurve & curve ,  IgesEntitySet &allent);
	virtual ~BSplineCurvePointGenerator();
	virtual VPoint GetPoint ( const vreal & u ) const;	
	virtual void GetDefIntervals ( DoubleVec & defint ) const;
	virtual void GetPointAndTangent (const vreal &v
									,VPoint &p
									,VVector &tng)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result,DoubleVec &params)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const;
	virtual void GetSnapPoints (PointSet &snapp)const;
	virtual void GetSnapLines (LineSet &snapl)const;
	sVector<double,4> GetPointRational ( const vreal &u ) const;
	void GetPointAndTangentRational ( const vreal &v
									, sVector<double,4> &p
									, sVector<double,4> &tng)const;
	bool IsRational()const;
	virtual VPoint GetTangentDerivative (const vreal &v)const;
	virtual vreal GetRadiusOfCurvature (const vreal &v)const;
protected:
	int GetBase (const vreal &realU)const;
	const BSplineCurve &m_curve;
#if !USENN
	VPoint GetPointImp ( const vreal & u ) const;
	void GetPointAndTangentImp (const vreal &v
								,VPoint &p
								,VVector &tng)const;
#endif
	int DivideInterval (const vreal &p1 , const vreal &p2,const vreal tol)const;
	void NewTest (const vreal u,VPoint p)const;
#if USENN
	mutable NurbsEval m_NE;
#else
	BSplineKnots *knots;
	BSplineControlPoints *controlpoints;
	BSplineWeights *weights;
	DeBoorEvaluation *m_deboor;
#endif
};

#endif