#pragma once
#include "VPolygon.h"
#include "VPoint2d.h"

class GridIntersect
{
public:
	GridIntersect(void);
	~GridIntersect(void);
	
	void SortedPolygon(const std::vector<int> &interi, 
						const std::vector<int> &loopPoints, 
						const std::vector<int> &corners, 
						const std::vector<VPoint2d> &pointPool 
						,std::vector<int> &pol);

};
