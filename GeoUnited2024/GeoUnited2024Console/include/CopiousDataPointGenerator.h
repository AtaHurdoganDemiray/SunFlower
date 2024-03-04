// CopiousDataPointGenerator.h

#ifndef __CopiousDataPointGenerator_h__
#define __CopiousDataPointGenerator_h__
#include "CurvePointGenerator.h"
#include "SequentialLineSet.h"
#include "BSplineCurve.h"

class CopiousData;
class BSplineCurvePointGenerator;

class CopiousDataPointGenerator : public CurvePointGenerator
{
public:
	CopiousDataPointGenerator (CopiousData &pdata , IgesEntitySet &allent);
	virtual ~CopiousDataPointGenerator ();
	virtual VPoint GetPoint ( const vreal & v ) const;
	virtual void GetPointAndTangent (const vreal &v , VPoint &p , VVector &tng)const;
	virtual void GetDefIntervals ( DoubleVec & defint ) const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const;
	virtual void GetSnapPoints (PointSet &snapp)const;
	virtual void GetSnapLines (LineSet &snapl)const;
	virtual VPoint GetTangentDerivative (const vreal &v)const;
	virtual vreal GetRadiusOfCurvature (const vreal &v)const;
	bool GetBeforePoints (double v , VPoint &pbeforeb , VPoint &pbefore)const;
	bool GetAfterPoints (double v , VPoint &pafterb , VPoint &pafter)const;
	bool DecrementPar (double v , double inc , double &vbef , double &vbefbef)const;
	bool IncrementPar (double v , double inc , double &vaft , double &vaftbef)const;
private:
	SequentialLineSet m_sp;	
	BSplineCurve m_bs;
	BSplineCurvePointGenerator *m_bsGen;
};

#endif