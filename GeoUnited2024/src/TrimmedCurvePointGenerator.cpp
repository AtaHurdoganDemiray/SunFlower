// TrimmedCurvePointGenerator.cpp

#include "stdafx.h"
#include "TrimmedCurvePointGenerator.h"
#include "IgesEntitySet.h"
#include "Edge.h"
#include "CurvePointGeneratorFactory.h"
#include "CurveSpace.h"
#include "dCurve.h"
#include "Vertex.h"
#include "VArc.h"
#include "BSplineCurve.h"
#include "ViewPointDefaults.h"

TrimmedCurvePointGenerator::TrimmedCurvePointGenerator (IgesEntity *ent , IgesEntitySet *allent,int ei , double tol)
: CurvePointGenerator (ent,allent) , m_ei (ei) , m_tol (tol) , m_srcCurv (0) , m_isClosed (false) , m_v0offset(0.0)
{
	MakeSrcCurve();
	CalculateStartAndTerminateParam();
}
TrimmedCurvePointGenerator::~TrimmedCurvePointGenerator()
{
	if (m_srcCurv)
		delete m_srcCurv;
}
VPoint TrimmedCurvePointGenerator::GetPoint ( const vreal & v ) const
{
	vreal vv = ReverseIf(v);
	if (m_isClosed)
		vv = OffsetParForClosed (vv);
	vv = TransformPar(vv);
	VPoint result = m_srcCurv->GetCurve()->GetPoint(vv);
	TransformIf (result);	
	return result;
}
void TrimmedCurvePointGenerator::GetPointAndTangent ( const vreal &v , VPoint &p , VVector &tng)const
{
	vreal vv = ReverseIf(v);
	if  (m_isClosed)
		vv = OffsetParForClosed (vv);
	vv = TransformPar(vv);
	m_srcCurv->GetCurve()->GetPointAndTangent(vv,p,tng);
	int recursiveCounter(0);
	ReverseTangentIf(tng);
	TransformIf (p,tng);
}
VPoint TrimmedCurvePointGenerator::GetTangentDerivative (const vreal &v)const
{
	return GetTangentDerivativeGeneric(v);
}
vreal TrimmedCurvePointGenerator::GetRadiusOfCurvature (const vreal &v)const
{
	VPoint derv,dervderv;
	return GetRadiusOfCurvatureApr (v,derv,dervderv);
}
void TrimmedCurvePointGenerator::GetDefIntervals ( DoubleVec & defint ) const
{
	DoubleVec srcDefInt;
	m_srcCurv->GetCurve()->GetDefIntervals (srcDefInt);
	int mysize = srcDefInt.Size();
	defint.Addvreal(0.);
	for (int i=0;i<mysize;++i)
	{
		vreal srcv = srcDefInt.operator [](i);
		if (srcv <= m_v0 || srcv >= m_v1)
			continue;
		vreal newpar = (srcv - m_v0) / m_deltav;
		defint.Addvreal(newpar);
	}
	defint.Addvreal(1.);
}
void TrimmedCurvePointGenerator::Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const
{
	GenericDigitise (tol,result,params);
}
void TrimmedCurvePointGenerator::Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params) const
{
	//m_srcCurv->GetCurve()->Digitise(tol,result,tangents,params);
	GenericDigitise(tol,result,params);
	int mysize = params.Size();
	for (int i=0;i<mysize;++i)
	{
		vreal curv = params[i];
		VPoint p,tang;
		GetPointAndTangent(curv , p , tang);
		tangents.InsertPoint(tang);
	}
}
void TrimmedCurvePointGenerator::GetSnapPoints (PointSet &snapp)const
{
	VPoint p0 =  GetPoint(0.);
	VPoint p1 = GetPoint (0.5);
	VPoint p2 = GetPoint (1.);
	VArc a;
	bool ck = GetArcIf (a);
	snapp.InsertPoint(p0);
	snapp.InsertPoint(p1);
	snapp.InsertPoint(p2);
	if (ck)
		snapp.InsertPoint (a.GetCenter());
}
void TrimmedCurvePointGenerator::GetSnapLines (LineSet &snapl)const
{
	m_srcCurv->GetCurve()->GetSnapLines (snapl);
}
void TrimmedCurvePointGenerator::MakeSrcCurve()
{
	Edge *e = (Edge *)m_ent;
	int crvde = e->m_CURV.at (m_ei);
	IgesEntity *crv = m_entset->GetEntityPointer(crvde);	
	if (crv == 0)
		throw VException (( IDS_TriExc),1);
	m_srcCurv = new dCurve (m_tol /*, VGlobalValues::viewpointdefaults.m_quickDisplayTol*/ , crv , m_entset , false);
	// record this source curve as subcurve to an edge so that it is not imported twice
	std::map<int,int> &curveEdgeDePairsMap = m_entset->GetCurveEdgeDePairsMap();
	curveEdgeDePairsMap.insert (std::pair<int,int> (crv->directorylineno,e->directorylineno));
	//DoShallowTransformationOnSrcCurveIf(crv);
	// setting start and terminate points
	int vertexDe0 = e->m_SVP.at(m_ei);
	int vertexDe1 = e->m_TVP.at(m_ei);
	IgesEntity *vr0 = m_entset->GetEntityPointer(vertexDe0);
	IgesEntity *vr1 = m_entset->GetEntityPointer(vertexDe1);
	if (vr0 == 0 || vr1 == 0)
		throw VException (( IDS_TriMakExcVerNul),1);
	Vertex *ver0 = (Vertex*)vr0;
	Vertex *ver1 = (Vertex*)vr1;
	m_SP = ver0->GetVertex (e->m_SV.at(m_ei));
	m_TP = ver1->GetVertex(e->m_TV.at(m_ei));
}
void TrimmedCurvePointGenerator::DoShallowTransformationOnSrcCurveIf (IgesEntity *src)
{
//	if (src->DE.pXform == 0)
//		return;
//	IgesEntity *im = m_entset->GetEntityPointer(src->DE.pXform);
//	if(im == 0 || im->entitytype != 124)
//		return;
//	igesTransformationMatrix *itm = (igesTransformationMatrix *)im;
//	TransformationMatrix mytm (*itm);
//	m_srcCurv->Transform(mytm);

}

