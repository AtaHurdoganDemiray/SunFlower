// igesTransformationMatrix.cpp
//Index__      Name____     Type___   Description___
// 1           R11         Real      Top Row
// 2           R12         Real      .
// 3           R13         Real      .
// 4           T1           Real      .
// 5           R21         Real      Second Row
// 6           R22         Real      .
// 7           R23         Real      .
// 8           T2           Real      .
// 9           R31         Real      Third Row
//10           R32         Real      .
//11           R33         Real      .
//12           T3           Real      .

#include "stdafx.h"
#include "igesTransformationMatrix.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


igesTransformationMatrix::igesTransformationMatrix()
{
	entitytype = 124;
	//m_prev = 0;
}

void igesTransformationMatrix::GetEntityIn ( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 12)
        throw VException (( IDS_A2),1);
	mR11 = myvect[1];
	mR12 = myvect[2];
	mR13 = myvect[3];
	mT1 = myvect[4];	
	mR21 = myvect[5];
	mR22 = myvect[6];
	mR23 = myvect[7];
	mT2 = myvect[8];
	mR31 = myvect[9];
	mR32 = myvect[10];
	mR33 = myvect[11];
	mT3 = myvect[12];
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut (*this,myvect);
#endif
}

void igesTransformationMatrix::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (mR11 , strvec);
	WriteParam (mR12 , strvec);
	WriteParam (mR13 , strvec);
	WriteParam (mT1 , strvec);
	WriteParam (mR21 , strvec);
	WriteParam (mR22 , strvec);
	WriteParam (mR23 , strvec);
	WriteParam (mT2 , strvec);
	WriteParam (mR31 , strvec);
	WriteParam (mR32 , strvec);
	WriteParam (mR33 , strvec);
	WriteParam (mT3 , strvec);
}

void igesTransformationMatrix::GetChildren (std::vector<int> &childVec)const
{

}

void igesTransformationMatrix::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

bool igesTransformationMatrix::IsEqualavent (const igesTransformationMatrix &toCompare)const
{
	return  (fabs (mT1 - toCompare.mT1) < EPSILON) &&
			(fabs (mT2 - toCompare.mT2) < EPSILON) &&
			(fabs (mT3 - toCompare.mT3) < EPSILON) &&
			
			(fabs (mR11 - toCompare.mR11) < EPSILON) &&
			(fabs (mR12 - toCompare.mR12) < EPSILON) &&	
			(fabs (mR13 - toCompare.mR13) < EPSILON) &&

			(fabs (mR21 - toCompare.mR21) < EPSILON) &&
			(fabs (mR22 - toCompare.mR22) < EPSILON) &&
			(fabs (mR23 - toCompare.mR23) < EPSILON) &&

			(fabs (mR31 - toCompare.mR31) < EPSILON) &&
			(fabs (mR32 - toCompare.mR32) < EPSILON) &&
			(fabs (mR33 - toCompare.mR33) < EPSILON) ;
}

//bool igesTransformationMatrix::IsPrimitiveEntity()const
//{
//	return true;
//}

//double igesTransformationMatrix::GetSortNo()const
//{
//	return 1.0;
//}

IgesEntity * igesTransformationMatrix::MakeCopyAndAddToSet (const IgesEntitySet &mainSet,IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer(m_subsetIndex);
	igesTransformationMatrix *mycopy = new igesTransformationMatrix (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}

//igesTransformationMatrix & igesTransformationMatrix::operator =(const igesTransformationMatrix &toCopy)
//{
//	IgesEntity::operator = (toCopy);	
//	mR11=toCopy.mR11;
//	mR12=toCopy.mR12;
//	mR13=toCopy.mR13;
//	mR21=toCopy.mR21;
//	mR22=toCopy.mR22;
//	mR23=toCopy.mR23;
//	mR31=toCopy.mR31;
//	mR32=toCopy.mR32;
//	mR33=toCopy.mR33;
//	mT1=toCopy.mT1;
//	mT2=toCopy.mT2;
//	mT3=toCopy.mT3;
//	return *this;
//}

void igesTransformationMatrix::OverrideBigZCoordinates(double zLimit, double zWork)
{
	if (fabs (mT3) > zLimit)
		mT3 = zWork;
}
