// VVoxelList.cpp

#include "stdafx.h"
#include "VVoxelList.h"
#include "VTransformable.h"
#include "LinePointMinDist.h"
#include "Model.h"

VVoxelList::VVoxelList(const VPoint &center,vreal cubeLen)
	:m_center(center), m_cubeLen(cubeLen)
{
	m_maxRadius = MIN_NUMBER_LIMIT;
	m_cubeRadius = /*0.8660*/1.5*cubeLen;//sqrt(0.75*cubeLen*cubeLen);
}

const VPoint & VVoxelList::GetCenter()const
{
	return m_center;
}
void VVoxelList::SetCenter(const VPoint &center)
{
	m_center = center;
}
vreal VVoxelList::GetMaxRadius()const
{
	return m_maxRadius;
}
bool VVoxelList::AddModelIf(VTransformable *model)
{
	// brude force check if already in the list
	bool ck = IsAlreadyInListBrudeForceCheck(model);
	if (ck == true)
		return false;

	m_modelList.push_back(model);
	VPoint mn = model->GetMinPoint();
	VPoint mx = model->GetMaxPoint();
	vreal rad = (mx - mn).GetLength()/**0.5*/ + m_cubeRadius;
	if (m_maxRadius < rad)
		m_maxRadius = rad;
	return true;
}
void VVoxelList::CollectModelIf(const VLine &hitLine, std::vector<VTransformable *> &potentialHits)const
{
	LinePointMinDist calc;
	VPoint lp = hitLine.GetP1();
	VPoint lv = hitLine.GetV();
	vreal mindist = calc.InfiniteLinePointMinDist(lp,lv,m_center);
	if (mindist > m_maxRadius + m_cubeRadius)
		return;
	std::list<VTransformable *>::const_iterator li = m_modelList.begin();
	std::list<VTransformable *>::const_iterator le = m_modelList.end();
	for (li; li != le; ++li)
	{
		VTransformable * curtr = *li;
		//if (curtr->GetTrModel()->IsVisible() == false)
		//	continue;
		VPoint mn = curtr->GetMinPoint();
		VPoint mx = curtr->GetMaxPoint();
		VPoint center = (mx + mn) * 0.5;
		vreal rad = (mx - mn).GetLength()*0.5;
		vreal curdist = calc.InfiniteLinePointMinDist(lp, lv, center);
		if (curdist <= rad + m_cubeRadius)
			potentialHits.push_back(curtr);
	}
}
void VVoxelList::CollectModelForBoxIf(const VLine &hitLine,vreal boxRadius , std::vector<VTransformable *> &potentialHits)const
{
	LinePointMinDist calc;
	VPoint lp = hitLine.GetP1();
	VPoint lv = hitLine.GetV();
	vreal mindist = calc.InfiniteLinePointMinDist(lp, lv, m_center);
	vreal radiusOffset = 1.5 * boxRadius + m_cubeRadius;
	if (mindist > (m_maxRadius + radiusOffset))
		return;
	std::list<VTransformable *>::const_iterator li = m_modelList.begin();
	std::list<VTransformable *>::const_iterator le = m_modelList.end();
	for (li; li != le; ++li)
	{
		VTransformable * curtr = *li;
		VPoint mn = curtr->GetMinPoint();
		VPoint mx = curtr->GetMaxPoint();
		VPoint center = (mx + mn) * 0.5;
		vreal rad = (mx - mn).GetLength()*0.5;
		vreal curdist = calc.InfiniteLinePointMinDist(lp, lv, center);
		if (curdist <= rad + radiusOffset)
			potentialHits.push_back(curtr);
	}
}
int VVoxelList::GetModelCount()const
{
	return (int)m_modelList.size();
}

bool VVoxelList::IsAlreadyInListBrudeForceCheck(VTransformable *tr)const
{
	citr i = m_modelList.begin();
	citr e = m_modelList.end();
	for (i; i != e; ++i)
	{
		if (*i == tr)
			return true;
	}
	return false;
}

bool VVoxelList::RemoveTransformIf(VTransformable *tr)
{
	citr i = m_modelList.begin();
	citr e = m_modelList.end();
	for (i; i != e; ++i)
	{
		if (*i == tr)
		{
			m_modelList.erase(i);
			return true;
		}
	}
	return false;
}