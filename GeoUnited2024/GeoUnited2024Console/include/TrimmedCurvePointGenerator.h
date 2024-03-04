// TrimmedCurvePointGenerator.h

#ifndef __TrimmedCurvePointGenerator_h__
#define __TrimmedCurvePointGenerator_h__

#include "CurvePointGenerator.h"
#include <map>
class dCurve;
class VArc;

class TrimmedCurvePointGenerator : public CurvePointGenerator
{
public:
	TrimmedCurvePointGenerator (IgesEntity *ent , IgesEntitySet *allent , int ei , double tol);
	virtual ~TrimmedCurvePointGenerator();
	virtual VPoint GetPoint ( const vreal & v ) const;
	virtual void GetPointAndTangent ( const vreal &v , VPoint &p , VVector &tng)const;
	virtual VPoint GetTangentDerivative (const vreal &v)const;
	virtual vreal GetRadiusOfCurvature (const vreal &v)const;
	virtual void GetDefIntervals ( DoubleVec & defint ) const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const;
	virtual void GetSnapPoints (PointSet &snapp)const;
	virtual void GetSnapLines (LineSet &snapl)const;

	static void SplitCurveByPoint (dCurve *crv , std::map<double,double> &splitParMap , const VPoint &witnessPoint,double tol,bool isStartPoint);
	bool GetArcIf (VArc &a)const;
	bool GetLineIf (VLine &l)const;
	IgesEntity * GetSrcCurvEnt ();
	double OffsetParForClosed (double voriginal)const;
private:
	dCurve *m_srcCurv;
	void MakeSrcCurve ();
	void DoShallowTransformationOnSrcCurveIf (IgesEntity *src);
	void CalculateStartAndTerminateParam ();
	VPoint m_SP , m_TP;
	int m_ei;	// edge no in the edge list - iges entity 
	double m_tol;
	bool m_isClosed;
	double m_v0offset;
public:
	bool IsLine()const;
	bool IsArc()const;
	bool IsPolyline()const;
	int GetEi()const;
};
#endif