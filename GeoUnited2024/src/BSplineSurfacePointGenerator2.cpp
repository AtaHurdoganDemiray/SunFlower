// BSplineSurfacePointGenerator2.cpp


#include "stdafx.h"
#include "BSplineSurfacePointGenerator2.h"
#include "BSplineCurve.h"
#include "BSplineCurvePointGenerator.h"
#include "BSplineSurface.h"
#include "DoubleVec.h"
#include "PointSet2d.h"

#if !USENN

BSplineSurfacePointGenerator2::BSplineSurfacePointGenerator2(const BSplineSurface &surf)
:BSplineSurfacePointGenerator (surf)
{

}


BSplineSurfacePointGenerator2::~BSplineSurfacePointGenerator2()
{
	std::vector<BSplineCurve *>::iterator i = m_preCurveV.begin();
	std::vector<BSplineCurve *>::iterator e = m_preCurveV.end();
	std::vector<BSplineCurvePointGenerator *>::iterator gi = m_preGenV.begin();
	std::vector<BSplineCurvePointGenerator *>::iterator ge = m_preGenV.end();	
	for (i;i!=e;i++)
		delete (*i);
	for (gi;gi!=ge;gi++)
		delete (*gi);

	std::vector<BSplineCurve *>::iterator i2 = m_curveU.begin();
	std::vector<BSplineCurve *>::iterator e2 = m_curveU.end();
	std::vector<BSplineCurvePointGenerator *>::iterator gi2 = m_genU.begin();
	std::vector<BSplineCurvePointGenerator *>::iterator ge2 = m_genU.end();	
	for (i2;i2!=e2;i2++)
		delete (*i2);
	for (gi2;gi2!=ge2;gi2++)
		delete (*gi2);
}

BSplineCurve * BSplineSurfacePointGenerator2::MakeUCurve (const vreal &u)
{
	std::vector<vreal> cp;
	std::vector<vreal> wp;

	int uno = m_surf.GetControlPointNoU();
	for (int i=0; i < uno; i++)
	{
		BSplineCurvePointGenerator *currgen = m_preGenV[i];
		
		if (currgen->IsRational() == false)
		{
			VPoint currp = currgen->GetPoint(u);
			cp.push_back(currp.x());
			cp.push_back(currp.y());
			cp.push_back(currp.z());
			wp.push_back (1.);
		}
		else
		{
			sVector<double,4> currp = currgen->GetPointRational(u);
			cp.push_back (currp[0]);
			cp.push_back (currp[1]);
			cp.push_back (currp[2]);
			wp.push_back (currp[3]);
		}
	}
	bool isRational , isPeriodic;
	if (m_surf.m_PROP3 == 0)
		isRational = true;
	else
		isRational = false;

	if (m_surf.m_PROP4 == 0)
		isPeriodic = false;
	else
		isPeriodic = true;

	BSplineCurve *newcurve = new BSplineCurve (m_surf.GetDegreeOfBasisFunctionsU()
										,m_surf.GetControlPointNoU()
										,isRational 
										,isPeriodic
										,m_surf.m_S
										,wp
										,cp
										,m_surf.m_U0
										,m_surf.m_U1);
	return newcurve;
}

BSplineCurve * BSplineSurfacePointGenerator2::MakeVCurve (const vreal &u)
{
	std::vector<vreal> cp;
	std::vector<vreal> wp;

	int vno = m_surf.GetControlPointNoV();
	for (int i=0; i < vno; i++)
	{
		BSplineCurvePointGenerator *currgen = m_preGenU[i];
		if (currgen->IsRational() == false)
		{
			VPoint currp = currgen->GetPoint(u);
			cp.push_back(currp.x());
			cp.push_back(currp.y());
			cp.push_back(currp.z());
			wp.push_back (1.);
		}
		else
		{
			sVector<double,4> currp = currgen->GetPointRational(u);
			cp.push_back (currp[0]);
			cp.push_back (currp[1]);
			cp.push_back (currp[2]);
			wp.push_back (currp[3]);
		}
	}
	bool isRational , isPeriodic;
	if (m_surf.m_PROP3 == 0)
		isRational = true;
	else
		isRational = false;

	if (m_surf.m_PROP5 == 0)
		isPeriodic = false;
	else
		isPeriodic = true;

	BSplineCurve *newcurve = new BSplineCurve (m_surf.GetDegreeOfBasisFunctionsV()
										,m_surf.GetControlPointNoV()
										,isRational 
										,isPeriodic
										,m_surf.m_T
										,wp
										,cp
										,m_surf.m_V0
										,m_surf.m_V1);
	return newcurve;
}

