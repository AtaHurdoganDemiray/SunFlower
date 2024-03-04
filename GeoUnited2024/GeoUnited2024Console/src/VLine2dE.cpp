// VLine2dE.cpp

#include "stdafx.h"
#include "VLine2dE.h"


VLine2dE::VLine2dE()
:m_L(VPoint2d(0.,0.),VPoint2d(1.,0.)),m_N(0.,1.),m_C(0.)
{

}
VLine2dE::VLine2dE(VLine2dE::DegenerateType t)
:m_L (VPoint2d (0.),VPoint2d (0.)),m_N (0.)
{
	switch (t)
	{
	case emptyset:
		m_C = 1.;
		break;
	case infiniteset:
		m_C = -1.;
		break;
	}
}
VLine2dE::VLine2dE (const VLine2d &l)
:m_L (l),m_N(m_L.GetNormal())
{
	m_C = m_N%m_L.GetP1();
}
VLine2dE::VLine2dE(VPoint2d p1,VPoint2d p2)
:m_L(p1,p2),m_N(m_L.GetNormal())
{
	//C=Dot(N,X) when X is on line
	m_C=m_N%p1;
}
VLine2dE::VLine2dE(const VLine2dE &toCopy)
:m_L(toCopy.m_L),m_N(toCopy.m_N),m_C(toCopy.m_C)
{

}
VLine2dE VLine2dE::operator = (const VLine2dE &toCopy)
{
	m_L=toCopy.m_L;
	m_N=toCopy.m_N;
	m_C=toCopy.m_C;

	return *this;
}
VLine2dE::~VLine2dE()
{

}
const VPoint2d & VLine2dE::GetP1() const
{
	return m_L.GetP1();
}
const VPoint2d & VLine2dE::GetP2() const
{
	return m_L.GetP2();
}
VPoint2d & VLine2dE::GetP1()
{
	return m_L.GetP1();
}
VPoint2d & VLine2dE::GetP2()
{
	return m_L.GetP2();
}
VPoint2d VLine2dE::GetP ( const vreal & sofline ) const
{
	return m_L.GetP(sofline);
}
VPoint2d VLine2dE::GetV() const
{
	return m_L.GetV();
}
void VLine2dE::SetP1 ( const VPoint2d & apoint )
{
	m_L.SetP1(apoint);
	m_N = m_L.GetNormal();
	m_C = m_L.GetP1() % m_N;
}
void VLine2dE::SetP2 ( const VPoint2d & apoint )
{
	m_L.SetP2(apoint);
	m_N = m_L.GetNormal();
	m_C = m_L.GetP1() % m_N;
}
void VLine2dE::Serialize (Bofstream &binfile) const
{	
	m_L.Serialize(binfile);
	m_N.Serialize(binfile);
	binfile.write((VChr*)&m_C,sizeof(vreal));
}	
void VLine2dE::Serialize (Bifstream &binfile)
{
	m_L.Serialize(binfile);
	m_N.Serialize(binfile);
	binfile.read((VChr*)&m_C,sizeof(vreal));
}
bool VLine2dE::operator == (const VLine2dE &toCompare)const
{
	if(this==&toCompare)
		return true;

	if(m_C!=toCompare.m_C)
		return false;
	if(m_N!=toCompare.m_N)
		return false;
	if(m_L!=toCompare.m_L)
		return false;

	return true;
}
bool VLine2dE::operator != (const VLine2dE &toCompare)const
{
	return !(operator==(toCompare));
}
bool VLine2dE::IsCorrupted () const
{
	return m_L.IsCorrupted();
}
const UnitVector2d & VLine2dE::GetNormal()const
{
	return m_N;	
}
const UnitVector2d & VLine2dE::N()const
{
	return m_N;	
}
const vreal & VLine2dE::C()const
{
	return m_C;
}
const VLine2d & VLine2dE::L()const
{
	return m_L;
}
UnitVector2d & VLine2dE::N()
{
	return m_N;	
}
vreal & VLine2dE::C()
{
	return m_C;
}
VLine2d & VLine2dE::L()
{
	return m_L;
}

VLine2dE VLine2dE::GetReversed() const
{
	VLine2dE rev (GetP2(),GetP1());
	return rev;
}

void VLine2dE::Reverse()
{
	VPoint2d tmp (GetP1());
	m_L.SetP1 (m_L.GetP2());
	m_L.SetP2 (tmp);
	m_C *= -1.;
	m_N *= -1.;
}

// if two line segments intersect return value is int param of this line ,between[0,1]
// if no intersection return value is -1
// if lines are parallel and coincide  return value is -2
vreal VLine2dE::FastIntCalc(const VLine2dE &l2) const
{
	vreal cl21 = m_N % l2.GetP1() - m_C;
	vreal cl22 = m_N % l2.GetP2() - m_C;
	
	if (cl22 * cl21 > 0.0 && fabs(cl21)>EPSILON && fabs(cl22)>EPSILON )
	{// linesegments are not intersecting
		return -1.0;
	}
	else if (fabs(cl21) < EPSILON && fabs (cl22) < EPSILON)
	{//linesegments are parallel and coincide
		return -2.0;
	}
	vreal cl11 = l2.GetNormal() % m_L.GetP1() - l2.C();
	vreal cl12 = l2.GetNormal() % m_L.GetP2() - l2.C();
	if (cl11 * cl12 > 0.0 && fabs(cl11)> EPSILON && fabs(cl12)> EPSILON)
	{// linesegments are not intersecting
		return -1.0;
	}
	else
	{
		// there is intersection and intersection param is
		return cl11 / (cl11 - cl12);
	}
}
