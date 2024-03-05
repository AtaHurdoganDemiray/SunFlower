// PointGrid.cpp

#include "stdafx.h"
#include "PointGrid.h"
#include "VPoint.h"
#include "LinePointMinDist.h"

PointGrid::PointGrid (int nPointsU , int nPointsV , std::vector<vreal> &pointCoord)
	:m_nPointsU(nPointsU),m_nPointsV(nPointsV),m_pointCoord(pointCoord)
{

}

int PointGrid::GetNPointsU()const
{
	return m_nPointsU;
}
int PointGrid::GetNPointsV()const
{
	return m_nPointsV;
}
const std::vector<vreal> & PointGrid::GetPointCoord()const
{
	return m_pointCoord;
}
std::vector<vreal> & PointGrid::GetPointCoord()
{
	return m_pointCoord;
}
VPoint PointGrid::GetPoint (int upointi , int vpointi)const
{
	int pbegini = upointi + vpointi * m_nPointsU;
	int coordi = pbegini * 3;
	vreal x = m_pointCoord.at(coordi);
	vreal y = m_pointCoord.at(coordi+1);
	vreal z = m_pointCoord.at(coordi+2);
	return VPoint (x,y,z);
}

VPoint PointGrid::SetPoint (int upointi , int vpointi , const VPoint &p)// returns the overrided point
{
	int pbegini = upointi + vpointi * m_nPointsU;
	int coordi = pbegini * 3;
	vreal x = m_pointCoord.at(coordi);
	vreal y = m_pointCoord.at(coordi+1);
	vreal z = m_pointCoord.at(coordi+2);
	m_pointCoord.at(coordi) = p.x();
	m_pointCoord.at(coordi + 1) = p.y();
	m_pointCoord.at(coordi + 2) = p.z();
	return VPoint (x,y,z);
}

void PointGrid::GetLinkedPoints (int upointi , int vpointi , std::list<std::pair<int,int>> &uvpairs)const
{
	int upointimm = upointi-1;
	int upointipp = upointi+1;
	int vpointimm = vpointi-1;
	int vpointipp = vpointi+1;
	if (upointimm >= 0)
	{
		uvpairs.push_back(std::pair<int,int>(upointimm,vpointi));
	}
	if (upointipp < m_nPointsU)
	{
		uvpairs.push_back (std::pair<int,int>(upointipp,vpointi));
	}
	if (vpointimm >= 0)
	{
		uvpairs.push_back (std::pair<int,int>(upointi,vpointimm));
	}
	if (vpointipp < m_nPointsV)
	{
		uvpairs.push_back (std::pair<int,int>(upointi,vpointipp));
	}
}

void PointGrid::GetUSequence (int vpointi , std::vector<VPoint> &uSeq)const
{
	for (int u=0;u<m_nPointsU;++u)
	{
		uSeq.push_back (GetPoint(u,vpointi));
	}
}

void PointGrid::GetVSequence (int upointi , std::vector<VPoint> &vSeq)const
{
	for (int v=0;v<m_nPointsV;++v)
	{
		vSeq.push_back (GetPoint(upointi,v));
	}
}

vreal PointGrid::GetNearestPoint (const VLine &l , int &upointi , int &vpointi , VPoint &p)const
{
	vreal mndist = MAX_NUMBER_LIMIT;
	LinePointMinDist clc;
	const VPoint &lp = l.GetP1();
	VPoint lv = l.GetV();
	vreal curdist (0.0);
	VPoint curp;
	vreal s;
	for (int v=0;v<m_nPointsV;++v)
	{
		for (int u=0;u<m_nPointsU;++u)
		{
			curp = GetPoint (u,v);
			curdist = clc.InfiniteLinePointMinDist (lp,lv,curp,s);
			if (curdist < mndist)
			{
				mndist = curdist;
				p = curp;
				upointi = u;
				vpointi = v;
			}
		}
	}
	return mndist;
}