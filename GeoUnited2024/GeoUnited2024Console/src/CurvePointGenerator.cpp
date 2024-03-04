// CurvePointGenerator.cpp

#include "stdafx.h"
#include "CurvePointGenerator.h"
#include "IgesEntity.h"
#include "IgesEntitySet.h"
#include "PointSet.h"
#include "LineSet.h"
#include "Arc.h"
#include "Line.h"
#include "ConicArc.h"
#include "SequentialLineSet2d.h"
#include "SequentialLineSet.h"
#include "igesTransformationMatrix.h"
#include "DoubleVec.h"
#include "VArc.h"
#include "UntrimmedSurface.h"


CurvePointGenerator::CurvePointGenerator (IgesEntity *ent , IgesEntitySet *allent)
:m_reversed (false),m_ent(ent),m_entset(allent),m_istransformed(false),m_v0(0.0),m_v1(1.0),m_deltav(1.0)
, m_isTransformedBase (false)
{
	//SetTransformationMatrixIf (ent,allent);
	SetTransformationMatrixBase ();
}
CurvePointGenerator::~CurvePointGenerator()
{
	//if (m_tm)
	//	delete m_tm;
}
void CurvePointGenerator::SetReversed (bool isReversed)
{
	m_reversed = isReversed;
}
bool CurvePointGenerator::GetReversed ()const
{
	return m_reversed;
}
vreal CurvePointGenerator::ReverseIf (const vreal &v)const
{
	vreal res ;
	if (m_reversed)
		res = 1.0 - v;
	else
		res = v;
	return res;
}

void CurvePointGenerator::IncrementTransform (const TransformationMatrix &tm)
{
	m_tm = tm * m_tm;
	m_istransformed = true;
	//int Xiref = m_ent->DE.pXform;
	//if (Xiref)
	//{
	//	igesTransformationMatrix *Xi =(igesTransformationMatrix *) m_entset->GetEntityPointer(Xiref);
	//	m_tm->SetIgesTransformation (*Xi); 
	//}
	//else
	//{
		//m_ent->DE.pXform = m_entset->InsertTM (m_tm);
	//}
}
void CurvePointGenerator::ResetTransform (const TransformationMatrix &tm)
{
	m_tm = tm;
	m_istransformed = true;
}
const TransformationMatrix & CurvePointGenerator::GetTransformation()const
{
	return m_tm;
}
	
void CurvePointGenerator::ClearTransformation()
{
	TransformationMatrix I;
	m_tm = I;
	m_istransformed = false;
}

bool CurvePointGenerator::IsTransformed ()const
{
	return m_istransformed;
}

void CurvePointGenerator::TransformIf (VPoint &p)const
{
	if (m_isTransformedBase)
	{
		p = m_tmBase.Transform (p);
	}
	if (m_istransformed)
	{
		p = m_tm.Transform (p);
	}
}

void CurvePointGenerator::TransformIf (VLine &l)const
{
	if (m_isTransformedBase)
	{
		l = m_tmBase.Transform (l);
	}
	if (m_istransformed)
	{
		l = m_tm.Transform (l);
	}
}

void CurvePointGenerator::TransformIf(VArc &a)const
{
	if (m_isTransformedBase)
	{
		a.Transform (m_tmBase);
	}
	if (m_istransformed)
	{
		a.Transform (m_tm);
	}
}

void CurvePointGenerator::TransformIf (VPoint &p, VVector &v)const
{
	if (m_isTransformedBase)
	{
		p = m_tmBase.Transform (p);
		v = m_tmBase.TransformVec(v);
	}
	if (m_istransformed)
	{
		p = m_tm.Transform (p);
		v = m_tm.TransformVec (v);
	}
}
void CurvePointGenerator::TransformTanVecIf(VVector& tan)const
{
	if (m_isTransformedBase)
	{
		tan = m_tmBase.TransformVec(tan);
	}
	if (m_istransformed)
	{
		tan = m_tm.TransformVec(tan);
	}
}
void CurvePointGenerator::ReverseTangentIf (VVector &tng)const
{
	if (m_reversed)
	{
		tng *= -1.0;
	}
}

