// Loop.cpp

#include "stdafx.h"
#include "Loop.h"
#include "IgesTrimLoop2d.h"
#include "IgesEntitySet.h"
#include "IgesTrimLoop2d.h"

Loop::Loop()
{
	entitytype = 508;
}

void Loop::GetEntityIn( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	//if (myvect.size() <= 6)
	//	throw VException (("Edge::GetEntityIn exception , not enough parameters!"),1);
	m_N = static_cast<int> (myvect[1]);
	int counter = 2;
	for (int i=0;i<m_N;++i)
	{
		int curtype = static_cast<int> (myvect[counter]);
		++counter;
		m_TYPE.push_back(curtype);
		int curedge = static_cast<int> (myvect[counter]+poffs);
		++counter;
		m_EDGE.push_back(curedge);
		int curndx = static_cast<int> (myvect[counter]);
		++counter;
		m_NDX.push_back(curndx);
		int curof = static_cast<int> (myvect[counter]);
		++counter;
		bool oflg = curof == 1 ? true : false;
		m_OF.push_back(oflg);
		int curk = static_cast<int> (myvect[counter]);
		++counter;
		m_K.push_back(curk);
		m_ISOP.push_back(std::vector<bool>());
		m_CURV.push_back(std::vector<int>());
		for (int j=0;j<curk;++j)
		{
			int curisop = static_cast<int> (myvect[counter]);
			++counter;
			m_ISOP[i].push_back(curisop == 1);
			int curcrv = static_cast<int> (myvect[counter]+poffs);
			++counter;
			m_CURV[i].push_back(curcrv);
		}
	}

}
void Loop::WriteEntityOut (std::vector <VStr> &strvec)const
{

}
void Loop::GetChildren (std::vector<int> &childVec)const
{

}
void Loop::AcceptVisitor (IgesEntityVisitor &visitor)const 
{

}
IgesEntity * Loop::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const 
{
	return 0;
}
void Loop::AddLoopCurves(IgesEntitySet &entset, std::map<int, std::map<int,dCurve*>> &alledges, std::vector<dCurve*> &loop)
{
//	std::vector<int> m_TYPE;	// Type of the first edge : 0=Edge , 1=Vertex 
//	std::vector<int> m_EDGE;	// Pointer to the DE of the Vertex List or Edge list entity 
//	std::vector<int> m_NDX;		// List index into Vertex List or Edge list entity
//	std::vector<bool> m_OF;		// Orientation flag of edge with respect to the direction of the model space curves : true=agrees
	int lsize = (int)m_EDGE.size();
	for (int li = 0; li < lsize; ++li)
	{
		int curtype = m_TYPE.at(li);
		if (curtype == 1) // this is vertex
			continue;
		int curEdgeDE = m_EDGE.at(li);
		std::map<int, std::map<int,dCurve*>>::iterator fi = alledges.find(curEdgeDE);
		if (fi == alledges.end())
			throw VException("Loop::AddLoopCurves exception , edge not added to project",1);
		int edgei = m_NDX.at(li);
		std::map<int,dCurve*>::iterator fii = fi->second.find(edgei);
		if (fii == fi->second.end())
			throw VException("Loop::AddLoopCurves exception , edge index edgei not found", 1);
		loop.push_back(fii->second);
	}
}
void Loop::AddLoopCurves2d(IgesEntitySet &entset, std::vector<IgesTrimLoop2d*> &loop2d)
{
	IgesTrimLoop2d *curtr = new IgesTrimLoop2d();
	
	int lsize = (int)m_CURV.size();
	for (int li = 0; li < lsize; ++li)
	{
		std::vector<int> &curList = m_CURV.at(li);
		int cursize = (int)curList.size();
		for (int lii = 0; lii < cursize; ++lii)
		{
			int curDe = curList.at(lii);
			IgesEntity *curent = entset.GetEntityPointer(curDe);
			if (curent == 0)
				throw VException("Loop::AddLoopCurve2d exception iges entity is null!", 1);
			else
			{
				curtr->loopcurves.push_back(curent);
				curtr->m_isReversed.push_back(false);
			}
		}
	}
	loop2d.push_back(curtr);
}
