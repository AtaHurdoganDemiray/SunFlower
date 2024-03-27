// LineCast.h
// ./doc/Casting_Line_To_2d_tool_plane.rtf -> related formulation

#ifndef __LineCast_h__
#define __LineCast_h__

template <class T> class LineCast
{
public:
	LineCast (const sVector<T,3> &Po
				,const sVector<T,3> &v
				,const sVector<T,3> &tipc);

	T m_a,m_b,m_c;
	T m_vz,m_dz;

};

#include "LineCast.inl"

#endif