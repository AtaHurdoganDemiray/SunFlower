// TriangleSphereTouch.cpp

#include "StdAfx.h"
#include "TriangleSphereTouch.h"
#include "VTriangleE.h"
#include "QdrEqSolver.h"
#include "VInterval.h"

bool TriangleSphereTouch::CalculatePlaneShiftAlongLine (const VPoint &sphereCenter , vreal sphereRadius , const VPoint &unitShiftVector , const VTriangleE &tri
	, vreal &shiftMag1 , VPoint &touchPoint1 , vreal &shiftMag2 , VPoint &touchPoint2)
{
	vreal div = unitShiftVector % tri.GetNormal();
	if (fabs (div) < EPSILON)
		return false;
	vreal divf = 1. / div;
	shiftMag1 = (sphereRadius + tri.GetNormal() % tri.GetCorner1() - sphereCenter % tri.GetNormal()) * divf;
	shiftMag2 = (-sphereRadius + tri.GetNormal() % tri.GetCorner1() - sphereCenter % tri.GetNormal()) * divf; 
	VPoint newCenter1 = sphereCenter + unitShiftVector * shiftMag1;
	VPoint newCenter2 = sphereCenter + unitShiftVector * shiftMag2;
	touchPoint1 = newCenter1 - tri.GetNormal() * sphereRadius; 
	touchPoint2 = newCenter2 + tri.GetNormal() * sphereRadius;
	return true;
}

bool TriangleSphereTouch::CalculateLineShiftAlongLine (const VPoint & sphereCenter , vreal sphereRadius , const VPoint &unitShiftVector
										, const VLine &edgeLine
										, vreal &shiftMag1 , VPoint &touchPoint1 , vreal &shiftMag2 , VPoint &touchPoint2)
{
	VVector ev = edgeLine.GetV().Unit();
	VPoint ep = edgeLine.GetP1();
	VPoint v1 = sphereCenter ^ ev;
	VPoint v2 = unitShiftVector ^ ev;
	VPoint v3 = -ep ^ ev;
	VPoint v4 = v1 + v3;
	vreal a = v2 % v2;
	vreal b = 2 * v2 % v4;
	vreal c = (v4 % v4) - sphereRadius * sphereRadius;
	vreal roots[2];
	int nRoots = QdrEqSolver<vreal>::FindRoots (a,b,c,roots);
	if (nRoots == 0)
		return false;
	if (nRoots == 1)
	{
		shiftMag1 = roots[0];
		shiftMag2 = roots[0];
		VPoint newCenter = sphereCenter + shiftMag1 * unitShiftVector;
		touchPoint1 = ep + ((newCenter - ep) % ev) * ev;
		touchPoint2 = touchPoint1;
		return true;
	}
	if (nRoots == 2)
	{
		shiftMag1 = roots[0];
		shiftMag2 = roots[1];
		VPoint newCenter1 = sphereCenter + shiftMag1 * unitShiftVector;
		touchPoint1 = ep + ((newCenter1 - ep) % ev) * ev;
		VPoint newCenter2 = sphereCenter + shiftMag2 * unitShiftVector;
		touchPoint2 = ep + ((newCenter2 - ep) % ev) * ev;
	}
	return true;
}