//void BSplineSurfacePointGenerator2::DivideSpans (const DoubleVec &spans , DoubleVec &divided , int divideNo)
//{
//	int mysize = spans.Size() - 1;
//	for (int i = 0; i<mysize; i++)
//	{
//		const vreal &v0 = spans[i];
//		const vreal &v1 = spans[i+1];
//		vreal delt = (v1 - v0) / divideNo;
//		for (int j=0; j<divideNo; j++)
//		{
//			vreal currp = v0 + j * delt;
//			divided.Addvreal (currp);
//		}
//		if (i == mysize-1)
//			divided.Addvreal (v1);
//	}
//}

void BSplineSurfacePointGenerator2::NetDivide (int uDivideNo , int vDivideNo , PointSet &ps , PointSet &vs , PointSet2d &pars)
{
	if (m_surf.IsRational() || m_surf.IsPeriodicInU() || m_surf.IsPeriodicInV())
	{
		DivideByOldWay (uDivideNo,vDivideNo,ps,vs,pars);
		return;
	}

	PreCalculateU (vDivideNo);
	PreCalculateV (uDivideNo);
	
	int size1 = m_vParam.Size();

	for (int i = 0; i<size1; i++)
	{
		BSplineCurvePointGenerator *uGen = m_genU[i];
		vreal currv = m_vParam[i];

		int size2 = m_uParam.Size();
		for (int j=0; j<size2; j++)
		{
			const vreal &curru = m_uParam[j];
			BSplineCurvePointGenerator *vGen = m_genV[j];

			VPoint myp1,mytng1;
			uGen->GetPointAndTangent (curru,myp1,mytng1);
			//ps.InsertPoint (myp1);
			VPoint2d mypar (curru , currv);
			pars.InsertPoint (mypar);
			// Calculating Normal
			VPoint myp2,mytng2;
			vGen->GetPointAndTangent(currv,myp2,mytng2);
			ps.InsertPoint (myp2);
			
			VVector n = mytng1 ^ mytng2;
			n.NormThis();
			vs.InsertPoint (n);
		}
	}
}

void BSplineSurfacePointGenerator2::PreCalculateU (int vDivideNo)
{
	int uno = m_surf.GetControlPointNoU();
	int vno = m_surf.GetControlPointNoV();

	for (int i=0; i<uno; i++)
	{
		std::vector<vreal> currcp;
		std::vector<vreal> currw;
		for (int j=0; j<vno; j++)
		{
			const VPoint &currp = controlpoints.GetPoint(i,j);
			const vreal &myw = weights.GetWeight(i,j);
			currcp.push_back(currp.x());
			currcp.push_back(currp.y());
			currcp.push_back(currp.z());
			currw.push_back (myw);
		}
		bool isRational , isPeriodic;
		if (m_surf.m_PROP3 == 0)
			isRational = true;
		else 
			isRational = false;

		if (m_surf.m_PROP5 == 0)
			isPeriodic = false;
		else
			isPeriodic = true;

		BSplineCurve *newcurve = new BSplineCurve (m_surf.GetDegreeOfBasisFunctionsV() 
								, m_surf.GetControlPointNoV()
								, isRational
								, isPeriodic
								, m_surf.m_T
								, currw
								, currcp
								, m_surf.m_V0
								, m_surf.m_V1);
		m_preCurveV.push_back (newcurve);
		BSplineCurvePointGenerator *newgen = new BSplineCurvePointGenerator (*newcurve);
		m_preGenV.push_back (newgen);
	}

	DoubleVec vdef,vdef2;
	GetVDefIntervals (vdef);
	int newVDivideNo = m_surf.GetDegreeOfBasisFunctionsV();
	if (newVDivideNo != 1)
		newVDivideNo*=2;
	DivideSpans (vdef , vdef2 , newVDivideNo);
	int mysize = vdef2.Size();
	for (int w=0;w<mysize;w++)
	{
		vreal currv = vdef2[w];
		m_vParam.Addvreal (currv);
		BSplineCurve *crv = MakeUCurve (currv);
		m_curveU.push_back (crv);		
		BSplineCurvePointGenerator * gen = new BSplineCurvePointGenerator (*crv);
		m_genU.push_back (gen);
	}
}

