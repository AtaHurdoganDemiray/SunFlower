// AABB.inl

#ifndef __AABB_inl__
#define __AABB_inl__

#include "AABB.h"
#include "VPoint2d.h"
#include "VPoint.h"
#include "VLine.h"
#include "VLine2d.h"
#include "VTriangleE.h"
#include "VTriangle2dE.h"



template <int N>
AABB<N>::AABB ()
:m_min(MAX_NUMBER_LIMIT),m_max(MIN_NUMBER_LIMIT)
{

};

template<int N>
AABB<N>::AABB(const AABB<N> &toCopy)
:m_min(toCopy.m_min),m_max(toCopy.m_max)
{
	
};

template <int N>
AABB<N> & AABB<N>::operator = (const AABB<N> &toCopy)
{
	m_min = toCopy.m_min;
	m_max = toCopy.m_max;
	return *this;
};

template <int N>
bool AABB<N>::operator == (const AABB<N> &toCompare)const
{
	if(this == &toCompare)
		return true;

	if(toCompare.m_min != toCompare.m_min)
		return false;
	if (toCompare.m_max != toCompare.m_max)
		return false;
	return true;
};

template <int N>
bool AABB<N>::operator != (const AABB<N> &toCompare)const
{
	return ! (operator == (toCompare) );
};

template <int N>
const sVector<double,N> & AABB<N>::GetMin()const
{
	return m_min;
};

template <int N>
const sVector<double,N> & AABB<N>::GetMax()const
{
	return m_max;
};
template <int N>
sVector<double,N> & AABB<N>::GetMin()
{
	return m_min;
};

template <int N>
sVector<double,N> & AABB<N>::GetMax()
{
	return m_max;
};
template <int N>
void AABB<N>::Serialize (Bifstream &infile)
{
	int version;
	infile.read ((VChr *)&version , sizeof(int));
	if (version == 1)
	{
		m_min.Serialize (infile);
		m_max.Serialize (infile);
	}
	else
		throw VException ("AABB serialization exception",1);

};
template <int N>
void AABB<N>::Serialize (Bofstream &outfile)const
{
	int version = 1;
	outfile.write((VChr*)&version , sizeof(int));
	if (version == 1)
	{
		m_min.Serialize (outfile);
		m_max.Serialize (outfile);
	}
	else
		throw VException ("AABB serialization exception" , 1);
};
//template <int N,class T>
//void AABB<N,T>::UpdateMinMax (const T &surroundedType)
//{
//	surroundedType.UpdateMinMax (m_min,m_max);
//};
//template<int N>
//void AABB<N>::Transform (const TransformationMatrix &tm)
//{
//	m_min = tm.Transform(m_min);
//	m_max = tm.Transform(m_max);
//}
template<int N>
void AABB<N>::Clear()
{
	m_min = MAX_NUMBER_LIMIT;
	m_max = MIN_NUMBER_LIMIT;
}

template<int N>
void AABB<N>::ReplaceMin (const sVector<double,N> &p)
{
	m_min.ReplaceMin (p);
}

template<int N>
void AABB<N>::ReplaceMax (const sVector<double,N> &p)
{
	m_max.ReplaceMax(p);
}

template <int N>
bool AABB<N>::CalculateNewAABBIntersectionXY
	(const VPoint &oldMin , const VPoint &oldMax
		, const VPoint &newMin , const VPoint &newMax 
	, VPoint &newAABBIntersectionMin , VPoint &newAABBIntersectionMax)
{
	VInterval ix1 (oldMin.x(),oldMax.x());
	VInterval ix2 (newMin.x(),newMax.x());

	VInterval iy1 (oldMin.y(),oldMax.y());
	VInterval iy2 (newMin.y(),newMax.y());

	std::vector<VInterval> xi = ix1.Intersect(ix2);
	std::vector<VInterval> yi = iy1.Intersect(iy2);
	if (xi.size() == 1 && yi.size() == 1)
	{
		newAABBIntersectionMin.x(xi.front().GetT0());
		newAABBIntersectionMin.y(yi.front().GetT0());
		newAABBIntersectionMax.x(xi.front().GetT1());
		newAABBIntersectionMax.y(yi.front().GetT1());
		return true;
	}
	return false;
}

#endif
