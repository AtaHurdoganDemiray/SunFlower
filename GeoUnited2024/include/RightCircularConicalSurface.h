
//RightCircularConicalSurface.h
//RIGHT CIRCULAR CONICAL SURFACE ENTITY (TYPE 194)
//Parameterized Right Circular Conical Surface Entity(Type 194, Form 0)
//Parameter Data
//Index__ Name__  Type__  Description
//1       DELOC   Pointer  Pointer to the DE of the point on axis(LOCATION)
//2       DEAXIS  Pointer  Pointer to the DE of the axis direction(AXIS)
//3       RADIUS  Real     Value of radius at axis point(>= 0.0)
//4       SANGLE  Real     Value of semi - angle in degrees(> 0.0 and < 90.0) 
//Parameterized Right Circular Conical Surface Entity(Type 194, Form 1)
//Parameter Data
//Index__ Name__  Type__   Description
//1		  DELOC   Pointer  Pointer to the DE of the point on axis(LOCATION)
//2       DEAXIS  Pointer  Pointer to the DE of the axis direction(AXIS)
//3       RADIUS  Real     Value of radius at axis point(>= 0.0)
//4       SANGLE  Real     Value of semi - angle in degrees(> 0.0 and < 90.0)
//5       DEREFD  Pointer  Pointer to the DE of the reference direction(REFDIR)

#ifndef RightCircularConicalSurface_h
#define RightCircularConicalSurface_h
#include "IgesEntity.h"

class RightCircularConicalSurface:public IgesEntity
{
public:
	RightCircularConicalSurface();
	~RightCircularConicalSurface();

	virtual void GetEntityIn(FileLines & mylines, int poffs);
	virtual void WriteEntityOut(std::vector<VStr> &strvec)const;
	virtual void GetChildren(std::vector<int> &childVec)const;
	virtual void AcceptVisitor(IgesEntityVisitor &visitor)const;
	virtual IgesEntity * MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const;

	long  m_DELOC;
	long  m_DEAXIS;
	vreal m_RADIUS;
	vreal m_SANGLE;
	long  m_DEREFD;
};

#endif