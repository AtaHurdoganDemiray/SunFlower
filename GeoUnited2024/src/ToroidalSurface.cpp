#include "stdafx.h"
#include "ToroidalSurface.h"
#include "IgesEntitySet.h"
#include "IgesEntityVisitor.h"

ToroidalSurface::ToroidalSurface()
{
	entitytype = 198;
}


ToroidalSurface::~ToroidalSurface()
{
}

void ToroidalSurface::GetEntityIn(FileLines & mylines, int poffs)
{
	DoubleVector myvect;
	GetAllParam(myvect, mylines);
	if (myvect.size() <= 4)
		throw VException("ToroidalSurface::GetEntityIn exception , parameter count less than 5", 1);

	m_DELOC = static_cast<int>(myvect[1])+poffs;
	m_DEAXIS = static_cast<int>(myvect[2])+poffs;
	m_MAJRAD = static_cast<vreal>(myvect[3]);
	m_MINRAD = static_cast<vreal>(myvect[4]);

	if (myvect.size() == 6)
	{
		m_DEREFD = static_cast<int>(myvect[5])+poffs;
		DE.m_formNo = 1;
	}
	else
	{
		DE.m_formNo = 0;
		m_DEREFD = -1;
	}
}
void ToroidalSurface::WriteEntityOut(std::vector <VStr> &strvec)const
{
	WriteParam(entitytype, strvec);
	WriteParam(m_DELOC, strvec);
	WriteParam(m_DEAXIS, strvec);
	WriteParam(m_MAJRAD, strvec);
	WriteParam(m_MINRAD, strvec);
	if (m_DEREFD != -1)
	{
		WriteParam(m_DEREFD, strvec);
	}
}
void ToroidalSurface::GetChildren(std::vector<int> &childVec)const
{
	childVec.push_back(m_DELOC);
	childVec.push_back(m_DEAXIS);
	if (DE.m_formNo == 1)
	{
		childVec.push_back(m_DEREFD);
	}
}
void ToroidalSurface::AcceptVisitor(IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual(*this);
}
IgesEntity * ToroidalSurface::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer(m_subsetIndex);
	ToroidalSurface *mycopy = new ToroidalSurface(*this);
	IgesEntity *loc = mainSet.GetEntityPointer(m_DELOC);
	if (loc != 0)
	{
		IgesEntity *copyLoc = loc->MakeCopyAndAddToSet(mainSet, newSet);
		if (copyLoc != 0)
		{
			mycopy->m_DELOC = copyLoc->directorylineno;
		}
		else
		{
			mycopy->m_DELOC = -1;
		}
	}
	else
		mycopy->m_DELOC = -1;

	IgesEntity *nrm = mainSet.GetEntityPointer(m_DEAXIS);
	if (nrm != 0)
	{
		IgesEntity *copyNor = nrm->MakeCopyAndAddToSet(mainSet, newSet);
		if (copyNor != 0)
			mycopy->m_DEAXIS = copyNor->directorylineno;
		else
			mycopy->m_DEAXIS = -1;
	}
	else
		mycopy->m_DEAXIS = -1;
	if (DE.m_formNo == 1)
	{
		IgesEntity *refd = mainSet.GetEntityPointer(m_DEREFD);
		if (refd != 0)
		{
			IgesEntity *copyRefd = refd->MakeCopyAndAddToSet(mainSet, newSet);
			if (copyRefd != 0)
			{
				mycopy->m_DEREFD = copyRefd->directorylineno;
			}
			else
				mycopy->m_DEREFD = -1;
		}
		else
			mycopy->m_DEREFD = -1;
	}
	else
	{
		mycopy->m_DEREFD = -1;
	}
	CopyDE(*mycopy, mainSet, newSet);
	newSet.InsertEntity(mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}