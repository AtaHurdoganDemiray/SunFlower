/*********************************************************************************
*			File: ParametricSplinePointGenerator.h									     *
**********************************************************************************
*			Description:														 *
*																				 *
**********************************************************************************
*			History:															 *
*																				 *
**********************************************************************************
*			(C)...																 *
*********************************************************************************/

#ifndef __ParametricSplinePointGenerator_h__
#define __ParametricSplinePointGenerator_h__

#include "CurvePointGenerator.h"

class ParametricSpline;

class ParametricSplinePointGenerator : public CurvePointGenerator
{
public:
	ParametricSplinePointGenerator( ParametricSpline &a , IgesEntitySet * allent);
	virtual VPoint GetPoint ( const vreal & v ) const;
	virtual void GetDefIntervals ( DoubleVec & defint ) const;
	virtual void GetPointAndTangent ( const vreal &v , VPoint &p , VVector &tng)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const;
	virtual void GetSnapPoints (PointSet &snapp)const;
	virtual void GetSnapLines (LineSet &snapl)const;
	virtual VPoint GetTangentDerivative (const vreal &v)const;
	virtual vreal GetRadiusOfCurvature (const vreal &v)const;
	VPoint GetTangent(const vreal &v)const;
private:
	const ParametricSpline &m_a;
	void MapParam( const vreal &v , vreal &s , int &i) const;
};

#endif