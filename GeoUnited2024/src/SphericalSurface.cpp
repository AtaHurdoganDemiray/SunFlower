#include "stdafx.h"
#include "SphericalSurface.h"
#include "IgesEntitySet.h"
#include "IgesEntityVisitor.h"

SphericalSurface::SphericalSurface()
{
	entitytype = 196;
}


SphericalSurface::~SphericalSurface()
{
}
void SphericalSurface::GetEntityIn(FileLines & mylines, int poffs)
{
	DoubleVector myvect;
	GetAllParam(myvect, mylines);
	if (myvect.size() <= 3)
		throw VException("SphereSurface::GetEntityIn exception , parameter count less than 4", 1);

	m_DELOC = static_cast<int>(myvect[1])+poffs;
	m_RADIUS = static_cast<vreal>(myvect[2]);
	m_DEAXIS = static_cast<int>(myvect[3])+poffs;
	
	if (myvect.size() == 5)
	{
		m_DEREFD = static_cast<int>(myvect[4])+poffs;
		DE.m_formNo = 1;
	}
	else
	{
		DE.m_formNo = 0;
		m_DEREFD = -1;
	}
}
void SphericalSurface::WriteEntityOut(std::vector <VStr> &strvec)const
{
	WriteParam(entitytype, strvec);
	WriteParam(m_DELOC, strvec);
	WriteParam(m_RADIUS, strvec);
	WriteParam(m_DEAXIS, strvec);
	if (m_DEREFD != -1)
	{
		WriteParam(m_DEREFD, strvec);
	}
}
void SphericalSurface::GetChildren(std::vector<int> &childVec)const
{
	childVec.push_back(m_DELOC);
	childVec.push_back(m_DEAXIS);
	if (DE.m_formNo == 1)
	{
		childVec.push_back(m_DEREFD);
	}
}
void SphericalSurface::AcceptVisitor(IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual(*this);
}
IgesEntity * SphericalSurface::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer(m_subsetIndex);
	SphericalSurface *mycopy = new SphericalSurface(*this);
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
