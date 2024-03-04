// LineSetToBSplineCurve.cpp

#include "stdafx.h"
#include "LineSetToBSplineCurve.h"
#include "SequentialLineSet2d.h"
#include "SequentialLineSet.h"
#include "LineSet2d.h"
#include "LineSet.h"
#include "BSplineCurve.h"
#include "BSplineCurvePointGenerator.h"

void LineSetToBSplineCurve::Convert (const SequentialLineSet2d &lset , BSplineCurve &res)
{
	res.m_PROP1 = 1;//curve is planar

	if ( (*(lset.GetPoint(0))) == (*(lset.GetPoint(lset.GetPointSize()-1))) ) 
	{
		res.m_PROP2 = 1;//curve is closed
	}
	else
	{
		res.m_PROP2 = 0;//curve is open
	}
	
	res.m_PROP3 = 1;//curve is polynomial
	
	res.m_PROP4 = 0;// curve is non periodic

	res.m_K = lset.GetPointSize() - 1;//n
	res.m_M = 1;//k-1(degree)
	res.m_N = 1 + res.m_K - res.m_M;
	res.m_A = res.m_N + 2*res.m_M;

	//setting knot values
	res.m_T.resize (res.m_A+1);
	res.m_T[0] = (vreal)(0.);
	res.m_T[1] = (vreal)(0.);
	for (int i=2;i<=res.m_K;i++)
	{
		res.m_T[i]=(vreal)(i-1);
	}
	res.m_T[res.m_K+1] = (vreal)(res.m_K);
	res.m_T[res.m_K+2] = (vreal)(res.m_K);

	//setting controlpoints
	res.controlpoints.resize ((res.m_K+1)*3);
	for (int j=0;j<lset.GetPointSize();j++)
	{
		const VPoint2d *currp = lset.GetPoint(j);
		int begini = j*3;
		res.controlpoints[begini]=(vreal)(currp->x());
		res.controlpoints[begini+1]=(vreal)(currp->y());
		res.controlpoints[begini+2]=(vreal)(0.0);//currp->z();
	}
	//setting weights
	res.m_W.resize((res.m_K+1));
	for (int k=0;k< (res.m_K+1); k++)
	{
		res.m_W[k]=(vreal)(1.);
	}

	//setting parameter range
	res.m_V0=(vreal)(0.);
	res.m_V1=(vreal)(lset.GetPointSize()-1);
	
	//setting normal
	res.m_XNORM = 0.0;
	res.m_YNORM = 0.0;
	res.m_ZNORM = 1.0;
}

void LineSetToBSplineCurve::Convert (const vreal &U0,const vreal &U1,
									 const vreal &V0,const vreal &V1,
									 const SequentialLineSet2d &lset , BSplineCurve &res)
{
	res.m_PROP1 = 1;//curve is planar

	if ( (*(lset.GetPoint(0))) == (*(lset.GetPoint(lset.GetPointSize()-1))) ) 
	{
		res.m_PROP2 = 1;//curve is closed
	}
	else
	{
		res.m_PROP2 = 0;//curve is open
	}
	
	res.m_PROP3 = 1;//curve is polynomial
	
	res.m_PROP4 = 0;// curve is non periodic

	res.m_K = lset.GetPointSize() - 1;//n
	res.m_M = 1;//k-1(degree)
	res.m_N = 1 + res.m_K - res.m_M;
	res.m_A = res.m_N + 2*res.m_M;

	//setting knot values
	res.m_T.resize (res.m_A+1);
	res.m_T[0] = (vreal)(0.);
	res.m_T[1] = (vreal)(0.);
	for (int i=2;i<=res.m_K;i++)
	{
		res.m_T[i]=(vreal)(i-1);
	}
	res.m_T[res.m_K+1] = (vreal)(res.m_K);
	res.m_T[res.m_K+2] = (vreal)(res.m_K);

	//setting controlpoints
	res.controlpoints.resize ((res.m_K+1)*3);
	vreal deltaU = U1 - U0;
	vreal deltaV = V1 - V0;
	for (int j=0;j<lset.GetPointSize();j++)
	{
		const VPoint2d *currp = lset.GetPoint(j);
		vreal backParX = (currp->x()*deltaU)+U0;
		vreal backParY = (currp->y()*deltaV)+V0;
		int begini = j*3;
		res.controlpoints[begini]=(vreal)(backParX);
		res.controlpoints[begini+1]=(vreal)(backParY);
		res.controlpoints[begini+2]=(vreal)(0.0);//currp->z();
	}
	//setting weights
	res.m_W.resize((res.m_K+1));
	for (int k=0;k< (res.m_K+1); k++)
	{
		res.m_W[k]=(vreal)(1.);
	}

	//setting parameter range
	res.m_V0=(vreal)(0.);
	res.m_V1=(vreal)(lset.GetPointSize()-1);
	
	//setting normal
	res.m_XNORM = 0.0;
	res.m_YNORM = 0.0;
	res.m_ZNORM = 1.0;
}

void LineSetToBSplineCurve::Convert (const SequentialLineSet &lset , BSplineCurve &res)
{
	res.m_PROP1 = 0;//curve is non planar ??

	if ( (*(lset.GetPoint(0))) == (*(lset.GetPoint(lset.GetPointSize()-1))) ) 
	{
		res.m_PROP2 = 1;//curve is closed
	}
	else
	{
		res.m_PROP2 = 0;//curve is open
	}
	
	res.m_PROP3 = 1;//curve is polynomial
	
	res.m_PROP4 = 0;// curve is non periodic

	res.m_K = lset.GetPointSize()-1;//n
	res.m_M = 1;//k (degree + 1)
	res.m_N = 1 + res.m_K - res.m_M;
	res.m_A = res.m_N + 2*res.m_M;

	//setting knot values
	res.m_T.resize (res.m_A+1);
	res.m_T[0] = (vreal)(0.);
	res.m_T[1] = (vreal)(0.);
	for (int i=2;i<=res.m_K;i++)
	{
		res.m_T[i]=(vreal)(i-1);
	}
	res.m_T[res.m_K+1] = (vreal)(res.m_K);
	res.m_T[res.m_K+2] = (vreal)(res.m_K);

	//setting controlpoints
	res.controlpoints.resize ((res.m_K+1)*3);
	for (int j=0;j<lset.GetPointSize();j++)
	{
		const VPoint *currp = lset.GetPoint(j);
		int begini = j*3;
		res.controlpoints[begini]=(vreal)(currp->x());
		res.controlpoints[begini+1]=(vreal)(currp->y());
		res.controlpoints[begini+2]=(vreal)(currp->z());
	}

	//setting weights
	res.m_W.resize((res.m_K+1));
	for (int k=0;k< (res.m_K+1); k++)
	{
		res.m_W[k]=(vreal)(1.);
	}

	//setting parameter range
	res.m_V0=(vreal)(0.);
	res.m_V1=(vreal)(lset.GetPointSize()-1);
	
	//setting normal
	res.m_XNORM = 0.0;
	res.m_YNORM = 0.0;
	res.m_ZNORM = 1.0;
}
void LineSetToBSplineCurve::Convert (const LineSet2d &lset , BSplineCurve &res)
{

}
void LineSetToBSplineCurve::Convert (const LineSet &lset , BSplineCurve &res)
{

}
