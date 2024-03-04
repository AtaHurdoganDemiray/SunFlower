// DeBoorEvaluation.cpp

#include "stdafx.h"
#include "DeBoorEvaluation.h"
#include "BSplineCurve.h"
#include "VPoint.h"

DeBoorEvaluation::DeBoorEvaluation (const BSplineCurve &crv)
:m_crv (crv)
{
	m_icount = 0;
	MakeI();	
	m_vstep = 1. / m_icount;
}

void DeBoorEvaluation::MakeI()
{
	int size1 = m_crv.m_T.size();
	int loopstart = m_crv.GetDegreeOfBasisFunctions();
	int loopend = m_crv.GetControlPointNo()+1;
	vreal prevt = m_crv.m_T[loopstart];
	for (int i=loopstart+1;i<loopend;i++)
	{
		vreal currt = m_crv.m_T[i];
		if (fabs (currt - prevt) > EPSILON)
		{
			// this is an interval 
			m_I.push_back(i-1);
			m_I.push_back(i);		
			++m_icount;
		}
		prevt = currt;
	}
	int i0 = m_I[0];
	int lastParamIndex = (m_icount-1) * 2 + 1;
	int i1 = m_I[lastParamIndex];
	m_v0 = m_crv.m_T[i0];
	m_v1 = m_crv.m_T[i1];
	m_dv = m_v1 - m_v0;
}

void DeBoorEvaluation::ConvertParam01ToKnotSpace  (const vreal &v 
													, int &i_no
													, vreal &cv)
{
	vreal sv = v;
	if (sv<0.0)
		sv = 0.0;
	if (sv>1.0)
		sv = 1.0;
	cv = sv * m_dv + m_v0; 
	int mysize = (int)(m_I.size() / 2);
	for (int i=0;i<mysize;i++)
	{
		int begini = i*2;
		int i0 = m_I[begini];
		int i1 = m_I[begini+1];
		const vreal &t0 = m_crv.m_T[i0];
		const vreal &t1 = m_crv.m_T[i1];
		if(cv > (t0-EPSILON) && cv < (t1+EPSILON))
		{
			i_no = i;
			break;
		}
	}
}

void DeBoorEvaluation::ConvertParamKnotSpaceTo01 (const vreal &t
													, vreal &ct)
{
	ct = (t - m_v0) / m_dv;
}
