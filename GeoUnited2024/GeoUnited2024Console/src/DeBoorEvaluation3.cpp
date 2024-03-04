// DeBoorEvaluation3.cpp

#include "stdafx.h"
#include "DeBoorEvaluation3.h"
#include "BSplineCurve.h"
#include "VPoint.h"

DeBoorEvaluation3::DeBoorEvaluation3 (const BSplineCurve &crv)
:DeBoorEvaluation (crv)
//:m_crv (crv)
{
	//m_icount = 0;
	//MakeI();	
	//m_vstep = 1. / m_icount;
	CalculateDenum();
}

VPoint DeBoorEvaluation3::GetPoint(const vreal &v)
{
	int i_no;
	vreal t;
	ConvertParam01ToKnotSpace (v,i_no,t);
	vreal mult[6];
	vreal div[6];
	VPoint P[4];
	GetRelatedMult (i_no , t , mult);
	//mult[0] = t - t1;
	//mult[1] = t - t2;
	//mult[2] = t - t3;
	//mult[3] = t4 - t;
	//mult[4] = t5 - t;
	//mult[5] = t6 - t;

	int begini = i_no * 6;
	div[0] = m_denum[begini];//t4-t3
	div[1] = m_denum[begini+1];//t4-t2
	div[2] = m_denum[begini+2];//t5-t3
	div[3] = m_denum[begini+3];//t4-t1
	div[4] = m_denum[begini+4];//t5-t2
	div[5] = m_denum[begini+5];//t6-t3
	
	GetRelatedControlPoint(i_no,P);

	VPoint P11 = (P[0] * mult[3] + P[1] * mult[0]) / div[3];
	VPoint P12 = (P[1] * mult[4] + P[2] * mult[1]) / div[4];
	VPoint P13 = (P[2] * mult[5] + P[3] * mult[2]) / div[5];
	
	VPoint P22 = (P11 * mult[3] + P12 * mult[1]) / div[1];
	VPoint P32 = (P12 * mult[4] + P13 * mult[2]) / div[2];

	VPoint P33 = (P22 * mult[3] + P32 * mult[2]) / div[0];

	return P33;
}

sVector<double,4> DeBoorEvaluation3::GetPointRational(const vreal &v)
{
	int i_no;
	vreal t;
	ConvertParam01ToKnotSpace (v,i_no,t);
	vreal mult[6];
	vreal div[6];
	sVector<double,4> P[4];
	GetRelatedMult (i_no , t , mult);
	//mult[0] = t - t1;
	//mult[1] = t - t2;
	//mult[2] = t - t3;
	//mult[3] = t4 - t;
	//mult[4] = t5 - t;
	//mult[5] = t6 - t;

	int begini = i_no * 6;
	div[0] = m_denum[begini];//t4-t3
	div[1] = m_denum[begini+1];//t4-t2
	div[2] = m_denum[begini+2];//t5-t3
	div[3] = m_denum[begini+3];//t4-t1
	div[4] = m_denum[begini+4];//t5-t2
	div[5] = m_denum[begini+5];//t6-t3
	
	GetRelatedControlPoint(i_no,P);

	sVector<double,4> P11 = (P[0] * mult[3] + P[1] * mult[0]) / div[3];
	sVector<double,4> P12 = (P[1] * mult[4] + P[2] * mult[1]) / div[4];
	sVector<double,4> P13 = (P[2] * mult[5] + P[3] * mult[2]) / div[5];
	
	sVector<double,4> P22 = (P11 * mult[3] + P12 * mult[1]) / div[1];
	sVector<double,4> P32 = (P12 * mult[4] + P13 * mult[2]) / div[2];

	sVector<double,4> P33 = (P22 * mult[3] + P32 * mult[2]) / div[0];
	vreal myw = P33[3];
	P33 /= myw;
	P33.SetElement (myw,3);
	return P33;
}

void DeBoorEvaluation3::GetPointAndTangent (const vreal &v
											  , VPoint &p
											  ,VVector &tng)
{
	int i_no;
	vreal t;
	ConvertParam01ToKnotSpace (v,i_no,t);
	vreal mult[6];
	vreal div[6];
	VPoint P[4];
	GetRelatedMult (i_no , t , mult);
	//mult[0] = t - t1;
	//mult[1] = t - t2;
	//mult[2] = t - t3;
	//mult[3] = t4 - t;
	//mult[4] = t5 - t;
	//mult[5] = t6 - t;

	int begini = i_no * 6;
	div[0] = m_denum[begini];//t4-t3
	div[1] = m_denum[begini+1];//t4-t2
	div[2] = m_denum[begini+2];//t5-t3
	div[3] = m_denum[begini+3];//t4-t1
	div[4] = m_denum[begini+4];//t5-t2
	div[5] = m_denum[begini+5];//t6-t3
	
	GetRelatedControlPoint(i_no,P);

	VPoint P11 = (P[0] * mult[3] + P[1] * mult[0]) / div[3];
	VPoint P12 = (P[1] * mult[4] + P[2] * mult[1]) / div[4];
	VPoint P13 = (P[2] * mult[5] + P[3] * mult[2]) / div[5];
	
	VPoint P22 = (P11 * mult[3] + P12 * mult[1]) / div[1];
	VPoint P32 = (P12 * mult[4] + P13 * mult[2]) / div[2];

	p = (P22 * mult[3] + P32 * mult[2]) / div[0];
	tng = P32 - P22;
}

