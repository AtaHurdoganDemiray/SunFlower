
//RightCircularCylindricalSurface.h
//RIGHT CIRCULAR CYLINDRICAL SURFACE ENTITY (TYPE 192)

// Parameterized Right Circular Cylindrical Surface Entity(Type 192, Form 0)
//Parameter Data
//Index__ Name__  Type__   Description__
//1       DELOC   Pointer  Pointer to the DE of the point on axis(LOCATION)
//2       DEAXIS  Pointer  Pointer to the DE of the axis direction(AXIS)
//3       RADIUS  Real     Value of radius(> 0.0)

//Parameterized Right Circular Cylindrical Surface Entity(Type 192, Form 1)
//Parameter Data
//Index__ Name__  Type__    Description__
//1       DELOC   Pointer   Pointer to the DE of the point on axis(LOCATION)
//2       DEAXIS  Pointer   Pointer to the DE of the axis direction(AXIS)
//3       RADIUS  Real      Value of radius(> 0.0)
//4       DEREFD  Pointer   Pointer to the DE of the reference direction(REFDIR)


#ifndef RightCircularCylindricalSurface_h
#define RightCircularCylindricalSurface_h
#include "IgesEntity.h"
class RightCircularCylindricalSurface: public IgesEntity
{
public:
	RightCircularCylindricalSurface();
	~RightCircularCylindricalSurface();
	virtual void GetEntityIn(FileLines & mylines, int poffs);
	virtual void WriteEntityOut(std::vector<VStr> &strvec)const;
	virtual void GetChildren(std::vector<int> &childVec)const;
	virtual void AcceptVisitor(IgesEntityVisitor &visitor)const;
	virtual IgesEntity * MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const;

	long  m_DELOC;
	long  m_DEAXIS;
	vreal m_RADIUS;
	long  m_DEREFD;

};
#endif

