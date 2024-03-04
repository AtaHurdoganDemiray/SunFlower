// UntrimmedSurface.cpp

#include "stdafx.h"
#include "IgesEntity.h"
#include "IgesEntitySet.h"
#include "UntrimmedSurface.h"
#include "BSplineSurface.h"
#include "BSplineSurfacePointGenerator.h"
#include "ProcessedTabulatedCylinder.h"
#include "TabulatedCylinderPointGenerator.h"
#include "Line.h"
#include "VLine.h"
#include "SurfaceOfRevolution.h"
#include "SurfaceOfRevolutionPointGenerator.h"
#include "OffsetSurface.h"
#include "OffsetSurfacePointGenerator.h"
#include "PlaneEntityPointGenerator.h"
#include "RuledSurfacePointGenerator.h"
#include "ParametricSplineSurfacePointGenerator.h"
#include "SequentialLineSet.h"
#include "CopiousDataFactory.h"
#include "CopiousData.h"
#include "VRefTriangleMesh.h"
#include "DoubleVec.h"
#include "HelicalRevolvedSection.h"
#include "PlaneSurfacePointGenerator.h"
#include "RightCircularCylindricalSurface.h"
#include "RightCircularConicalSurface.h"
#include "SphericalSurface.h"
#include "ToroidalSurface.h"
#include "PlaneSurfacePointGenerator.h"
#include "RightCylindricalSurfacePointGenerator.h"
#include "RightConicalSurfacePointGenerator.h"
#include "SphereSurfacePointGenerator.h"
#include "ToroidalSurfacePointGenerator.h"
#include "SurfaceSpace2.h"

UntrimmedSurface::UntrimmedSurface()
	:m_surf(0),digitiser(0),istm_given (false),m_isTmBaseGiven(false), m_surfSpace(0)
{

}

UntrimmedSurface::UntrimmedSurface ( const IgesEntity * newsurf , IgesEntitySet & allent )
:m_surf (newsurf),istm_given(false),m_isTmBaseGiven(false)
{
	SetTransformationMatrixBase ( allent , newsurf );
	if ( newsurf->entitytype == 128 )
	{
		digitiser = new BSplineSurfacePointGenerator ( (const BSplineSurface &)(*newsurf) );	
	}
	else if ( newsurf->entitytype == 122 )
	{
		ProcessedTabulatedCylinder ptc ( (const TabulatedCylinder &)(*newsurf) , allent );
		digitiser = new TabulatedCylinderPointGenerator ( ptc ); 
	}
	else if ( newsurf->entitytype == 120 )
	{
		SurfaceOfRevolution * sr = (SurfaceOfRevolution *) (newsurf);
		if (sr->DE.m_formNo != 4)
		{
			Line * il = (Line *)(allent.GetEntityPointer ( sr->m_L ));
			VPoint p1 ( il->X1 , il->Y1 , il->Z1 );
			VPoint p2 ( il->X2 , il->Y2 , il->Z2 );
			VLine l ( p1 , p2 );
			IgesEntity * generatrix = allent.GetEntityPointer ( sr->m_C );
			vreal sa = sr->SA;
			vreal ta = sr->TA;
			digitiser = new SurfaceOfRevolutionPointGenerator (*sr, l , generatrix , allent , sa , ta );
		}
		else
		{
			// This is a helical revolution
			digitiser = new HelicalRevolvedSection (sr,allent);
		}
	}
	else if ( newsurf->entitytype == 140 )
	{
		OffsetSurface * os = (OffsetSurface *) (newsurf);
		VPoint offsetindicator ( os->m_NX
								, os->m_NY
								, os->m_NZ );
		vreal offsetvalue = static_cast<vreal>(os->m_D);
		IgesEntity * basesurf = allent.GetEntityPointer ( os->m_DE );
		digitiser = new OffsetSurfacePointGenerator (*os , basesurf , allent 
										, offsetindicator , offsetvalue );

	}
	else if ( newsurf->entitytype == 108)
	{
		PlaneEntity *plnent = (PlaneEntity *)(newsurf);
		digitiser = new PlaneEntityPointGenerator (*plnent,allent);
	}
	else if ( newsurf->entitytype == 118)
	{
		RuledSurface *rs = (RuledSurface *)(newsurf);
		digitiser = new RuledSurfacePointGenerator (rs , allent);
	}
	else if ( newsurf->entitytype == 114)
	{
		ParametricSplineSurface *ps = (ParametricSplineSurface *)newsurf;
		digitiser = new ParametricSplineSurfacePointGenerator(*ps);
	}
	else if (newsurf->entitytype == 190)
	{
		PlaneSurface *ps = (PlaneSurface *)newsurf;
		digitiser = new PlaneSurfacePointGenerator(*ps, allent);
	}
	else if (newsurf->entitytype == 192)
	{
		RightCircularCylindricalSurface *rccs = (RightCircularCylindricalSurface *)newsurf;
		digitiser = new RightCylindricalSurfacePointGenerator(*rccs, allent);
	}
	else if (newsurf->entitytype == 194)
	{
		RightCircularConicalSurface *rccs = (RightCircularConicalSurface*)newsurf;
		digitiser = new RightConicalSurfacePointGenerator(*rccs, allent);
	}
	else if (newsurf->entitytype == 196)
	{
		SphericalSurface *ss = (SphericalSurface *)newsurf;
		digitiser =  new SphereSurfacePointGenerator(*ss, allent);
	}
	else if (newsurf->entitytype == 198)
	{
		ToroidalSurface *trs = (ToroidalSurface *)newsurf;
		digitiser = new ToroidalSurfacePointGenerator(*trs, allent);
	}
	else
	{
		digitiser = 0;
		throw VException("UntrimmedSurface constructor exception , surface point generator is null. Unsupported iges entity type for surface",1);
	}
	SetUVMarginParams ();

	int m = 60;
	int n = 60;
	m_surfSpace = new SurfaceSpace2(m, n, *this);
}

