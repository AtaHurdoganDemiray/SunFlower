// OffsetCurvePointGenerator.h

#ifndef __OffsetCurvePointGenerator_h__
#define __OffsetCurvePointGenerator_h__

#include "CurvePointGenerator.h"

class OffsetCurvePointGenerator:public CurvePointGenerator
{
public:
	OffsetCurvePointGenerator ( IgesEntity *ent , IgesEntitySet &entset , const vreal &offs);
	virtual ~OffsetCurvePointGenerator();
	virtual VPoint GetPoint ( const vreal & v ) const;
	virtual void GetPointAndTangent ( const vreal &v , VPoint &p , VVector &tng)const;
	virtual VPoint GetTangentDerivative (const vreal &v)const;
	virtual vreal GetRadiusOfCurvature (const vreal &v)const;
	virtual void GetDefIntervals ( DoubleVec & defint ) const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const;
	virtual void GetSnapPoints (PointSet &snapp)const;
	virtual void GetSnapLines (LineSet &snapl)const;
private:
	CurvePointGenerator *m_curve;
	vreal m_offs;
};

#endif