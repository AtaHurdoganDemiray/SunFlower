// NurbsBlendFunc.cpp

#include "stdafx.h"
#include "NurbsBlendFunc.h"
#include "BSplineKnots.h"

NurbsBlendFunc::NurbsBlendFunc(const BSplineKnots &t)
:m(t)
,m_k(t.Getk()) 
,m_n(t.Getn())
{
	// make cashed divisors for N and empty N array
	int spanno = m_n - m_k + 2;
	int n1 = spanno + m_k - 1; // n1 = m_n + 1
	ca.resize (n1+1);
	m_N.resize (n1+1);
	for (int i=0;i<n1;++i)
	{
		ca.at(i).resize(m_k,0.0);
		m_N.at(i).resize(m_k+1,0.0);
		for (int j=1;j<m_k;++j)
		{
			vreal div = (m_t.GetKnot(i+j) - m_t.GetKnot(i));
			if (fabs(div) > EPSILON)
				ca.at(i).at(j) = 1.0 / div;
			else
				ca.at(i).at(j) = 0.0;
		}
	}
	ca.at(n1).resize(m_k,0.0);
	m_N.at(n1).resize(m_k,0.0);
	ca.at(n1).at(m_k-1)=0.0;
}
  
void NurbsBlendFunc::CalcN (const vreal u , int &m)//from N[m] to N[m-k+1] are calculated , m is the span no for u
{
	m = m_t.GetSegmentNo (u)+m_k-1;
	
	// initialising N
	for (int i=0; i<m_n+2; ++i)
		m_N.at(i).at(1) = 0.0;
	m_N.at(m).at(1) = 1.0;
	
	for (int k=2;k<m_k+1;++k)
	{
		for (int i=m; i>=m-m_k+1;--i)
		{
			//if (m_N.at(i).size() > k)
				m_N.at(i).at(k) = 0.0;
		}
	}
	// calculating N
	for (int k=1;k<m_k;++k)
	{
		for (int i=m; i>=m-k+1; --i)
		{
			//if (m_N.at(i).size() > k+1)
				m_N.at(i).at(k+1) += (u-m_t.GetKnot(i)) * ca.at(i).at(k) * m_N.at(i).at(k);
			//if (m_N.at(i - 1).size() > k+1)
				m_N.at(i-1).at(k+1) += (m_t.GetKnot(i+k)-u) * ca.at(i).at(k) * m_N.at(i).at(k);
		}
	}
}