void CurvePointGenerator::SetTransformationMatrixBase ()
{
	if ( m_ent->DE.pXform > 0 )
	{
		igesTransformationMatrix * igstm = ( igesTransformationMatrix *) 
		m_entset->GetEntityPointer ( m_ent->DE.pXform );
		
		if ( !igstm )
		{
			throw VException (( IDS_IgeMatIsNotFou),1);
		}
		TransformationMatrix tmp(*igstm);
		//ResetTransform (tmp);		
		m_isTransformedBase = true;
		m_tmBase = tmp;
	}
}

//void CurvePointGenerator::Digitise (const vreal &tol 
//								  , const vreal & U0 , const vreal & U1 
//								  , const vreal & V0 , const vreal & V1
//								  , SequentialLineSet2d &result
//								  , DoubleVec &params)const
//{
//	SequentialLineSet tmp(100);
//	Digitise (tol , tmp , params);
//	VPoint2d P0 (U0,V0);
//	VPoint2d P1 (U1,V1);
//	int size2 = tmp.GetPointSize();
//	for (int j=0;j<size2;j++)
//	{
//		const VPoint *currp = tmp.GetPoint(j);
//		VPoint2d parp = RePar (*currp , P0 , P1);
//		result.InsertPoint (parp);
//	}
//}

void CurvePointGenerator::Digitise (const vreal tol 
									, const UntrimmedSurface &surf
									, SequentialLineSet2d &result
									, DoubleVec &params)const
{
	SequentialLineSet tmp(100);
	Digitise (tol , tmp , params);
	int size2 = tmp.GetPointSize();
	for (int j=0;j<size2;j++)
	{
		const VPoint *currp = tmp.GetPoint(j);
		//VPoint2d parp(currp->x(),currp->y());
		VPoint2d parp = surf.RePar(*currp);
		result.InsertPoint (parp);
	}
}

VPoint2d CurvePointGenerator::RePar (const VPoint &p , const VPoint2d &P0 , const VPoint2d &P1)const
{
	VPoint2d repar ( ( p.x() - P0.x() ) / ( P1.x() - P0.x() )
						, ( p.y() - P0.y() ) / ( P1.y() - P0.y() ) );
	return repar;	
}

// dummy function for reducing numeric errors
//void DoNothing ( const vreal & tmp )
//{
//
//}

VPoint CurvePointGenerator::GetDervApr(const vreal &v,const VPoint &p)const
{
	vreal temp = v + VGlobalValues::MYDERV_DIFF;
	//DoNothing (temp);
	vreal myderv_diff = temp - v;	
	
	VPoint dp;
	vreal vbef = v - myderv_diff;

	if ( vbef > 0.0)
	{
		VPoint pp = GetPoint (vbef);
		dp = p - pp;
	}
	else
	{
		vreal vaft = v + myderv_diff;
		VPoint np = GetPoint ( vaft);
		dp = np-p;
	}
	vreal inv_derv_diff = 1. / myderv_diff;
	return dp * inv_derv_diff;
}
VPoint CurvePointGenerator::GetDervDervApr (const vreal &v,const VPoint &p,const VPoint &derv)const
{
	vreal temp = v + VGlobalValues::MYDERV_DIFF;
	//DoNothing (temp);
	vreal myderv_diff = temp - v;	
	
	VPoint ddp;
	vreal vbef = v - myderv_diff;

	if ( vbef > 0.0)
	{
		VPoint pp = GetPoint (vbef);
		VPoint pdp = GetDervApr (vbef,pp);
		ddp = derv - pdp;
	}
	else
	{
		vreal vaft = v + myderv_diff;
		VPoint np = GetPoint ( vaft);
		VPoint ndp = GetDervApr (vaft,np);
		ddp = ndp-derv;
	}
	vreal inv_derv_diff = 1. / myderv_diff;
	return ddp * inv_derv_diff;
}
vreal CurvePointGenerator::GetCurvatureApr (const vreal &v,VPoint &derv,VPoint &dervderv)const
{
	// K = | derv x dervderv | / |derv^3|
	VPoint p = GetPoint (v);
	derv = GetDervApr (v,p);
	dervderv = GetDervDervApr (v,p,derv);
	VPoint cros = derv ^ dervderv;
	double denu = derv.GetLength();
	return cros.GetLength() / (denu*denu*denu);
}