void TrimmedCurvePointGenerator::CalculateStartAndTerminateParam()
{
	std::map<double,double> splitParMap;
	SplitCurveByPoint(m_srcCurv , splitParMap , m_SP,m_tol,true);
	if (splitParMap.size() == 0)
		throw VException (( IDS_TriCalExcEdgLim),1);
	double coincidentTolerance = m_tol * 0.00001;
	if (m_SP.Compare (m_TP , coincidentTolerance) == true)
	{
		// This is closed curve !
		m_isClosed = true;		
		SetV0V1 (0.0 , 1.0);

		VPoint srcstartp = m_srcCurv->GetCurve()->GetPoint (0.0);
		VPoint srctermp = m_srcCurv->GetCurve()->GetPoint (1.0);
		if (srcstartp.Compare (srctermp , coincidentTolerance) == false)
			throw VException (( IDS_TriCalExcEdgSta),1);
		if (srcstartp.Compare (m_SP , coincidentTolerance) == true)
			m_v0offset = 0.0;
		else
			m_v0offset = splitParMap.begin()->first;
		
	}
	SplitCurveByPoint(m_srcCurv , splitParMap , m_TP,m_tol,false);
	vreal vv0 = splitParMap.begin()->first;
	vreal vv1 = splitParMap.rbegin()->first;
	if (vv0 > vv1)
		std::swap(vv0,vv1);
	//if (fabs(vv0-1) < 1.e-8)
	//	vv0 = 0;
	//if (fabs(vv1) <1.e-8)
	//	vv1 = 1.;
	SetV0V1 (vv0,vv1);
}

void TrimmedCurvePointGenerator::SplitCurveByPoint (dCurve *crv , std::map<double,double> &splitParMap , const VPoint &witnessPoint , double tol , bool isStartPoint)
{
	double parOnCurve;
	dCurve::CurveType mycrvtype = crv->GetCurveType();
	if (mycrvtype == dCurve::arc)
	{
		VArc myarc;
		bool ckarc = crv->GetArcIf (myarc);
		if (!ckarc)
			throw VException (( IDS_DCuGetAndGetCon),1);
		double myangle = myarc.GetAngle (witnessPoint);
		parOnCurve = myangle / myarc.GetSweepAngle();
		if (parOnCurve < 0.0)
			parOnCurve += 1.0;
	}
	else if (mycrvtype == dCurve::line)
	{
		VLine myl;
		bool ckline = crv->GetLineIf (myl);
		if (!ckline)
			throw VException (( IDS_DCuGetAndGetCon1),1);
		parOnCurve = myl.GetT (witnessPoint);
	}
	else
	{
		CurveSpace sp (*crv);
		double refinedDist;
		VPoint refinedWitnessPoint;
		int maxIterCount = 20;
		sp.Convert (witnessPoint , refinedDist , parOnCurve , refinedWitnessPoint,tol ,maxIterCount);
		if (refinedDist > 2.*tol)
			throw VException (( IDS_TriSplExcSplPoi),1);
	}
	if (fabs(parOnCurve) < EPSILON)
	{
		if (crv->GetLines().Start()->Compare(*crv->GetLines().End(),tol))
		{
			if (isStartPoint == false)
				parOnCurve = 1.0;
		}
	}
	if (fabs(parOnCurve-1.0)<EPSILON)
	{
		if (crv->GetLines().Start()->Compare(*crv->GetLines().End(),tol))
		{
			if (isStartPoint)
				parOnCurve = 0.0;
		}
	}
	splitParMap.insert(std::pair<double,double>(parOnCurve,parOnCurve));
}

