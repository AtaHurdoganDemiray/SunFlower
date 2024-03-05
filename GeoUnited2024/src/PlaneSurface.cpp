
#include "stdafx.h"
#include "PlaneSurface.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"

PlaneSurface::PlaneSurface()
{
	m_DELOC = -1;
	m_DENRML = -1;
	m_DEREFD = -1;
	entitytype = 190;
}
PlaneSurface::~PlaneSurface()
{

}
void PlaneSurface::GetEntityIn(FileLines & mylines, int poffs)
{
	DoubleVector myvect;
	GetAllParam(myvect, mylines);
	if (myvect.size() <= 2)
		throw VException("PlaneSurface::GetEntityIn exception , parameter count less than 2", 1);

	m_DELOC = static_cast<int>(myvect[1])+poffs;
	m_DENRML = static_cast<int>(myvect[2])+poffs;
	if (myvect.size() == 4)
	{
		m_DEREFD = static_cast<int>(myvect[3])+poffs;
		DE.m_formNo = 1;
	}
	else
	{
		DE.m_formNo = 0;
		m_DEREFD = -1;
	}
}
void PlaneSurface::WriteEntityOut(std::vector <VStr> &strvec)const
{
	WriteParam(entitytype, strvec);
	WriteParam(m_DELOC, strvec);
	WriteParam(m_DENRML, strvec);
	if (m_DEREFD != -1)
	{
		WriteParam(m_DEREFD, strvec);
	}
}
void PlaneSurface::GetChildren(std::vector<int> &childVec)const
{
	childVec.push_back(m_DELOC);
	childVec.push_back(m_DENRML);
	if (DE.m_formNo == 1)
	{
		childVec.push_back(m_DEREFD);
	}
}
void PlaneSurface::AcceptVisitor(IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual(*this);
}
IgesEntity * PlaneSurface::MakeCopyAndAddToSet(const IgesEntitySet &mainSet, IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer(m_subsetIndex);
	PlaneSurface *mycopy = new PlaneSurface(*this);
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

	IgesEntity *nrm = mainSet.GetEntityPointer(m_DENRML);
	if (nrm != 0)
	{
		IgesEntity *copyNor = nrm->MakeCopyAndAddToSet(mainSet, newSet);
		if (copyNor != 0)
			mycopy->m_DENRML = copyNor->directorylineno;
		else
			mycopy->m_DENRML = -1;
	}
	else
		mycopy->m_DENRML = -1;
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