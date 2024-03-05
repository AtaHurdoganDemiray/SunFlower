// TabulatedCylinder.h
// E122
// 1 DE Pointer to the DE of directrix curve
// 2 LX Real	coord of terminate point of generatrix.
// 3 LY Real	( start point of generatrix is identical
// 4 LZ Real		with start point of directrix. )

#ifndef __TabulatedCylinder_h__
#define __TabulatedCylinder_h__

#include "IgesEntity.h"

class TabulatedCylinder:public IgesEntity
{
public:
	TabulatedCylinder();
	virtual void GetEntityIn ( FileLines &mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;	
	//virtual bool IsPrimitiveEntity()const;	
	//virtual double GetSortNo()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	long m_DE;
	vreal LX,LY,LZ;
};

#endif
