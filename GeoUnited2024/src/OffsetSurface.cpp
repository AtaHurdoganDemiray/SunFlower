// OffsetSurface.cpp
/*

Entity type 140

Parameter Data


Index__      Name____     Type___   Description___
 1           NX           Real      The x-coordinate of the offset indicator N (U m; V m)
 2           NY           Real      The y-coordinate of the offset indicator N (U m; V m)
 3           NZ           Real      The z-coordinate of the offset indicator N (U m; V m)
 4           D            Real      The distance by which the surface is normally offset on the side
                                    of the offset indicator if d > 0 and on the opposite side if d < 0
 5           DE           Pointer   Pointer to the DE of the surface entity to be offset

*/

#include "stdafx.h"
#include "OffsetSurface.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


OffsetSurface::OffsetSurface ()
{
	this->entitytype = 140;
}

//#############################################################################

void OffsetSurface::GetEntityIn ( FileLines &mylines ,int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 5)
		throw VException (( IDS_OffGetExcNotEno),1);
	m_NX = static_cast <vreal> (myvect[1]);
	m_NY = static_cast <vreal> (myvect[2]);
	m_NZ = static_cast <vreal> (myvect[3]);
	m_D =  static_cast <vreal> (myvect[4]);
	m_DE = static_cast <long> (myvect[5]) + poffs;
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut (*this , myvect);	
#endif
}

//#############################################################################

void OffsetSurface::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_NX , strvec);
	WriteParam (m_NY , strvec);
	WriteParam (m_NZ , strvec);
	WriteParam (m_D , strvec);
	WriteParam (m_DE , strvec);
}

void OffsetSurface::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (m_DE);
	GetDirectoryChild (childVec);
}

void OffsetSurface::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool OffsetSurface::IsPrimitiveEntity()const
//{
//	return false;
//}

//double OffsetSurface::GetSortNo ()const
//{
//	return 16.0;
//}

IgesEntity * OffsetSurface::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);
	OffsetSurface *mycopy = new OffsetSurface (*this);
	IgesEntity *surf = mainSet.GetEntityPointer (m_DE);
	IgesEntity *newsurf = surf->MakeCopyAndAddToSet (mainSet,newSet);
	mycopy->m_DE = newsurf->directorylineno;
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
