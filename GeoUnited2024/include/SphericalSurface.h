//SphericalSurface.h

//SPHERICAL SURFACE ENTITY (TYPE 196)
 
//Parameter Data
//Index__ Name__  Type__   Description__
//1       DELOC   Pointer  Pointer - to the DE of the center point(LOCATION)
//2       RADIUS  Real     Value of radius(> 0.0)
//3       DEAXIS  Pointer  Pointer to the DE of the axis direction(AXIS)
//4       DEREFD  Pointer  Pointer to the DE of the reference direction(REFDIR)

#ifndef SphericalSurface_h
#define SphericalSurface_h

#include "IgesEntity.h"

class SphericalSurface: public IgesEntity
{
public:
	SphericalSurface();
	~SphericalSurface();
	virtual void GetEntityIn(FileLines & mylines, int poffs);
	virtual void WriteEntityOut(std::vector<VStr> &strvec)const;
	virtual void GetChildren(std::vector<int> &childVec)const;
	virtual void AcceptVisitor(IgesEntityVisitor &visitor)const;
	virtual IgesEntity * MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const;

	long  m_DELOC;
	vreal m_RADIUS;
	long  m_DEAXIS;
	long  m_DEREFD;
};

#endif