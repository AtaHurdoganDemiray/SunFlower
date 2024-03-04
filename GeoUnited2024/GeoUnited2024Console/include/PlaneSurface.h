
// PlaneSurface.h
//PLANE SURFACE ENTITY (TYPE 190)
//Parameter Data 
//Form 0___
//Index__ Name__   Type__   Description___ 
//1       DELOC    Pointer  Pointer to the DE of the point on the surface(LOCATION)
//2       DENRML   Pointer  Pointer to the DE of the surface normal direction(NORMAL)
// 
//Form 1__
//Index__ Name__   Type__   Description___ 
//1       DELOC    Pointer  Pointer to the DE of the point on the surface(LOCATION)
//2       DENRML   Pointer  Pointer to the DE of the surface normal direction(NORMAL)
//3       DEREFD   Pointer  Pointer to the DE of the reference direction(REFDIR)

//
#ifndef PlaneSurface_h
#define PlaneSurface_h

#include "IgesEntity.h"

class PlaneSurface : public IgesEntity
{
public:
	PlaneSurface();
	~PlaneSurface();
	virtual void GetEntityIn(FileLines & mylines, int poffs);
	virtual void WriteEntityOut(std::vector<VStr> &strvec)const;
	virtual void GetChildren(std::vector<int> &childVec)const;
	virtual void AcceptVisitor(IgesEntityVisitor &visitor)const; 
	virtual IgesEntity * MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const;

	long m_DELOC;
	long m_DENRML;
	long m_DEREFD;

};

#endif