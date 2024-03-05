/*********************************************************************************
*			File: ConicArcPointGenerator.h									     *
**********************************************************************************
*			Description:														 *
*																				 *
**********************************************************************************
*			History:															 *
*																				 *
**********************************************************************************
*			(C)...																 *
*********************************************************************************/

#ifndef __ConicArcPointGenerator_h__
#define __ConicArcPointGenerator_h__

#include "Matrix.h"

class CurvePointGenerator;
class ConicArc;
class IgesEntitySet;
class IgesEntity;
struct VPoint2d;
class 	EllipsePointGenerator;
class ArcPointGenerator;
class LinePointGenerator;

#include "CurvePointGenerator.h"

class ConicArcPointGenerator : public CurvePointGenerator
{
public:
	enum Type {point, ellipse, circle, hyperbola, cone, line, cylinder, parabola,unsupported};
	ConicArcPointGenerator ( ConicArc &a , IgesEntitySet &allent);
	CurvePointGenerator * MakeGenerator (const IgesEntitySet &allent);	
	//Type GetType ()const;
	virtual ~ConicArcPointGenerator();
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
	ConicArc &m_a;
	IgesEntitySet *m_allent;
	Type m_type;
	Matrix m_A, m_B, m_D, m_E;
	void SetDR();
	vreal m_l0,m_l1;
	vreal GetAngle (const VPoint2d &arcPoint)const;
	EllipsePointGenerator * MakeEllipse()const;
	ArcPointGenerator * MakeArc ()const;
	LinePointGenerator * MakeLine()const;

	VPoint GetPointEllipse (vreal v)const;
	VPoint GetPointParabola (vreal v)const;
	VPoint GetPointHyperbola (vreal v)const;

	double m_ellipseA;
	double m_ellipseB;
	void CalcEllipseV0V1();

	double m_parabolaXCoef;
	double m_parabolaYCoef;
	void CalcParabolaV0V1();

	double m_hyperbolaXCoef;
	double m_hyperbolaYCoef;
	double m_hyperbolaTCoef;
	bool m_hyperbolaType1;
	void CalcHyperbolaV0V1();

	void DecideType();
	double A,B,C,D,E,F,X1,X2,Y1,Y2,ZT;

};

#endif