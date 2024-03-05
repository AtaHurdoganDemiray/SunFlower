// CurveSpace.cpp

#include "stdafx.h"
#include "CurveSpace.h"
#include "VPoint.h"
#include "dCurve.h"
#include "CurvePointGenerator.h"
#include "DoubleVec.h"
#include "NewtonRaphson.h"

CurveSpace::CurveSpace(const dCurve &crv)
:m_crv (crv) , m_distCalc (crv.GetLines())
{
	//VPoint mn = m_crv.GetLines().GetMin();
	//VPoint mx = m_crv.GetLines().GetMax();
	//VPoint deltaVec = mx - mn;
	//if (deltaVec.x() > deltaVec.y() && deltaVec.x() > deltaVec.z())
	//	m_funcXYZ = 0;
	//else if (deltaVec.y() > deltaVec.x() && deltaVec.y() > deltaVec.z())
	//	m_funcXYZ = 1;
	//else
	//	m_funcXYZ = 2;
	if (m_crv.GetLines().Start()->Compare(*m_crv.GetLines().End(),1.e-8))
		m_isCrvClosed = true;
	else
		m_isCrvClosed = false;
}

void CurveSpace::Convert(const VPoint &p, double &distToCrv, double &parOnCrv , VPoint &witnessPoint , double tol , int maxIterCount)
{
	// Find the nearest point (witness point) on curve for point p
	int seti;
	distToCrv = m_distCalc.MinDist(p,witnessPoint,&seti);
	
	// find the parameter for witness point 
	int pi0 = seti;
	int pi1 = pi0 + 1;
	const DoubleVec &parvec = m_crv.GetParams();
	if (parvec.Size() != m_crv.GetLines().GetPointSize())
		throw VException (( IDS_CurConExcDigLin),1);
	double v0 = parvec.operator [](pi0);
	double v1 = parvec.operator [](pi1);
	VLine curl = m_crv.GetLines().operator [](seti);
	double t = curl.GetT(witnessPoint);
	double apprV = v0 + (v1 - v0)*t;
	parOnCrv = apprV;

	// refine the solution around witness point for curve
	//double dv = 0.1;
	//distToCrv = (distToCrv * distToCrv);
	//VPoint p2 (p);
	//double parOnCrv2(parOnCrv);
	//VPoint witnessPoint2(witnessPoint);
	//double distToCrv2(distToCrv);
	//for (int j=0;j<2;++j)
	//	RefineSolutionAround(p,parOnCrv,witnessPoint,dv,distToCrv);
	//distToCrv = sqrt (distToCrv);

	RefineSolutionNewtonRaphson(p,parOnCrv,witnessPoint,tol,maxIterCount,distToCrv);
	//distToCrv = (witnessPoint-p).GetLength();
}

int CurveSpace::GetRefinePointCount()const
{
	return m_refinePointCount;
}

void CurveSpace::SetRefinePointCount(int refinePointCount)
{
	m_refinePointCount = refinePointCount;
}
void CurveSpace::RefineSolutionAround(const VPoint &p , double &refinedPar , VPoint &refinedMod , double &dprev , double &mindist)
{
	double dnew = (3*dprev) / m_refinePointCount;
	if (dnew < 1.e-14)
		return;
	double vstart = refinedPar-dprev*1.5;
	double vend = refinedPar+dprev*1.5;

	if (vstart<0.0)
		vstart = 0.0;
	else if (vstart > 1.0)
		vstart = 1.0;

	if(vend < 0.0)
		vend = 0.0;
	else if (vend > 1.0)
		vend = 1.0;

	for (double v=vstart;v<vend;v+=dnew)
	{
		VPoint curp = m_crv.GetCurve()->GetPoint(v);
		double curdist = (p-curp).GetLength2();
		if (curdist < mindist)
		{
			refinedPar = v;
			refinedMod = curp;
			mindist = curdist;
		}
	}
	dprev = dnew;
}

double CurveSpace::GetY(double x)
{
	if (x < 0.0)
	{
		if (m_isCrvClosed == false)
			x = 0.0;
		else
		{
			x = 1.0 + x;
		}
	}
	if (x > 1.0)
	{
		if (m_isCrvClosed == false)
			x = 1.0;
		else
		{
			x = x - 1.0;
		}
	}
	VPoint ep = m_crv.GetCurve()->GetPoint(x);
	double res = (ep-m_P).GetLength();
	return res;
	//switch (m_funcXYZ)
	//{
	//case 0:
	//	return (m_P.x() - ep.x());
	//case 1:
	//	return (m_P.y() - ep.y());
	//case 2:
	//	return (m_P.z() - ep.z());
	//}
	//throw VException (("CurveSpace::GetY exception , funcXYZ not set!"),1);
}

double CurveSpace::GetDerv (double x)
{
	double x1 = x + 1.e-5;
	if (x1 > 1.0)
		x1 = x - 1.e-5;
	double y0 = GetY (x);
	double y1 = GetY (x1);
	double derv = (y1 - y0) / (x1 - x);
	return derv;
	//if (x<0.0)
	//	x=0.;
	//if (x> 1.0)
	//	x= 1.;
	//VPoint ep = m_crv.GetCurve()->GetPoint(x);
	//VPoint derv = m_crv.GetCurve()->GetDervApr(x,ep);
	//switch (m_funcXYZ)
	//{
	//case 0:
	//	return -(derv.x());
	//case 1:
	//	return -(derv.y());
	//case 2:
	//	return -(derv.z());
	//}
	//throw VException (("CurveSpace::GetDerv exception , funcXYZ not set!"),1);
}

void CurveSpace::RefineSolutionNewtonRaphson (const VPoint &p , double &refinedPar , VPoint &refinedMod , double errortol , int maxiter , double &curerror)
{
	m_P = p;
	NewtonRaphson s;
	double newPar = refinedPar;
	bool ck = s.GetRoot(refinedPar,errortol,maxiter,*this,newPar);
	refinedPar = newPar;
	refinedMod = m_crv.GetCurve()->GetPoint(refinedPar);
	curerror = GetY(refinedPar);
}