bool TriangleSphereTouch::CalculatePointShiftAlongLine (const VPoint & sphereCenter , vreal sphereRadius , const VPoint &unitShiftVector
										, const VPoint &vertex
										, vreal &shiftMag1 , vreal &shiftMag2)
{
	VPoint v2 = sphereCenter - vertex;
	vreal a = unitShiftVector % unitShiftVector;
	vreal b = 2 * v2 % unitShiftVector;
	vreal c = v2 % v2 - sphereRadius * sphereRadius;
	vreal roots[2];
	int nRoots = QdrEqSolver<vreal>::FindRoots (a,b,c,roots);
	if (nRoots == 0)
		return false;
	if (nRoots == 1)
	{
		shiftMag1 = roots[0];
		shiftMag2 = roots[0];
	}
	if (nRoots == 2)
	{
		shiftMag1 = roots[0];
		shiftMag2 = roots[1];
	}
	return true;
}
bool TriangleSphereTouch::GetCollInterval (const VPoint &sphereCenter , vreal sphereRadius , const VPoint &unitShiftVector
										, const VTriangleE &tri , vreal tol
										, VInterval &collInterval
										, VPoint &touchPoint1 , VPoint &touchPoint2
										, int &pointLocation1,int &pointLocation2)
{
	vreal shiftMag1 , shiftMag2;
	vreal tightTMax (MIN_NUMBER_LIMIT) , tightTMin (MAX_NUMBER_LIMIT);
	VPoint planeTouchPoint1,planeTouchPoint2;
	bool ckPlane = CalculatePlaneShiftAlongLine (sphereCenter , sphereRadius , unitShiftVector 
		, tri 
		, shiftMag1 , planeTouchPoint1 , shiftMag2 , planeTouchPoint2);
	bool lockMin (false),lockMax(false);
	if (ckPlane)
	{
		UpdateTightInterval (shiftMag1,shiftMag2
							,planeTouchPoint1,planeTouchPoint2
							,tri.IsInTriangle(planeTouchPoint1),tri.IsInTriangle(planeTouchPoint2)
							,tightTMin,tightTMax
							,collInterval
							,touchPoint1,touchPoint2
							,lockMin,lockMax
							,0
							,pointLocation1,pointLocation2);
		
		if (fabs(tightTMin - MAX_NUMBER_LIMIT) > EPSILON)
		{
			lockMin = true;
		}
		if (fabs(tightTMax - MIN_NUMBER_LIMIT) > EPSILON)
		{
			lockMax = true;
			if (lockMin)
				return true;
		}
	}
	else
	{
		return false; // no intersection with plane
	}

	VLine el1 (tri.GetCorner1(),tri.GetCorner2());
	VPoint etp1 , etp2;
	VInterval ei(-sphereRadius , sphereRadius);
	bool ckEdge1 = CalculateLineShiftAlongLine (sphereCenter,sphereRadius , unitShiftVector 
		, el1 
		, shiftMag1 , etp1 , shiftMag2 , etp2);  
	vreal t;
	if (ckEdge1)
	{
		UpdateTightInterval (shiftMag1,shiftMag2
			,etp1,etp2
			,el1.IsOnLine(etp1,t,tol),el1.IsOnLine(etp2,t,tol)
			,tightTMin , tightTMax
			,collInterval
			,touchPoint1,touchPoint2
			,lockMin,lockMax
			,1
			,pointLocation1,pointLocation2);
	}
	VLine el2 (tri.GetCorner2(),tri.GetCorner3());
	bool ckEdge2 = CalculateLineShiftAlongLine (sphereCenter,sphereRadius,unitShiftVector,el2,shiftMag1,etp1,shiftMag2,etp2);
	if (ckEdge2)
	{
		UpdateTightInterval (shiftMag1,shiftMag2
			,etp1,etp2
			,el2.IsOnLine(etp1,t,tol),el2.IsOnLine(etp2,t,tol)
			,tightTMin,tightTMax
			,collInterval
			,touchPoint1,touchPoint2
			,lockMin,lockMax
			,2
			,pointLocation1,pointLocation2);
	}
	VLine el3 (tri.GetCorner3(),tri.GetCorner1());
	bool ckEdge3 = CalculateLineShiftAlongLine (sphereCenter,sphereRadius,unitShiftVector,el3,shiftMag1,etp1,shiftMag2,etp2);
	if (ckEdge3)
	{
		UpdateTightInterval (shiftMag1,shiftMag2
			,etp1,etp2
			,el3.IsOnLine(etp1,t,tol),el3.IsOnLine(etp2,t,tol)
			,tightTMin,tightTMax
			,collInterval
			,touchPoint1,touchPoint2
			,lockMin,lockMax
			,3
			,pointLocation1,pointLocation2);
	}
	VPoint ver1 (tri.GetCorner1());
	bool ckVertex1 = CalculatePointShiftAlongLine (sphereCenter,sphereRadius,unitShiftVector,ver1,shiftMag1,shiftMag2);
	if (ckVertex1)
	{
		UpdateTightInterval (shiftMag1,shiftMag2
			,ver1,ver1
			,true,true
			,tightTMin,tightTMax
			,collInterval
			,touchPoint1
			,touchPoint2
			,lockMin,lockMax
			,-1
			,pointLocation1,pointLocation2);
	}
	VPoint ver2 (tri.GetCorner2());
	bool ckVertex2 = CalculatePointShiftAlongLine (sphereCenter,sphereRadius,unitShiftVector,ver2,shiftMag1,shiftMag2);
	if (ckVertex2)
	{
		UpdateTightInterval (shiftMag1,shiftMag2
			,ver2,ver2
			,true,true
			,tightTMin,tightTMax
			,collInterval
			,touchPoint1,touchPoint2
			,lockMin,lockMax
			,-2
			,pointLocation1,pointLocation2);
	}
	VPoint ver3 (tri.GetCorner3());
	bool ckVertex3 = CalculatePointShiftAlongLine (sphereCenter,sphereRadius,unitShiftVector,ver3,shiftMag1,shiftMag2);
	if (ckVertex3)
	{
		UpdateTightInterval (shiftMag1,shiftMag2
			,ver3,ver3
			,true,true
			,tightTMin,tightTMax
			,collInterval
			,touchPoint1,touchPoint2
			,lockMin,lockMax
			,-3
			,pointLocation1,pointLocation2);
	}
	if (fabs(tightTMin - MAX_NUMBER_LIMIT) > EPSILON && fabs(tightTMax - MIN_NUMBER_LIMIT) > EPSILON)
	{
		return true;
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog("Unset interval limit exist"," TriangleSphereTouch::GetCollInterval");
		return false;
	}
}

