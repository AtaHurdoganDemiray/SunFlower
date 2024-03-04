#ifndef __ContourCCWCheck_h__
#define __ContourCCWCheck_h__

#include "LineOrArc.h"

class ContourCCWCheck
{
public:
	enum CCWtype { CW = 0, CCW = 1, Sshaped = 2, Linear = 3 };
private:
	ConstLineOrArcVecPtr m_originalGeoVec;
	//LineOrArcVecPtr m_convexHull;
	vreal m_tol;
	static vreal m_convexHullCalculationAngleStep;
	int m_hullElementi;
	int m_originalLoopiForHullMatch;
	bool m_isOriginalLoopMonotone;// This means convex if closed. If open not like S shape but rotating all same direction
	bool m_isOriginalLoopLinear;
	bool m_isOriginalLoopClosed;
	//int FindHullElementThatOverlap(int& originalLoopi);// finds the first hull element that touch original loop , -1 if none
	bool CheckIfOriginalLoopClosed()const;
	void CheckIfOriginalLoopMonotoneAndLinear(bool& isMonotone, bool& isLinear);
	CCWtype m_isOriginalLoopCCW;
	//std::vector<std::pair<int, int>> m_positiveGeoRefVec;// first int is for index of element in originalLoop 
	//std::vector<std::pair<int, int>> m_negativeGeoRefVec;// second int is for true false flag for element being outer shell element or not. if element is touching outer shell it is 1 ifnot 0 
	int m_positiveEvidenceCount;
	int m_negativeEvidenceCount;
	
	bool m_isReversed;
	VPoint m_min;
	VPoint m_max;
	int IsTouchingConvexHull(const LineOrArc& curgeo)const;
	void AddToPositiveOrNegativeVecIf(int elementi, int isPrevOuterShellElement , int isOuterShellELement, const VPoint& prevTan, const VPoint& nextTan, bool& isThereCurveture);
public:
	ContourCCWCheck(ConstLineOrArcVecPtr originalGeoVec, vreal tol);// Work plane z direction must be UZ and Contour is planar
	~ContourCCWCheck();
	ConstLineOrArcVecPtr GetOriginalGeoVec()const;
	bool IsOriginalLoopClosed()const;
	bool IsOriginalLoopMonotone()const;
	//LineOrArcVecPtr GetConvexHullIf();

	CCWtype IsOriginalLoopCCW()const;
	CCWtype IsLoopCCW()const;
	static void SetConvexHullCalculationAngleStep(vreal angleStep);
	static vreal GetConvexHullCalculationAngleStep();
	VStr GetIsOriginalLoopCCWStr()const;
	void Reverse();
	bool GetIsReversed()const;
};
typedef std::shared_ptr<ContourCCWCheck> ContourCCWCheckPtr;

#endif
