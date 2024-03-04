// BoundedSurface.h
// Entity Type 143
/*
Parameter Data:
Index__      Name____     Type___   Description___
1            TYPE        Integer   The type of bounded surface representation:
                                    0 = The boundary entities may only reference model space
                                    curves.   The  associated  surface  representation  (located  by
                                    SPTR) may be parametric.
                                    1 = The boundary entities must reference both model space
                                    curves and the associated parameter space curve collections.
                                    The associated surface (located by SPTR) must be a parametric
                                    representation.
2            SPTR        Pointer   Pointer  to  the  DE  of  the  untrimmed  surface  entity  to  be
                                   bounded. If parameter space trimming curves are being trans-
                                   ferred (TYPE = 1) the surface representations must be paramet-
                                   ric.
3            N            Integer   The number of boundary entities
4            BDPT1       Pointer   Pointer to the DE of the first Boundary Entity (Type 141)
..           .            .
.            ..           ..
3+N         BDPTN       Pointer   Pointer to the DE of the last Boundary Entity (Type 141)

*/

#ifndef __BoundedSurface_h__
#define __BoundedSurface_h__
#include "IgesEntity.h"

class BoundedSurface:public IgesEntity
{
public:
	BoundedSurface();
	~BoundedSurface();
	virtual void GetEntityIn ( FileLines &mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	int m_TYPE;
	int m_SPTR;
	int m_N;
	std::vector<int> m_BDPT;

};

#endif //__BoundedSurface_h__