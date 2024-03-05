// PlaneEntity.cpp
//Parameter Data
//Index__      Name____     Type___   Description___
//1            A            Real      Coefficients of Plane
//2            B            Real      Coefficients of Plane
//3            C            Real      Coefficients of Plane
//4            D            Real      Coefficients of Plane
//5            PTR         Pointer   Pointer to the DE of the closed curve entity
//6            X            Real      XT coordinate of location point for display symbol
//7            Y            Real      YT coordinate of location point for display symbol
//8            Z            Real      ZT coordinate of location point for display symbol
//9            SIZE        Real      Size parameter for display symbol
#include "stdafx.h"
#include "PlaneEntity.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


PlaneEntity::PlaneEntity ()
{
	entitytype = 108;
}
PlaneEntity::~PlaneEntity ()
{

}
void PlaneEntity::GetEntityIn(FileLines &mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size()<=9)
		throw VException (( IDS_PlaGetExcNotEno),1);
	m_A = myvect[1];
	m_B = myvect[2];
	m_C = myvect[3];
	m_D = myvect[4];
	m_PTR = (int)(myvect[5])+poffs;
	m_X = myvect[6];
	m_Y = myvect[7];
	m_Z = myvect[8];
	m_SIZE = myvect[9];
}

void PlaneEntity::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_A , strvec);
	WriteParam (m_B , strvec);
	WriteParam (m_C , strvec);
	WriteParam (m_D , strvec);
	WriteParam (m_PTR , strvec);
	WriteParam (m_X , strvec);
	WriteParam (m_Y , strvec);
	WriteParam (m_Z , strvec);
	WriteParam (m_SIZE , strvec);
}

void PlaneEntity::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (m_PTR);
	GetDirectoryChild (childVec);
}

void PlaneEntity::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool PlaneEntity::IsPrimitiveEntity()const
//{
//	return false;
//}

//double PlaneEntity::GetSortNo ()const
//{
//	return 16.5;
//}

IgesEntity * PlaneEntity::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);
	PlaneEntity * mycopy = new PlaneEntity (*this);
	IgesEntity *crv = mainSet.GetEntityPointer (m_PTR);
	if (crv)
	{
		IgesEntity *newcrv = crv->MakeCopyAndAddToSet (mainSet,newSet);
		mycopy->m_PTR = newcrv->directorylineno;
	}
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
