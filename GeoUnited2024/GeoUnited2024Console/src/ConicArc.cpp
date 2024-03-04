// ConicArc.cpp
// Entity No: 104
//Parameter Data
//Index__      Name____     Type___   Description___
// 1           A            Real      Conic Coefficient
// 2           B            Real      Conic Coefficient
// 3           C            Real      Conic Coefficient
// 4           D            Real      Conic Coefficient
// 5           E            Real      Conic Coefficient
// 6           F            Real      Conic Coefficient
// 7           ZT           Real      ZT Coordinate of plane of definition
// 8           X1           Real      Start Point Abscissa
// 9           Y1           Real      Start Point Ordinate
//10           X2           Real      Terminate Point Abscissa
//11           Y2           Real      Terminate Point Ordinate
//  A * XT 2 + B * XT * Y T  + C * Y T 2 + D * XT  + E * Y T  + F  = 0
#include "stdafx.h"
#include "ConicArc.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"


ConicArc::ConicArc()
{
	entitytype = 104;
}	
ConicArc::~ConicArc()
{

}

void ConicArc::GetEntityIn (FileLines & mylines ,int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size()<=11)
		throw VException (( IDS_ConGetExcNotEno),1);
	m_A = myvect[1];
	m_B = myvect[2];	
	m_C = myvect[3];	
	m_D = myvect[4];	
	m_E = myvect[5];	
	m_F = myvect[6];	
	m_ZT = myvect[7];
	m_X1 = myvect[8];
	m_Y1 = myvect[9];
	m_X2 = myvect[10];
	m_Y2 = myvect[11];
	ReadModelId(12, myvect);
}

void ConicArc::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_A , strvec);
	WriteParam (m_B , strvec);
	WriteParam (m_C , strvec);
	WriteParam (m_D , strvec);
	WriteParam (m_E , strvec);
	WriteParam (m_F , strvec);
	WriteParam (m_ZT , strvec);
	WriteParam (m_X1 , strvec);
	WriteParam (m_Y1 , strvec);
	WriteParam (m_X2 , strvec);
	WriteParam (m_Y2 , strvec);
	WriteModelId(strvec);
}

void ConicArc::GetChildren (std::vector<int> &childVec)const
{
	GetDirectoryChild (childVec);
}

void ConicArc::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool ConicArc::IsPrimitiveEntity()const
//{
//	return true;
//}

//double ConicArc::GetSortNo ()const
//{
//	return 6.0;
//}

IgesEntity * ConicArc::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer(m_subsetIndex);

	ConicArc *mycopy = new ConicArc (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity(mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
