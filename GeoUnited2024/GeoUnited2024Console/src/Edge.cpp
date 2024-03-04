// Edge.cpp

#include "stdafx.h"
#include "Edge.h"

#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"

Edge::Edge()
{
	entitytype = 504;
}

void Edge::GetEntityIn( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	if (myvect.size() <= 6)
		throw VException (( IDS_EdgGetExcNotEno),1);
	m_N = static_cast<int>(myvect[1]);
	size_t totalreadcount = m_N * 5 + 1;
	if (myvect.size() <= totalreadcount)
		throw VException (( IDS_EdgGetExcNotEno1),1);
	m_CURV.resize(m_N);
	m_SVP.resize(m_N);
	m_SV.resize(m_N);
	m_TVP.resize(m_N);
	m_TV.resize(m_N);
	int finali = 0;
	for (int i=0;i<m_N;++i)
	{
		int si = (i+1)*5 - 3;
		m_CURV[i] = static_cast<int> (myvect[si]) + poffs;
		m_SVP[i] = static_cast<int> (myvect[si+1]) + poffs;
		m_SV[i] = static_cast<int> (myvect[si+2]);
		m_TVP[i] = static_cast<int> (myvect[si+3]) + poffs;
		m_TV[i] = static_cast<int> (myvect[si+4]);
		finali = si + 5;
	}
	if (finali > 0)
	{
		ReadModelId(finali, myvect);
	}
}
void Edge::WriteEntityOut (std::vector <VStr> &strvec)const
{
	WriteParam(entitytype , strvec);
	WriteParam (m_N , strvec);
	for (int i=0;i<m_N;++i)
	{
		WriteParam (m_CURV[i],strvec);
		WriteParam (m_SVP[i],strvec);
		WriteParam (m_SV[i],strvec);
		WriteParam (m_TVP[i],strvec);
		WriteParam (m_TV[i],strvec);
	}
	WriteModelId(strvec);
}
void Edge::GetChildren (std::vector<int> &childVec)const
{
	for (int i=0;i<m_N;++i)
	{
		childVec.push_back(m_CURV[i]);
		childVec.push_back(m_SVP[i]);
		childVec.push_back(m_TVP[i]);
	}
	GetDirectoryChild (childVec);
}
void Edge::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual(*this);
}
IgesEntity * Edge::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer(m_subsetIndex);
	Edge *mycopy = new Edge (*this);
	for (int i=0;i<m_N;++i)
	{
		IgesEntity *crv = mainSet.GetEntityPointer(m_CURV.at(i));
		if (crv == 0)
			throw VException (( IDS_EdgMakExcNulCur),1);
		else
		{
			IgesEntity *copycrv = crv->MakeCopyAndAddToSet(mainSet , newSet);
			mycopy->m_CURV[i] = copycrv->directorylineno; 
		}
		IgesEntity *startvertex = mainSet.GetEntityPointer(m_SVP.at(i));
		if (startvertex == 0)
			throw VException (( IDS_EdgMakExcNulSta),1);
		else
		{
			IgesEntity *copystartvertex = startvertex->MakeCopyAndAddToSet(mainSet , newSet);
			mycopy->m_SVP[i] = copystartvertex->directorylineno;
		}
		IgesEntity *terminatevertex = mainSet.GetEntityPointer(m_TVP.at(i));
		if (terminatevertex == 0)
			throw VException (( IDS_EdgMakExcNulTer),1);
		else
		{
			IgesEntity *copyterminatevertex = terminatevertex->MakeCopyAndAddToSet(mainSet , newSet);
			mycopy->m_TVP[i] = copyterminatevertex->directorylineno;
		}
	}

	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}

bool Edge::IsChildEntity(IgesEntity* e)const
{
	for (int i = 0; i < m_CURV.size(); ++i)
	{
		int curDENo = m_CURV.at(i);
		if (curDENo == e->directorylineno)
			return true;
	}
	return false;
}

