// ToolCircle.inl

#ifndef __ToolCircle_inl__
#define __ToolCircle_inl__

#include "ToolCircle.h"

template <class T>
ToolCircle<T>::ToolCircle(const T &rad, const sVector<T,2> cen)
:m_rad(rad),m_cen(cen)
{
	m_rad2 = m_rad*m_rad;
	m_type = circle;
}

template <class T>
bool ToolCircle<T>::IsIncluding(const sVector<T,2> &p) const
{
	sVector<T,2> d = p - m_cen;
	if (d.GetLength2() > m_rad2)
		return false;
	return true;
};

template <class T>
T ToolCircle<T>::GetMaxRadius() const
{
	return (m_rad+m_cen[0]);
};
template <class T>
T ToolCircle<T>::GetStartH()const
{
	return 0.;
}
template <class T>
void ToolCircle<T>::AcceptVisitor(ToolPrimVisitor<T> &visitor)const
{
	visitor.Visit(*this);
}

template <class T>
const sVector<T,2> & ToolCircle<T>::GetCenter()const
{
	return m_cen;
}

template <class T>
const T & ToolCircle<T>::GetRadius()const
{
	return m_rad;
}

template <class T>
T ToolCircle<T>::GetRadiusAt(T height) const
{
	T h1 = m_cen[1] - m_rad;
	T h2 = m_cen[1] + m_rad;
	if (height < h1)
		return -1;
	if (height > h2)
		return m_rad;
	T ceny = fabs(height - m_cen[1]);
	T myx = sqrt (m_rad2 - ceny*ceny);
	return m_cen[0] + myx;
}

template <class T>
void ToolCircle<T>::GetMinMax (T &minx , T &maxx , T &miny , T &maxy)const
{
	minx = 0;
	maxx = m_cen[0] + m_rad;
	miny = 0;
	maxy = m_cen[1];
}

template <class T>
T ToolCircle<T>::GetHeightAtRadius (T rad)const
{
	T baseh = m_cen[1] - m_rad;
	if (rad < m_cen[0])
		return baseh;
	else
	{
		if (rad > m_rad + m_cen[0])
			return baseh + m_rad;
		//else if (rad < m_cen[0])
		//	return baseh;
		else
		{
			T h = m_rad - sqrt (m_rad*m_rad - (rad-m_cen[0])*(rad-m_cen[0]));
			return h+baseh;
		}
	}
}

#endif