// NurbsSurfEval.h

#ifndef __NurbsSurfEval_h__
#define __NurbsSurfEval_h__

#include "BSplineControlPoints.h"
#include "BSplineKnots.h"
#include "NurbsSurfControlPoints.h"
#include "NurbsBlendFunc.h"

struct VPoint;
class BSplineSurface;

class NurbsSurfEval
{
public:
	NurbsSurfEval (const BSplineSurface &surf);
	VPoint GetPoint (const vreal u , const vreal v);
	void GetPointAndNormal (const vreal u , const vreal v, VPoint &p , VPoint &n);
	const BSplineKnots & GetUKnots()const;
	const BSplineKnots & GetVKnots()const;
	void Update();
private:
	VPoint Eval (const vreal realU , const vreal realV);
	sVector<double,4> Eval4d (const vreal realU , const vreal realV);
	void Eval (const vreal realU , const vreal realV , VPoint &p , VPoint &du , VPoint &dv);
	void Eval4d (const vreal realU , const vreal realV , sVector<double,4> &p,sVector<double,4> &du , sVector<double,4> &dv);
	const BSplineSurface &m_surf;
	BSplineKnots m_ut;
	BSplineKnots m_vt;
	NurbsSurfControlPoints m_CP;
	NurbsBlendFunc m_UB;
	NurbsBlendFunc m_VB;
	int m_ku;
	int m_kv;
};

#endif