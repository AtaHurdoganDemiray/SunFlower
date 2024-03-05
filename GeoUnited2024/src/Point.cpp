// Point.cpp

#include "stdafx.h"
#include "Point.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"
#include "VPoint.h"

IgesPoint::IgesPoint ()
{
	entitytype = 116;
	m_PTR = 0;
}
IgesPoint::IgesPoint(const VPoint &p)
{
	entitytype = 116;
	m_PTR = 0;
	m_X = p.x();
	m_Y = p.y();
	m_Z = p.z();
}
void IgesPoint::GetEntityIn ( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 3)
		throw VException (( IDS_PoiGetExcNotEno),1);
	m_X = static_cast<vreal>(myvect[1]);
	m_Y = static_cast<vreal>(myvect[2]);
	m_Z = static_cast<vreal>(myvect[3]);
	if (myvect.size()>4)
		m_PTR = static_cast<int>(myvect[4]) + poffs;
	else 
		m_PTR=0;
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut (*this,myvect);	
#endif
}

void IgesPoint::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam(entitytype,strvec);
	WriteParam (m_X,strvec);
	WriteParam (m_Y,strvec);
	WriteParam (m_Z,strvec);
	if (m_PTR)
		WriteParam (m_PTR,strvec);
}

void IgesPoint::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (m_PTR);
	GetDirectoryChild (childVec);
}

void IgesPoint::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool IgesPoint::IsPrimitiveEntity()const
//{
//	return false;
//}

//double IgesPoint::GetSortNo ()const
//{
//	return 9.8;
//}

IgesEntity * IgesPoint::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);
	IgesPoint *mycopy = new IgesPoint (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
