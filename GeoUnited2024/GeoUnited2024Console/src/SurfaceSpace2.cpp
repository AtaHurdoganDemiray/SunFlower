// SurfaceSpace2.cpp

#include "stdafx.h"
#include "SurfaceSpace2.h"
#include "UntrimmedSurface.h"

int SurfaceSpace2::m_refineCount = 3;
int SurfaceSpace2::m_refineXm = 50;
int SurfaceSpace2::m_refineYn = 50;
int SurfaceSpace2::m_divergeException = 177;

SurfaceSpace2::SurfaceSpace2(int m, int n, UntrimmedSurface &surf)
	:m_surf(surf), m_m(m), m_n(n)
{
	vreal u0 = 0.0;//m_surf.U0();
	vreal u1 = 1.0;//m_surf.U1();
	vreal v0 = 0.0;//m_surf.V0();
	vreal v1 = 1.0;//m_surf.V1();

	FillSpaces(m_surf
		, u0 , u1
		, v0 , v1
		, m_m , m_n
		, m_paramSpace
		, m_modelSpace);
}
	
int SurfaceSpace2::GetM()const
{
	return m_m;
}
	
int SurfaceSpace2::GetN()const
{
	return m_n;
}
	
const UntrimmedSurface & SurfaceSpace2::GetSurf()const
{
	return m_surf;
}
	
UntrimmedSurface & SurfaceSpace2::GetSurf()
{
	return m_surf;
}
	
void SurfaceSpace2::FillSpaces(UntrimmedSurface &surf
	, vreal u0 , vreal u1
	, vreal v0 , vreal v1
	, int m , int n
	, std::vector<std::vector<VPoint2d>> &parSpace
	, std::vector<std::vector<VPoint>> &modSpace)
{
	vreal deltau = u1 - u0;
	vreal deltav = v1 - v0;
	vreal invu = deltau / m;
	vreal invv = deltav / n;
	for (int mi = 0; mi <= m; ++mi)
	{
		vreal u = u0 + invu * mi;
		parSpace.push_back(std::vector<VPoint2d>(n + 1));
		modSpace.push_back(std::vector<VPoint>(n + 1));
		for (int ni = 0; ni <= n; ++ni)
		{
			vreal v = v0 + invv * ni;
			VPoint2d parp(u, v);
			VPoint modp = surf.GetPoint(u, v);
			parSpace.at(mi).at(ni) = parp;
			modSpace.at(mi).at(ni) = modp;
		}
	}
}
	
// returns -1 if unsuccessfull , returrns xi otherwise
int SurfaceSpace2::SearchNearest(const VPoint &modelPoint
	, const std::vector<std::vector<VPoint2d>> &parSpace
	, const std::vector<std::vector<VPoint>> &modSpace
	, int m , int n
	, int &xi, int &yi
	)const
{
	vreal mnDist2 = MAX_NUMBER_LIMIT;
	xi = -1;
	yi = -1;
	
	for (int mi = 0; mi <= m; ++mi)
	{
		for (int ni = 0; ni <= n; ++ni)
		{
			const VPoint &modp = modSpace.at(mi).at(ni);
			vreal curDist2 = (modp - modelPoint).GetLength2();
			if (curDist2 < mnDist2)
			{
				mnDist2 = curDist2;
				xi = mi;
				yi = ni;
			}
		}
	}
	return xi;
}
	
