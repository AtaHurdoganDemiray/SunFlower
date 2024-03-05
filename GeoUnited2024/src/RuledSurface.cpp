// RuledSurface.cpp

// Entity No : 118
//Index__      Name____     Type___   Description___
//1            DE1         Pointer   Pointer to the DE of the first curve entity
//2            DE2         Pointer   Pointer to the DE of the second curve entity
//3            DIRFLG      Integer   Direction flag:
//                                      0=Join first to first, last to last
//                                      1=Join first to last, last to first
//4            DEVFLG      Integer   Developable surface flag:
//                                      1=Developable
//                                      0=Possibly not
#include "stdafx.h"
#include "RuledSurface.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


RuledSurface::RuledSurface (RuledSurface::Type atype)
:m_type (atype)
{
	entitytype = 118;
	m_DIRFLG = 0;
	m_DEVFLG=1;
}

RuledSurface::~RuledSurface()
{

}

void RuledSurface::GetEntityIn(FileLines &mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size()<=4)
		throw VException (( IDS_RulGetExcNotEno),1);
	m_DE1 = (int)myvect[1] + poffs;
	m_DE2 = (int)myvect[2] + poffs;	
	m_DIRFLG = (int)myvect[3];
	m_DEVFLG = (int)myvect[4];
}

void RuledSurface::WriteEntityOut(std::vector<VStr> &strvec) const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_DE1 , strvec);
	WriteParam (m_DE2 , strvec);
	WriteParam (m_DIRFLG , strvec);
	WriteParam (m_DEVFLG , strvec);
}

void RuledSurface::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (m_DE1);
	childVec.push_back (m_DE2);
	GetDirectoryChild (childVec);
}

void RuledSurface::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool RuledSurface::IsPrimitiveEntity()const
//{
//	return false;
//}

//double RuledSurface::GetSortNo ()const
//{
//	return 13.0;
//}

IgesEntity * RuledSurface::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);
	RuledSurface *mycopy = new RuledSurface (*this);
	
	IgesEntity *crv1 = mainSet.GetEntityPointer (m_DE1);
	if (crv1)
	{
		IgesEntity *newcrv1 = crv1->MakeCopyAndAddToSet (mainSet,newSet);
		mycopy->m_DE1 = newcrv1->directorylineno;
	}

	IgesEntity *crv2 = mainSet.GetEntityPointer (m_DE2);
	if (crv2)
	{
		IgesEntity *newcrv2=crv2->MakeCopyAndAddToSet (mainSet,newSet);
		mycopy->m_DE2 = newcrv2->directorylineno;
	}
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
