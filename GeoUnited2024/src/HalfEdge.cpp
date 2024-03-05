// HalfEdge.cpp

#include "stdafx.h"
#include "HalfEdge.h"

double HalfEdge::m_epsilon = 1.e-8;

HalfEdge::HalfEdge(int v0 , int v1)
:m_v0 (v0) , m_v1 (v1) , m_opposite (0) , m_next (0)
{

}

int HalfEdge::GetV0() const
{
	return m_v0;
}

int HalfEdge::GetV1() const
{
	return m_v1;
}

void HalfEdge::SetV0(int v0)
{
	m_v0 = v0;
}

void HalfEdge::SetV1(int v1)
{
	m_v1 = v1;
}

HalfEdge * HalfEdge::GetOpposite() const
{
	return m_opposite;
}

void HalfEdge::SetOpposite(HalfEdge *opposite)
{
	m_opposite = opposite;
}

HalfEdge * HalfEdge::GetNext()const
{
	return m_next;
}
void HalfEdge::SetNext (HalfEdge *next)
{
	m_next = next;
}

HalfEdge * HalfEdge::GetContaining (const VPoint &p)const
{
	return 0;
}

int HalfEdge::GetLoopId ()const
{
	return m_loopid;
}

void HalfEdge::SetLoopId (int loopid)
{
	m_loopid = loopid;
}
