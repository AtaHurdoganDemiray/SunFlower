// ArcPointGenerator.h

#ifndef __ArcPointGenerator_h__
#define __ArcPointGenerator_h__

#include "CurvePointGenerator.h"
#include "VArc.h"


struct VPoint;
struct DoubleVec;
struct CoordinateSystem;
class igesArc;
class IgesEntitySet;
class VArc2d;
class SequentialLineSet2d;

class ArcPointGenerator:public CurvePointGenerator
{
public:
	//ArcPointGenerator ( const VArc & _arc , IgesEntitySet &allent );
	ArcPointGenerator ( igesArc & igesarc , IgesEntitySet & allent );
	//void ResetArc (const VArc &_arc, IgesEntitySet &allent);
	virtual ~ArcPointGenerator ();
	virtual VPoint GetPoint ( const vreal & v ) const;
	virtual void GetPointAndTangent ( const vreal & v , VPoint &p , VVector &tng) const;
	virtual void GetDefIntervals ( DoubleVec & defint ) const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const;
	virtual void GetSnapPoints (PointSet &snapp)const;
	virtual void GetSnapLines (LineSet &snapl)const;
	virtual VPoint GetTangentDerivative (const vreal &v)const;
	virtual vreal GetRadiusOfCurvature (const vreal &v)const;
	VArc arc;
	CoordinateSystem * arccoord;
	int GetDigitiseSegmentNo (const vreal &tol)const;
	static void Digitise (const VArc2d &a , const vreal tol , SequentialLineSet2d &sl);
private:
};

#endif