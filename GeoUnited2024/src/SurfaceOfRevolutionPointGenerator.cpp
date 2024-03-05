// SurfaceOfRevolutionPointGenerator.cpp

#include "stdafx.h"
#include "SurfaceOfRevolutionPointGenerator.h"
#include "CoordinateSystem.h"
#include "DoubleVec.h"
#include "VRefTriangleMesh.h"
#include "ArcPointGenerator.h"
#include "RotationMatrix.h"
#include "SequentialLineSet.h"
#include "CurvePointGenerator.h"
#include "CurvePointGeneratorFactory.h"
#include "IgesEntity.h"
#include "CurvePointGenerator.h"
#include "ScaleMatrix.h"

SurfaceOfRevolutionPointGenerator::SurfaceOfRevolutionPointGenerator ( const SurfaceOfRevolution &surf , const VLine & _l , const IgesEntity *generatrixcurve , const IgesEntitySet & allent , const vreal & _sa , const vreal & _ta )
	: SurfacePointGenerator(0.0, 1.0, 0.0, TWO_PI), sa(_sa), ta(_ta), m_surf(surf)
 ,l (_l) 
{
	allent.MakeAllSubsetIndex0();
	m_generatrixCurve = generatrixcurve->MakeCopyAndAddToSet (allent , m_eSet);
	generatrix = CurvePointGeneratorFactory::MakeCurve( m_generatrixCurve , m_eSet);
	//generatrix->SetTransformationMatrixIf ();
	
	//if (m_generatrixCurve->entitytype == 100)
	//{
		//ArcPointGenerator *temp = (ArcPointGenerator *)generatrix;
		//double umargin = fabs(temp->arc.GetSweepAngle()) * DEGREE_TO_RAD; 
		//SetU1(umargin);
	//}
	SetU0(generatrix->GetV0());
	SetU1(generatrix->GetV1());
	SetV0(sa);
	SetV1(ta);

	m_nonAxisPoint = GetNonAxisPoint();	
	VVector vec = m_nonAxisPoint - l.GetP2();
	VVector dirz = l.GetV().Unit();	
	VVector diry = dirz ^ vec;
	if (diry.GetLength2() < EPSILON)
		throw VException (( IDS_SurConExc),1);
	diry.NormThis();
	VVector dirx = diry ^ dirz;
	dirx.NormThis();
	localcoord = new CoordinateSystem ( l.GetP1() , dirx , diry , dirz );
	//localcoord = new CoordinateSystem(CoordinateSystem::ArbitraryAxisAlg(dirz));
	localcoord->SetOrigin(l.GetP1());

	m_deltaa = ta - sa;
	
	//if (fabs(sa) < EPSILON && fabs (ta-TWO_PI) < EPSILON)
	//{
	SurfacePointGenerator::SetIsPeriodicInV(true);
	SurfacePointGenerator::SetVPeriod (TWO_PI/m_deltaa);	
	//}
	vreal tol = 1.e-6;
	if (generatrix->GetPoint(0.0).Compare(generatrix->GetPoint(1.0), tol) == true)
	{
		SurfacePointGenerator::SetIsPeriodicInU(true);
		SurfacePointGenerator::SetUPeriod(1.0);
	}
}
VPoint SurfaceOfRevolutionPointGenerator::GetNonAxisPoint()
{
	VPoint dif (0.);
	VPoint nap;
	vreal t = 0.0;
	vreal stp = 0.05;
	VPoint luvec = l.GetV().Unit();
	while (dif.GetLength() < 1.e-2 && t <= 1.)
	{
		nap = generatrix->GetPoint (t);
		t+=stp;
		VPoint dvec = nap - l.GetP1();
		if (dvec.Compare(VPoint(0.0, 0.0, 0.0), 1.e-4) == false)
			dif = dvec.Unit() ^ luvec;
		else
			dif = VPoint(0.0, 0.0, 0.0);
	}
	//VPoint nap;
	//vreal prjlen = 1.0;
	//vreal t = 0.0;
	//vreal stp = 0.02;
	//VPoint uax = l.GetV().Unit();
	//while (fabs(prjlen - 1.) < 1.e-2)
	//{
	//	if (t > 1.0)
	//		throw VException("SurfaceOfRevolutionPointGenerator::GetNonAxisPoint exception , t=1 no solution",1);
	//	nap = generatrix->GetPoint(t);
	//	VPoint tmpvec = nap - l.GetP1();
	//	tmpvec.NormThis();
	//	prjlen = fabs(uax%tmpvec);
	//	t += stp;
	//}
	return nap;
}

