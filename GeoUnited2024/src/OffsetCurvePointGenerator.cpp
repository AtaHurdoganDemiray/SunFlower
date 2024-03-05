// OffsetCurvePointGenerator.cpp

#include "stdafx.h"
#include "OffsetCurvePointGenerator.h"
#include "CurvePointGeneratorFactory.h"

OffsetCurvePointGenerator::OffsetCurvePointGenerator ( IgesEntity *ent
													  , IgesEntitySet &entset
													  ,const vreal &offs)
:CurvePointGenerator (ent,&entset)
,m_offs (offs)
{
	m_curve = CurvePointGeneratorFactory::MakeCurve (ent , entset);
}

OffsetCurvePointGenerator::~OffsetCurvePointGenerator()
{
	delete m_curve;
}
	
 VPoint OffsetCurvePointGenerator::GetPoint ( const vreal & v ) const
{
	vreal vv = ReverseIf(v);
	vv = TransformPar(vv);
	return m_curve->GetPoint(vv);
}
	
 void OffsetCurvePointGenerator::GetPointAndTangent ( const vreal &v , VPoint &p , VVector &tng)const
{
	vreal vv = ReverseIf(v);
	vv = TransformPar(vv);
	m_curve->GetPointAndTangent (v,p,tng);
}
	
 VPoint OffsetCurvePointGenerator::GetTangentDerivative (const vreal &v)const
{
	return m_curve->GetTangentDerivative (v);
}
	
 vreal OffsetCurvePointGenerator::GetRadiusOfCurvature (const vreal &v)const
{
	return m_curve->GetRadiusOfCurvature (v);
}
	
 void OffsetCurvePointGenerator::GetDefIntervals ( DoubleVec & defint ) const
{
	m_curve->GetDefIntervals (defint);	
}
	
 void OffsetCurvePointGenerator::Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const
{
	m_curve->Digitise (tol,result,params);
}

 void OffsetCurvePointGenerator::Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const
{
	m_curve->Digitise (tol,result,tangents,params);
}
	
 void OffsetCurvePointGenerator::GetSnapPoints (PointSet &snapp)const
{
	m_curve->GetSnapPoints (snapp);
}
	
 void OffsetCurvePointGenerator::GetSnapLines (LineSet &snapl)const
{
	m_curve->GetSnapLines(snapl);
}
