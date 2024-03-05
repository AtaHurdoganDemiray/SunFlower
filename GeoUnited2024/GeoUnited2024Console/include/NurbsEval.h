// NurbsEval.h

#ifndef __NurbsEval_h__
#define __NurbsEval_h__
#include "NurbsBlendFunc.h"
#include "BSplineKnots.h"
#include "BSplineControlPoints.h"
#include "BSplineWeights.h"
#include "NurbsControlPoints.h"

struct VPoint;
class BSplineCurve;

class NurbsEval
{
public:
	NurbsEval ( const BSplineCurve &crv);
	VPoint Eval (const vreal realU);
	sVector<double ,4> Eval4d (const vreal realU);
	void Eval4d (const vreal realU , sVector<double,4> &p , sVector<double,4> &t);
	void Eval (const vreal realU , VPoint &m , VPoint &d);
	VPoint GetPoint (const vreal u);
	//VPoint GetTangent (const vreal u);
	void GetPointAndTangent (const vreal u,VPoint &p,VPoint &t);
	const BSplineKnots & GetKnot()const;
private:
	BSplineKnots m_t;
	BSplineControlPoints m_cp;
	BSplineWeights m_w;
	NurbsBlendFunc m_B;
	int m_k;
	NurbsControlPoints m_CP;
};

#endif