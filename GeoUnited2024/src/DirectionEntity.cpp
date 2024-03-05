// DirectionEntity.cpp

#include "stdafx.h"
#include "DirectionEntity.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"

DirectionEntity::DirectionEntity()
{
	entitytype = 116;
}

void DirectionEntity::GetEntityIn(FileLines & mylines, int poffs)
{
	DoubleVector myvect;
	GetAllParam(myvect, mylines);
	if (myvect.size() <= 3)
		throw VException((IDS_PoiGetExcNotEno), 1);
	m_X = static_cast<vreal>(myvect[1]);
	m_Y = static_cast<vreal>(myvect[2]);
	m_Z = static_cast<vreal>(myvect[3]);
#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut(*this, myvect);
#endif
}

void DirectionEntity::WriteEntityOut(std::vector<VStr> &strvec)const
{
	WriteParam(entitytype, strvec);
	WriteParam(m_X, strvec);
	WriteParam(m_Y, strvec);
	WriteParam(m_Z, strvec);
}

void DirectionEntity::GetChildren(std::vector<int> &childVec)const
{
	GetDirectoryChild(childVec);
}

void DirectionEntity::AcceptVisitor(IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual(*this);
}

IgesEntity * DirectionEntity::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer(m_subsetIndex);
	DirectionEntity *mycopy = new DirectionEntity(*this);
	CopyDE(*mycopy, mainSet, newSet);
	newSet.InsertEntity(mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}