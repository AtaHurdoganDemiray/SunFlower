/*********************************************************************************
*			File: ParametricSplinePointGenerator.cpp									     *
**********************************************************************************
*			Description:														 *
*																				 *
**********************************************************************************
*			History:															 *
*																				 *
**********************************************************************************
*			(C)...																 *
*********************************************************************************/

#include "stdafx.h"
#include "ParametricSplinePointGenerator.h"
#include "ParametricSpline.h"
#include "VPoint.h"
#include "DoubleVec.h"
#include "SequentialLineSet.h"

ParametricSplinePointGenerator::ParametricSplinePointGenerator(ParametricSpline &a , IgesEntitySet *allent)
:CurvePointGenerator(&a,allent),m_a(a)
{
	
}

//#################################################################################

VPoint ParametricSplinePointGenerator::GetPoint( const vreal &v ) const
{
	vreal vv = ReverseIf(v);
	vv = TransformPar(vv);
	vreal s;
	int i;
	MapParam(vv,s,i);
	const std::vector<vreal> &AX = m_a.m_AX;
	const std::vector<vreal> &BX = m_a.m_BX;
	const std::vector<vreal> &CX = m_a.m_CX;
	const std::vector<vreal> &DX = m_a.m_DX;
	
	const std::vector<vreal> &AY = m_a.m_AY;
	const std::vector<vreal> &BY1 = m_a.m_BY;
	const std::vector<vreal> &CY = m_a.m_CY;
	const std::vector<vreal> &DY = m_a.m_DY;
	
	const std::vector<vreal> &AZ = m_a.m_AZ;
	const std::vector<vreal> &BZ = m_a.m_BZ;
	const std::vector<vreal> &CZ = m_a.m_CZ;
	const std::vector<vreal> &DZ = m_a.m_DZ;

	vreal X, Y, Z;
	X = AX[i] + BX[i]*s + CX[i]*s*s + DX[i]*s*s*s;
	Y = AY[i] + BY1[i]*s + CY[i]*s*s + DY[i]*s*s*s;
	Z = AZ[i] + BZ[i]*s + CZ[i]*s*s + DZ[i]*s*s*s;
	VPoint p(X,Y,Z);
	TransformIf (p);
	return p;
}

//#################################################################################

void ParametricSplinePointGenerator::GetDefIntervals( DoubleVec &defint ) const
{
	vreal delta = m_a.m_T.back() - m_a.m_T.front();
	int mysize = 100;//m_a.m_T.size();
	for (int j=0;j<mysize+1;j++)
	{
		//vreal currT = m_a.m_T[j];
		//vreal mapped = (currT - m_a.m_T[0]) / delta;
		vreal mapped = j * 0.01;
		defint.Addvreal (mapped);
	}
}

//#################################################################################

void ParametricSplinePointGenerator::MapParam( const vreal &v , vreal &s , int &i) const
{
	vreal u;
	vreal T0 = m_a.m_T[0];
	vreal T1 = m_a.m_T.back();
	u = T0 + (T1-T0) * v;
	int mysize = m_a.m_T.size();
	for (int j=0;j<mysize-1;j++)
	{
		vreal currT0 = m_a.m_T[j];
		vreal currT1 = m_a.m_T[j+1];
		if (u>=currT0 && u<=currT1)
		{
			i = j;
			s = (u - currT0) * (currT1 - currT0);
			return;
		}
	}
	throw VException (( IDS_ParMapExc) , 1);
}

void ParametricSplinePointGenerator::GetPointAndTangent ( const vreal &v , VPoint &p , VVector &tng)const
{
	vreal vv = ReverseIf(v);
	vv = TransformPar(vv);
	p = GetPoint (vv);
	//VPoint np = GetPoint (v + 1.e-6);
	//tng = np - p;
	//tng.NormThis();
	tng = GetTangent(vv);
	ReverseTangentIf(tng);
}

const int PARAMETRICSPLINE_SEGMENT_NO =  24;

