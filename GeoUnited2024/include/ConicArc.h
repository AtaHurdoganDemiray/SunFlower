// ConicArc.h
// Entity No: 104
//Parameter Data
//
//
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

#ifndef __ConicArc_h__
#define __ConicArc_h__

#include "IgesEntity.h"

class ConicArc:public IgesEntity
{
public:
	ConicArc();
	virtual ~ConicArc();
	virtual void GetEntityIn (FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	vreal m_A,m_B,m_C,m_D,m_E,m_F;
	vreal m_ZT;
	vreal m_X1,m_Y1,m_X2,m_Y2;
};

#endif