bool TrimmedCurvePointGenerator::GetArcIf(VArc &a) const
{
	Edge *e = (Edge *)m_ent;
	int crvde = e->m_CURV.at (m_ei);
	IgesEntity *crv = m_entset->GetEntityPointer(crvde);	
	if (crv == 0)
		throw VException (( IDS_TriGetExcSrcCur),1);
	if (crv->entitytype == 100)
	{
		VArc untrimmeda;
		bool ckinner = m_srcCurv->GetArcIf (untrimmeda);
		if (!ckinner)
			throw VException(( IDS_TriGetArcCon),1);

		VPoint sp = GetPoint (0.0);
		VPoint ep = GetPoint (1.0);
		VArc tmpclosed (untrimmeda.GetCenter() , untrimmeda.GetRadius() , untrimmeda.GetAx() , sp , 360.0);
		double myendangle = tmpclosed.GetAngle (ep);
		if (fabs (myendangle) < EPSILON)
			myendangle = 360.0;
		a = VArc (tmpclosed.GetCenter() , tmpclosed.GetRadius() , tmpclosed.GetAx() , tmpclosed.GetStartPoint() , myendangle);
		//if (sp.Compare (ep , 1.e-5) == false)
		//	a = VArc (sp,mp,ep);	// not closed
		//else
		//{
		//	VArc tmp (sp,GetPoint(0.25),mp);
		//	a = VArc (tmp.GetCenter() , tmp.GetRadius() , tmp.GetAx() , tmp.GetStartPoint() , 360.0);
		//}
		return true;
	}
	else
		return false;
}

IgesEntity * TrimmedCurvePointGenerator::GetSrcCurvEnt ()
{
	Edge *e = (Edge *)m_ent;
	int crvde = e->m_CURV.at (m_ei);
	IgesEntity *crv = m_entset->GetEntityPointer(crvde);	
	return crv;
}

bool TrimmedCurvePointGenerator::GetLineIf(VLine &l) const
{
	Edge *e = (Edge *)m_ent;
	int crvde = e->m_CURV.at (m_ei);
	IgesEntity *crv = m_entset->GetEntityPointer(crvde);	
	if (crv == 0)
		throw VException (( IDS_TriGetExcSrcCur1),1);
	if (crv->entitytype == 110)
	{
		VPoint sp = GetPoint (0.0);
		VPoint ep = GetPoint (1.0);
		l = VLine (sp,ep);
		return true;
	}
	else
		return false;
}

double TrimmedCurvePointGenerator::OffsetParForClosed (double voriginal)const
{
	double vactual = voriginal + m_v0offset;
	if (vactual > 1.)
		vactual -= 1.;
	return vactual;
}

bool TrimmedCurvePointGenerator::IsLine()const
{
	const Edge *e = (Edge *)m_ent;
	int crvde = e->m_CURV.at (m_ei);
	const IgesEntity *crv = m_entset->GetEntityPointer(crvde);	
	if (crv == 0)
		throw VException (( IDS_TriGetExcSrcCur1),1);
	if (crv->entitytype == 110)
	{
		return true;
	}
	else if (crv->entitytype == 126)
	{
		const BSplineCurve *curBSpline = (BSplineCurve *)crv;
		return curBSpline->IsLine();
	}
	else
		return false;
}
bool TrimmedCurvePointGenerator::IsArc()const
{
	const Edge *e = (Edge *)m_ent;
	int crvde = e->m_CURV.at (m_ei);
	const IgesEntity *crv = m_entset->GetEntityPointer(crvde);	
	if (crv == 0)
		throw VException (( IDS_TriGetExcSrcCur),1);
	if (crv->entitytype == 100)
	{
		return true;
	}
	else if (crv->entitytype == 126)
	{
		const BSplineCurve *curBSpline = (BSplineCurve *)crv;
		return curBSpline->IsCircularArc();
	}
	else
		return false;
}
bool TrimmedCurvePointGenerator::IsPolyline ()const
{
	const Edge *e = (Edge *)m_ent;
	int crvde = e->m_CURV.at (m_ei);
	const IgesEntity *ent = m_entset->GetEntityPointer(crvde);	
	if (ent == 0)
		return false;
	if (ent->entitytype == 106)
	{
		return true;
	}
	else if (ent->entitytype == 126)
	{
		const BSplineCurve *curBSpline = (BSplineCurve *)ent;
		return curBSpline->IsPolyline();
	}
	return false;
}
int TrimmedCurvePointGenerator::GetEi()const
{
	return m_ei;
}