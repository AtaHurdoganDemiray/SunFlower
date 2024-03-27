// ToolVerticalLine.inl

#ifndef __ToolVerticalLine_inl__
#define __ToolVerticalLine_inl__
#include "ToolVerticalLine.h"

template <class T>
ToolVerticalLine<T>::ToolVerticalLine (const sVector<T,2> &lowerp , const T &ytop)
:m_lowerp(lowerp),m_ytop(ytop)
{
	m_r2 = m_lowerp[0] * m_lowerp[0];
	m_type = verticalline;
}

template <class T>
bool ToolVerticalLine<T>::IsIncluding (const sVector<T,2> &p)const
{
	if (p[0] > m_lowerp[0])
		return false;
	if ( p[1] < m_lowerp[1] || p[1] > m_ytop)
		return false;

	return true;
};

template <class T>
T ToolVerticalLine<T>::GetMaxRadius() const
{
	return m_lowerp[0];
};
template<class T>
T ToolVerticalLine<T>::GetStartH()const
{
	return m_lowerp[1];
}
template <class T>
void ToolVerticalLine<T>::AcceptVisitor (ToolPrimVisitor<T> &visitor)const
{
	visitor.Visit (*this);
}

template <class T>
const sVector<T,2> & ToolVerticalLine<T>::GetLowerPoint()const
{
	return m_lowerp;
}

template <class T>
const T  ToolVerticalLine<T>::GetHeight ()const
{
	return m_ytop - m_lowerp[1];
}

template <class T>
bool ToolVerticalLine<T>::IsColl (const LineCast<T> &l)const
{
	if (QdrEqSolver<T>::IsPositiveIn01 (l.m_a,l.m_b,l.m_c-m_r2) )
		return true;
	else
		return false;
}

template<class T>
T ToolVerticalLine<T>::GetRadiusAt(T height) const
{
	T h1 = m_lowerp[1];
	T h2 = m_ytop;
	if (height < h1)
		return -1.;
	if (height > h2)
		return m_lowerp[0];
	return m_lowerp[0];
}

template<class T>
void ToolVerticalLine<T>::GetMinMax (T &minx , T &maxx , T &miny , T &maxy)const
{
	minx = 0;
	maxx = m_lowerp[0];
	miny = m_lowerp[1];
	maxy = m_ytop;
}

template<class T>
T ToolVerticalLine<T>::GetHeightAtRadius (T rad)const
{
	if (rad < m_lowerp[0])
		return m_lowerp[1];
	else
		return m_ytop;
}
#endif