// NurbsEval.cpp

#include "stdafx.h"
#include "NurbsEval.h"
#include "BSplineKnots.h"
#include "BSplineControlPoints.h"
#include "BSplineCurve.h"

NurbsEval::NurbsEval ( const BSplineCurve &crv)
:m(crv.GetDegreeOfBasisFunctions()+1,crv.GetControlPointNo()-1,crv.m_V0,crv.m_V1,crv.m_T)
,m_cp(crv.GetControlPointNo(),crv.controlpoints)
,m_w(crv.GetControlPointNo(),crv.m_W,crv.IsRational())
,m_B(m_t)
,m_CP (crv.GetControlPointNo(),crv.controlpoints,crv.m_W,crv.IsRational())
{
	m_k = m_t.Getk();
}
	
VPoint NurbsEval::Eval (const vreal realU)
{
	int m;
	m_B.CalcN (realU , m);
	int i1 = m - m_k + 1;
	VPoint tot (0.);
	for (int i=m;i>=i1;--i)
	{
		//tot+= m_B.m_N[i][m_k] * m_cp.GetPoint(i);
		tot+= m_CP.Blend (m_B.m_N[i][m_k],i);
	}
	return tot;	
}
	
void NurbsEval::Eval (const vreal realU , VPoint &p , VPoint &t)
{
	int m;
	m_B.CalcN (realU , m);
	int i1 = m - m_k + 1;
	p[0] = p[1] = p[2] = 0.;
	for (int i=m;i>=i1;--i)
	{
		//tot+= m_B.m_N[i][m_k] * m_cp.GetPoint(i);
		p += m_CP.Blend (m_B.m_N[i][m_k],i);
	}
	t = m_CP.Blend(m_B.m_N[m][m_k-1] * m_B.ca[m][m_k-1],m);
	for (int i=m-1; i>=i1; --i)
	{
		t+= m_CP.Blend (m_B.m_N[i][m_k-1] * m_B.ca[i][m_k-1] 
						- m_B.m_N[i+1][m_k-1]*m_B.ca[i+1][m_k-1] , i);
	}
}
	
VPoint NurbsEval::GetPoint (const vreal u)
{
	vreal realU = m_t.Reparametrize_Range0_1ToV0_V1 (u);	
	if (m_CP.IsRational() == false)
		return Eval (realU);
	else
	{
		sVector<double,4> res = Eval4d (realU);
		double divf = 1.0 / res[3];
		VPoint res3 (res[0] * divf , res[1] * divf , res[2] * divf);
		return res3;
	}
}

void NurbsEval::GetPointAndTangent(const vreal u,VPoint &p,VPoint &t)
{
	vreal realU = m_t.Reparametrize_Range0_1ToV0_V1 (u);
	if (m_CP.IsRational() == false)
		Eval (realU,p,t);
	else
	{
		sVector<vreal,4> p4,t4;
		Eval4d (realU,p4,t4);
		double divf1 = 1.0 / p4[3];
		p.x(p4[0] * divf1);
		p.y(p4[1] * divf1);
		p.z(p4[2] * divf1);

		t.x(t4[0]);
		t.y(t4[1]);
		t.z(t4[2]);
	}
}

sVector<double,4> NurbsEval::Eval4d(const vreal realU)
{
	int m;
	m_B.CalcN (realU,m);
	int i1 = m - m_k + 1;
	sVector<double,4> tot(0.0);
	for (int i=m;i>=i1;--i)
	{
		tot+=m_CP.Blend4d(m_B.m_N[i][m_k],i);	
	}
	return tot;
}

void NurbsEval::Eval4d(const vreal realU, sVector<double,4> &p, sVector<double,4> &t)
{
	int m;
	m_B.CalcN (realU,m);
	int i1 = m - m_k +1;
	p[0] = p[1] = p[2] = p[3] = 0.;
	for (int i=m;i>=i1;--i)
	{
		p += m_CP.Blend4d (m_B.m_N[i][m_k],i);
	}
	t = m_CP.Blend4d (m_B.m_N[m][m_k-1] * m_B.ca[m][m_k-1] 
					- m_B.m_N[m+1][m_k-1] * m_B.ca[m+1][m_k-1] ,m);

	for (int i=m-1;i>=i1;--i)
	{
		t +=  m_CP.Blend4d (( m_B.m_N[i][m_k-1] * m_B.ca[i][m_k-1]
							- m_B.m_N[i+1][m_k-1] * m_B.ca[i+1][m_k-1] ) ,i);
	}
}

const BSplineKnots & NurbsEval::GetKnot() const
{
	return m_t;
}
