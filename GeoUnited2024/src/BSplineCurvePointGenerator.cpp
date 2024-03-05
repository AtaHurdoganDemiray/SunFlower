// BSplineCurvePointGenerator.cpp

#include "stdafx.h"
#include "BSplineCurvePointGenerator.h"
#include "BSplineCurve.h"
#include "BSplineNFormer.h"
#include "BSplineN.h"

#include "DoubleVec.h"
#include "Matrix.h"
#include "DeBoorEvaluation3.h"
#include "DeBoorEvaluation2.h"
#include "SequentialLineSet.h"
#include "PolarCalc2d.h"

BSplineCurvePointGenerator::BSplineCurvePointGenerator ( BSplineCurve & curve , IgesEntitySet &allent )
:CurvePointGenerator(&curve,&allent)
,m_curve(curve)
#if USENN
,m_NE (curve)
#else
,knots(0)
,controlpoints(0)
,weights(0)
,m_deboor(0)
#endif
{
#if USENN

#else
	knots = new BSplineKnots(curve.GetDegreeOfBasisFunctions()+1 
								, curve.GetControlPointNo()-1 
								, curve.m_V0 
								, curve.m_V1 
								, curve.m_T );
	
	weights = new BSplineWeights (curve.GetControlPointNo() 
										, curve.m_W , curve.IsRational());
	
	int mydegree = curve.GetDegreeOfBasisFunctions();
	
	if (mydegree == 3 
		//&& curve.IsRational() == false 
		&& curve.IsPeriodic()==false)
	{
		m_deboor = new DeBoorEvaluation3(curve);
	}
	else if (mydegree == 2 
		//&& curve.IsRational() == false 
		&& curve.IsPeriodic() == false)
	{
		m_deboor = new DeBoorEvaluation2(curve);
	}
	else
	{
		controlpoints = new BSplineControlPoints (curve.GetControlPointNo()
													, curve.controlpoints );
	}
#endif
}

BSplineCurvePointGenerator::~BSplineCurvePointGenerator()
{
#if !USENN
	delete knots;
	delete controlpoints;
	delete weights;
	delete m_deboor;
#endif
}

VPoint BSplineCurvePointGenerator::GetPoint (const vreal &u)const
{
#if USENN
	vreal uu = ReverseIf (u);
	uu = TransformPar(uu);
	VPoint np =  m_NE.GetPoint (uu);
	TransformIf (np);
	return np;
#else
	if (m_curve.IsRational() == false)
	{
		return GetPointImp(u);
	}
	else
	{
		sVector<double,4> tmpres = GetPointRational (u);
		VPoint res (tmpres[0] , tmpres[1] , tmpres[2]);
		return res;
	}
#endif
}
#if  !USENN
VPoint BSplineCurvePointGenerator::GetPointImp ( const vreal & param ) const
{
	try
	{
		vreal vv = ReverseIf (param);
		if (m_deboor)
		{
			VPoint result = m_deboor->GetPoint(vv);
			TransformIf (result);
			
			
			VPoint derv;
			m_deboor->GetPointAndTangent(vv,result,derv);
			//if (/*m_curve.IsPeriodic() == false &&*/ m_curve.m_K > 1 && m_curve.IsRational())
			//	NewTest (vv,derv);
			
			return result;
		}

	vreal realparam = knots->Reparametrize_Range0_1ToV0_V1 ( vv );
	BSplineNFormer former ( *knots , realparam );
	const BSplineNSet & nset = former.Form();
	vreal deno = 0.0;
	
	VPoint result ( 0.0 , 0.0 , 0.0 );
	for ( int i = 0; i < nset.Size(); i++ )
	{
		const BSplineN &currN = nset[i];
		
		if ( weights->IsRational() == false )
		{
			result += ( controlpoints->GetPoint ( currN.Geti()  ) 
						* /*static_cast<vreal>*/ (currN.GetValue()) );
			//deno += currN.GetValue();		
		}
		else
		{	
			int no = currN.Geti();
			result += ( controlpoints->GetPoint (no) 
						* /*static_cast<vreal>*/ (currN.GetValue()) 					
						* weights->GetWeight (no) );
			
			deno += ( currN.GetValue() * weights->GetWeight (no) );
		}
	}
	if(weights->IsRational() == false)
	{
		deno = 1.0;
	}
	VPoint finalRes = (result/deno);
	
	TransformIf (finalRes);
	
	//if (m_curve.IsRational() && m_curve.m_K > 1)
	//	NewTest (vv,finalRes);
	return finalRes;
	}
	catch ( VException & e )
	{
		throw ( e );
		//exp.ShowMessage();
		return ( VPoint ( 0,0,0 ) );
	}
}
#endif
void BSplineCurvePointGenerator::GetPointAndTangent (const vreal &v
													 ,VPoint &p
													 ,VVector&tng)const
{
#if USENN
	vreal vv = ReverseIf (v);
	vv = TransformPar(vv);
	m_NE.GetPointAndTangent (vv,p,tng);
	if (fabs(tng.x()) < EPSILON && fabs(tng.y()) < EPSILON && fabs(tng.z()) < EPSILON)
	{
		if (vv < 6.0 * EPSILON)
		{
			vv += 5.0 * EPSILON;
			m_NE.GetPointAndTangent(vv, p, tng);
		}
		else //if (1 - t < EPSILON)
		{
			vv -= 5.0 * EPSILON;
			m_NE.GetPointAndTangent(vv, p, tng);
		}
	}
	ReverseTangentIf(tng);
	tng.NormThis();
	TransformIf (p,tng);
	return;
#else
	if (m_curve.IsRational() == false)
	{
		GetPointAndTangentImp (v,p,tng);
	}
	else
	{
		sVector<double,4> tmpp , tmptng;
		GetPointAndTangentRational (v,tmpp,tmptng);
		p.x(tmpp[0]);
		p.y(tmpp[1]);
		p.z(tmpp[2]);
		tng.x(tmptng[0]);
		tng.y(tmptng[1]);
		tng.z(tmptng[2]);
	}
#endif
}


