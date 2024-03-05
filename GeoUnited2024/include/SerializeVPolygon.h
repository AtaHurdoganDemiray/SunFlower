#pragma once
#include "VPoint2d.h"

class SerializeVPolygon
{
public:
	SerializeVPolygon(void);
	~SerializeVPolygon(void);
	void ReadPolygon(std::vector<VPoint2d> &pointPool, 
					std::vector<int> &polygonPoints, 
					const VStr file);
	void WritePolygon(const std::vector<VPoint2d> &pointPool, 
				    const std::vector<int> &polygonPoints, 
					const VStr file);
};
