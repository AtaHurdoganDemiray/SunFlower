// VariableOffsetManager.h

#ifndef __VariableOffsetManager_h__
#define __VariableOffsetManager_h__

#include "LineOrArc.h"
#include "VPoint.h"
struct VPoint2d;

class VariableOffsetManager
{
public:
	enum FunctionType {constantLinear , changingLinear};
	enum OptSharpCorners {round , straight};
	VariableOffsetManager (double tol , double initialOffsetValue , FunctionType functionType , double deltaOffsetPerUnitLength , double initialDiameter);
	void Walk (double length);
	void WalkBack (double length);
	double GetCurOffsetValue()const;
	FunctionType GetFunctionType ()const;
	void SpiralOffset (const std::vector<LineOrArc> &srcVec , std::vector<LineOrArc> &goalVec);
	OptSharpCorners GetOptSharpCorners ()const;
	void SetOptSharpCorners (OptSharpCorners optSharpCorners);
	double GetNextOffsetValueWithoutApplying (double walkLength)const;
	bool IsColliding (const std::vector<LineOrArc> &srcVec , const VPoint &p , double curRadius,double collTol);
	bool GetDoArcFit ()const;
	void SetDoArcFit (bool doArcFit);
	void SetInitialOffsetValue (double initialOffsetValue);
	double GetInitialOffsetValue ()const;
	void SetDeltaOffsetPerUnitLength (double deltaOffsetPerUnitLength);
	double GetDeltaOffsetPerUnitLength ()const;
	void NegateOffsetValues ();
	double GetWalkLength ()const;
	void SetWalkLength (double walkLength);
	double GetInitialDiameter()const;
	void SetInitialDiameter (double initialDiameter);
	double GetTol()const;
	void SetTol (double tol);
private:
	double m_initialOffsetValue;
	double m_offsetValue;
	double m_walkLength;
	FunctionType m_functionType;
	double m_deltaOffsetPerUnitLength;
	
	void DigitiseSrcVec (const std::vector<LineOrArc> &srcVec);
	std::vector<std::vector<VPoint>> m_seq;
	std::vector<std::vector<VPoint2d>> m_nor;
	//std::vector<std::vector<int>> m_indexVec;
	static double m_indexUnitLength;
	static double m_ik;
	void MakeIndexVec (const VPoint &mn , const VPoint &mx);
	VPoint m_mn,m_mx;
	void ReconstructOffsetLineOrArcVec (const std::vector<LineOrArc> &srcVec , std::vector<LineOrArc> &goalVec);
	OptSharpCorners m_optSharpCorners;
	int GetPeriodicIndex (int i,int mysize);
	void CollCheckAndAddIf (const std::vector<LineOrArc> &srcVec , const std::vector<VPoint> &newPoints , std::vector<VPoint> &resPoints , double curRadius , double collTol);
	bool m_doArcFit;
	double m_initialDiameter;
	void HandleSharpCorners (const std::vector<LineOrArc> &srcVec , double tol , const VPoint &prevp , const VPoint2d &prevn , const VPoint &curP , const VPoint2d &curN , std::vector<VPoint> &ofpoVec , double curoffset);
	void OnePointJob (const std::vector<LineOrArc> &srcVec , VPoint &prevp , VPoint2d &prevn , const VPoint &curP , const VPoint2d &curN , double tol , std::vector<VPoint> &ofpoVec , double cosThreshold);
	double m_tol;
};

#endif