#if !USENN
void BSplineCurvePointGenerator::GetPointAndTangentImp ( const vreal &v 
														 ,VPoint &p
														,VVector &tng) const
{
	if (m_deboor)
	{
		vreal vv = ReverseIf (v);
		m_deboor->GetPointAndTangent(vv
									,p
									,tng);
		ReverseTangentIf (tng);
		TransformIf (p,tng);
		return;
	}
	// GetPoint (v) reverses and transform point if needed
	p = GetPointImp (v);
	if (fabs (v) > VGlobalValues::MYDERV_DIFF)
	{
		VPoint pbef = GetPointImp (v - VGlobalValues::MYDERV_DIFF);
		tng = p - pbef;
		tng.NormThis();
	}
	else
	{
		VPoint paft = GetPointImp (v + VGlobalValues::MYDERV_DIFF);
		tng = paft - p;
		tng.NormThis();
	}
	return;
}
#endif
void BSplineCurvePointGenerator::GetDefIntervals ( DoubleVec & defint ) const
{
#if USENN
	m_NE.GetKnot().GetDefIntervals(defint);
#else
	knots->GetDefIntervals ( defint );
#endif
}

void BSplineCurvePointGenerator::Digitise (const vreal &tol, SequentialLineSet &result , DoubleVec &params) const
{
	return GenericDigitise (tol , result , params );
	//DoubleVec par1;
	//GetDefIntervals (par1);
	//int size1=par1.Size();
	//DoubleVec par2;
	//for (int i=0;i<size1-1;i++)
	//{
	//	const vreal &p1 = par1[i];
	//	const vreal &p2 = par1[i+1];
	//	int segNo = DivideInterval (p1,p2,tol);
	//	InsertSegments (segNo,p1,p2,par2);
	//}
	//par2.Addvreal (1.0);
	//int size2 = par2.Size();
	//for (int j=0;j<size2;j++)
	//{
	//	const vreal &v = par2[j];
	//	VPoint p = GetPoint (v);
	//	bool ck = result.InsertPoint (p);
	//	if (ck)
	//		params.Addvreal(v);
	//}
}

void BSplineCurvePointGenerator::Digitise (const vreal &tol
										   , SequentialLineSet &result 
										   , PointSet &tangents
										   , DoubleVec &params) const
{
	DoubleVec par1;
	GetDefIntervals (par1);
	int size1=par1.Size();
	DoubleVec par2;
	for (int i=0;i<size1-1;i++)
	{
		const vreal &p1 = par1[i];
		const vreal &p2 = par1[i+1];
		int segNo = DivideInterval (p1,p2,tol);
		InsertSegments (segNo,p1,p2,par2);
	}
	par2.Addvreal (1.0);
	int size2 = par2.Size();
	for (int j=0;j<size2;j++)
	{
		const vreal &v = par2[j];
		VPoint p,t;
		GetPointAndTangent (v,p,t);
		bool ck = result.InsertPoint (p);
		if (ck)
		{
			tangents.InsertPoint (t);
			params.Addvreal(v);
		}
	}
}

