// SurfaceOfRevolution.h
// E120
// 1 L Pointer pointer to the DE of Line entity
// 2 C Pointer pointer to the DE of generatrix curve 
// 3 SA Real start angle in radians
// 4 TA Real terminate angle in radians

#ifndef __SurfaceOfRevolution_h__
#define __SurfaceOfRevolution_h__

#include "IgesEntity.h"

class SurfaceOfRevolution:public IgesEntity
{
public:
	SurfaceOfRevolution ();
	virtual void GetEntityIn ( FileLines & mylines , int poffs);		
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;
	
	long m_L;
	long m_C;
	vreal SA,TA;
};

#endif