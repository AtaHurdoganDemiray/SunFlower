// SingularSubFigureInstance.h

#ifndef __SingularSubFigureInstance_h__
#define __SingularSubFigureInstance_h__

#include "IgesEntity.h"

class SingularSubFigureInstance : public IgesEntity
{
public:
	SingularSubFigureInstance ();
	virtual void GetEntityIn ( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;	
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	int m_subFigure;
	vreal m_X;
	vreal m_Y;
	vreal m_Z;
	vreal m_S;
};

#endif