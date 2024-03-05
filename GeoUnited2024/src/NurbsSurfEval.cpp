// NurbsSurfEval.cpp

#include "stdafx.h"
#include "NurbsSurfEval.h"
#include "BSplineSurface.h"
#include "VPoint.h"


NurbsSurfEval::NurbsSurfEval (const BSplineSurface &surf)
:m_surf(surf)
,m_ut (surf.GetDegreeOfBasisFunctionsU()+1,surf.GetControlPointNoU()-1,surf.m_U0,surf.m_U1,surf.m_S)
,m_vt (surf.GetDegreeOfBasisFunctionsV()+1,surf.GetControlPointNoV()-1,surf.m_V0,surf.m_V1,surf.m_T)
,m_CP (surf.GetControlPointNoU(),surf.GetControlPointNoV(),surf.controlpoints,surf.m_W,surf.IsRational())
,m_UB (m_ut)
,m_VB (m_vt)
{
	m_ku = m_surf.GetDegreeOfBasisFunctionsU() + 1;
	m_kv = m_surf.GetDegreeOfBasisFunctionsV() + 1;
}

VPoint NurbsSurfEval::GetPoint (const vreal u,const vreal v)
{
	vreal realU = m_ut.Reparametrize_Range0_1ToV0_V1 (u);
	vreal realV = m_vt.Reparametrize_Range0_1ToV0_V1 (v);
	
	if (m_surf.IsRational() == false)
	{
		return Eval (realU,realV);		
	}
	else
	{
		sVector<double,4> wp = Eval4d (realU,realV);
		vreal f = 1. / wp[3];
		VPoint p (wp[0]*f , wp[1]*f , wp[2]*f);
		return p;
	}
	return 1.;
}	

// S4d = { Swp i + Swp j + Swp k + Sw l }
// S (u,v) = Swp (u,v) / Sw (u,v)
// dS / du = (1/Sw) * ( dSwp/du - (dSw/du) S)
// dS / dv = (1/Sw) * (dSwp / dv - (dSw/dv) S)

void NurbsSurfEval::GetPointAndNormal (const vreal u , const vreal v , VPoint &p , VPoint &n)
{
	vreal realU = m_ut.Reparametrize_Range0_1ToV0_V1 (u);
	vreal realV = m_vt.Reparametrize_Range0_1ToV0_V1 (v);
	if (m_surf.IsRational() == false)
	{
		VPoint du,dv;
		Eval (realU,realV,p,du,dv);
		n = du ^ dv;
		n.NormThis();
	}
	else
	{
		sVector<double,4> P,DU,DV;
		Eval4d (realU,realV,P,DU,DV);
		vreal f = 1. / P[3];
		p.x(P[0]*f);
		p.y(P[1]*f);
		p.z(P[2]*f);
		VPoint du (f*(DU[0] - DU[3]*p.x())
					,f*(DU[1] - DU[3]*p.y())
					,f*(DU[2] - DU[3]*p.z()) );
		VPoint dv (f*(DV[0] - DV[3]*p.x())
					,f*(DV[1]-DV[3]*p.y())
					,f*(DV[2]-DV[3]*p.z()) );
		n = du ^ dv;
		n.NormThis();
	}
}

VPoint NurbsSurfEval::Eval(const vreal realU,const vreal realV)
{
	int m,n;
	m_UB.CalcN (realU,m);
	m_VB.CalcN (realV,n);
	int o1 = m - m_ku + 1;
	int o2 = n - m_kv +1;
	VPoint tot (0.0);
	for (int i=m; i>=o1; --i)
	{
		for (int j=n;j>=o2;--j)
		{
			tot += (m_CP.Blend (m_VB.m_N[j][m_kv],i,j) * m_UB.m_N[i][m_ku]);
		}
	}
	return tot;
}

sVector<double,4> NurbsSurfEval::Eval4d(const vreal realU,const vreal realV)
{
	int m,n;
	m_UB.CalcN (realU,m);
	m_VB.CalcN (realV,n);
	int o1 = m - m_ku +1;
	int o2 = n - m_kv +1;
	sVector <double,4> tot (0.0);
	for (int i=m; i>=o1; --i)
	{
		for (int j=n; j>=o2; --j)
		{
			tot += (m_CP.Blend4d (m_VB.m_N[j][m_kv],i,j) * m_UB.m_N[i][m_ku]);
		}
	}
	return tot;
}

void NurbsSurfEval::Eval(const vreal realU, const vreal realV , VPoint &p, VPoint &du , VPoint &dv)
{
	int m,n;
	m_UB.CalcN (realU,m);
	m_VB.CalcN (realV,n);
	int o1 = m - m_ku +1;
	int o2 = n - m_kv +1;
	
	VPoint O (0.);
	p = O;
	du = O;
	dv = O;
	m_UB.m_N[m+1][m_ku-1] = 0.0;
	m_VB.m_N[n+1][m_kv-1] = 0.0;
	for (int i = m; i>=o1; --i)
	{
		for (int j= n; j>=o2; --j)
		{
			p += (m_CP.Blend (m_VB.m_N[j][m_kv],i,j) * m_UB.m_N[i][m_ku]);
			du += (m_CP.Blend (m_VB.m_N[j][m_kv],i,j) * (m_UB.m_N[i][m_ku-1] * m_UB.ca[i][m_ku-1] - m_UB.m_N[i+1][m_ku-1] * m_UB.ca[i+1][m_ku-1]) );
			dv += (m_CP.Blend (m_VB.m_N[j][m_kv-1]*m_VB.ca[j][m_kv-1] - m_VB.m_N[j+1][m_kv-1]*m_VB.ca[j+1][m_kv-1],i,j) * m_UB.m_N[i][m_ku]);
		}
	}
	du *= (m_ku-1);
	dv *= (m_kv-1);
}

void NurbsSurfEval::Eval4d(const vreal realU, const vreal realV , sVector<double,4> &p, sVector<double,4> &du , sVector<double,4> &dv)
{
	int m,n;
	m_UB.CalcN (realU,m);
	m_VB.CalcN (realV,n);
	int o1 = m - m_ku +1;
	int o2 = n - m_kv +1;
	sVector<double,4> O(0.);
	p=O;
	du=O;
	dv = O;
	m_UB.m_N[m+1][m_ku-1] = 0.0;
	m_VB.m_N[n+1][m_kv-1] = 0.0;
	for (int i=m;i>=o1;--i)
	{
		for (int j=n; j>=o2; --j)
		{
			p += (m_CP.Blend4d (m_VB.m_N[j][m_kv],i,j) * m_UB.m_N[i][m_ku]);
			du += (m_CP.Blend4d (m_VB.m_N[j][m_kv],i,j) * (m_UB.m_N[i][m_ku-1]*m_UB.ca[i][m_ku-1] - m_UB.m_N[i+1][m_ku-1] * m_UB.ca[i+1][m_ku-1]) );
			dv += (m_CP.Blend4d (m_VB.m_N[j][m_kv-1]*m_VB.ca[j][m_kv-1] - m_VB.m_N[j+1][m_kv-1]*m_VB.ca[j+1][m_kv-1],i,j) * m_UB.m_N[i][m_ku]);
		}
	}
	du *= (m_ku-1);
	dv *= (m_kv-1);
}

const BSplineKnots & NurbsSurfEval::GetUKnots()const
{
	return m_ut;
}

const BSplineKnots & NurbsSurfEval::GetVKnots() const
{
	return m_vt;
}

void NurbsSurfEval::Update()
{
	m_CP.UpdateWeighCoord();
}