int BSplineCurvePointGenerator::DivideInterval (const vreal &p1 , const vreal &p2,const vreal tol)const
{
	int degree = m_curve.GetDegreeOfBasisFunctions();
	if (degree == 1)
		return 1;
	//else 
	//	return degree * 2;

	//  making a circle approximation for interval of [p1 p2]
	vreal mp = (p1+p2)*0.5;
	VPoint derv,dervderv;
	vreal rc = GetRadiusOfCurvatureApr (mp,derv,dervderv);;
	if (rc > 1.e8)
		return 1;
	vreal teta = (derv.GetLength() / rc) * (p2-p1);
	// this interval is modelled as arc of radius rc and arc angle teta
	vreal tmp = 1. - tol / rc;
	if (tol > rc)
		return 1;
	vreal divideTeta = 2. * vp_acos ( tmp);
	int res = (int)(teta / divideTeta + 0.5);
	if (res == 0)
		++res;
	return res;
}
#if !USENN
sVector<double,4> BSplineCurvePointGenerator::GetPointRational ( const vreal &u ) const
{
	//VPoint tmp;
	//NewTest (u,tmp);
	if (m_deboor)
	{
		vreal vv = ReverseIf (u);
		sVector<double,4> p = m_deboor->GetPointRational (vv);
		VPoint tmp (p[0],p[1],p[2]);
		TransformIf (tmp);
		p.SetElement (tmp.x(),0);
		p.SetElement (tmp.y(),1);
		p.SetElement (tmp.z(),2);
		return p;
	}
	VPoint p2 = GetPointImp (u);
	vreal myarray[4] = {p2.x(),p2.y(),p2.z(),1.0};//weight is set to 1.0
	sVector<double,4>res (myarray);
	return res;
}
	
void BSplineCurvePointGenerator::GetPointAndTangentRational ( const vreal &v
									, sVector<double,4> &p
									, sVector<double,4> &tng)const
{
	//VPoint tmp;
	//NewTest (v,tmp);
	if (m_deboor)
	{
		vreal vv = ReverseIf (v);
		m_deboor->GetPointAndTangentRational (vv
											  ,p
											  ,tng);
		VPoint tmptng (tng[0],tng[1],tng[2]);
		ReverseTangentIf (tmptng);
		VPoint tmpp (p[0],p[1],p[2]);
		TransformIf (tmpp,tmptng);
		tng.SetElement(tmptng.x(),0);
		tng.SetElement(tmptng.y(),1);
		tng.SetElement(tmptng.z(),2);
		p.SetElement (tmpp.x(),0);
		p.SetElement (tmpp.y(),1);
		p.SetElement (tmpp.z(),2);
		return;
	}
	VPoint tmpp2,tmptng2;
	GetPointAndTangentImp (v,tmpp2,tmptng2);
	p.SetElement (tmpp2.x(),0);
	p.SetElement (tmpp2.y(),1);
	p.SetElement (tmpp2.z(),2);
	p.SetElement (1.0 , 3);
	tng.SetElement (tmptng2.x(),0);
	tng.SetElement (tmptng2.y(),1);
	tng.SetElement (tmptng2.z(),2);
	tng.SetElement (1.0,3);
}
#endif
bool BSplineCurvePointGenerator::IsRational()const
{
	return m_curve.IsRational();
}

void BSplineCurvePointGenerator::GetSnapPoints (PointSet &snapp)const
{
	//DoubleVec tmp;
	//GetDefIntervals (tmp);
	//int mysize = tmp.Size();
	//for (int i= 0;i<mysize;i++)
	//{
	//	vreal currp = tmp[i];
	//	VPoint currpo = GetPoint (currp);
	//	snapp.InsertPoint(currpo);
	//}
	if (m_curve.GetDegreeOfBasisFunctions() > 1)
	{
		DoubleVec mydef;
		GetDefIntervals (mydef);
		int mysize = mydef.Size();
		for (int i=0;i<mysize;++i)
		{
			snapp.InsertPoint (GetPoint (mydef.operator [](i)));
		}
	}
	else
	{
		snapp.InsertPoint (GetPoint (0.0));
		snapp.InsertPoint (GetPoint (0.5));
		if (m_curve.IsClosed() == false)
			snapp.InsertPoint (GetPoint (1.0));
	}
}
void BSplineCurvePointGenerator::GetSnapLines (LineSet &snapl)const
{

}

VPoint BSplineCurvePointGenerator::GetTangentDerivative (const vreal &v)const
{
	return GetTangentDerivativeGeneric(v);
}
	
vreal BSplineCurvePointGenerator::GetRadiusOfCurvature (const vreal &v)const
{
	VPoint derv,dervderv;
	return GetRadiusOfCurvatureApr (v,derv,dervderv);
}

void BSplineCurvePointGenerator::NewTest(const vreal u,VPoint p)const
{
	if (1-u < EPSILON)
		int stp = 1;
	NurbsEval mytest (m_curve);
	VPoint myp = mytest.GetPoint (u);
	VPoint derv;
	mytest.GetPointAndTangent (u,myp,derv);
	VPoint tmp = mytest.GetPoint(u);
	derv.NormThis();
	p.NormThis();
	//if (fabs ( (derv-p)%(derv-p) ) > EPSILON )//&& fabs(u-1.)>1.e-3)
	//{
	//	int uuu=1;
	//	throw VException (("eyvah"),1);
	//}
}
