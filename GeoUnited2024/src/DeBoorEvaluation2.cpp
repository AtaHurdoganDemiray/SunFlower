// DeBoorEvaluation2.cpp

#include "stdafx.h"
#include "DeBoorEvaluation2.h"
#include "BSplineCurve.h"
#include "VPoint.h"

DeBoorEvaluation2::DeBoorEvaluation2 (const BSplineCurve &crv)
:DeBoorEvaluation (crv)
{
	CalculateDenum();
}

VPoint DeBoorEvaluation2::GetPoint(const vreal &v)
{
	int i_no;
	vreal t;
	ConvertParam01ToKnotSpace (v,i_no,t);
	vreal mult[4];
	vreal div[3];
	VPoint P[3];
	GetRelatedMult (i_no , t , mult);
	//mult[0] = t5 - t;
	//mult[1] = t - t2;
	//mult[2] = t - t3;
	//mult[3] = t4 - t;

	int begini = i_no * 3;
	div[0] = m_denum[begini];//t4-t3
	div[1] = m_denum[begini+1];//t4-t2
	div[2] = m_denum[begini+2];//t5-t3
	
	GetRelatedControlPoint(i_no,P);

	VPoint P12 = (P[0] * mult[3] + P[1] * mult[1]) / div[1];
	VPoint P13 = (P[1] * mult[0] + P[2] * mult[2]) / div[2];

	VPoint P23 = (P12 * mult[3] + P13 * mult[2]) / div[0];
	return P23;
}
sVector<double,4> DeBoorEvaluation2::GetPointRational(const vreal &v)
{
	int i_no;
	vreal t;
	ConvertParam01ToKnotSpace (v,i_no,t);
	vreal mult[4];
	vreal div[3];
	sVector<double,4> P[3];
	GetRelatedMult (i_no , t , mult);
	//mult[0] = t5 - t;
	//mult[1] = t - t2;
	//mult[2] = t - t3;
	//mult[3] = t4 - t;

	int begini = i_no * 3;
	div[0] = m_denum[begini];//t4-t3
	div[1] = m_denum[begini+1];//t4-t2
	div[2] = m_denum[begini+2];//t5-t3
	
	GetRelatedControlPoint(i_no,P);

	sVector<double,4> P12 = (P[0] * mult[3] + P[1] * mult[1]) / div[1];
	sVector<double,4> P13 = (P[1] * mult[0] + P[2] * mult[2]) / div[2];

	sVector<double,4> P23 = (P12 * mult[3] + P13 * mult[2]) / div[0];
	vreal myw = P23[3];
	P23 /= myw;
	P23.SetElement (myw , 3);
	return P23;
}
void DeBoorEvaluation2::GetPointAndTangent (const vreal &v
											,VPoint &p
											,VVector &tng)
{
	int i_no;
	vreal t;
	ConvertParam01ToKnotSpace (v,i_no,t);
	vreal mult[4];
	vreal div[3];
	VPoint P[3];
	GetRelatedMult (i_no , t , mult);
	//mult[0] = t5 - t;
	//mult[1] = t - t2;
	//mult[2] = t - t3;
	//mult[3] = t4 - t;

	int begini = i_no * 3;
	div[0] = m_denum[begini];//t4-t3
	div[1] = m_denum[begini+1];//t4-t2
	div[2] = m_denum[begini+2];//t5-t3
	
	GetRelatedControlPoint(i_no,P);

	VPoint P12 = (P[0] * mult[3] + P[1] * mult[1]) / div[1];
	VPoint P13 = (P[1] * mult[0] + P[2] * mult[2]) / div[2];

	p = (P12 * mult[3] + P13 * mult[2]) / div[0];
	tng = P13 - P12;
}
void DeBoorEvaluation2::GetPointAndTangentRational (const vreal &v
													,sVector<double,4> &p
													,sVector<double,4> &tng)
{
	int i_no;
	vreal t;
	ConvertParam01ToKnotSpace (v,i_no,t);
	vreal mult[4];
	vreal div[3];
	sVector<double,4> P[3];
	GetRelatedMult (i_no , t , mult);
	//mult[0] = t5 - t;
	//mult[1] = t - t2;
	//mult[2] = t - t3;
	//mult[3] = t4 - t;

	int begini = i_no * 3;
	div[0] = m_denum[begini];//t4-t3
	div[1] = m_denum[begini+1];//t4-t2
	div[2] = m_denum[begini+2];//t5-t3
	
	GetRelatedControlPoint(i_no,P);

	sVector<double,4> P12 = (P[0] * mult[3] + P[1] * mult[1]) / div[1];
	sVector<double,4> P13 = (P[1] * mult[0] + P[2] * mult[2]) / div[2];

	p = (P12 * mult[3] + P13 * mult[2]) / div[0];
	vreal myw1 = p[3];
	p /= myw1;
	p.SetElement(myw1,3);
	tng = P13 - P12;
	vreal myw2 = P13[3];
	tng /= myw2;
	tng.SetElement (myw2,3);
}

