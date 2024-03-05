// dCurve.cpp

#include "stdafx.h"
#include "dCurve.h"
#include "IgesEntity.h"
#include "CopiousData.h"
#include "CurvePointGenerator.h"
#include "CurvePointGeneratorFactory.h"
#include "Edge.h"
#include "ArcPointGenerator.h"
#include "TrimmedCurvePointGenerator.h"
#include "LinePointGenerator.h"
#include "BSplineCurve.h"

dCurve::dCurve (const vreal &tol /*, vreal roughTol*/ , IgesEntity *ent , IgesEntitySet *entset, bool isReversed)
:m_tol (tol) , m_deno (ent->directorylineno)//,m_roughTol(roughTol)//,m_ent(ent),m_entset(entset)
{
	MakeCurve(ent,entset,isReversed);
	MakeLines();
	MakeSnapPoints();
	MakeSnapLines();
	m_trimSrcDeno = m_deno;
}
dCurve::dCurve (const vreal tol /*, vreal roughTol*/ , Edge *ent , IgesEntitySet*entset , bool isReversed , int ei)
: m_tol (tol)//,m_roughTol(roughTol)
{
	MakeCurve (ent , entset , isReversed , ei);
	MakeLines();
	MakeSnapPoints();
	MakeSnapLines();
	m_trimSrcDeno = ent->m_CURV.at(ei);
	m_deno = ent->directorylineno;
}

dCurve::~dCurve()
{
	delete m_crv;
	delete m_lines;
	delete m_snapl;
	delete m_snapp;
	delete m_params;
}
const DoubleVec & dCurve::GetParams()const
{
	return *m_params;
}
DoubleVec & dCurve::GetParam()
{
	return *m_params;
}
const SequentialLineSet & dCurve::GetLines()const
{
	return *m_lines;
}
SequentialLineSet & dCurve::GetLines()
{
	return *m_lines;
}
const CurvePointGenerator * dCurve::GetCurve()const
{
	return m_crv;
}
CurvePointGenerator * dCurve::GetCurve()
{
	return m_crv;
}
const PointSet & dCurve::GetSnapPointSet()const
{
	return *m_snapp;
}
PointSet & dCurve::GetSnapPointSet()
{
	return *m_snapp;
}
LineSet & dCurve::GetSnapLineSet()
{	
	return *m_snapl;
}
const LineSet & dCurve::GetSnapLineSet()const
{
	return *m_snapl;
}
int dCurve::GetDeNo()const
{
	return m_deno;
}
int dCurve::GetTrimSrcDeNo()const
{
	return m_trimSrcDeno;
}

void dCurve::Transform(const TransformationMatrix &tm)
{
	m_lines->Transform (tm);
	//m_quickLines->Transform (tm);
	m_crv->IncrementTransform (tm);
	m_snapl->Transform (tm);
	m_snapp->Transform (tm);
}

void dCurve::MakeCurve(IgesEntity *ent,IgesEntitySet *entset,bool isReversed)
{
	m_crv = CurvePointGeneratorFactory::MakeCurve (ent,*entset);
	if (isReversed != m_crv->GetReversed())
		Reverse(); //m_crv->SetReversed (isReversed);
}

void dCurve::MakeCurve(Edge *ent,IgesEntitySet *entset,bool isReversed,int ei)
{
	m_crv = CurvePointGeneratorFactory::MakeCurve (ent,*entset,ei,m_tol);
	if (isReversed != m_crv->GetReversed())
		Reverse(); //m_crv->SetReversed (isReversed);
}

