// Line.cpp
// E 110
//Parameter Data
//
//
//Index__      Name____     Type___   Description___
//1            X1           Real      Start Point P 1
//2            Y1           Real
//3            Z1           Real
//4            X2           Real      Terminate Point P 2
//5            Y2           Real
//6            Z2           Real

#include "stdafx.h"
#include "Line.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


Line::Line()
{
	entitytype = 110;
}

void Line::GetEntityIn ( FileLines & mylines ,int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 6)
		throw VException (( IDS_LinGetExcNotEno),1);

	X1 = static_cast<vreal>(myvect[1]);
	Y1 = static_cast<vreal>(myvect[2]);
	Z1 = static_cast<vreal>(myvect[3]);
	
	X2 = static_cast<vreal>(myvect[4]);
	Y2 = static_cast<vreal>(myvect[5]);
	Z2 = static_cast<vreal>(myvect[6]);
	ReadModelId(7,myvect);

#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut (*this , myvect);
#endif
}

void Line::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (X1 , strvec);
	WriteParam (Y1 , strvec);
	WriteParam (Z1 , strvec);
	WriteParam (X2 , strvec);
	WriteParam (Y2 , strvec);
	WriteParam (Z2 , strvec);
	WriteModelId(strvec);
}

void Line::GetChildren (std::vector<int> &childVec)const
{
	GetDirectoryChild (childVec);
}

void Line::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool Line::IsPrimitiveEntity()const
//{
//	return true;
//}

//double Line::GetSortNo ()const
//{
//	return 4.0;
//}

IgesEntity * Line::MakeCopyAndAddToSet (const IgesEntitySet &mainSet,IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);
	Line *mycopy = new Line (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}

void Line::OverrideBigZCoordinates(double zLimit, double zWork)
{
	if (fabs (Z1) > zLimit)
		Z1 = zWork;
	if (fabs (Z2) > zLimit)
		Z2 = zWork;
}
