// Edge.h

#ifndef __Edge_h__
#define __Edge_h__

#include "IgesEntity.h"

class Edge : public IgesEntity
{
public:
	Edge();
	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;
	int m_N;	// number of edges
	std::vector<int> m_CURV;	// m_N items in the vector , for each edge pointer to the DE of model space curve
	std::vector<int> m_SVP;		// m_N items in the vector , for each edge pointer to the DE of vertex list entity	where start vertex is in
	std::vector<int> m_SV;		// m_N items in the vector , for each edge the index for the start vertex in the correspanding vertex list
	std::vector<int> m_TVP;		// m_N items in the vector , for each edge pointer to the DE of the vertext list entity where terminate v is in
	std::vector<int> m_TV;		// m_N items in the vector , for each edge the index for the terminate vertex in the correspanding vertex list
	bool IsChildEntity(IgesEntity *e)const;
};

#endif