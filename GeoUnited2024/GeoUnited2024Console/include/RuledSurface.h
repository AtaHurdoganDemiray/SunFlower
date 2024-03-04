// RuledSurface.h

// Entity No : 118
//Index__      Name____     Type___   Description___
//1            DE1         Pointer   Pointer to the DE of the first curve entity
//2            DE2         Pointer   Pointer to the DE of the second curve entity
//3            DIRFLG      Integer   Direction flag:
//                                      0=Join first to first, last to last
//                                      1=Join first to last, last to first
//4            DEVFLG      Integer   Developable surface flag:
//                                      1=Developable
//                                      0=Possibly not

#ifndef __RuledSurface_h__
#define __RuledSurface_h__

#include "IgesEntity.h"

class RuledSurface:public IgesEntity
{
public:
	enum Type {EQUAL_PAR_LENGTH , EQUAL_ARC_LENGTH};
	RuledSurface(Type atype);
	virtual ~RuledSurface();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;	
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;
	
	Type m_type;
	int m_DE1;
	int m_DE2;
	int m_DIRFLG;
	int m_DEVFLG;
};

#endif