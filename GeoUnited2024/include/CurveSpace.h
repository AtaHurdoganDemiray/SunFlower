// CurveSpace.h

#ifndef __CurveSpace_h__
#define __CurveSpace_h__

#include "LineSetPointMinDist.h"
#include "Function.h"
#include "VPoint.h"

class dCurve;

class CurveSpace : private Function
{
public:
	// This class calculates the distance of p to crv and witnesspoint that is nearest position on curve to point and parameter of witness point on curve 
	// It makes a distance calculator on constructor use this more than once for all Convert calculations
	CurveSpace (const dCurve &crv);
	void Convert (const VPoint &p ,double &distToCrv , double &parOnCrv , VPoint &witnessPoint , double tol , int maxIterCount);
	int GetRefinePointCount ()const;
	void SetRefinePointCount (int refinePointCountInOneDirection);
private:
	const dCurve &m_crv;
	SequentialLineSetPointMinDist m_distCalc;
	int m_refinePointCount;
	void RefineSolutionAround(const VPoint &p , double &refinedPar , VPoint &refinedMod , double &dprev , double &mindist);
	void RefineSolutionNewtonRaphson (const VPoint &p , double &refinedPar , VPoint &refinedMod,double errortol , int maxiter , double &curerror);
	virtual double GetY(double x);
	virtual double GetDerv (double x);
	// Parameters used during NewtonRaphson refining
	//int m_funcXYZ;// 0 means X , 1 means Y , 2 means Z
	VPoint m_P;		// current position
	bool m_isCrvClosed;
};

#endif