void DeBoorEvaluation3::GetPointAndTangentRational (const vreal &v
													 , sVector<double,4> &p
													 , sVector<double,4> &tng)
{
	int i_no;
	vreal t;
	ConvertParam01ToKnotSpace (v,i_no,t);
	vreal mult[6];
	vreal div[6];
	sVector<double,4> P[4];
	GetRelatedMult (i_no , t , mult);
	//mult[0] = t - t1;
	//mult[1] = t - t2;
	//mult[2] = t - t3;
	//mult[3] = t4 - t;
	//mult[4] = t5 - t;
	//mult[5] = t6 - t;

	int begini = i_no * 6;
	div[0] = m_denum[begini];//t4-t3
	div[1] = m_denum[begini+1];//t4-t2
	div[2] = m_denum[begini+2];//t5-t3
	div[3] = m_denum[begini+3];//t4-t1
	div[4] = m_denum[begini+4];//t5-t2
	div[5] = m_denum[begini+5];//t6-t3
	
	GetRelatedControlPoint(i_no,P);

	sVector<double,4> P11 = (P[0] * mult[3] + P[1] * mult[0]) / div[3];
	sVector<double,4> P12 = (P[1] * mult[4] + P[2] * mult[1]) / div[4];
	sVector<double,4> P13 = (P[2] * mult[5] + P[3] * mult[2]) / div[5];
	
	sVector<double,4> P22 = (P11 * mult[3] + P12 * mult[1]) / div[1];
	sVector<double,4> P32 = (P12 * mult[4] + P13 * mult[2]) / div[2];

	p = (P22 * mult[3] + P32 * mult[2]) / div[0];
	vreal myw1 = p[3];
	p /= myw1;
	p.SetElement (myw1,3);

	tng = P32 - P22;
	vreal myw2 = P32[3];
	tng/=myw2;
	tng.SetElement(myw2,3);

}

//void DeBoorEvaluation3::MakeI()
//{
//	int size1 = m_crv.m_T.size();
//	vreal prevt = m_crv.m_T[0];
//	for (int i=1;i<size1;i++)
//	{
//		vreal currt = m_crv.m_T[i];
//		if (fabs (currt - prevt) > EPSILON)
//		{
//			// this is an interval 
//			m_I.push_back(i-1);
//			m_I.push_back(i);		
//			++m_icount;
//		}
//		prevt = currt;
//	}
//	int i0 = m_I[0];
//	int lastParamIndex = (m_icount-1) * 2 + 1;
//	int i1 = m_I[lastParamIndex];
//	m_v0 = m_crv.m_T[i0];
//	m_v1 = m_crv.m_T[i1];
//	m_dv = m_v1 - m_v0;
//}

//void DeBoorEvaluation3::ConvertParam01ToKnotSpace  (const vreal &v 
//													, int &i_no
//													, vreal &cv)
//{
//	cv = v * m_dv + m_v0; 
//	int mysize = (int)(m_I.size());
//	for (int i=0;i<mysize;i++)
//	{
//		int begini = i*2;
//		int i0 = m_I[begini];
//		int i1 = m_I[begini+1];
//		const vreal &t0 = m_crv.m_T[i0];
//		const vreal &t1 = m_crv.m_T[i1];
//		if(cv >= t0 && cv <= t1)
//		{
//			i_no = i;
//			break;
//		}
//	}
//}

//void DeBoorEvaluation3::ConvertParamKnotSpaceTo01 (const vreal &t
//													, vreal &ct)
//{
//	ct = (t - m_v0) / m_dv;
//}

void DeBoorEvaluation3::CalculateDenum()
{
	for (int i=0;i<m_icount;i++)
	{
		int begini = i*2;
		int i3 = m_I[begini];
		int i4 = m_I[begini+1];
		int i1 = i3 - 2;
		int i2 = i3 - 1;
		int i5 = i3 + 2;
		int i6 = i3 + 3;		
		vreal t3 = m_crv.m_T[i3];
		vreal t4 = m_crv.m_T[i4];
		vreal t1 = m_crv.m_T[i1];
		vreal t2 = m_crv.m_T[i2];
		vreal t5 = m_crv.m_T[i5];
		vreal t6 = m_crv.m_T[i6];
		vreal d1 = t4 - t3;
		vreal d2 = t4 - t2;
		vreal d3 = t5 - t3;
		vreal d4 = t4 - t1;
		vreal d5 = t5 - t2;
		vreal d6 = t6 - t3;
		m_denum.push_back(d1);
		m_denum.push_back(d2);
		m_denum.push_back(d3);
		m_denum.push_back(d4);
		m_denum.push_back(d5);
		m_denum.push_back(d6);
	}
}