UntrimmedSurface::UntrimmedSurface (SurfacePointGenerator *surfPointGen)
	:digitiser(surfPointGen),m_surf (0),istm_given(false),m_isTmBaseGiven(false)
{
	int m = 60;
	int n = 60;
	m_surfSpace = new SurfaceSpace2(m, n, *this);
}

//UntrimmedSurface::UntrimmedSurface(const IgesEntity *sectioncurve , const IgesEntitySet &allent 
//								   , double r0 , double c0 , double z0 , double z1 , double pitch)
//{
//	m_surf = 0;
//	digitiser = new HelicalRevolvedSection (sectioncurve , allent , r0 , c0 , z0 , z1 , pitch);
//	
//
//}

UntrimmedSurface::~UntrimmedSurface()
{
	delete digitiser;
};

//#############################################################################

/*
const IgesEntity * UntrimmedSurface::Surf() const
{
	return psurf;
}
*/
//#############################################################################

const vreal & UntrimmedSurface::U0() const
{
	return (digitiser->U0());
}

//#############################################################################

const vreal & UntrimmedSurface::U1() const
{
	return (digitiser->U1());
}

//#############################################################################

const vreal & UntrimmedSurface::V0() const
{
	return (digitiser->V0());
}

//#############################################################################

const vreal & UntrimmedSurface::V1() const
{
	return (digitiser->V1());
}

//#############################################################################
// normals are given as unit vector
void UntrimmedSurface::GetPointAndNormal 
( const vreal &f_u , const vreal & f_v , VPoint &mypoint , VPoint &mynormal ) const
{
	VPoint tmppoint,tmpnormal;
	digitiser->GetPointAndNormal (f_u,f_v,tmppoint,tmpnormal);
	if ( m_isTmBaseGiven )
	{
		tmppoint = m_tmBase.Transform (tmppoint);
		tmpnormal = m_tmBase.TransformVec(tmpnormal);
	}
	if ( istm_given )
	{
		tmppoint = m_tm.Transform ( tmppoint );
		tmpnormal = m_tm.TransformVec ( tmpnormal );
	}

	mypoint.x(tmppoint.x());
	mypoint.y(tmppoint.y());
	mypoint.z(tmppoint.z());
	
	mynormal.x(tmpnormal.x());
	mynormal.y(tmpnormal.y());
	mynormal.z(tmpnormal.z());

}

