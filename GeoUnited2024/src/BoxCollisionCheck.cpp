// BoxCollisionCheck.cpp

#include "stdafx.h"
#include "BoxCollisionCheck.h"

BoxCollisionCheck::BoxCollisionCheck(const std::vector<VPoint> &allStaticBox
	, const std::vector<VPoint> &allMovingBox
	,const VPoint &staticEnlargeDeltaMin
	, const VPoint &staticEnlargeDeltaMax
	, const VPoint &staticTr)
	:m_allStaticBox(allStaticBox), m_allMovingBox(allMovingBox), m_staticEnlargeDeltaMin(staticEnlargeDeltaMin), m_staticEnlargeDeltaMax(staticEnlargeDeltaMax), m_staticTr(staticTr)
{
	// Enlarge and transform all static boxes
	VPoint totalAddMn = m_staticEnlargeDeltaMin + m_staticTr;
	VPoint totalAddMx = m_staticEnlargeDeltaMax + m_staticTr;
	int statSize = (int)m_allStaticBox.size();
	for (int si = 0; si < statSize; si+=2)
	{
		VPoint &curStatMn = m_allStaticBox.at(si);
		VPoint &curStatMx = m_allStaticBox.at(si+1);
		curStatMn += totalAddMn;
		curStatMx += totalAddMx;
	}
}

std::vector<VInterval> BoxCollisionCheck::GetCollIntervalForXAxis()
{
	int moveSize = (int)m_allMovingBox.size();
	int statSize = (int)m_allStaticBox.size();
	std::vector<VInterval> totalColl;
	for (int mi = 0; mi < moveSize; mi+=2)
	{
		const VPoint &curMovingMin = m_allMovingBox.at(mi);
		const VPoint &curMovingMax = m_allMovingBox.at(mi+1);
		for (int si = 0; si < statSize; si+=2)
		{
			const VPoint &curStatMin = m_allStaticBox.at(si);
			const VPoint &curStatMax = m_allStaticBox.at(si + 1);
			VInterval curColl = GetSingleCollIntervalForXaxis(curStatMin, curStatMax, curMovingMin, curMovingMax);
			if (curColl.IsEmptySet() == false)
			{
				//totalColl.push_back(curColl);
				std::vector<VInterval> tmpVec;
				tmpVec.push_back(curColl);
				totalColl = VInterval::Merge(tmpVec, totalColl);
			}
		}
	}
	return totalColl;
}

VInterval BoxCollisionCheck::GetSingleCollIntervalForXaxis
(const VPoint &boxStaticMn,
const VPoint &boxStaticMx,
const VPoint &boxMovingMn,
const VPoint &boxMovingMx)
{
	// first check if y and z interfere
	if (boxMovingMx.y() < boxStaticMn.y() || boxMovingMx.z() < boxStaticMn.z())
	{
		// no interference
		VInterval emptySet(0.0, 0.0);
		return emptySet;
	}
	if (boxMovingMn.y() > boxStaticMx.y() || boxMovingMn.z() > boxStaticMx.z())
	{
		// no interference
		VInterval emptySet(0.0, 0.0);
		return emptySet;
	}

	// there is collision and in x axis it is between 
	//vreal deltaMovingX = boxMovingMx.x() - boxMovingMn.x();
	VInterval collisionInt(boxStaticMn.x() - boxMovingMx.x(), boxStaticMx.x() - boxMovingMn.x());
	return collisionInt;
}

const std::vector<VPoint> & BoxCollisionCheck::GetAllStaticBox()const
{
	return m_allStaticBox;
}

void BoxCollisionCheck::AddStaticBox(const VPoint &statBoxMin, const VPoint &statBoxMax)
{
	VPoint newStMn(statBoxMin);
	VPoint newStMx(statBoxMax);
	newStMn += m_staticEnlargeDeltaMin;
	newStMn += m_staticTr;
	newStMx += m_staticEnlargeDeltaMax;
	newStMx += m_staticTr;
	m_allStaticBox.push_back(newStMn);
	m_allStaticBox.push_back(newStMx);
}