// returns -1 if unsuccessfull , returrns subxi otherwise
int SurfaceSpace2::RefineSolution(const VPoint &modelPoint
	, const std::vector <std::vector<VPoint2d>> &parSpace
	, const std::vector <std::vector<VPoint >> &modSpace
	, int xi, int yi
	, int xdivide, int ydivide
	, std::vector<std::vector<VPoint2d>> &subParSpace
	, std::vector<std::vector<VPoint>> &subModSpace
	, int &subxi, int &subyi
	)const
{
	int xpi = xi - 1;
	if (xpi < 0)
		xpi = 0;
	int xni = xi + 1;
	if (xni > (int)parSpace.size() - 1)
		xni = (int)parSpace.size() -1;
	int ypi = yi - 1;
	if (ypi < 0)
		ypi = 0;
	int yni = yi + 1;
	if (yni > (int)parSpace.at(0).size() - 1)
		yni = (int)parSpace.at(0).size() - 1;
	// fill sub space
	const VPoint2d &qmn = parSpace.at(xpi).at(ypi);
	const VPoint2d &qmx = parSpace.at(xni).at(yni);
	VPoint2d qqmn(qmn);
	qqmn.ReplaceMin(qmx);
	VPoint2d qqmx(qmx);
	qqmx.ReplaceMax(qmn);
	vreal u0 = qqmn.x();
	vreal u1 = qqmx.x();
	vreal v0 = qqmn.y();
	vreal v1 = qqmx.y();
	FillSpaces(m_surf, u0, u1, v0, v1,xdivide,ydivide,subParSpace,subModSpace);
	return SearchNearest(modelPoint, subParSpace, subModSpace,xdivide,ydivide,subxi,subyi);
}
	
bool SurfaceSpace2::Unproject(const VPoint &modelPoint, vreal tol, VPoint2d &par)const
{
	int xi, yi;
	int ck = SearchNearest(modelPoint, m_paramSpace, m_modelSpace,m_m,m_n,xi, yi);
	if (ck == -1)
	{
		throw VException("SurfaceSpace2::Unproject exception , searchnearest fails",1);
	}

	std::vector<std::vector<VPoint2d>> subParSpace;
	std::vector<std::vector<VPoint>> subModSpace;
	int subxi(-1);
	int subyi(-1);
	int ck1 = RefineSolution(modelPoint, m_paramSpace, m_modelSpace , xi, yi, m_refineXm, m_refineYn, subParSpace, subModSpace, subxi, subyi);
	if (ck1 == -1)
		throw VException("SurfaceSpace2::Unproject exception , refine solution failed",1);
	const VPoint &closePoint = subModSpace.at(subxi).at(subyi);

	vreal err = (closePoint - modelPoint).GetLength();
	if (err < tol*1.5)
	{
		par = subParSpace.at(subxi).at(subyi);
		return true;
	}

	std::vector<std::vector<VPoint2d>> subParSpace2;
	std::vector<std::vector<VPoint>> subModSpace2;
	int subxi2(-1);
	int subyi2(-1);
	int ck2 = RefineSolution(modelPoint, subParSpace, subModSpace , subxi, subyi, m_refineXm, m_refineYn, subParSpace2, subModSpace2, subxi2, subyi2);
	if (ck2 == -1)
		throw VException("SurfaceSpace2::Unproject exception , refine solution failed", 1);
	const VPoint &closePoint2 = subModSpace2.at(subxi2).at(subyi2);
	vreal err2 = (closePoint2 - modelPoint).GetLength();
	if (err2 < tol*1.5)
	{
		par = subParSpace2.at(subxi2).at(subyi2);
		return true;
	}

	std::vector<std::vector<VPoint2d>> subParSpace3;
	std::vector<std::vector<VPoint>> subModSpace3;
	int subxi3(-1);
	int subyi3(-1);
	int ck3 = RefineSolution(modelPoint, subParSpace2, subModSpace2, subxi2, subyi2, m_refineXm, m_refineYn, subParSpace3, subModSpace3, subxi3, subyi3);
	if (ck3 == -1)
		throw VException("SurfaceSpace2::Unproject exception , refine solution failed", 1);
	const VPoint &closePoint3 = subModSpace3.at(subxi3).at(subyi3);
	vreal err3 = (closePoint3 - modelPoint).GetLength();
	if (err3 < tol*1.5)
	{
		par = subParSpace3.at(subxi3).at(subyi3);
		return true;
	}
	throw VException("SurfaceSpace2::Unproject failed!",m_divergeException);
}