void dCurve::MakeLines()
{
	m_params = new DoubleVec;
	m_lines = new SequentialLineSet (2);
	//m_quickParam = new DoubleVec;
	//m_quickLines = new SequentialLineSet (2);
	if (m_crv->GetEnt()->entitytype == 110)
	{
		// line
	}
	else if (m_crv->GetEnt()->entitytype == 106)
	{
		//copious data
		CopiousData *tmp = (CopiousData *)(m_crv->GetEnt());
		m_lines->Reserve (tmp->m_N);
		m_params->Reserve (tmp->m_N);
		//m_quickLines->Reserve (tmp->m_N);
		//m_quickParam->Reserve (tmp->m_N);
	}
	else
	{
		m_lines->Reserve (100);
		m_params->Reserve(100);
		//m_quickLines->Reserve (50);
		//m_quickParam->Reserve (50);
	}	
	m_crv->Digitise (m_tol , *m_lines , *m_params);	
	//m_crv->Digitise (m_roughTol , *m_quickLines,*m_quickParam);
}
void dCurve::RemakeLines(const double tol)//,vreal roughTol)
{
	try
	{
		DoubleVec *params = new DoubleVec;
		SequentialLineSet *lines = new SequentialLineSet (2);
		//DoubleVec *quickParam = new DoubleVec;
		//SequentialLineSet *quickLines = new SequentialLineSet (2);
		if (m_crv->GetEnt()->entitytype == 110)
		{
			// line
		}
		else if (m_crv->GetEnt()->entitytype == 106)
		{
			//copious data
			CopiousData *tmp = (CopiousData *)(m_crv->GetEnt());
			lines->Reserve (tmp->m_N);
			params->Reserve (tmp->m_N);
			//quickLines->Reserve (tmp->m_N);
			//quickParam->Reserve (tmp->m_N);
		}
		else
		{
			lines->Reserve (100);
			params->Reserve (100);
			//quickLines->Reserve (50);
			//quickParam->Reserve(50);
		}	
		m_tol = tol;
		//m_roughTol = roughTol;
		//m_isTransformedBackUp = m_crv->IsTransformed();
		//if (m_isTransformedBackUp)
		//{
		//	m_tmBackUp = m_crv->GetTransformation();
		//	m_crv->ClearTransformation ();		
		//}
		//else
		//	m_tmBackUp = TransformationMatrix ();//Identity matrix
		m_crv->Digitise (m_tol , *lines , *params);
		//m_crv->Digitise (m_roughTol , *quickLines,*quickParam);
		delete m_params;
		delete m_lines;
		//delete m_quickParam;
		//delete m_quickLines;
		m_params = params;
		m_lines = lines;
		//m_quickParam = quickParam;
		//m_quickLines = quickLines;
		//if (m_isTransformedBackUp)
		//{
		//	m_crv->IncrementTransform (m_tmBackUp);
			//m_lines->Transform (m_tmBackUp);  // moved one level up to VCurve , after forming the display list transformation is done!
		//}
	}
	catch (VException &e)
	{
		//if (m_isTransformedBackUp)
		//	m_crv->IncrementTransform (m_tmBackUp);
		throw e;
	}
	catch (std::exception &e)
	{
		//if (m_isTransformedBackUp)
		//	m_crv->IncrementTransform (m_tmBackUp);
		throw e;
	}
	catch (...)
	{
		//if (m_isTransformedBackUp)
		//	m_crv->IncrementTransform (m_tmBackUp);
		throw VException (( IDS_DCuRemUnkExc),1);
	}
}

void dCurve::MakeSnapPoints()
{
	m_snapp = new PointSet(4);
	m_crv->GetSnapPoints(*m_snapp);
	std::vector<double> sharpCorPar;
	double parInc;
	this->GetSharpCorners (30.,sharpCorPar,VGlobalValues::ONEMICRON_TO_VUNIT * 10,parInc);
	size_t mysize = sharpCorPar.size();
	for (size_t i=0;i<mysize;++i)
	{
		VPoint corPoint = GetCurve()->GetPoint (sharpCorPar[i]);
		m_snapp->InsertPoint (corPoint);
	}
}

void dCurve::MakeSnapLines()
{
	m_snapl = new LineSet (1);
	m_crv->GetSnapLines(*m_snapl);
}

void dCurve::Reverse()
{
	m_crv->SetReversed(!m_crv->GetReversed());
	m_lines->Reverse();
	m_params->Reverse();
}

const IgesEntity * dCurve::GetIgesEntity() const
{
	return m_crv->GetEnt();
}

IgesEntity * dCurve::GetIgesEntity()
{
	return m_crv->GetEnt();
}

