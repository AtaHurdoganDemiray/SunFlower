// OffsetSurface.h
/*

Entity type 140

Parameter Data

Index__      Name____     Type___   Description___
 1           NX           Real      The x-coordinate of the offset indicator N (U m; V m)
 2           NY           Real      The y-coordinate of the offset indicator N (U m; V m)
 3           NZ           Real      The z-coordinate of the offset indicator N (U m; V m)
 4           D            Real      The distance by which the surface is normally offset on the side
                                    of the offset indicator if d > 0 and on the opposite side if d < 0
 5           DE           Pointer   Pointer to the DE of the surface entity to be offset
*/

#ifndef __OffsetSurface_h__
#define __OffsetSurface_h__

#include "IgesEntity.h"
//class FileLines;

class OffsetSurface : public IgesEntity
{
public:
	OffsetSurface();
	virtual void GetEntityIn( FileLines &mylines , int poffs);	
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;	
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	vreal m_NX , m_NY , m_NZ;
	vreal m_D;
	long m_DE;

};

#endif //__OffsetSurface_h__