//#############################################################################

VPoint UntrimmedSurface::GetPoint ( const vreal &f_u , const vreal & f_v ) const
{
	VPoint tmppoint = digitiser->GetPoint ( f_u , f_v );
	if ( m_isTmBaseGiven )
	{
		tmppoint = m_tmBase.Transform (tmppoint);
	}
	if ( istm_given )
	{
		tmppoint = m_tm.Transform ( tmppoint );
	}	
	return tmppoint;
}
// returns unit vector
VPoint UntrimmedSurface::GetNormal ( const vreal & u , const vreal & v 
							, const VPoint & surfpoint ) const
{
	VPoint mynormal = digitiser->GetNormal ( u , v , surfpoint );
	if (m_isTmBaseGiven)
	{
		mynormal = m_tmBase.TransformVec (mynormal);
	}
	if ( istm_given )
	{
		mynormal = m_tm.TransformVec (mynormal);
	}
	return mynormal;
}

void UntrimmedSurface::GetUDefIntervals ( DoubleVec & defint ) const
{
	digitiser->GetUDefIntervals ( defint );
}

void UntrimmedSurface::GetVDefIntervals ( DoubleVec & defint ) const
{
	digitiser->GetVDefIntervals ( defint );
}

void UntrimmedSurface::SetTransformationMatrixBase ( const IgesEntitySet & allent 
											  , const IgesEntity * current )
{
	if ( current->DE.pXform > 0 )
	{
		igesTransformationMatrix * igstm = ( igesTransformationMatrix *) 
		allent.GetEntityPointer ( current->DE.pXform );
		
		if ( !igstm )
		{
			//istm_given = false;
			m_isTmBaseGiven = false;
			return;
		}

		TransformationMatrix tmp(*igstm);
		//m_tm = tmp;
		//istm_given = true;
		m_tmBase = tmp;
		m_isTmBaseGiven = true;
	}
	else
	{
		//istm_given = false;
		m_isTmBaseGiven = false;
	}
}

void UntrimmedSurface::ResetTransform ( const TransformationMatrix &tm )
{
	istm_given = true;
	m_tm = tm;
}
	
void UntrimmedSurface::IncrementTransform (const TransformationMatrix &tm)
{
	m_tm = tm * m_tm;
	istm_given = true;
}
	
const TransformationMatrix & UntrimmedSurface::GetTransformation()const
{
	return m_tm;
}
	
void UntrimmedSurface::ClearTransformation()
{
	TransformationMatrix tmp;
	m_tm = tmp;
	istm_given = false;
}
	
bool UntrimmedSurface::IsTransformed()const
{
	return istm_given;
}

VRefTriangleMesh * UntrimmedSurface::Digitise(const vreal &tol) const
{
	if (istm_given == false)
		return digitiser->GenericDigitise(tol);
	else
	{
		VRefTriangleMesh *res = digitiser->GenericDigitise(tol);
		res->Transform (m_tm);
		return res;
	}
}

//void UntrimmedSurface::GetSnapEnt (const IgesEntity * current , const IgesEntitySet *entset , PointSet &snapp , LineSet &snapl)
//{
//	digitiser->GetSnapEnt (current , entset , snapp , snapl);
//}

SurfacePointGenerator & UntrimmedSurface::GetDigitiser()
{
	return *digitiser;
}
const SurfacePointGenerator & UntrimmedSurface::GetDigitiser()const
{
	return *digitiser;
}

int UntrimmedSurface::UNTRIMMED_SURF_BOUNDARY_STEP = 40;
vreal UntrimmedSurface::UNTRIMMED_SURF_PAR_STEP = 1. / UNTRIMMED_SURF_BOUNDARY_STEP;

