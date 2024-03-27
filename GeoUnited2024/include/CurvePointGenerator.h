// CurvePointGenerator.h

#ifndef __CurvePointGenerator_h__
#define __CurvePointGenerator_h__

#include "TransformationMatrix.h"
#include "VPoint.h"
#include <map>
struct DoubleVec;
class SequentialLineSet;
class PointSet;
class IgesEntity;
class IgesEntitySet;
class LineSet;
struct VLine;
class SequentialLineSet2d;
class VArc;
class UntrimmedSurface;

class CurvePointGenerator
{
public:
	CurvePointGenerator(IgesEntity *ent ,  IgesEntitySet *allent);
	virtual ~CurvePointGenerator();
	virtual VPoint GetPoint ( const vreal & v ) const = 0;
	virtual void GetPointAndTangent ( const vreal &v , VPoint &p , VVector &tng)const = 0;
	virtual VPoint GetTangentDerivative (const vreal &v)const = 0;
	virtual vreal GetRadiusOfCurvature (const vreal &v)const =0;
	virtual VPoint GetPrincipalNormal (const vreal &v)const{return 0.;};//to be implemented
	virtual VPoint GetBiNormal (const vreal &v)const {return 0.;};//to be implemented
	virtual void GetDefIntervals ( DoubleVec & defint ) const =0;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const =0;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const =0;
	//void Digitise (const vreal &tol 
	//				, const vreal & U0 , const vreal & U1 
	//				, const vreal & V0 , const vreal & V1
	//				, SequentialLineSet2d &result
	//				, DoubleVec &params)const;
	virtual void Digitise (const vreal tol
					, const UntrimmedSurface &surf
					, SequentialLineSet2d &result
					, DoubleVec &params)const;
	virtual void GetSnapPoints (PointSet &snapp)const =0;
	virtual void GetSnapLines (LineSet &snapl)const =0;
	void SetReversed (bool isReversed);
	bool GetReversed ()const;
	void IncrementTransform (const TransformationMatrix &tm);
	void ResetTransform (const TransformationMatrix &tm);
	const TransformationMatrix & GetTransformation()const;
	void ClearTransformation();
	bool IsTransformed()const;
	VPoint GetDervApr(const vreal &v,const VPoint &p)const;
	VPoint GetDervDervApr (const vreal &v,const VPoint &p,const VPoint &derv)const;
	vreal GetCurvatureApr (const vreal &v,VPoint &derv,VPoint &dervderv)const;
	vreal GetRadiusOfCurvatureApr (const vreal &v , VPoint &derv, VPoint &dervderv)const;
	void SetTransformationMatrixBase ();
	IgesEntity * GetEnt();
	IgesEntitySet * GetEntSet();
	void GenericDigitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const;
	void GenericDigitiseImp(const vreal &tol , SequentialLineSet &result , DoubleVec &params) const;
	void TransformIf (VLine &l)const;
	void TransformIf (VArc &a)const;
	void SetV0V1 (double v0 , double v1);
	double GetV0 ()const;
	double GetV1 ()const;
	vreal ReverseIf (const vreal &v)const;
	VPoint GetTangentDerivativeGeneric (const vreal &v)const;

	vreal GetErrorForLinearazation (const vreal v1 , const vreal v2 , VPoint &p1 , VPoint &p2)const;
	void SubDivideTill (const vreal v1 , const vreal v2 , const vreal tol , std::map<vreal,VPoint> &digitiseResult,int &recdepth)const;
	static int m_subDivideMaxRecursionLimit;
	bool IsSharpCorner (double cosAngleLimit , double v , double parInc , bool isClosedCurve);

protected:
	void TransformIf (VPoint &p)const;
	void TransformIf (VPoint &p , VVector &v)const;
	void ReverseTangentIf (VVector &tng)const;
	VPoint2d RePar (const VPoint &p , const VPoint2d &P0 , const VPoint2d &P1)const;
	bool m_reversed;
	TransformationMatrix m_tm;
	IgesEntity *m_ent;
	IgesEntitySet *m_entset;
	bool m_istransformed;
	double m_v0 , m_v1 , m_deltav;
	int CalculateDivideCountForInterval  (const vreal v1 
										, const vreal v2
										, const vreal tol)const;
	void InsertSegments (int segNo , const vreal &p1 , const vreal &p2,DoubleVec &param2)const;
	double TransformPar (double v)const;
public:
	const IgesEntity * GetEnt()const;
protected:
	bool m_isTransformedBase;
	TransformationMatrix m_tmBase;
public:
	bool GetIsTransformedBase()const;
	const TransformationMatrix & GetTmBase()const;
	static int m_divideTillRecDepth;
	void TransformTanVecIf(VVector& tan)const;
};

#endif