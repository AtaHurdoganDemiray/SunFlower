// MultiWireCollCheck.h

#ifndef __MultiWireCollCheck_h__
#define __MultiWireCollCheck_h__

struct VLine;
class VArc;
class SequentialLineSet;
class VInterval;
#include "LineOrArc.h"
class VArc2d;

class MultiWireCollCheck
{
public:
	static std::vector<VLine> TrimLine (const VLine &l , const std::vector<SequentialLineSet *> &curvVec , double offsetRad , int ignorei);
	static std::vector<VArc> TrimArc (const VArc &a , double tol , const std::vector<SequentialLineSet *> &curvVec , double offsetRad , int ignorei );
	static std::vector<VInterval> GetCollInt (const VLine &l , const std::vector<SequentialLineSet *> &curvVec , double offsetRad , int ignorei);
	static std::vector<VLine> TrimLine (const VLine &l , const std::vector<LineOrArc> &crvVec , double offsetRad , int parenti , int startneighbori , int endneighbori , double jointol);
	static std::vector<VLine> TrimLine (const VLine &l , const std::vector<LineOrArc *> &crvVec , double offsetRad , int parenti , int startneighbori , int endneighbori , double jointol);
	static std::vector<VArc> TrimArc (const VArc &a , double tol , const std::vector<LineOrArc> &curvVec , double offsetRad , int parenti , int startneighbori , int endneighbori , double jointol);
	//static std::vector<VArc> NewTrimArc(const VArc &a, double tol, const std::vector<LineOrArc> &curvVec, double offsetRad, int parenti, int startneighbori, int endneighbori, double jointol);
	static std::vector<VArc> TrimArc (const VArc &a , double tol , const std::vector<LineOrArc *> &curvVec , double offsetRad , int parenti , int startneighbori , int endneighbori , double jointol);
	static std::vector<VInterval> GetCollInt (const VLine &l , const std::vector<LineOrArc> &curvVec , double offsetRad , int parenti , int startneighbori , int endneighbori , double myt0,double myt1 , double arclen , double jointol);
	static std::vector<VInterval> GetCollInt (const VLine &l , const std::vector<LineOrArc *> &curvVec , double offsetRad , int parenti , int startneighbori , int endneighbori , double myt0,double myt1 , double arclen , double jointol);
	static VArc2d Get2dArc (VArc &as);
	static void CleanBoundaryCollisionAtStart (VInterval &ck , double myt0 , double myt1 , double arclen , double jointol);
	static void CleanBoundaryCollisionAtEnd (VInterval &ck , double myt0 , double myt1 , double arclen , double jointol);
	static bool IsColliding ( const LineOrArc &tock , const SequentialLineSet &sl , double r , double tol);
private:
	static bool IsColliding ( const VLine *l , const SequentialLineSet &sl , double r);
	static bool IsColliding(vreal nearDistance , const VPoint &curp, const VArc &srcArc, const std::vector<LineOrArc> &curves , vreal tol);
	static void RefineResultAndAddToRemaining(vreal nearDistance , int bi, int ei, const std::vector<VPoint> &pv , const VArc &a , const std::vector<LineOrArc> &curves
		, vreal roughTol, vreal tol, std::vector<VArc> &remaining);
	static bool FirstNonCollidingAtStartAndEnd(vreal nearDistance , const std::vector<VPoint> &pvsub, const LineOrArc &src, const std::vector<LineOrArc> &curves, vreal roughTol, vreal tol, vreal &newst, vreal &newet);

};

#endif