CopiousData * UntrimmedSurface::MakeIsoParametricCurve(UntrimmedSurface::IsoParametricType ityp,IgesEntitySet &entset,const vreal tol,int &refi) 
{
	//int mysize = (UNTRIMMED_SURF_BOUNDARY_STEP + 2) ;
	//vreal parStep = 1. / UNTRIMMED_SURF_BOUNDARY_STEP;
	vreal par;
	VPoint2d currpar;
	VPoint currp;
	SequentialLineSet sl(UNTRIMMED_SURF_BOUNDARY_STEP);
	DoubleVec udef,vdef;
	DoubleVec todo;
	digitiser->GetUDefIntervals(udef);
	digitiser->GetVDefIntervals(vdef);
	int mysize;
	vreal p1,p2;
	switch (ityp)
	{
		case (v0uchanging):
			mysize = udef.Size();
			// v = 0.0
			for (int i=0;i<mysize-1;i++)
			{
				p1 = udef[i];
				p2 = udef[i+1];
				int segNo = CalculateDivideCountForInterval (p1,0.0,p2,0.0,tol);
				InsertSegments (segNo,p1,p2,todo);
			}
			todo.Addvreal (1.0);
			mysize = todo.Size();
			for (int i=0;i<mysize;++i)
			{
				par = todo[i];
				currp = GetPoint(par,0.0);
				sl.InsertPoint(currp);
			}
		break;
		case (u1vchanging):
			mysize = vdef.Size();
			// u = 1.0
			for (int i=0;i<mysize-1;i++)
			{
				p1 = vdef[i];
				p2 = vdef[i+1];
				int segNo = CalculateDivideCountForInterval (1.0,p1,1.0,p2,tol);
				InsertSegments (segNo,p1,p2,todo);
			}
			todo.Addvreal (1.0);
			mysize = todo.Size();
			for (int i=0;i<mysize;++i)
			{
				par = todo[i];
				currp = GetPoint(1.0,par);
				sl.InsertPoint(currp);
			}
		break;
		case (v1uchanging):
			// v = 1.0
			mysize = udef.Size();
			for (int i=0;i<mysize-1;i++)
			{
				p1 = udef[i];
				p2 = udef[i+1];
				int segNo = CalculateDivideCountForInterval (p1,1.0,p2,1.0,tol);
				InsertSegments (segNo,p1,p2,todo);
			}
			todo.Addvreal (1.0);
			mysize = todo.Size();
			for (int i=0;i<mysize;++i)
			{
				par = todo[i];
				currp = GetPoint(par,1.0);
				sl.InsertPoint(currp);
			}
		break;
		case (u0vchanging):
			mysize = vdef.Size();
			// u = 0.0
			for (int i=0;i<mysize-1;i++)
			{
				p1 = vdef[i];
				p2 = vdef[i+1];
				int segNo = CalculateDivideCountForInterval (0.0,p1,0.0,p2,tol);
				InsertSegments (segNo,p1,p2,todo);
			}
			todo.Addvreal (1.0);
			mysize = todo.Size();
			for (int i=0;i<mysize;++i)
			{
				par = todo[i];
				currp = GetPoint(0.0,par);
				sl.InsertPoint(currp);
			}
		break;

	}

	CopiousDataFactory myfactory;
	CopiousData * newCop = myfactory.Make (sl);
	refi = entset.InsertEntity (newCop);
	entset.MakePartOfTrimmedSurface(newCop);
	//Curve *newIsoCrv = new Curve (newCop , entset);
	return newCop;
}

VRefTriangleMesh * UntrimmedSurface::DigitiseForModelSpaceStep (const vreal &modelSpaceStep)const
{
	VRefTriangleMesh *newmesh = digitiser->DigitiseForModelSpaceStep (modelSpaceStep);
	if (istm_given)
		newmesh->Transform (m_tm);
	return newmesh;
}

int UntrimmedSurface::GetSurfDe ()const
{
	if (m_surf)
		return m_surf->directorylineno;
	else 
		return 0;
}

