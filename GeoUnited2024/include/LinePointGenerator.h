// LinePointGenerator.h

#ifndef __LinePointGenerator_h__
#define __LinePointGenerator_h__

#include "CurvePointGenerator.h"
#include "VLine.h"
class Line;
class IgesEntitySet;

class LinePointGenerator:public CurvePointGenerator
{
public:
	LinePointGenerator ( Line & _line , IgesEntitySet &allent);
	virtual VPoint GetPoint ( const vreal & v ) const;
	virtual void GetPointAndTangent (const vreal &v , VPoint &p , VVector &tng)const;
	virtual void GetDefIntervals ( DoubleVec & defint ) const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const;
	virtual void GetSnapPoints (PointSet &snapp)const;
	virtual void GetSnapLines (LineSet &snapl)const;
	virtual VPoint GetTangentDerivative (const vreal &v)const;
	virtual vreal GetRadiusOfCurvature (const vreal &v)const;
VLine line;
};

#endif