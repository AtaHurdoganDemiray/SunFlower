// ToolLine.inl

#ifndef __ToolLine_inl__
#define __ToolLine_inl__

#include "ToolLine.h"
#include "QdrEqSolver.h"

template <class T> 
ToolLine<T>::ToolLine (const sVector<T,2> &lowerp , const sVector <T,2> &upperp)
:m_lowerp (lowerp) , m_upperp(upperp)
{
	sVector<T,2> d = m_upperp - m_lowerp;
	d.NormThis();
	m_n.SetElement (d[1] , 0);
	m_n.SetElement (-d[0], 1);
	m_c = m_lowerp % m_n;

	m_kt = (upperp[0] - lowerp[0]) / (upperp[1] - lowerp[1]);
	m_ct = lowerp[0] - m_kt * lowerp[1];
	m_kt2 = m_kt*m_kt;
	m_ct2 = m_ct*m_ct;
	m_2ktct=2 * m_kt * m_ct;
	m_type = line;
}

//template <class T> 
//const ToolPrim::PrimType ToolLine<T>::GetType()const
//{
//	return line;
//};

template <class T>
bool ToolLine<T>::IsIncluding (const sVector<T,2> &p)const
{
	if (p[1] < m_lowerp[1] || p[1] > m_upperp[1])
		return false;
	if (p[0]*m_n[0] + p[1]*m_n[1]  > m_c)
		return false;
	return true;
}

template <class T> 
T ToolLine<T>::GetMaxRadius() const
{
	return (m_upperp[0] > m_lowerp[0])? m_upperp[0] : m_lowerp[0];
}
template <class T>
T ToolLine<T>::GetStartH()const
{
	return m_lowerp[1];
}
template <class T>
void ToolLine<T>::AcceptVisitor (ToolPrimVisitor<T> &visitor)const
{
	visitor.Visit (*this);
}

template <class T>
const sVector<T,2> & ToolLine<T>::GetLowerPoint()const
{
	return m_lowerp;
}

template <class T>
const sVector<T,2> & ToolLine<T>::GetUpperPoint()const
{
	return m_upperp;
}

template <class T>
const T ToolLine<T>::GetHeight()const
{
	return m_upperp[1] - m_lowerp[1];
}

template <class T>
bool ToolLine<T>::IsColl (const LineCast<T> &l)const
{
	T a2 = m_kt2 * l.m_vz * l.m_vz;
	T b2 = 2 * l.m_dz * l.m_vz * m_kt2 + m_2ktct * l.m_vz;
	T c2 = m_kt2 * l.m_dz*l.m_dz + m_2ktct * l.m_dz + m_ct2;
	T Ea = l.m_a-a2;
	T Eb = l.m_b-b2;
	T Ec = l.m_c-c2;
	if (QdrEqSolver<T>::IsPositiveIn01 (l.m_a-a2
									,l.m_b-b2
									,l.m_c-c2) )
		return true;
	else
		return false;
}

template<class T>
T ToolLine<T>::GetRadiusAt(T height)const
{
	T h1 = m_lowerp[1];
	T h2 = m_upperp[1];
	if (height < h1)
		return -1;
	T r1 = m_lowerp[0];
	T r2 = m_upperp[0];
	if (height > h2)
		return r2;
	T dr = r2 - r1;
	T dh = h2 - h1;

	T myr = r1 + (dr/dh) * (height - h1);
	return myr;
}

template<class T>
void ToolLine<T>::GetMinMax (T &minx , T &maxx , T &miny , T &maxy)const
{
	minx = 0;
	maxx = m_upperp[0];
	miny = m_lowerp[1];
	maxy = m_upperp[1];
}

template<class T>
T ToolLine<T>::GetHeightAtRadius (T rad)const
{
	if (rad < m_lowerp[0])
		return m_lowerp[1];
	else if (rad < m_upperp[0] + EPSILON)
	{
		T h = m_lowerp[1] + (m_upperp[1] - m_lowerp[1]) * (rad - m_lowerp[0]) / (m_upperp[0] - m_lowerp[0]) ;
		return h;
	}
	else
		return m_upperp[1];
}
#endif