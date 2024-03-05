// PlaneEntity.h
// Entity No:108
//Parameter Data
//
//
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

#ifndef __PlaneEntity_h__
#define __PlaneEntity_h__
#include "IgesEntity.h"

class PlaneEntity:public IgesEntity
{
public:
	PlaneEntity();
	~PlaneEntity();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;	
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	vreal m_A,m_B,m_C,m_D;
	long m_PTR;
	vreal m_X,m_Y,m_Z;
	vreal m_SIZE;
};

#endif
