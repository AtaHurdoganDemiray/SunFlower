// NurbsControlPoints.cpp

#include "stdafx.h"
#include "NurbsControlPoints.h"
#include "VPoint.h"

NurbsControlPoints::NurbsControlPoints(const int pointno
									   , const std::vector<vreal> &coordinates
									   , const std::vector<vreal> &weights
									   , bool isRational
									   )
:m_n (pointno-1) , m_pointno(pointno) , m_coordinates(coordinates),m_weights(weights)
,m_isRational(isRational),m_weighcoord (0)
{
	if (m_isRational)
	{
		m_weighcoord.resize (m_pointno*4,0.0);
		for (int i=0;i<pointno;++i)
		{
			int pno = i*4;
			int pno3=3*i;
			m_weighcoord[pno]=m_coordinates[pno3]*m_weights[i];
			m_weighcoord[pno+1]=m_coordinates[pno3+1]*m_weights[i];
			m_weighcoord[pno+2]=m_coordinates[pno3+2]*m_weights[i];
			m_weighcoord[pno+3]=m_weights[i];
		}
	}
}

VPoint NurbsControlPoints::GetPoint(const int i) const
{
	int pno = 3*i;
	VPoint res (m_coordinates[pno],m_coordinates[pno+1],m_coordinates[pno+2]);
	return res;
}

sVector<double,4> NurbsControlPoints::GetPoint4d(const int i) const
{
	int pno=4*i;
	double res[4] = {m_weighcoord[pno],m_weighcoord[pno+1],m_weighcoord[pno+2],m_weighcoord[pno+3]};
	return res;
}

const vreal & NurbsControlPoints::GetWeight(const int i) const
{
	return m_weights[i];
}

VPoint NurbsControlPoints::Blend(const vreal N, const int i) const
{
	int pno = i*3;
	VPoint res (N*m_coordinates[pno],N*m_coordinates[pno+1],N*m_coordinates[pno+2]);
	return res;
}

sVector<double,4> NurbsControlPoints::Blend4d(const vreal N, const int i) const
{
	int pno = i*4;
	double res[4] = {N*m_weighcoord[pno]
					,N*m_weighcoord[pno+1]
					,N*m_weighcoord[pno+2]
					,N*m_weighcoord[pno+3]};
	return res;
}

bool NurbsControlPoints::IsRational() const
{
	return m_isRational;
}