const IgesEntitySet * dCurve::GetIgesEntitySet() const
{
	return m_crv->GetEntSet();
}

double dCurve::GetTol ()const
{
	return m_tol;
}

void dCurve::TrimTo(vreal trimV0 , vreal trimV1)
{
	if (trimV0 < -EPSILON)
		trimV0 = 0.0;
	if (trimV0 > 1 + EPSILON)
		trimV0 = 1.;
	if (trimV1 < -EPSILON)
		trimV1 = 0.0;
	if (trimV1 > 1. + EPSILON)
		trimV1 = 1.;
	if (trimV1 - trimV0 < EPSILON)
		return;		// No trimming
		
	double trimV0_new = m_crv->ReverseIf(trimV0);
	double trimV1_new = m_crv->ReverseIf(trimV1);
	if (trimV0_new > trimV1_new)
		std::swap(trimV0_new,trimV1_new);
	double originalV0 = m_crv->GetV0();
	double originalV1 = m_crv->GetV1();
	double deltav = originalV1 - originalV0;
	double newV0 = originalV0 + deltav * trimV0_new;
	double newV1 = originalV0 + deltav * trimV1_new;
	m_crv->SetV0V1(newV0,newV1);
	RemakeLines(m_tol);//,m_roughTol);
}

bool dCurve::GetArcIf (VArc &a)
{
	IgesEntity *ent = GetCurve()->GetEnt();
	if (ent == 0)
		return false;
	if (ent->entitytype == 100)
	{
		ArcPointGenerator *apg = (ArcPointGenerator *)GetCurve();
		if (GetCurve()->GetReversed() == false)
			a = apg->arc;
		else
			a = apg->arc.GetReversed();
		apg->TransformIf(a);
		return true;
	}
	else if (ent->entitytype == 504)
	{
		// This is a trimmed curve
		TrimmedCurvePointGenerator *epg = (TrimmedCurvePointGenerator *)GetCurve();
		bool cktr = epg->GetArcIf (a);
		return cktr;
	}

	return false;
}

bool dCurve::GetLineIf (VLine &l)
{
	IgesEntity *ent = GetCurve()->GetEnt();
	if (ent == 0)
		return false;
	if (ent->entitytype == 110)
	{
		LinePointGenerator *lpg = (LinePointGenerator *)GetCurve();
		if (GetCurve()->GetReversed() == false)
			l = lpg->line;
		else
			l = lpg->line.GetReversed();
	
		lpg->TransformIf(l);
		return true;
	}
	else if (ent->entitytype == 504)
	{
		TrimmedCurvePointGenerator *epg = (TrimmedCurvePointGenerator *)GetCurve();
		bool ckline = epg->GetLineIf (l);
		return ckline;
	}
	
	return false;
}

dCurve::CurveType dCurve::GetCurveType()
{
	IgesEntity *ent = GetCurve()->GetEnt();
	if (ent == 0)
		return unknown;
	int crvtype = ent->entitytype;
	if (crvtype == 504)
	{
		IgesEntity *subent = ((TrimmedCurvePointGenerator *)GetCurve())->GetSrcCurvEnt();
		crvtype = subent->entitytype;
	}
	switch (crvtype)
	{
	case 100:
		return arc;
	case 110:
		return line;
	case 126:
		return bspline;
	case 106:
		return copiousdata;
	case 112:
		return parametricspline;
	case 104:
		return conicarc;
	};
	return unknown;
}

void dCurve::RollbackPreviousTransform ()
{
	if (m_isTransformedBackUp)
	{
		m_crv->ResetTransform (m_tmBackUp);
		m_lines->Transform (m_tmBackUp);
		//m_quickLines->Transform (m_tmBackUp);
	}
}

