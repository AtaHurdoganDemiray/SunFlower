// TabulatedCylinder.cpp
// E122
// 1 DE Pointer pointer to the DE of the directrix curve.
// 2 LX Real coord of the terminate point of generatrix.
// 3 LY Real	( the start point of generatrix is identical 
// 4 LZ Real		 with the start of directrix ) 

#include "stdafx.h"
#include "TabulatedCylinder.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


TabulatedCylinder::TabulatedCylinder ()
{
	entitytype = 122;
}

void TabulatedCylinder::GetEntityIn ( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 4)
		throw VException (( IDS_TabGetExcNotEno),1);
	m_DE = (long) myvect[1] + poffs;
	LX = static_cast <vreal> (myvect[2]);
	LY = static_cast <vreal> (myvect[3]);
	LZ = static_cast <vreal> (myvect[4]);
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut(*this,myvect);
#endif
}
void TabulatedCylinder::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam (entitytype,strvec);
    WriteParam (m_DE,strvec);
	WriteParam(LX,strvec);
	WriteParam(LY,strvec);
	WriteParam(LZ,strvec);
}

void TabulatedCylinder::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (m_DE);
	GetDirectoryChild (childVec);
}

void TabulatedCylinder::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool TabulatedCylinder::IsPrimitiveEntity()const
//{
//	return false;
//}

//double TabulatedCylinder::GetSortNo ()const
//{
//	return 15.0;
//}

IgesEntity * TabulatedCylinder::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);	
	TabulatedCylinder *mycopy = new TabulatedCylinder (*this);
	IgesEntity *crv = mainSet.GetEntityPointer(m_DE);
	IgesEntity *newcrv = crv->MakeCopyAndAddToSet (mainSet,newSet);
	mycopy->m_DE = newcrv->directorylineno;
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
