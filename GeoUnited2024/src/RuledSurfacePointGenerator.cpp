// RuledSurfacePointGenerator.cpp

#include "stdafx.h"
#include "RuledSurfacePointGenerator.h"
#include "RuledSurface.h"
#include "IgesEntitySet.h"
#include "VPoint.h"
#include "CurvePointGenerator.h"
#include "CurvePointGeneratorFactory.h"
#include "DoubleVec.h"
#include "SequentialLineSet.h"
#include "DoubleVec.h"
#include "VRefTriangleMesh.h"

RuledSurfacePointGenerator::RuledSurfacePointGenerator (const RuledSurface *rs , const IgesEntitySet &allent)
:SurfacePointGenerator (0.0,1.0,0.0,1.0),m_surf(rs)
{
	int dc1 = rs->m_DE1;
	IgesEntity * crv1 = allent.GetEntityPointer (dc1);
	if (!crv1)
		throw VException (( IDS_RulConExc) ,1);
	allent.MakeAllSubsetIndex0();
	m_icrv1 = crv1->MakeCopyAndAddToSet (allent,m_eSet);
	m_crv1 =  CurvePointGeneratorFactory::MakeCurve (m_icrv1 , m_eSet);
	//m_crv1->SetTransformationMatrixIf();
	int dc2 = rs->m_DE2;
	IgesEntity *crv2 = allent.GetEntityPointer (dc2);
	if (!crv2)
		throw VException (( IDS_RulConExc) ,2);
	m_icrv2 = crv2->MakeCopyAndAddToSet(allent,m_eSet);
	m_crv2 = CurvePointGeneratorFactory::MakeCurve (m_icrv2 , m_eSet);
	//m_crv2->SetTransformationMatrixIf();
	if (m_surf->m_DIRFLG == 1)
		m_crv2->SetReversed(true);

	if (rs->m_type == RuledSurface::EQUAL_ARC_LENGTH)
	{
		vreal mytol = 0.04;
		DoubleVec params1,params2;
		m_ds1 = new SequentialLineSet (60);
		m_crv1->Digitise (mytol , *m_ds1 , params1);
		m_len1 = m_ds1->GetLength();
		m_ds2 = new SequentialLineSet (60);
		m_crv2->Digitise (mytol , *m_ds2, params2);
		m_len2 = m_ds2->GetLength();
	}
	else
	{
		m_ds1 = 0;
		m_ds2 = 0;
	}

}
RuledSurfacePointGenerator::~RuledSurfacePointGenerator()
{
	delete m_crv1;
	delete m_crv2;
	delete m_ds1;
	delete m_ds2;
}
VPoint RuledSurfacePointGenerator::GetPoint( const vreal & u , const vreal & v ) const
{
	vreal su,sv;
	ShiftParIf(u,v,su,sv);
	if (m_surf->m_type == RuledSurface::EQUAL_ARC_LENGTH)
	{
		int sli1,sli2;
		VPoint p1 = m_ds1->GetP (su,m_len1,sli1);
		VPoint p2 = m_ds2->GetP (su,m_len2,sli2);
		VPoint res = p1 + (p2 - p1) * sv;
		return res;
	}
	else
	{
		VPoint p1 = m_crv1->GetPoint (su);
		VPoint p2 = m_crv2->GetPoint (su);
		VPoint res = p1 + (p2 - p1) * sv;//= p2 * v + (1. - v) * p1;
		return res;
	}
}
VPoint RuledSurfacePointGenerator::GetNormal( const vreal & u , const vreal & v , const VPoint & surfpoint ) const
{
	return GetNormalAppr(u,v,surfpoint);
}
void RuledSurfacePointGenerator::GetUDefIntervals ( DoubleVec & defint ) const
{
	m_crv1->GetDefIntervals (defint);
	m_crv2->GetDefIntervals (defint);
	defint.SortAndRemoveDuplicate ();
}
	
void RuledSurfacePointGenerator::GetVDefIntervals ( DoubleVec & defint ) const
{
	defint.Addvreal (0.0);
	defint.Addvreal (1.0);
}
	
VRefTriangleMesh * RuledSurfacePointGenerator::Digitise (const vreal &tol) const
{
	return GenericDigitise (tol);
}

void RuledSurfacePointGenerator::GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const
{
	m = GetPoint (u,v);
	n = GetNormal (u,v,m);
}

//bool RuledSurfacePointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return GenericUnproject(mp, u, v, tol);
//}

IgesEntity * RuledSurfacePointGenerator::GetIgesEntity()
{
	return (IgesEntity *)m_surf;
}