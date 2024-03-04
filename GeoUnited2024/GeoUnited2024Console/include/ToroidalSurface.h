//ToroidalSurface.h
//TOROIDAL SURFACE ENTITY (TYPE 198)

//Parameter Data
//Index__  Name__   Type__   Description__
//1        DELOC    Pointer  Pointer to the DE of the center point(LOCATION)
//2        DEAXIS   Pointer  Pointer to the DE of the axis direction’(AXIS)
//3        MAJRAD   Real     Value of major radius(> 0.0)
//4        MINRAD   Real     Value of minor radius(> 0.0 and < MAJRAD)
//5        DEREFD   Pointer  Pointer to the DE of the reference direction(REFDIR)
#ifndef ToroidalSurface_h
#define ToroidalSurface_h

#include "IgesEntity.h"

class ToroidalSurface: public IgesEntity
{
public:
	ToroidalSurface();
	~ToroidalSurface();
	virtual void GetEntityIn(FileLines & mylines, int poffs);
	virtual void WriteEntityOut(std::vector<VStr> &strvec)const;
	virtual void GetChildren(std::vector<int> &childVec)const;
	virtual void AcceptVisitor(IgesEntityVisitor &visitor)const;
	virtual IgesEntity * MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const;

	long  m_DELOC;
	long  m_DEAXIS;
	vreal m_MAJRAD;
	vreal m_MINRAD;
	long m_DEREFD;
};

#endif