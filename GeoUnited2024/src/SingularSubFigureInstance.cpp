// SingularSubFigureInstance.cpp

#include "stdafx.h"
#include "SingularSubFigureInstance.h"
#include "TypeConverters.h"
#include "IgesEntitySet.h"
#include "IgesEntityVisitor.h"

SingularSubFigureInstance::SingularSubFigureInstance()
{
	m_subFigure = 0;
	m_X = 0;
	m_Y = 0;
	m_Z = 0;
	m_S = 1.0;
	entitytype = 408;
}

void SingularSubFigureInstance::GetEntityIn ( FileLines & mylines , int poffs)
{
	std::vector<vreal> myvect;
	GetAllParam ( myvect , mylines );
	m_subFigure = (int)myvect.at(1) + poffs;
	m_X = myvect.at(2);
	m_Y = myvect.at(3);
	m_Z = myvect.at(4);
	m_S = myvect.at(5);
}
void SingularSubFigureInstance::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_subFigure , strvec);
	WriteParam (m_X , strvec);
	WriteParam (m_Y , strvec);
	WriteParam (m_Z , strvec);
	WriteParam (m_S , strvec);
}
void SingularSubFigureInstance::GetChildren (std::vector<int> &childVec)const
{
	childVec.push_back (m_subFigure);
	GetDirectoryChild (childVec);
}
void SingularSubFigureInstance::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}
IgesEntity * SingularSubFigureInstance::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer(m_subsetIndex);
	SingularSubFigureInstance *mycopy = new SingularSubFigureInstance (*this);
	
	IgesEntity *someSubEnt = mainSet.GetEntityPointer(m_subFigure);
	if (someSubEnt == 0)
		throw VException (( IDS_EdgMakExcNulCur),1);
	else
	{
		IgesEntity *copyEnt = someSubEnt->MakeCopyAndAddToSet(mainSet , newSet);
		mycopy->m_subFigure = copyEnt->directorylineno; 
	}

	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
