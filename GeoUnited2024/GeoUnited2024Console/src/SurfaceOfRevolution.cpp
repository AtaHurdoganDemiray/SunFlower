// SurfaceOfRevolution.cpp
// E = 120
// 1 L Pointer pointer to the DE of line entity
// 2 C Pointer pointer to the DE of curve entity
// 3 SA Real start angle in radians
// 4 TA Real terminate angle in radians

#include "stdafx.h"
#include "SurfaceOfRevolution.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


SurfaceOfRevolution::SurfaceOfRevolution ()
{
	entitytype = 120;
}

void SurfaceOfRevolution::GetEntityIn ( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size()<=4)
		throw VException (( IDS_SurGetExcNotEno),1);
	m_L = static_cast <long> (myvect[1]) + poffs;
	m_C = static_cast <long> (myvect[2]) + poffs;
	SA = static_cast <vreal> (myvect[3]);
	TA = static_cast <vreal> (myvect[4]);
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut (*this,myvect);
#endif
}

void SurfaceOfRevolution::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_L,strvec);
	WriteParam (m_C,strvec);
	WriteParam (SA,strvec);
	WriteParam (TA,strvec);
}

void SurfaceOfRevolution::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (m_L);
	childVec.push_back (m_C);
	GetDirectoryChild (childVec);
}

void SurfaceOfRevolution::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool SurfaceOfRevolution::IsPrimitiveEntity()const
//{
//	return false;
//}

//double SurfaceOfRevolution::GetSortNo ()const
//{
//	return 14.0;
//}

IgesEntity * SurfaceOfRevolution::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);
	SurfaceOfRevolution * mycopy = new SurfaceOfRevolution (*this);

	IgesEntity *myl = mainSet.GetEntityPointer (m_L);
	if (myl)
	{
		IgesEntity *newl = myl->MakeCopyAndAddToSet (mainSet,newSet);
		mycopy->m_L = newl->directorylineno;
	}

	IgesEntity *myc = mainSet.GetEntityPointer (m_C);
	if(myc)
	{
		IgesEntity *newc = myc->MakeCopyAndAddToSet (mainSet,newSet);
		mycopy->m_C = newc->directorylineno;
	}
	
	CopyDE(*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