void BSplineSurfacePointGenerator2::PreCalculateV (int uDivideNo)
{
	int uno = m_surf.GetControlPointNoU();
	int vno = m_surf.GetControlPointNoV();

	for (int i=0; i<vno; i++)
	{
		std::vector<vreal> currcp;
		std::vector<vreal> currw;

		for (int j=0; j<uno; j++)
		{
			const VPoint &currp = controlpoints.GetPoint(j,i);
			currcp.push_back(currp.x());
			currcp.push_back(currp.y());
			currcp.push_back(currp.z());
			const vreal &myw = weights.GetWeight (j,i);
			currw.push_back (myw);
		}
		bool isRational , isPeriodic;
		if (m_surf.m_PROP3 == 0)
			isRational = true;
		else 
			isRational = false;

		if (m_surf.m_PROP5 == 0)
			isPeriodic = false;
		else
			isPeriodic = true;

		BSplineCurve *newcurve = new BSplineCurve (m_surf.GetDegreeOfBasisFunctionsU() 
								, m_surf.GetControlPointNoU()
								, isRational
								, isPeriodic
								, m_surf.m_S
								, currw
								, currcp
								, m_surf.m_U0
								, m_surf.m_U1);
		m_preCurveU.push_back (newcurve);
		BSplineCurvePointGenerator *newgen = new BSplineCurvePointGenerator (*newcurve);
		m_preGenU.push_back (newgen);
	}

	DoubleVec udef,udef2;
	GetUDefIntervals (udef);
	int newUDivideNo = m_surf.GetDegreeOfBasisFunctionsU();
	if (newUDivideNo != 1)
		newUDivideNo*=2;
	DivideSpans (udef , udef2 , newUDivideNo);
	int mysize = udef2.Size();
	for (int w=0;w<mysize;w++)
	{
		vreal curru = udef2[w];
		m_uParam.Addvreal (curru);
		BSplineCurve *crv = MakeVCurve (curru);
		m_curveV.push_back (crv);
		BSplineCurvePointGenerator * gen = new BSplineCurvePointGenerator (*crv);
		m_genV.push_back (gen);
	}
}

void BSplineSurfacePointGenerator2::DivideByOldWay (int uDivideNo , int vDivideNo , PointSet &ps , PointSet &vs , PointSet2d &pars)
{
	DoubleVec udef,udef2;
	GetVDefIntervals (udef);
	DivideSpans (udef , udef2 , uDivideNo);
	int usize = udef2.Size();

	DoubleVec vdef,vdef2;
	GetVDefIntervals (vdef);
	DivideSpans (vdef , vdef2 , vDivideNo);
	int vsize = vdef2.Size();

	for (int i=0;i<usize;i++)
	{
		vreal curru = udef2[i];
		for (int j=0;j<vsize;j++)
		{
			vreal currv = vdef2[j];
			VPoint p,n;
			p = GetPoint(curru,currv);
			n = GetNormal (curru,currv,p);
			VPoint2d currpar (curru,currv);
			pars.InsertPoint (currpar);
			ps.InsertPoint (p);
			vs.InsertPoint (n);
		}
	}
}

#endif