void TriangleSphereTouch::UpdateTightInterval (vreal shiftMag1 , vreal shiftMag2 , VPoint &geoTouchPoint1 , VPoint &geoTouchPoint2,bool isOnGeo1 , bool isOnGeo2
	,vreal &tightTMin,vreal &tightTMax
	,VInterval &collInterval , VPoint &touchPoint1 , VPoint &touchPoint2
	,bool lockMin , bool lockMax
	,int locationToSet
	,int &pointLocation1,int &pointLocation2)
{
	if (shiftMag1 > shiftMag2)
	{
		//std::swap<vreal> (shiftMag1,shiftMag2);
		vreal temp = shiftMag1;
		shiftMag1 = shiftMag2;
		shiftMag2 = temp;
		//std::swap<VPoint> (geoTouchPoint1,geoTouchPoint2);
		VPoint tmp = geoTouchPoint1;
		geoTouchPoint1 = geoTouchPoint2;
		geoTouchPoint2 = tmp;
		//std::swap<bool> (isOnGeo1,isOnGeo2);
		bool tmmp = isOnGeo1;
		isOnGeo1 = isOnGeo2;
		isOnGeo2 = tmmp;
	}
	
	if (isOnGeo1 && lockMin == false)
	{
		if(shiftMag1 < tightTMin)
		{
			tightTMin = shiftMag1;
			collInterval.SetT0 (tightTMin);
			touchPoint1 = geoTouchPoint1;
			pointLocation1 = locationToSet;
		}
	}
	if (isOnGeo2 && lockMax == false)
	{
		if (shiftMag2 > tightTMax)
		{
			tightTMax = shiftMag2;
			collInterval.SetT1 (tightTMax);
			touchPoint2 = geoTouchPoint2;
			pointLocation2 = locationToSet;
		}
	}
}