SurfaceOfRevolutionPointGenerator::~SurfaceOfRevolutionPointGenerator()
{
	delete generatrix;
	delete localcoord;
}

void SurfaceOfRevolutionPointGenerator::GetUDefIntervals ( DoubleVec & defint ) const
{
	generatrix->GetDefIntervals ( defint );
}

const int ARC_BREAK_NO=128;

void SurfaceOfRevolutionPointGenerator::GetVDefIntervals ( DoubleVec & defint ) const
{
	vreal divideNof = ARC_BREAK_NO * m_deltaa / TWO_PI;
	int divideNo = static_cast<int>(divideNof);
	vreal step=1./divideNo;
	for(int i=0;i<divideNo+1;i++)
	{
		vreal 	currangle=step*i;
		defint.Addvreal(currangle);
		//defint.Addvreal ( 0.0 );
		//defint.Addvreal ( 1.0 );
	}
}

VPoint SurfaceOfRevolutionPointGenerator::GetPoint ( const vreal & u , const vreal & v ) const
{
	vreal su,sv;
	ShiftParIf(u,v,su,sv);
	VPoint gp = generatrix->GetPoint ( su );
	VPoint local = localcoord->AbsoluteCoordToRelativeCoord ( gp );
	
	vreal angle = GetAngle ( sv );
	//vreal radialLen = sqrt(local.x()*local.x() + local.y()*local.y());
	vreal radialLen = sqrt(local.x()*local.x()+local.y()*local.y());
	VPoint rp ( radialLen*cos(angle) , radialLen*sin(angle) , local.z() );
	VPoint result = localcoord->RelativeCoordToAbsoluteCoord ( rp );
	return result;
}

VPoint SurfaceOfRevolutionPointGenerator::GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const
{
	return GetNormalAppr(u,v,surfpoint);
	
	//vreal angle = GetAngle (v);
	//VPoint rp (cos(angle) , sin(angle) , 0.0);
	//VVector result = localcoord->RelativeVectorToAbsoluteVector (rp);
	//return result;

	//VPoint uderv , vderv;
	//vreal ubef = u - VGlobalValues::MYDERV_DIFF;
	//vreal vbef = v - VGlobalValues::MYDERV_DIFF;

	//if ( ubef > 0.0 )
	//{
	//	VPoint dp = GetPoint ( ubef , v );
	//	uderv = surfpoint - dp;
	//	uderv.NormThis();
	//}
	//else
	//{
	//	vreal uaft = u + VGlobalValues::MYDERV_DIFF;
	//	VPoint dp = GetPoint ( uaft , v );
	//	uderv = dp - surfpoint;
	//	uderv.NormThis();
	//}

	//if ( vbef > 0.0 )
	//{
	//	VPoint dp = GetPoint ( u /*unext*/ , vbef );
	//	vderv = surfpoint - dp;
	//	vderv.NormThis();
	//}
	//else
	//{
	//	vreal vaft = v + VGlobalValues::MYDERV_DIFF;
	//	VPoint dp = GetPoint ( u /*unext*/ , vaft );
	//	vderv = dp - surfpoint;
	//	vderv.NormThis();
	//}

	//VPoint result = uderv ^ vderv;
	//result.NormThis();

	//return result;
}

vreal SurfaceOfRevolutionPointGenerator::GetAngle ( const vreal & v ) const
{
	return ( sa + ( m_deltaa * v ) );
	//return v * TWO_PI;
}

