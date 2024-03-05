#include "stdafx.h"
#include "ContourCCWCheck.h"

vreal ContourCCWCheck::m_convexHullCalculationAngleStep = 1.;

void ContourCCWCheck::SetConvexHullCalculationAngleStep(vreal angleStep)
{
	m_convexHullCalculationAngleStep = angleStep;
}
vreal ContourCCWCheck::GetConvexHullCalculationAngleStep()
{
	return m_convexHullCalculationAngleStep;
}

ContourCCWCheck::ContourCCWCheck(ConstLineOrArcVecPtr originalGeoVec, vreal tol)// Work plane z direction must be UZ and Contour is planar
	:m_originalGeoVec(originalGeoVec), m_tol(tol)/*, m_convexHull(nullptr)*/, m_hullElementi(-1), m_originalLoopiForHullMatch(-1), m_isReversed(false)
	,m_positiveEvidenceCount(0),m_negativeEvidenceCount(0)
{
	m_isOriginalLoopClosed = CheckIfOriginalLoopClosed();
	CheckIfOriginalLoopMonotoneAndLinear(m_isOriginalLoopMonotone, m_isOriginalLoopLinear);
	if (m_isOriginalLoopClosed)
	{
		if (m_isOriginalLoopMonotone == false)
		{
			if (m_isOriginalLoopLinear)
			{
				m_isOriginalLoopCCW = CCWtype::Linear;
			}
			else
			{
				//m_convexHull = RotatingCalipers::MakeConvexHullOnXYPlane(m_originalGeoVec, m_convexHullCalculationAngleStep, m_tol);
				//m_hullElementi = FindHullElementThatOverlap(m_originalLoopiForHullMatch);
				//if (m_hullElementi == -1)
				//	throw VException("ContourCCWCheck::Constructor exception. Hull is calculated but no hull-original loop match found.",1);
				//int counter = 0;
				//size_t ni = m_hullElementi + 1;
				//const LineOrArc& hullEl = m_convexHull->at(m_hullElementi);
				//VPoint firsttan = hullEl.GetEndTangent();
				//size_t hullSize = m_convexHull->size();
				//bool issuccessfull = false;
				//CCWtype hullLoopCCW;
				//while (true)
				//{
				//	const LineOrArc& nextEl = m_convexHull->at(ni % hullSize);
				//	VPoint nextTan = nextEl.GetStartTangent();

				//	VPoint cp = firsttan ^ nextTan;
				//	if (cp.Compare(VPoint(0.0, 0.0, 0.0), m_tol) == false)
				//	{
				//		if (cp % UZ > 0.0)
				//			hullLoopCCW = CCWtype::CCW;
				//		else
				//			hullLoopCCW = CCWtype::CW;
				//		issuccessfull = true;
				//		break;
				//	}
				//	if (ni == m_hullElementi || counter > m_convexHull->size() + 1)
				//		break;
				//	++ni;
				//	++counter;
				//}
				//if (issuccessfull == false)
				//	throw VException("ContourCCWCheck::Constructor exception. Monotone convexhull direction can not be determined.", 1);
				//const LineOrArc& originalEl = m_originalGeoVec->at(m_originalLoopiForHullMatch);
				//VPoint midp = originalEl.GetP(0.5);
				//VPoint originalTan = originalEl.GetTan(0.5);
				//vreal t;
				//bool ck = hullEl.GetParIf(midp, t, m_tol);
				//if (ck == false)
				//	throw VException("ContourCCWCheck::Constructor exception. Hull-loop match element mid point is not on convexhull element.", 1);
				//VPoint curHallTan = hullEl.GetTan(t);
				//if (hullLoopCCW == CCWtype::CW)
				//	curHallTan *= -1.;
				//if (originalTan % curHallTan > 0.0)
				//	m_isOriginalLoopCCW = CCWtype::CCW;
				//else
				//	m_isOriginalLoopCCW = CCWtype::CW;
				//int positiveCounter = 0;
				//int negativeCounter = 0;
				//auto pe = m_positiveGeoRefVec.end();
				//for (auto pi = m_positiveGeoRefVec.begin(); pi != pe; ++pi)
				//{
				//	++positiveCounter;
				//}
				//auto ne = m_negativeGeoRefVec.end();
				//for (auto ni = m_negativeGeoRefVec.begin(); ni != ne; ++ni)
				//{
				//	++negativeCounter;
				//}
				if (m_positiveEvidenceCount > m_negativeEvidenceCount)
					m_isOriginalLoopCCW = CCWtype::CCW;
				else if (m_negativeEvidenceCount > m_positiveEvidenceCount)
					m_isOriginalLoopCCW = CCWtype::CW;
				else
					throw VException("ContourCCWCheck constructor exception. Positive counter is equal to negative counter.", 1);
			}
		}
		else
		{
			if (m_isOriginalLoopLinear)
			{
				m_isOriginalLoopCCW = CCWtype::Linear;
			}
			else
			{
				//if (m_negativeGeoRefVec.size() == 0)
				//	m_isOriginalLoopCCW = CCWtype::CCW;
				//else
				//	m_isOriginalLoopCCW = CCWtype::CW;
				//int positiveCounter = (int)m_positiveGeoRefVec.size();
				//int negativeCounter = (int)m_negativeGeoRefVec.size();
				if (m_positiveEvidenceCount > m_negativeEvidenceCount)
					m_isOriginalLoopCCW = CCWtype::CCW;
				else if (m_negativeEvidenceCount > m_positiveEvidenceCount)
					m_isOriginalLoopCCW = CCWtype::CW;
				else
					throw VException("ContourCCWCheck constructor exception. Positive counter is equal to negative counter.r2", 1);
			}
		}
	}
	else
	{
		// open loop
		if (m_isOriginalLoopMonotone == false)
		{
			if (m_isOriginalLoopLinear == false)
				m_isOriginalLoopCCW = CCWtype::Sshaped;
			else
				m_isOriginalLoopCCW = CCWtype::Linear;
		}
		else
		{
			if (m_isOriginalLoopLinear)
				m_isOriginalLoopCCW = CCWtype::Linear;
			else
			{
				//if (m_negativeGeoRefVec.size() == 0)
				//	m_isOriginalLoopCCW = CCWtype::CCW;
				//else
				//	m_isOriginalLoopCCW = CCWtype::CW;
				//int positiveCounter = (int)m_positiveGeoRefVec.size();
				//int negativeCounter = (int)m_negativeGeoRefVec.size();
				if (m_positiveEvidenceCount > m_negativeEvidenceCount)
					m_isOriginalLoopCCW = CCWtype::CCW;
				else if (m_negativeEvidenceCount > m_positiveEvidenceCount)
					m_isOriginalLoopCCW = CCWtype::CW;
				else
					throw VException("ContourCCWCheck constructor exception. Positive counter is equal to negative counter.r3", 1);
			}
		}
	}
}
ContourCCWCheck::~ContourCCWCheck()
{
	//delete m_originalGeoVec;
	//delete m_convexHull;
}
bool ContourCCWCheck::CheckIfOriginalLoopClosed()const
{
	size_t mysize = m_originalGeoVec->size();
	if (mysize == 0)
		throw VException("ContourCCWCheck::CheckIfOriginalLoopClosed exception, loop have no elements.", 1);
	VPoint sp = m_originalGeoVec->at(0).GetStartPoint();
	VPoint ep = m_originalGeoVec->at(mysize - 1).GetEndPoint();
	return (sp.Compare(ep, m_tol));
}
int ContourCCWCheck::IsTouchingConvexHull(const LineOrArc& curgeo)const
{
	VPoint localmn, localmx;
	VPoint midtan = (curgeo.GetStartTangent() + curgeo.GetEndTangent()) * 0.5;
	VPoint csdirx = midtan.Unit();
	VPoint csdiry = UZ ^ csdirx;
	CoordinateSystem cs(VPoint(0.0,0.0,0.0),csdirx,csdiry,UZ);
	curgeo.GetMinMaxInCs(cs, localmn, localmx);
	VPoint min, max;
	LineOrArc::GetMinMaxInCs(*m_originalGeoVec, cs, min, max);
	int isOuterShellElement = 0;
	if (fabs(min.x() - localmn.x()) < m_tol || fabs(max.x() - localmx.x()) < m_tol || fabs(min.y() - localmn.y()) < m_tol || fabs(max.y() - localmx.y()) < m_tol)
		isOuterShellElement = 1;
	return isOuterShellElement;
}
void ContourCCWCheck::AddToPositiveOrNegativeVecIf(int elementi, int isPrevOuterShellElement , int isOuterShellELement, const VPoint& prevTan, const VPoint& nextTan, bool& isThereCurvature)
{
	if (isPrevOuterShellElement == FALSE || isOuterShellELement == FALSE)
		return;
	//checkif last element is this elementi
	//auto lip = m_positiveGeoRefVec.rbegin();
	//auto lin = m_negativeGeoRefVec.rbegin();
	//if ((lip != m_positiveGeoRefVec.rend() && lip->first == elementi) || (lin != m_negativeGeoRefVec.rend() && lin->first == elementi))
	//	return;

	vreal fixtol = VGlobalValues::TENMICRONS_TO_VUNIT * 5.0;//1.e-6;
	if (prevTan.Compare(nextTan, fixtol) == true)
		return;// vectors are same
	VPoint crossPro = prevTan ^ nextTan;

	VPoint nullvector(0.0, 0.0, 0.0);
	if (crossPro.Compare(nullvector, fixtol) == false)
	{
		vreal toCk = crossPro.Unit() % UZ;
		if (toCk > 0.0)
		{
			//m_positiveGeoRefVec.push_back(std::pair<int, int>(elementi, isOuterShellELement));
			++m_positiveEvidenceCount;
		}
		else
		{
			//m_negativeGeoRefVec.push_back(std::pair<int, int>(elementi, isOuterShellELement));
			++m_negativeEvidenceCount;
		}
		isThereCurvature = true;
	}
}
void ContourCCWCheck::CheckIfOriginalLoopMonotoneAndLinear(bool& isMonotone, bool& isLinear)
{
	// This functions assume vec is on XY plane and plane normal is UZ !!!
	vreal fixtol = 0.1;
	LineOrArc::GetMinMax(*m_originalGeoVec, m_min, m_max);

	int geoSize = (int)m_originalGeoVec->size();
	const LineOrArc& curgeo = m_originalGeoVec->at(0);
	VPoint prevTan = curgeo.GetTan(1.0-fixtol);//GetEndTangent();
	VPoint nextTan;

	//VPoint midTan0 = curgeo.GetTan(0.5);
	//VPoint midTan1 = curgeo.GetTan(0.5 + fixtol);//m_tol);
	//VPoint cp = midTan0 ^ midTan1;
	bool isThereCurvature = false;
	int isOuterShellElement = IsTouchingConvexHull(curgeo);
	int isPrevOuterShellElement = isOuterShellElement;
	//AddToPositiveOrNegativeVecIf(0, isPrevOuterShellElement , isOuterShellElement, midTan0, midTan1, isThereCurvature);

	if (IsOriginalLoopClosed() && geoSize > 1)
	{
		nextTan = curgeo.GetTan(fixtol);//GetStartTangent();//m_originalGeoVec->at(geoSize-1).GetStartTangent();
		const LineOrArc& lastGeo = m_originalGeoVec->at(geoSize - 1);
		VPoint prevprevTan = lastGeo.GetTan(1.0-fixtol);//.GetEndTangent();
		int isPrevPrevOuterShellElement = IsTouchingConvexHull(lastGeo);
		//VPoint crosspro = prevprevTan ^ nextTan;
		AddToPositiveOrNegativeVecIf(0, isPrevPrevOuterShellElement , isOuterShellElement, prevprevTan, nextTan, isThereCurvature);
	}
	else if (IsOriginalLoopClosed() && geoSize == 1)
	{
		const LineOrArc& singleGeo = m_originalGeoVec->at(0);
		if (singleGeo.GetType() == LineOrArc::line)
			throw VException("ContourCCWCheck::CheckIfOriginalLoopMonotoneAndLinear exception. Single item and closed geometry can not be line.",1);
		const VArc* singlea = singleGeo.GetArc();
		isMonotone = true;
		isLinear = false;
		if (singlea->GetSweepAngle() > 0)
			++m_positiveEvidenceCount;
		else
			++m_negativeEvidenceCount;
		return;
	}
	for (int gi = 1; gi < geoSize; ++gi)
	{
		const LineOrArc& curgeo2 = m_originalGeoVec->at(gi);
		nextTan = curgeo2.GetTan(fixtol);//GetStartTangent();
		isOuterShellElement = IsTouchingConvexHull(curgeo2);
		//VPoint crosspro = prevTan ^ nextTan;
		AddToPositiveOrNegativeVecIf(gi,isPrevOuterShellElement , isOuterShellElement, prevTan, nextTan, isThereCurvature);

		//VPoint midTan0 = curgeo2.GetTan(0.5);
		//VPoint midTan1 = curgeo2.GetTan(0.5 + fixtol);//+ m_tol);
		//VPoint cp = midTan0 ^ midTan1;
		isPrevOuterShellElement = isOuterShellElement;
		//AddToPositiveOrNegativeVecIf(gi, isPrevOuterShellElement , isOuterShellElement, midTan0, midTan1, isThereCurvature);

		prevTan = curgeo2.GetTan(1.0-fixtol);//GetEndTangent();
	}

	if (m_negativeEvidenceCount == 0)// m_negativeGeoRefVec.size() == 0)
		isMonotone = true;
	else if (m_positiveEvidenceCount == 0)//(m_positiveGeoRefVec.size() == 0)
		isMonotone = true;
	else
		isMonotone = false;
	isLinear = !isThereCurvature;
}
ConstLineOrArcVecPtr ContourCCWCheck::GetOriginalGeoVec()const
{
	return m_originalGeoVec;
}
//LineOrArcVecPtr ContourCCWCheck::GetConvexHullIf()
//{
//	return m_convexHull;
//}
//int ContourCCWCheck::FindHullElementThatOverlap(int& originalLoopi)// returns an index in hull vector , -1 if no overlap is found
//{
//	size_t originalElSize = m_originalGeoVec->size();
//	size_t convexHullSize = m_convexHull->size();
//	vreal tmpt;
//	originalLoopi = -1;
//	for (size_t i = 0; i < originalElSize; ++i)
//	{
//		const LineOrArc& curEl = m_originalGeoVec->at(i);
//		for (size_t hi = 0; hi < convexHullSize; ++hi)
//		{
//			const LineOrArc& hullEl = m_convexHull->at(hi);
//			if (hullEl.GetType() == LineOrArc::line)
//			{
//				if (hullEl.GetLine()->GetV().Compare(VPoint(0.0, 0.0, 0.0), m_tol))
//					continue;
//			}
//			else
//			{
//				if (hullEl.GetArc()->GetArcLength() < m_tol)
//					continue;
//			}
//			VPoint elMidP = curEl.GetP(0.5);
//			bool isPointOn = hullEl.GetParIf(elMidP, tmpt, m_tol);
//			if (isPointOn)
//			{
//				originalLoopi = (int)i;
//				return (int)hi;
//			}
//		}
//	}
//	return -1;
//}
bool ContourCCWCheck::IsOriginalLoopClosed()const
{
	return m_isOriginalLoopClosed;
}
bool ContourCCWCheck::IsOriginalLoopMonotone()const
{
	return m_isOriginalLoopMonotone;
}
ContourCCWCheck::CCWtype ContourCCWCheck::IsOriginalLoopCCW()const
{
	return m_isOriginalLoopCCW;
}
ContourCCWCheck::CCWtype ContourCCWCheck::IsLoopCCW()const
{
	CCWtype toReturn(m_isOriginalLoopCCW);
	if (m_isReversed)
	{
		if (toReturn == CCW)
			toReturn = CW;
		else if (toReturn == CW)
			toReturn = CCW;
	}
	return toReturn;
}
VStr ContourCCWCheck::GetIsOriginalLoopCCWStr()const
{
	VStr isCCWStr("Undefined");
	switch (IsOriginalLoopCCW())
	{
	case ContourCCWCheck::CCWtype::CCW:
		isCCWStr = "CCW";
		break;
	case ContourCCWCheck::CCWtype::CW:
		isCCWStr = "CW";
		break;
	case ContourCCWCheck::CCWtype::Linear:
		isCCWStr = "Linear";
		break;
	case ContourCCWCheck::CCWtype::Sshaped:
		isCCWStr = "Sshaped";
		break;
	}
	return isCCWStr;
}
bool ContourCCWCheck::GetIsReversed()const
{
	return m_isReversed;
}
void ContourCCWCheck::Reverse()
{
	m_isReversed = !m_isReversed;
}