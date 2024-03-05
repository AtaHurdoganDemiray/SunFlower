// Vertex.h

#ifndef __Vertex_h__
#define __Vertex_h__

#include "IgesEntity.h"
struct VPoint;

class Vertex : public IgesEntity
{
public:
	Vertex();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	//virtual bool IsPrimitiveEntity()const;
	//virtual double GetSortNo ()const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;
	
	int m_N;
	std::vector<vreal> m_v;

	VPoint GetVertex (int vi)const;// vi is the list index / 1 based , starting from one

};

#endif