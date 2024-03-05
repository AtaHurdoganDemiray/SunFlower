// SurfPointsPackBucketed.cpp

#include "stdafx.h"
#include "SurfPointsPackBucketed.h"

SurfPointsPackBucketed::SurfPointsPackBucketed (size_t bucketNoInOneDirection)
:m_pack (400) , m_bucketNoInOneDirection(bucketNoInOneDirection) 
{
	m_buckets.resize(m_bucketNoInOneDirection);
	for (size_t i=0;i<m_bucketNoInOneDirection;++i)
	{
		m_buckets[i].resize(m_bucketNoInOneDirection);
	}
}
const SurfPointsPack & SurfPointsPackBucketed::GetPack()const
{
	return m_pack;
}
SurfPointsPack & SurfPointsPackBucketed::GetPack()
{
	return m_pack;
}
void SurfPointsPackBucketed::GetBucketIndicesForOnePoint (const VPoint2d &p , size_t &xi , size_t &yi)const
{
	xi = (size_t) p.x()*m_bucketNoInOneDirection;
	yi = (size_t) p.y()*m_bucketNoInOneDirection;
	
	if (xi < 0)
		xi = 0;
	if (xi >= m_bucketNoInOneDirection)
		xi = m_bucketNoInOneDirection -1;
	if (yi < 0)
		yi = 0;
	if (yi >= m_bucketNoInOneDirection)
		yi = m_bucketNoInOneDirection;
}
int SurfPointsPackBucketed::InsertPoint (const VPoint2d &par,const VPoint &mod,const VPoint &nor,double coinTol)
{
	size_t xi,yi;
	GetBucketIndicesForOnePoint(par , xi , yi);
	std::list<int> &curbuck = m_buckets[xi][yi];
	int coind (-1);
	std::list<int>::const_iterator ii=curbuck.begin();
	std::list<int>::const_iterator ee=curbuck.end();
	for(ii;ii!=ee;++ii)
	{
		const VPoint2d &cp = m_pack.GetParamPoint(*ii);
		if (cp.Compare(par,coinTol))
			coind = *ii;
	}
	if (coind != -1)
		return coind;
	else
	{
		int res = m_pack.InsertPoint(par,mod,nor);
		curbuck.push_back(res);
		return res;
	}
}
const std::list<int> & SurfPointsPackBucketed::GetPointsAround (const VPoint2d &p)const
{
	size_t xi,yi;
	GetBucketIndicesForOnePoint(p , xi , yi);
	const std::list<int> &curbuck = m_buckets[xi][yi];
	return curbuck;
}

