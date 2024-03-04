// SubFigureDefinition.cpp

#include "stdafx.h"
#include "SubFigureDefinition.h"
#include "TypeConverters.h"
#include "IgesEntitySet.h"
#include "IgesEntityVisitor.h"

SubFigureDefinition::SubFigureDefinition()
{
	m_depth = 0;
	m_numberOfEntities = 0;
	m_name = "";
	entitytype = 308;
}

void SubFigureDefinition::GetEntityIn ( FileLines & mylines , int poffs)
{
	std::vector<VStr> myvect;
	GetAllParam ( myvect , mylines );
	m_depth = (int)TypeConverters::ConvertStringTovreal(myvect.at(1));
	m_name = myvect.at(2);
	m_numberOfEntities = (int)TypeConverters::ConvertStringTovreal (myvect.at(3));
	for (int i=0;i<m_numberOfEntities;++i)
	{
		m_entityVec.push_back ( ((int)TypeConverters::ConvertStringTovreal(myvect.at(4+i))) + poffs);
	}
}
void SubFigureDefinition::WriteEntityOut (std::vector<VStr> &strvec)const
{
	WriteParam (entitytype , strvec);
	WriteParam (m_depth , strvec);
	WriteParam (m_name , strvec);
	WriteParam (m_numberOfEntities , strvec);
	for (int i=0;i<m_numberOfEntities;++i)
	{
		WriteParam (m_entityVec.at(i),strvec);
	}
}
void SubFigureDefinition::GetChildren (std::vector<int> &childVec)const
{
	size_t mysize = m_entityVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		childVec.push_back (m_entityVec.at(i));
	}
	GetDirectoryChild (childVec);
}
void SubFigureDefinition::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}
IgesEntity * SubFigureDefinition::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer(m_subsetIndex);
	SubFigureDefinition *mycopy = new SubFigureDefinition (*this);
	for (int i=0;i<m_numberOfEntities;++i)
	{
		IgesEntity *someSubEnt = mainSet.GetEntityPointer(m_entityVec.at(i));
		if (someSubEnt == 0)
			throw VException (( IDS_EdgMakExcNulCur),1);
		else
		{
			IgesEntity *copyEnt = someSubEnt->MakeCopyAndAddToSet(mainSet , newSet);
			mycopy->m_entityVec.at(i) = copyEnt->directorylineno; 
		}
	}
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity (mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}