void ParametricSplinePointGenerator::Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const
{
	return GenericDigitise (tol , result , params );
	//vreal ustep = 1.0 / PARAMETRICSPLINE_SEGMENT_NO;
	//vreal u = 0.;
	//for (int i=0; i < PARAMETRICSPLINE_SEGMENT_NO+1; i++)
	//{
	//	VPoint currp = GetPoint (u);
	//	bool ck = result.InsertPoint (currp);
	//	if (ck)
	//		params.Addvreal (u);
	//	u+= ustep;
	//}
}

void ParametricSplinePointGenerator::Digitise (const vreal &tol 
											   , SequentialLineSet &result 
											   , PointSet &tangents
											   , DoubleVec &params)const
{
	vreal ustep = 1.0 / PARAMETRICSPLINE_SEGMENT_NO;
	vreal u = 0.;
	for (int i=0; i < PARAMETRICSPLINE_SEGMENT_NO+1; i++)
	{
		VPoint currp,currtn;
		GetPointAndTangent (u,currp,currtn);
		bool ck = result.InsertPoint (currp);
		if (ck)
		{
			params.Addvreal (u);					
			tangents.InsertPoint (currtn);
		}
		u+= ustep;
	}
}

void ParametricSplinePointGenerator::GetSnapPoints (PointSet &snapp)const
{
	VPoint startp = GetPoint(0.0);
	snapp.InsertPoint(startp);
	VPoint midp = GetPoint (0.5);
	snapp.InsertPoint(midp);
	VPoint endp = GetPoint (1.0);
	if (startp.Compare (endp , 1.e-6) == false)
		snapp.InsertPoint (endp);
}
void ParametricSplinePointGenerator::GetSnapLines (LineSet &snapl)const
{

}

VPoint ParametricSplinePointGenerator::GetTangentDerivative (const vreal &v)const
{
	//calculating tangent
	vreal s;
	int i;
	MapParam(v,s,i);
	const std::vector<vreal> &CX = m_a.m_CX;
	const std::vector<vreal> &DX = m_a.m_DX;
	
	const std::vector<vreal> &CY = m_a.m_CY;
	const std::vector<vreal> &DY = m_a.m_DY;
	
	const std::vector<vreal> &CZ = m_a.m_CZ;
	const std::vector<vreal> &DZ = m_a.m_DZ;

	vreal ddX, ddY, ddZ;
	ddX = 2.*CX[i] + 6.*DX[i]*s;
	ddY = 2.*CY[i] + 6.*DY[i]*s;
	ddZ = 2.*CZ[i] + 6.*DZ[i]*s;
	VPoint ppp(ddX,ddY,ddZ);
	VPoint tmp;
	TransformIf (tmp,ppp);
	return ppp;
}
	
vreal ParametricSplinePointGenerator::GetRadiusOfCurvature (const vreal &v)const
{
	//VPoint d,dd;
	//d = GetTangent (v);
	//dd = GetTangentDerivative(v);
	//VPoint crs = d ^ dd;
	//double s = d.GetLength();
	//double ro = s*s*s / crs.GetLength();
	//return ro;
	VPoint derv,dervderv;
	return 1. / GetCurvatureApr (v,derv,derv);
}	

VPoint ParametricSplinePointGenerator::GetTangent(const vreal &v)const
{
	//calculating tangent
	vreal s;
	int i;
	MapParam(v,s,i);
	const std::vector<vreal> &BX = m_a.m_BX;
	const std::vector<vreal> &CX = m_a.m_CX;
	const std::vector<vreal> &DX = m_a.m_DX;
	
	const std::vector<vreal> &BY1 = m_a.m_BY;
	const std::vector<vreal> &CY = m_a.m_CY;
	const std::vector<vreal> &DY = m_a.m_DY;
	
	const std::vector<vreal> &BZ = m_a.m_BZ;
	const std::vector<vreal> &CZ = m_a.m_CZ;
	const std::vector<vreal> &DZ = m_a.m_DZ;

	vreal dX, dY, dZ;
	dX = BX[i] + 2.* CX[i]*s + 3.*DX[i]*s*s;
	dY = BY1[i] + 2. * CY[i]*s + 3.*DY[i]*s*s;
	dZ = BZ[i] + 2.*CZ[i]*s + 3.*DZ[i]*s*s;
	VPoint pp(dX,dY,dZ);
	VPoint tmp;
	TransformIf (tmp,pp);
	return pp;
}
