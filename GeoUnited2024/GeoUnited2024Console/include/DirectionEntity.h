// DirectionEntity.h

#ifndef __DirectionEntity_h__
#define __DirectionEntity_h__

//        Index__      Name____     Type___   Description___
//          1            X            Real      Coordinates of direction X^2 + Y^2 + Z^2 > 0
//          2            Y            Real
//          3            Z            Real
// Entity Type 123

#include "IgesEntity.h"

class DirectionEntity : public IgesEntity
{
public:
	DirectionEntity();
	virtual void GetEntityIn(FileLines &mylines, int poffs);
	virtual void WriteEntityOut(std::vector<VStr> &strvec)const;
	virtual void GetChildren(std::vector<int> &childVec)const;
	virtual void AcceptVisitor(IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const;

	vreal m_X;
	vreal m_Y;
	vreal m_Z;
};

#endif