VRefTriangleMesh * SurfaceOfRevolutionPointGenerator::Digitise(const vreal &tol)const
{
	return GenericDigitise (tol);
	//VPoint firstpoint = generatrix.GetPoint ( 0.0 );	
	//VVector vec = firstpoint - l.GetP1();
	//VPoint rc=l.GetP1();
	//UnitVector rax = l.GetV().Unit();
	//VVector vec2 = rax^vec;
	////VArc tmparc (rc,vec.GetLength(),rax,firstpoint,m_deltaa*RAD_TO_DEGREE);
	////ArcPointGenerator apg(tmparc);
	//int segno = 40;//= apg.GetDigitiseSegmentNo (tol);
	//SequentialLineSet sl (100);
	//PointSet tn (100);
	//DoubleVec params;	
	//generatrix.Digitise(tol,sl,tn,params);
	//int mysize = sl.GetPointSize();
	//VRefTriangleMesh *result = new VRefTriangleMesh(mysize * (segno+1));
	//vreal anglestep = (m_deltaa / segno) * RAD_TO_DEGREE;
	//vreal parstep = 1. / segno;
	//for (int i=0;i<segno+1;i++)
	//{
	//	vreal currv = static_cast<vreal>(i) * parstep;
	//	if (i!=0)
	//	{
	//		RotationMatrix RM (rc,rax,anglestep);	
	//		sl.Transform(RM);
	//		tn.TransformAsVectors (RM);
	//		vec2 = RM.TransformVec (vec2);
	//	}
	//
	//	//result->surfpack.modelpoints.AddBack (sl.GetPointSet());
	//	//result->surfpack.normals.AddBack (tn);
	//	for (int j=0; j<mysize; j++)
	//	{
	//		const vreal &currparu = params[j];
	//		const VPoint *currm = sl.GetPoint(j);
	//		const VVector *currt = tn[j];
	//		VVector currn = *currt ^ vec2;
	//		currn.NormThis();
	//		VPoint2d currpar (currparu , currv);
	//		result->surfpack.parampoints.InsertPoint (currpar);
	//		result->surfpack.modelpoints.InsertPoint (*currm);
	//		result->surfpack.normals.InsertPoint (currn);
	//	}
	//}
	//// making ref triangles
	//for (int i=0; i<segno; i++)
	//{
	//	int basei = i * mysize;	
	//	int nexti = basei + mysize;
	//	for (int j=0; j<mysize-1;j++)
	//	{
	//		RefTriangle reft1 (basei+j , basei+j+1 , nexti+j+1); 
	//		RefTriangle reft2 (nexti+j+1 , nexti+j , basei+j);
	//		result->AddTriangle (reft1);
	//		result->AddTriangle (reft2);
	//	}
	//}
	//return result;
}

void SurfaceOfRevolutionPointGenerator::GetPointAndNormal ( const vreal &u , const vreal &v , VPoint &surfpoint , VVector &normal) const
{
	surfpoint = GetPoint (u,v);
	normal = GetNormal (u,v,surfpoint);
	//VPoint gp,t1;
	//generatrix->GetPointAndTangent (u,gp,t1);
	//VPoint local = localcoord->AbsoluteCoordToRelativeCoord (gp);
	//VPoint localend = localcoord->AbsoluteCoordToRelativeCoord(gp+t1);

	//vreal angle = GetAngle ( v );
	//vreal radialLen = sqrt(local.x()*local.x() + local.y()*local.y());
	//vreal radialLen2 = sqrt (localend.x()*localend.x() + localend.y()*localend.y());
	//
	//vreal costeta = cos(angle);
	//vreal sinteta = sqrt (1. - costeta*costeta);
	//vreal c1 = radialLen*costeta;
	//vreal c2 = radialLen*sinteta;
	//VPoint rp ( c1 , c2 , local.z() );
	//VPoint rpend (radialLen2*costeta , radialLen2*sinteta , localend.z());
	//VVector localt2 (-c2,c1,0.0);
	//VVector n = localt2 ^ (rpend - rp);
	//n.NormThis();
	//surfpoint = localcoord->RelativeCoordToAbsoluteCoord ( rp );
	//normal = localcoord->RelativeVectorToAbsoluteVector ( n );	
}

//bool SurfaceOfRevolutionPointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return GenericUnproject(mp, u, v, tol);
//}

IgesEntity * SurfaceOfRevolutionPointGenerator::GetIgesEntity()
{
	return (IgesEntity *)&m_surf;
}