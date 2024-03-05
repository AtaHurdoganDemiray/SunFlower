// FakeSLSet.cpp

#include "stdafx.h"
#include "FakeSLSet.h"
#include "IgesEntity.h"
#include "BSplineCurve.h"
#include "Line.h"
#include "Arc.h"
#include "CopiousData.h"
#include "ConicArc.h"
#include "TransformationMatrix.h"

FakeSLSet::FakeSLSet (int i)
{
	m_ent = 0;
	m_reversed = false;
}

int FakeSLSet::Size()const
{
	return 4;
}

int FakeSLSet::GetPointSize()const
{
	return 5;
}

const VPoint * FakeSLSet::Start()const
{
	if (m_reversed)
		return &m_end;
	else
		return &m_start;
}
	
const VPoint * FakeSLSet::End()const
{
	if (m_reversed)
		return &m_start;
	else
		return &m_end;
}
	
void FakeSLSet::AddBack (const FakeSLSet &toAdd)
{
	m_child.push_back (&toAdd);
}
	
void FakeSLSet::Initialise (IgesEntity *ent,const TransformationMatrix &tm)
{
	m_ent = ent;
	if (m_ent->entitytype == 126)
	{
		BSplineCurve *crv = (BSplineCurve *)(m_ent);
		m_start.x (crv->controlpoints[0]);
		m_start.y (crv->controlpoints[1]);
		m_start.z (crv->controlpoints[2]);
		int mysize = crv->controlpoints.size();
		m_end.x(crv->controlpoints[mysize-3]);
		m_end.y(crv->controlpoints[mysize-2]);
		m_end.z(crv->controlpoints[mysize-1]);
	}
	else if (m_ent->entitytype == 110)
	{
		Line *crv = (Line*)(m_ent);
		m_start.x(crv->X1);
		m_start.y(crv->Y1);
		m_start.z(crv->Z1);
		m_end.x(crv->X2);
		m_end.y(crv->Y2);
		m_end.z(crv->Z2);
	}
	else if (m_ent->entitytype == 100)
	{
		igesArc *crv = (igesArc*)(m_ent);
		m_start.x(crv->mX2);
		m_start.y(crv->mY2);
		m_start.z(crv->mZT);
		m_end.x(crv->mX3);
		m_end.y(crv->mY3);
		m_end.z(crv->mZT);
	}
	else if (m_ent->entitytype == 106)
	{
		CopiousData *cps = (CopiousData*)(m_ent);
		if (cps->m_IP == 1)
		{
			m_start.x(cps->tuples.front().at(0));
			m_start.y(cps->tuples.front().at(1));
			m_start.z(cps->m_zoffset2d);
			m_end.x(cps->tuples.back().at(0));
			m_end.y(cps->tuples.back().at(1));
			m_end.z(cps->m_zoffset2d);
		}
		else if (cps->m_IP == 2)
		{
			m_start.x(cps->tuples.front().at(0));
			m_start.y(cps->tuples.front().at(1));
			m_start.z(cps->tuples.front().at(2));
			m_end.x(cps->tuples.back().at(0));
			m_end.y(cps->tuples.back().at(1));
			m_end.z(cps->tuples.back().at(2));
		}
		else
			throw VException (( IDS_FakIniExc),1);
	}
	else if (m_ent->entitytype == 104)
	{
		ConicArc *ca = (ConicArc *)m_ent;
		m_start.x(ca->m_X1);
		m_start.y(ca->m_Y1);
		m_start.z(ca->m_ZT);
		m_end.x(ca->m_X2);
		m_end.y(ca->m_Y2);
		m_end.z(ca->m_ZT);
	}
	else
		throw VException (( IDS_FakIniExc),2);
	if (m_ent->DE.pXform != 0)
	{
		m_start = tm.Transform (m_start);
		m_end = tm.Transform (m_end);
	}
}

void FakeSLSet::Reverse()
{
	m_reversed = true;
}

bool FakeSLSet::IsReversed()const
{
	return m_reversed;
}

std::list<const FakeSLSet *> & FakeSLSet::GetChild()
{
	return m_child;
}

IgesEntity * FakeSLSet::GetEntity()const
{
	return m_ent;
}