void DeBoorEvaluation2::CalculateDenum()
{
	for (int i=0;i<m_icount;i++)
	{
		int begini = i*2;
		int i3 = m_I[begini];
		int i4 = m_I[begini+1];
		int i2 = i3 - 1;
		int i5 = i3 + 2;

		vreal t3 = m_crv.m_T[i3];
		vreal t4 = m_crv.m_T[i4];
		vreal t2 = m_crv.m_T[i2];
		vreal t5 = m_crv.m_T[i5];
	
		vreal d1 = t4 - t3;
		vreal d2 = t4 - t2;
		vreal d3 = t5 - t3;
	
		m_denum.push_back(d1);
		m_denum.push_back(d2);
		m_denum.push_back(d3);
	}
}

void DeBoorEvaluation2::GetRelatedControlPoint (int i
												, VPoint p[3])
{
	int begini = i * 2;
	int i0 = m_I[begini];
	
	int ic1 = i0-2;
	int begini1 = ic1 * 3;
	vreal xc1 = m_crv.controlpoints[begini1];
	vreal yc1 = m_crv.controlpoints[begini1+1];
	vreal zc1 = m_crv.controlpoints[begini1+2];

	p[0].x(xc1);
	p[0].y(yc1);
	p[0].z(zc1);

	int ic2 = i0-1;
	int begini2 = ic2 * 3;
	vreal xc2 = m_crv.controlpoints[begini2];
	vreal yc2 = m_crv.controlpoints[begini2+1];
	vreal zc2 = m_crv.controlpoints[begini2+2];

	p[1].x(xc2);
	p[1].y(yc2);
	p[1].z(zc2);

	int ic3 = i0;
	int begini3 = ic3 * 3;
	vreal xc3 = m_crv.controlpoints[begini3];
	vreal yc3 = m_crv.controlpoints[begini3+1];
	vreal zc3 = m_crv.controlpoints[begini3+2];
	p[2].x(xc3);
	p[2].y(yc3);
	p[2].z(zc3);

}

void DeBoorEvaluation2::GetRelatedControlPoint (int i
												, sVector<double,4> p[3])
{
	int begini = i * 2;
	int i0 = m_I[begini];
	
	int ic1 = i0-2;
	int begini1 = ic1 * 3;
	vreal xc1 = m_crv.controlpoints[begini1];
	vreal yc1 = m_crv.controlpoints[begini1+1];
	vreal zc1 = m_crv.controlpoints[begini1+2];
	vreal w1 = m_crv.m_W[ic1];

	p[0].SetElement(xc1*w1,0);
	p[0].SetElement(yc1*w1,1);
	p[0].SetElement(zc1*w1,2);
	p[0].SetElement(w1,3);

	int ic2 = i0-1;
	int begini2 = ic2 * 3;
	vreal xc2 = m_crv.controlpoints[begini2];
	vreal yc2 = m_crv.controlpoints[begini2+1];
	vreal zc2 = m_crv.controlpoints[begini2+2];
	vreal w2 = m_crv.m_W[ic2];

	p[1].SetElement(xc2*w2,0);
	p[1].SetElement(yc2*w2,1);
	p[1].SetElement(zc2*w2,2);
	p[1].SetElement(w2,3);

	int ic3 = i0;
	int begini3 = ic3 * 3;
	vreal xc3 = m_crv.controlpoints[begini3];
	vreal yc3 = m_crv.controlpoints[begini3+1];
	vreal zc3 = m_crv.controlpoints[begini3+2];
	vreal w3 = m_crv.m_W[ic3];
	p[2].SetElement(xc3*w3,0);
	p[2].SetElement(yc3*w3,1);
	p[2].SetElement(zc3*w3,2);
	p[2].SetElement(w3,3);

}

void DeBoorEvaluation2::GetRelatedMult (int i
										, const vreal &t
										, vreal m[6])
{
	int begini = i * 2;
	int i0 = m_I[begini];
	
	const vreal &t3 = m_crv.m_T[i0];
	const vreal &t4 = m_crv.m_T[i0+1];
	const vreal &t5 = m_crv.m_T[i0+2];
	const vreal &t2 = m_crv.m_T[i0-1];

	m[0] = t5 - t;
	m[1] = t - t2;
	m[2] = t - t3;
	m[3] = t4 - t;
}