//void UntrimmedSurface::SetTransformationMatrixIf()
//{
//	
//}

int UntrimmedSurface::CalculateDivideCountForInterval  (const vreal u1 , const vreal v1
														   , const vreal u2 , const vreal v2
														   , const vreal tol)const
{
	// linear calculation
	vreal vmid = (v1+v2)*0.5;
	vreal umid = (u1+u2)*0.5;
	VPoint nominalPoint = this->GetPoint (umid,vmid);
	VPoint p1 = GetPoint (u1,v1);
	VPoint p2 = GetPoint (u2,v2);
	VPoint apprPoint = (p1+p2)*0.5;
	vreal err = (nominalPoint-apprPoint).GetLength();
	if (fabs(err)<EPSILON)
		return 1;
	vreal totalLength = fabs (v2-v1)>fabs(u2-u1)?fabs(v2-v1):fabs(u2-u1);
	vreal subLength = tol * totalLength / err;
	int segmentCount = (int)(totalLength / subLength);
	return segmentCount+1;
}

void UntrimmedSurface::InsertSegments (int segNo , const vreal &p1 , const vreal &p2,DoubleVec &param2)const
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

//void UntrimmedSurface::SetUParShift (vreal uParShift)
//{
//	digitiser->SetUParShift(uParShift);
//}
//
//vreal UntrimmedSurface::GetUParShift ()const
//{
//	return digitiser->GetUParShift ();
//}
//
//void UntrimmedSurface::SetVParShift (vreal vParShift)
//{
//	digitiser->SetVParShift (vParShift);
//}
//
//vreal UntrimmedSurface::GetVParShift ()const
//{
//	return digitiser->GetVParShift();
//}

VPoint2d UntrimmedSurface::RePar (const VPoint &p)const
{
	VPoint2d repar ( ( p.x() - m_P0.x() ) * m_dUinv
		, ( p.y() - m_P0.y() ) * m_dVinv);
	return repar;	
}

void UntrimmedSurface::SetUVMarginParams ()
{
	m_P0.x (U0());
	m_P0.y (V0());
	m_dUinv = 1.0 / (U1() - U0());
	m_dVinv = 1.0 / (V1() - V0());
}

void UntrimmedSurface::SetTmBase (const TransformationMatrix &tm)
{
	m_tmBase = tm;
}

bool UntrimmedSurface::GetIsTmBaseGiven()
{
	return m_isTmBaseGiven;
}

const TransformationMatrix & UntrimmedSurface::GetTmBase ()const
{
	return m_tmBase;
}

int UntrimmedSurface::GetSurfType()const
{
	if (m_surf)
		return m_surf->entitytype;
	else
		return 0;
}

bool UntrimmedSurface::GenericUnproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
{
	//if (m_surfSpace == 0)
	//{

	//}
	try
	{
		VPoint2d parPoint;
		bool ck = m_surfSpace->Unproject(mp, tol, parPoint);
		u = parPoint.x();
		v = parPoint.y();
		return true;
	}
	catch (VException &e)
	{
		if (e.GetNumber() == SurfaceSpace2::m_divergeException)
		{
			if (m_surfSpace->GetM() == 180)
				throw VException("SurfacePointGenerator::GenericUnproject exception , diverge after increasing initial points", 1);
			// increase initial point count
			int mm = 180;
			int nn = 180;
			m_surfSpace = new SurfaceSpace2(mm, nn, *this);
			VPoint2d parPoint2;
			bool ck2 = m_surfSpace->Unproject(mp, tol, parPoint2);
			if (ck2 == false)
				throw VException("SurfacePointGenerator::GenericUnproject exception , diverge after increasing initial points", 1);
			u = parPoint2.x();
			v = parPoint2.y();
			return true;
		}
		else
			throw VException("SurfacePointGenerator::GenericUnproject exception , unknown exception", 1);
	}
	catch (...)
	{
		throw VException("SurfacePointGenerator::GenericUnproject exception , unknown exception", 1);
	}
}