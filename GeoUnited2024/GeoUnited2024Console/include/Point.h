// Point.h
// Entity no : 116

//        Index__      Name____     Type___   Description___
//          1            X            Real      Coordinates of point
//          2            Y            Real
//          3            Z            Real
//ECO564    4            PTR         Pointer   Pointer to the DE of the Subfigure 
//								     Definition Entity specifying the display symbol 
//									 or zero. If zero, no display symbol specified.

#ifndef __Point_h__
#define __Point_h__

#include "IgesEntity.h"
struct VPoint;

class IgesPoint:public IgesEntity
{
public:
	IgesPoint ();
	IgesPoint(const VPoint &p);
	virtual void GetEntityIn (FileLines &mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	vreal m_X,m_Y,m_Z;
	int m_PTR;

};

#endif
