// DeBoorEvaluation.h

#ifndef __DeBoorEvaluation_h__
#define __DeBoorEvaluation_h__
#include "BasicDefinitions.h"

class BSplineCurve;
#include "VPoint.h"

class DeBoorEvaluation
{
public:
	DeBoorEvaluation (const BSplineCurve &crv);
	virtual VPoint GetPoint(const vreal &v) = 0;
	virtual void GetPointAndTangent (const vreal &v
									,VPoint &p
									,VVector &tng) =0;
	virtual sVector<double,4> GetPointRational (const vreal &v) = 0;
	virtual void GetPointAndTangentRational (const vreal &v
											, sVector<double,4> &p
											, sVector<double,4> &tng)=0;
protected:
	const BSplineCurve &m_crv;
	int m_icount;
	vreal m_vstep;
	std::vector<int> m_I;
	std::vector<vreal> m_denum;
	vreal m_v0,m_v1,m_dv;

	void ConvertParam01ToKnotSpace (const vreal &v 
									, int &i_no
									, vreal &cv);
	void ConvertParamKnotSpaceTo01 (const vreal &t
									, vreal &ct);
	void MakeI();
};

#endif