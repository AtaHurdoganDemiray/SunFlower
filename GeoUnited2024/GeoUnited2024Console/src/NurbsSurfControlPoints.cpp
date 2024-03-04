// NurbsSurfControlPoints.cpp

#include "stdafx.h"
#include "NurbsSurfControlPoints.h"
#include "VPoint.h"
#include "sVector.h"

NurbsSurfControlPoints::NurbsSurfControlPoints (const int upointno
												,const int vpointno
												, const std::vector<vreal> &coordinates
												, const std::vector<vreal> &weights
												,bool isRational )
:m_upointno(upointno),m_vpointno(vpointno),m_coordinates(coordinates),m_weights(weights)
,m_isRational(isRational)
{
	if (m_isRational)
	{
		m_weighcoord.resize(m_upointno*m_vpointno*4);
		for (int v=0;v<m_vpointno;++v)
		{
			for (int u=0;u<m_upointno;++u)
			{
				int pno = v*m_upointno + u;
				int cno = 3 * pno;
				int cno4 = 4 * pno;
				m_weighcoord[cno4] = m_coordinates[cno] * m_weights[pno];
				m_weighcoord[cno4+1] = m_coordinates[cno+1] * m_weights[pno];
				m_weighcoord[cno4+2] = m_coordinates[cno+2] * m_weights[pno];
				m_weighcoord[cno4+3] = m_weights[pno];
			}
		}
	}
}
	
VPoint NurbsSurfControlPoints::GetPoint (const int i , const int j)const
{
	int pno = j * m_upointno + i;
	int cno = pno * 3;
	VPoint res (m_coordinates[cno]
				,m_coordinates[cno+1]
				,m_coordinates[cno+2] );
	return res;
}
	
sVector<double,4> NurbsSurfControlPoints::GetPoint4d (const int i,const int j)const
{
	int pno = j * m_upointno + i;
	int cno = pno * 4;
	//double a[4] = {m_weighcoord[cno]
	//				,m_weighcoord[cno]
	//				,m_weighcoord[cno+1]
	//				,m_weighcoord[cno+2]
	//				};
	double a[4] = {m_weighcoord[cno]
					,m_weighcoord[cno+1]
					,m_weighcoord[cno+2]
					,m_weighcoord[cno+3]
					};
	return a;
}
	
VPoint NurbsSurfControlPoints::Blend (const vreal N,const int i,const int j)const
{
	int pno = j * m_upointno + i;
	int cno = pno * 3;
	VPoint res (m_coordinates[cno]*N
				,m_coordinates[cno+1]*N
				,m_coordinates[cno+2]*N
				);
	return res;
}
	
sVector<double,4> NurbsSurfControlPoints::Blend4d (const vreal N,const int i,const int j)const
{
	int pno = j * m_upointno + i;
	int cno = pno *4;
	double a[4] = {
						m_weighcoord[cno]*N
						,m_weighcoord[cno+1]*N
						,m_weighcoord[cno+2]*N
						,m_weighcoord[cno+3]*N
					};
	return a;
}

void NurbsSurfControlPoints::UpdateWeighCoord()
{
	if (m_isRational)
	{
		for (int v=0;v<m_vpointno;++v)
		{
			for (int u=0;u<m_upointno;++u)
			{
				int pno = v*m_upointno + u;
				int cno = 3 * pno;
				int cno4 = 4 * pno;
				m_weighcoord[cno4] = m_coordinates[cno] * m_weights[pno];
				m_weighcoord[cno4+1] = m_coordinates[cno+1] * m_weights[pno];
				m_weighcoord[cno4+2] = m_coordinates[cno+2] * m_weights[pno];
				m_weighcoord[cno4+3] = m_weights[pno];
			}
		}
	}
}