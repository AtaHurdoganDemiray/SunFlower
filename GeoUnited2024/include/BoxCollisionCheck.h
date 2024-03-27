// BoxCollisionCheck.h

#ifndef __BoxCollisionCheck_h__
#define __BoxCollisionCheck_h__

#include "VPoint.h"
#include "VInterval.h"

class BoxCollisionCheck
{
public:
	BoxCollisionCheck(const std::vector<VPoint> &allStaticBox, const std::vector<VPoint> &allMovingBox,const VPoint &staticEnlargeDeltaMin , const VPoint &staticEnlargeDeltaMax , const VPoint &staticTr);
	std::vector<VInterval> GetCollIntervalForXAxis();
	VInterval GetSingleCollIntervalForXaxis
		(const VPoint &boxStaticMn,
	const VPoint &boxStaticMx,
	const VPoint &boxMovingMn,
	const VPoint &boxMovingMx);
	const std::vector<VPoint> & GetAllStaticBox()const;
	void AddStaticBox(const VPoint &statBoxMin, const VPoint &statBoxMax);
private:
	std::vector<VPoint> m_allStaticBox;
	std::vector<VPoint> m_allMovingBox;
	VPoint m_staticEnlargeDeltaMin;
	VPoint m_staticEnlargeDeltaMax;
	VPoint m_staticTr;
};

#endif