// LineCast.inl

#ifndef __LineCast_inl__
#define __LineCast_inl__

template <class T>
LineCast<T>::LineCast(const sVector<T,3> &Po, const sVector<T,3> &v, const sVector<T,3> &tipc)
{
	sVector<T,3> d = Po - tipc;
	sVector<T,2> d2d (d.GetArray());
	sVector<T,2> v2d (v.GetArray());
	m_a = v2d % v2d;
	m_b = 2 * v2d % d2d;
	m_c = d2d % d2d;

	m_dz = d[2];
	m_vz = v[2];
};

#endif