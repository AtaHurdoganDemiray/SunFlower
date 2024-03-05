// Loop.h

#ifndef __Loop_h__
#define __Loop_h__

#include "IgesEntity.h"

class dCurve;
class IgesTrimLoop2d;

class Loop : public IgesEntity
{
public:
	Loop();

	virtual void GetEntityIn( FileLines & mylines , int poffs);
	virtual void WriteEntityOut (std::vector <VStr> &strvec)const;
	virtual void GetChildren (std::vector<int> &childVec)const;
	virtual void AcceptVisitor (IgesEntityVisitor &visitor)const;
	virtual IgesEntity * MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const;


	int m_N;					// Number of edge tuples
	std::vector<int> m_TYPE;	// Type of the first edge : 0=Edge , 1=Vertex 
	std::vector<int> m_EDGE;	// Pointer to the DE of the Vertex List or Edge list entity 
	std::vector<int> m_NDX;		// List index into Vertex List or Edge list entity
	std::vector<bool> m_OF;		// Orientation flag of edge with respect to the direction of the model space curves : true=agrees
	std::vector<int>  m_K;		// Number of underlying parameter space curves or 0
    std::vector<std::vector<bool> > m_ISOP;	// Isoparametric flag of correspanding parameter space curve : true=parametric curve is isoparametric
    std::vector<std::vector<int> > m_CURV;	// Pointer to the DE of correspanding parameter space curve in the edge
	void AddLoopCurves(IgesEntitySet &entset, std::map<int, std::map<int,dCurve*>> &alledges, std::vector<dCurve*> &loop);
	void AddLoopCurves2d(IgesEntitySet &entset, std::vector<IgesTrimLoop2d*> &loop2d);
};

#endif