void dCurve::GetSharpCorners (double angleLimitInDeg , std::vector<double> &sharpCorPar , double aDistanceTol , double &parInc)
{
	// find a minimal increment for checking  
	 
	double totalLen = GetLines().GetLength();
	
	parInc = aDistanceTol / totalLen;
	if (parInc > VGlobalValues::ONEMICRON_TO_VUNIT * 10) 
		parInc = VGlobalValues::ONEMICRON_TO_VUNIT * 10;	// this may be because tolerance is big compared to totalLen then use a 1 / 100 ratio for epsilon

	double cosAngleLimit = cos (angleLimitInDeg * DEGREE_TO_RAD);

	// Try definition int.
	DoubleVec defint;
	GetCurve()->GetDefIntervals (defint);
	bool isClosedCurve = GetLines().Start()->Compare (*GetLines().End() ,  aDistanceTol);
	int mysize = defint.Size();
	for (int i=0;i<mysize;++i)
	{
		double curpar = defint.operator [](i);
		bool ck = GetCurve()->IsSharpCorner (cosAngleLimit , curpar , parInc , isClosedCurve);
		if (ck)
		{
			sharpCorPar.push_back (curpar);
		}
	}
}

bool dCurve::IsArc()const
{
	const IgesEntity *ent = GetCurve()->GetEnt();
	if (ent == 0)
		return false;
	if (ent->entitytype == 100)
	{
		return true;
	}
	else if (ent->entitytype == 126)
	{
		BSplineCurve *curBSpline = (BSplineCurve *)ent;
		return curBSpline->IsCircularArc();
	}
	else if (ent->entitytype == 504)
	{
		// This is a trimmed curve
		TrimmedCurvePointGenerator *epg = (TrimmedCurvePointGenerator *)GetCurve();
		bool cktr = epg->IsArc ();
		return cktr;
	}

	return false;
}
bool dCurve::IsLine()const
{
	const IgesEntity *ent = GetCurve()->GetEnt();
	if (ent == 0)
		return false;
	if (ent->entitytype == 110)
	{
		return true;
	}
	else if (ent->entitytype == 126)
	{
		BSplineCurve *curBSpline = (BSplineCurve *)ent;
		return curBSpline->IsLine();
	}
	else if (ent->entitytype == 504)
	{
		const TrimmedCurvePointGenerator *epg = (TrimmedCurvePointGenerator *)GetCurve();
		bool ckline = epg->IsLine ();
		return ckline;
	}
	
	return false;
}

bool dCurve::IsPolyline()const
{
	const IgesEntity *ent = GetCurve()->GetEnt();
	if (ent == 0)
		return false;
	if (ent->entitytype == 106)
	{
		return true;
	}
	else if (ent->entitytype == 126)
	{
		BSplineCurve *curBSpline = (BSplineCurve *)ent;
		return curBSpline->IsPolyline();
	}
	else if (ent->entitytype == 504)
	{
		const TrimmedCurvePointGenerator *epg = (TrimmedCurvePointGenerator *)GetCurve();
		return epg->IsPolyline();
	}
	
	return false;
}

const TransformationMatrix & dCurve::GetTmBackUp()const
{
	return m_tmBackUp;
}

//DoubleVec * dCurve::GetQuickParam()
//{
//	return m_quickParam;
//}
//
//SequentialLineSet * dCurve::GetQuickLines()
//{
//	return m_quickLines;
//}
//
//vreal dCurve::GetRoughTol()const
//{
//	return m_roughTol;
//}
//	
//void dCurve::SetRoughTol (vreal roughTol)
//{
//	m_roughTol = roughTol;
//}

int dCurve::GetEdgeiIf()
{
	try
	{
		TrimmedCurvePointGenerator *trcpg = static_cast<TrimmedCurvePointGenerator *>(m_crv);
		return trcpg->GetEi();
	}
	catch (...)
	{
		return -1;
	}
}

void dCurve::GetMinMax(std::vector<dCurve *> &dvec, VPoint &mn, VPoint &mx)
{
	mn = VPoint(MAX_NUMBER_LIMIT);
	mx = VPoint(MIN_NUMBER_LIMIT);
	int dsize = (int)dvec.size();
	for (int di = 0; di < dsize; ++di)
	{
		mn.ReplaceMin(dvec.at(di)->GetLines().GetMin());
		mx.ReplaceMax(dvec.at(di)->GetLines().GetMax());
	}
}
