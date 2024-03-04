// Arc.cpp
// E100
//Index__      Name____     Type___   Description___
//1            ZT           Real      Parallel ZT displacement of arc from XT, YT plane
//2            X1           Real      Arc center abscissa
//3            Y1           Real      Arc center ordinate
//4            X2           Real      Start point abscissa
//5            Y2           Real      Start point ordinate
//6            X3           Real      Terminate point abscissa
//7            Y3           Real      Terminate point ordinate

#include "stdafx.h"
#include "Arc.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"
#include "VArc.h"

igesArc::igesArc ()
{
	entitytype = 100;
}

void igesArc::GetEntityIn( FileLines & mylines ,int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 7)
		throw VException (( IDS_IgeGetExcNotEno),1);
	mZT = myvect[1];
	mX1 = myvect[2];
	mY1 = myvect[3];
	mX2 = myvect[4];
	mY2 = myvect[5];
	mX3 = myvect[6];
	mY3 = myvect[7];
	ReadModelId(8,myvect);
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut (*this,myvect);
#endif
}

void igesArc::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (mZT , strvec);
	WriteParam (mX1,strvec);
	WriteParam (mY1,strvec);
	WriteParam (mX2,strvec);
	WriteParam (mY2,strvec);
	WriteParam (mX3,strvec);
	WriteParam (mY3,strvec);
	WriteModelId(strvec);
}

void igesArc::GetChildren (std::vector<int> &childVec)const
{
	GetDirectoryChild (childVec);
}

void igesArc::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool igesArc::IsPrimitiveEntity()const
//{
//	return true;
//}

//double igesArc::GetSortNo ()const
//{
//	return 5.0;
//}

IgesEntity * igesArc::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer (m_subsetIndex);

	igesArc *mycopy = new igesArc (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}

void igesArc::OverrideBigZCoordinates(double zLimit, double zWork)
{
	if (fabs(mZT) > zLimit)
		mZT = zWork;
}

//VArc igesArc::GetUntransformedVArc()const
//{
//	VPoint centerPoint (mX1,mY1,mZT);
//	VPoint startPoint (mX2,mY2,mZT);
//	VPoint endPoint (mX3,mY3,mZT);
//	vreal rad = (startPoint-centerPoint).GetLength();
//	VArc myArc (centerPoint,rad,UZ,startPoint,endPoint);
//	return myArc;
//}