void DeBoorEvaluation3::GetRelatedControlPoint (int i
												, VPoint p[4])
{
	int begini = i * 2;
	int i0 = m_I[begini];
	
	int ic1 = i0-3;
	int begini1 = ic1 * 3;
	vreal xc1 = m_crv.controlpoints[begini1];
	vreal yc1 = m_crv.controlpoints[begini1+1];
	vreal zc1 = m_crv.controlpoints[begini1+2];
	p[0].x(xc1);
	p[0].y(yc1);
	p[0].z(zc1);

	int ic2 = i0-2;
	int begini2 = ic2 * 3;
	vreal xc2 = m_crv.controlpoints[begini2];
	vreal yc2 = m_crv.controlpoints[begini2+1];
	vreal zc2 = m_crv.controlpoints[begini2+2];
	p[1].x(xc2);
	p[1].y(yc2);
	p[1].z(zc2);

	int ic3 = i0-1;
	int begini3 = ic3 * 3;
	vreal xc3 = m_crv.controlpoints[begini3];
	vreal yc3 = m_crv.controlpoints[begini3+1];
	vreal zc3 = m_crv.controlpoints[begini3+2];
	p[2].x(xc3);
	p[2].y(yc3);
	p[2].z(zc3);
	
	int ic4 = i0;
	int begini4 = ic4 * 3;
	vreal xc4 = m_crv.controlpoints[begini4];
	vreal yc4 = m_crv.controlpoints[begini4+1];
	vreal zc4 = m_crv.controlpoints[begini4+2];
	p[3].x(xc4);
	p[3].y(yc4);
	p[3].z(zc4);

}

void DeBoorEvaluation3::GetRelatedControlPoint (int i
												, sVector<double,4> p[4])
{
	int begini = i * 2;
	int i0 = m_I[begini];
	
	int ic1 = i0-3;
	int begini1 = ic1 * 3;
	vreal xc1 = m_crv.controlpoints[begini1];
	vreal yc1 = m_crv.controlpoints[begini1+1];
	vreal zc1 = m_crv.controlpoints[begini1+2];
	vreal w1 = m_crv.m_W[ic1];

	p[0].SetElement(xc1*w1,0);
	p[0].SetElement(yc1*w1,1);
	p[0].SetElement(zc1*w1,2);
	p[0].SetElement(w1,3);

	int ic2 = i0-2;
	int begini2 = ic2 * 3;
	vreal xc2 = m_crv.controlpoints[begini2];
	vreal yc2 = m_crv.controlpoints[begini2+1];
	vreal zc2 = m_crv.controlpoints[begini2+2];
	vreal w2 = m_crv.m_W[ic2];

	p[1].SetElement(xc2*w2,0);
	p[1].SetElement(yc2*w2,1);
	p[1].SetElement(zc2*w2,2);
	p[1].SetElement(w2,3);

	int ic3 = i0-1;
	int begini3 = ic3 * 3;
	vreal xc3 = m_crv.controlpoints[begini3];
	vreal yc3 = m_crv.controlpoints[begini3+1];
	vreal zc3 = m_crv.controlpoints[begini3+2];
	vreal w3 = m_crv.m_W[ic3];

	p[2].SetElement(xc3*w3,0);
	p[2].SetElement(yc3*w3,1);
	p[2].SetElement(zc3*w3,2);
	p[2].SetElement(w3,3);

	int ic4 = i0;
	int begini4 = ic4 * 3;
	vreal xc4 = m_crv.controlpoints[begini4];
	vreal yc4 = m_crv.controlpoints[begini4+1];
	vreal zc4 = m_crv.controlpoints[begini4+2];
	vreal w4 = m_crv.m_W[ic4];
	
	p[3].SetElement(xc4*w4,0);
	p[3].SetElement(yc4*w4,1);
	p[3].SetElement(zc4*w4,2);
	p[3].SetElement(w4,3);
}

void DeBoorEvaluation3::GetRelatedMult (int i
										, const vreal &t
										, vreal m[6])
{
	int begini = i * 2;
	int i0 = m_I[begini];
	
	const vreal &t3 = m_crv.m_T[i0];
	const vreal &t4 = m_crv.m_T[i0+1];
	const vreal &t5 = m_crv.m_T[i0+2];
	const vreal &t6 = m_crv.m_T[i0+3];
	const vreal &t2 = m_crv.m_T[i0-1];
	const vreal &t1 = m_crv.m_T[i0-2];

	m[0] = t - t1;
	m[1] = t - t2;
	m[2] = t - t3;
	m[3] = t4 - t;
	m[4] = t5 - t;
	m[5] = t6 - t;

}
