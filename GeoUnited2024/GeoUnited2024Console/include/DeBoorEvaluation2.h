// DeBoorEvaluation2.h

#ifndef __DeBoorEvaluation2_h__
#define __DeBoorEvaluation2_h__
#include "DeBoorEvaluation.h"
#include "sVector.h"

class DeBoorEvaluation2:public DeBoorEvaluation
{
public:
	DeBoorEvaluation2 (const BSplineCurve &crv);
	virtual VPoint GetPoint(const vreal &v);
	virtual void GetPointAndTangent (const vreal &v
							,VPoint &p
							,VVector &tng);
	virtual sVector<double,4> GetPointRational (const vreal &v);
	virtual void GetPointAndTangentRational (const vreal &v
											, sVector<double,4> &p
											, sVector<double,4> &tng);
private:
	void GetRelatedControlPoint (int i
								, VPoint p[3]);
	void GetRelatedControlPoint (int i
								, sVector<double,4> p[3]);
	void GetRelatedMult (int i
						, const vreal &t
						, vreal m[6]);
	void CalculateDenum();
};

#endif