vreal CurvePointGenerator::GetRadiusOfCurvatureApr(const vreal &v,VPoint &derv,VPoint &dervderv) const
{
	return 1. / GetCurvatureApr (v,derv,dervderv);
}

IgesEntity * CurvePointGenerator::GetEnt()
{
	return m_ent;
}

IgesEntitySet * CurvePointGenerator::GetEntSet()
{
	return m_entset;
}

void CurvePointGenerator::GenericDigitise(const vreal &tol, SequentialLineSet &result, DoubleVec &params) const
{
	GenericDigitiseImp(tol,result,params);
	return;

	//DoubleVec par1;
	//GetDefIntervals (par1);
	//int size1=par1.Size();
	//DoubleVec par2;
	//for (int i=0;i<size1-1;i++)
	//{
	//	const vreal &p1 = par1[i];
	//	const vreal &p2 = par1[i+1];
	//	int segNo = CalculateDivideCountForInterval (p1,p2,tol);
	//	InsertSegments (segNo,p1,p2,par2);
	//	std::map<vreal,VPoint> digiMap;
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

void CurvePointGenerator::GenericDigitiseImp(const vreal &tol , SequentialLineSet &result , DoubleVec &params) const
{
	DoubleVec par1;
	GetDefIntervals (par1);
	int size1=par1.Size();
	for (int i=0;i<size1-1;i++)
	{
		const vreal &p1 = par1[i];
		const vreal &p2 = par1[i+1];
		std::map<vreal,VPoint> digiMap;
		int recdepth = 0;
		try
		{
			SubDivideTill(p1,p2,tol,digiMap,recdepth);
		}
		catch(VException &e)
		{
			LogFileWriter::WriteExceptionLog (e);
		}
		size_t mysize1 = digiMap.size();
		std::map<vreal,VPoint>::iterator iii=digiMap.begin();
		for (size_t j=0;j<mysize1;++j)
		{
			bool ck = params.Addvrealif(iii->first);
			if (ck)
				result.InsertPoint(iii->second);
			++iii;
		}
	}
	bool ck = params.Addvrealif (1.0);
	if (ck)
		result.InsertPoint(GetPoint(1.0));
}

int CurvePointGenerator::m_subDivideMaxRecursionLimit = 1000;
int CurvePointGenerator::m_divideTillRecDepth = 32;
void CurvePointGenerator::SubDivideTill (const vreal v1 , const vreal v2 , const vreal tol , std::map<vreal,VPoint> &digiMap , int &recdepth)const
{
	++recdepth;
	if (recdepth > m_subDivideMaxRecursionLimit)
		throw VException (( IDS_CurSubMaxRecLim),1);
	VPoint p1,p2;
	double err = GetErrorForLinearazation (v1 , v2 , p1 , p2);
	if (err < tol || recdepth >= m_divideTillRecDepth)
	{
		digiMap.insert (std::pair<vreal,VPoint>(v1 , p1));
		digiMap.insert (std::pair<vreal,VPoint>(v2 , p2));
	}
	else
	{
		vreal vmid = (v1 + v2)*0.5;
		SubDivideTill (v1,vmid,tol,digiMap,recdepth);
		SubDivideTill (vmid,v2,tol,digiMap,recdepth);
	}
}

vreal CurvePointGenerator::GetErrorForLinearazation (const vreal v1 , const vreal v2 , VPoint &p1 , VPoint &p2)const
{
	vreal vmid = (v1+v2)*0.5;
	VPoint nominalPoint = this->GetPoint (vmid);
	p1 = GetPoint (v1);
	p2 = GetPoint (v2);
	VPoint apprPoint = (p1+p2)*0.5;
	vreal err = (nominalPoint-apprPoint).GetLength();
	return err;
}
int CurvePointGenerator::CalculateDivideCountForInterval  (const vreal v1 
														, const vreal v2
														, const vreal tol)const
{
	// linear calculation
	vreal vmid = (v1+v2)*0.5;
	VPoint nominalPoint = this->GetPoint (vmid);
	VPoint p1 = GetPoint (v1);
	VPoint p2 = GetPoint (v2);
	VPoint apprPoint = (p1+p2)*0.5;
	vreal err = (nominalPoint-apprPoint).GetLength();
	if (fabs(err)<EPSILON)
		return 1;
	int segmentCount = (int)(err / tol);
	return segmentCount+1;
}

void CurvePointGenerator::InsertSegments (int segNo , const vreal &p1 , const vreal &p2,DoubleVec &param2)const
{
	vreal delta = p2-p1;
	vreal myStep = delta / segNo;
	vreal newP2 = p1;

	for (int j=0; j<segNo; j++)
	{
		param2.Addvreal (newP2);
		newP2 += myStep;
	}
}

double CurvePointGenerator::GetV0()const
{
	return m_v0;
}

double CurvePointGenerator::GetV1()const
{
	return m_v1;
}

void CurvePointGenerator::SetV0V1(double v0, double v1)
{
	m_v0 = v0;
	m_v1 = v1;
	m_deltav = m_v1 - m_v0;
}

double CurvePointGenerator::TransformPar(double v)const
{
	return (m_v0 + m_deltav * v);	
}

VPoint CurvePointGenerator::GetTangentDerivativeGeneric(const vreal &v)const
{
	VPoint p1,p2,t1,t2;
	GetPointAndTangent (v,p1,t1);
	if (fabs (v) > VGlobalValues::MYDERV_DIFF)
	{
		GetPointAndTangent (v - VGlobalValues::MYDERV_DIFF,p2,t2);
		VPoint dd = t1 - t2;
		dd *= (1.0 / VGlobalValues::MYDERV_DIFF);
		return dd;
	}
	else
	{
		GetPointAndTangent (v + VGlobalValues::MYDERV_DIFF,p2,t2);
		VPoint dd = t2 - t1;
		dd *= (1.0 / VGlobalValues::MYDERV_DIFF);
		return dd;
	}
}

bool CurvePointGenerator::IsSharpCorner (double cosAngleLimit , double v , double parInc , bool isClosedCurve)
{
	double vprev = v - parInc;
	double vafter = v + parInc;
	if (v < parInc || v > (1.-parInc))
	{
		if (isClosedCurve)
		{
			if (v < parInc)
			{
				vprev = 1. - parInc;
			}
			else if ( v > (1-parInc))
			{
				vafter = parInc;
			}
		}
		else
			return false;	// this is an open ended curve start or end can not be a sharp corner!
	}

	VPoint prevp = GetPoint (vprev);
	VPoint p = GetPoint (v);
	VPoint afterp = GetPoint (vafter);
	return VPoint::IsSharpCorner (prevp,p,afterp,cosAngleLimit);
}

const IgesEntity * CurvePointGenerator::GetEnt()const
{
	return m_ent;
}

bool CurvePointGenerator::GetIsTransformedBase()const
{
	return m_isTransformedBase;
}
const TransformationMatrix & CurvePointGenerator::GetTmBase()const
{
	return m_tmBase;
}