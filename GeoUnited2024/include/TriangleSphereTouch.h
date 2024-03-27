// TriangleSphereTouch.h

#ifndef __TriangleSphereTouch_h__
#define __TriangleSphereTouch_h__

struct VPoint;
struct VTriangleE;
class VInterval;

class TriangleSphereTouch
{
public:
	static bool CalculatePlaneShiftAlongLine (const VPoint &sphereCenter , vreal sphereRadius , const VPoint &unitShiftVector
										, const VTriangleE &tri 
										, vreal &shiftMag1 , VPoint &touchPoint1 , vreal &shiftMag2 , VPoint &touchPoint2);
	static bool CalculateLineShiftAlongLine (const VPoint & sphereCenter , vreal sphereRadius , const VPoint &unitShiftVector
											, const VLine &edgeLine 
											, vreal &shiftMag1 , VPoint &touchPoint1 , vreal &shiftMag2 , VPoint &touchPoint2);
	static bool CalculatePointShiftAlongLine (const VPoint & sphereCenter , vreal sphereRadius , const VPoint &unitShiftVector
											, const VPoint &vertex
											, vreal &shiftMag1 , vreal &shiftMag2); 
	static bool GetCollInterval (const VPoint &sphereCenter , vreal sphereRadius , const VPoint &unitShiftVector
									, const VTriangleE &tri , vreal tol
									, VInterval & collInterval
									, VPoint &touchPoint1 , VPoint &touchPoint2
									, int &point1Location , int &point2Location);// 0 on tri,1 on edge1,2 on edge2,3 on edge3,-1 on corner1...
	static void UpdateTightInterval (vreal shiftMag1 , vreal shiftMag2 , VPoint &geoTouchPoint1 , VPoint &geoTouchPoint2,bool isOnGeo1 , bool isOnGeo2
	,vreal &tightTMin,vreal &tightTMax
	,VInterval &collInterval , VPoint &touchPoint1 , VPoint &touchPoint2
	, bool lockMin , bool lockMax
	, int locationToSet 
	,int &pointLocation1,int &pointLocation2);
};

#endif