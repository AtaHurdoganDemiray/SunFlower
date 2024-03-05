// CopiousDataFactory.cpp

#include "stdafx.h"
#include "CopiousDataFactory.h"
#include "SequentialLineSet.h"
#include "CopiousData.h"
#include "SequentialLineSet2d.h"

CopiousData * CopiousDataFactory::Make(const SequentialLineSet &sl)
{
	CopiousData * newcop = new CopiousData;
	newcop->m_IP = 2; // x y z 
	newcop->DE.m_formNo = 12;
	int pointcount = sl.GetPointSize();
	newcop->m_N = pointcount;
	newcop->tuples.resize(pointcount);
	for (int i=0;i<pointcount;i++)
	{
		const VPoint *currp = sl.GetPoint(i);
		newcop->tuples[i].resize(3);
		newcop->tuples[i][0] = currp->x();
		newcop->tuples[i][1] = currp->y();
		newcop->tuples[i][2] = currp->z();
	}
	return newcop;
}

CopiousData * CopiousDataFactory::Make(const SequentialLineSet2d &sl)
{
	CopiousData * newcop = new CopiousData;
	newcop->m_IP = 1; // x y 
	newcop->DE.m_formNo = 11;
	newcop->m_zoffset2d = 0.0;
	int pointcount = sl.GetPointSize();
	newcop->m_N = pointcount;
	newcop->tuples.resize(pointcount);
	for (int i=0;i<pointcount;i++)
	{
		const VPoint2d *currp = sl.GetPoint(i);
		newcop->tuples[i].resize(2);
		newcop->tuples[i][0] = currp->x();
		newcop->tuples[i][1] = currp->y();
	}
	return newcop;
}
