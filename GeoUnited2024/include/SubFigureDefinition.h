// SubFigureDefinition.h

#ifndef __SubFigureDefinition_h__
#define __SubFigureDefinition_h__

#include "IgesEntity.h"

class SubFigureDefinition : public IgesEntity
{
public:
	SubFigureDefinition ();
	virtual void GetEntityIn ( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector<VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;	
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;

	int m_depth;
	VStr m_name;
	int m_numberOfEntities;
	std::vector<int> m_entityVec;
};

#endif