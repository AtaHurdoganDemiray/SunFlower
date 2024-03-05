// TrianglesToPoints.h

#ifndef __TrianglesToPoints_h__
#define __TrianglesToPoints_h__

class Triangles;
struct VPoint;

class TrianglesToPoints
{
public:
	void TriToPoiWithStep ( const vreal &step
						, const Triangles &tri 
						, std::vector<VPoint> &pvec 
						, std::vector<VPoint> &nvec); 

};

#endif