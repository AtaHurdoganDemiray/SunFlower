// Vertex.cpp

#include "stdafx.h"
#include "Vertex.h"
#include "IgesEntityVisitor.h"

#include "VPoint.h"
#include "IgesEntitySet.h"

Vertex::Vertex()
{
	entitytype = 502;
}

void Vertex::GetEntityIn( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 4)
		throw VException (( IDS_VerGetExcNotEno),1);
	m_N = static_cast<int>(myvect[1]);
	for (int i=0;i<m_N;++i)
	{
		int si = i*3 + 2;
		double myX = static_cast<vreal>(myvect[si]);
		m_v.push_back(myX);
		double myY = static_cast<vreal>(myvect[si+1]);
		m_v.push_back(myY);
		double myZ = static_cast<vreal>(myvect[si+2]);
		m_v.push_back(myZ);
	}
}

void Vertex::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam(entitytype,strvec);
	WriteParam (m_N,strvec);

	size_t mysize = m_v.size();
	for (size_t i=0;i<mysize;++i)
	{
		WriteParam (m_v[i],strvec);
	}

}

void Vertex::GetChildren (std::vector<int> &childVec)const
{

}

void Vertex::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual(*this);
}

//bool Vertex::IsPrimitiveEntity()const
//{
//	return true;
//}

//double Vertex::GetSortNo ()const
//{
//	return 1;
//}

IgesEntity * Vertex::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex !=0)
		return newSet.GetEntityPointer(m_subsetIndex);
	Vertex *mycopy = new Vertex (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}

VPoint Vertex::GetVertex(int vi) const
{
	int starti = (vi-1) * 3;
	if (starti + 2 >= (int)m_v.size())
		throw VException (( IDS_VerGetExcIndOut),1);
	VPoint v (m_v[starti],m_v[starti+1] , m_v[starti+2]);
	return v;
}
