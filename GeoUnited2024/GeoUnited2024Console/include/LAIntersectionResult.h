// LAIntersectionResult.h

#ifndef __LAIntersectionResult_h__
#define __LAIntersectionResult_h__

#include "VPoint.h"
#include  "LineOrArc.h"

class LAIntersectionResult
{
public:
	LAIntersectionResult()
	{
		m_isMoreThanPoints = false;
	};
	std::vector<VPoint> m_intp;
	bool m_isMoreThanPoints;
	//std::vector<LineOrArc> m_firstint;
	std::vector<VInterval> m_firstint;
	//std::vector<LineOrArc> m_secondint;
	std::vector<VInterval> m_secondint;
};

#endif