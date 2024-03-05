// DeBoorEvaluation3.h

#ifndef __DeBoorEvaluation3_h__
#define __DeBoorEvaluation3_h__
#include "DeBoorEvaluation.h"

class DeBoorEvaluation3:public DeBoorEvaluation
{
public:
	DeBoorEvaluation3 (const BSplineCurve &crv);
	VPoint GetPoint(const vreal &v);
	virtual void GetPointAndTangent (const vreal &v
									,VPoint &p
									,VVector &tng);
	virtual sVector<double,4> GetPointRational (const vreal &v);
	virtual void GetPointAndTangentRational (const vreal &v
											, sVector<double,4> &p
											, sVector<double,4> &tng);
private:
	//const BSplineCurve &m_crv;
	//int m_icount;
	//vreal m_vstep;
	//std::vector<int> m_I;
	//std::vector<vreal> m_denum;
	//vreal m_v0,m_v1,m_dv;

	//void ConvertParam01ToKnotSpace (const vreal &v 
	//								, int &i_no
	//								, vreal &cv);
	//void ConvertParamKnotSpaceTo01 (const vreal &t
	//								, vreal &ct);
	void GetRelatedControlPoint (int i
								, VPoint p[4]);
	void GetRelatedControlPoint (int i
								, sVector<double,4> p[4]);

	void GetRelatedMult (int i
						, const vreal &t
						, vreal m[6]);

	//void MakeI();
	void CalculateDenum();
};

#endif