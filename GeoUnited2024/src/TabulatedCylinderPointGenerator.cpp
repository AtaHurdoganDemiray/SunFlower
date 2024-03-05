// TabulatedCylinderPointGenerator.cpp

#include "stdafx.h"
#include "TabulatedCylinderPointGenerator.h"
#include "ProcessedTabulatedCylinder.h"
#include "TabulatedCylinder.h"
#include "CurvePointGenerator.h"
#include "DoubleVec.h"

#include "VRefTriangleMesh.h"
#include "SequentialLineSet.h"
#include "CurvePointGeneratorFactory.h"
#include <stdexcept>

TabulatedCylinderPointGenerator::TabulatedCylinderPointGenerator ( ProcessedTabulatedCylinder & surf )
	:SurfacePointGenerator(0.0, 1.0, 0.0, 1.0), m_surf(surf.surf)
{
	surf.allent.MakeAllSubsetIndex0();
	m_toExtrude = surf.crv->MakeCopyAndAddToSet (surf.allent , m_eSet);
	pcrv = CurvePointGeneratorFactory::MakeCurve ( m_toExtrude , m_eSet );
	//pcrv->SetTransformationMatrixIf ();
	VPoint firstpoint = pcrv->GetPoint ( 0.0 );
	VPoint lastpoint ( surf.surf.LX , surf.surf.LY , surf.surf.LZ );
	direct = lastpoint - firstpoint;
}

TabulatedCylinderPointGenerator::~TabulatedCylinderPointGenerator ()
{
	delete pcrv;
}

VPoint TabulatedCylinderPointGenerator::GetPoint ( const vreal & u , const vreal & v ) const
{
	try
	{	
		vreal su,sv;
		ShiftParIf(u,v,su,sv);
		VPoint up = pcrv->GetPoint ( su );
		VPoint vp = direct * sv;
		return (up + vp);
	}
	catch ( exception & exc )
	{
		throw ( exc );
	}
	catch (...)
	{		
		throw UntypedException(( IDS_UnkExcInTabGet),999);
	}
}

VPoint TabulatedCylinderPointGenerator::GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const
{
	return GetNormalAppr(u,v,surfpoint);
	
	/*VPoint uderv;
	vreal ubef = u - VGlobalValues::MYDERV_DIFF;

	if ( ubef > 0.0 )
	{
		VPoint dp = pcrv->GetPoint ( ubef);
		dp += (direct*v);
		uderv = surfpoint - dp;
		uderv.NormThis();
	}
	else
	{
		vreal uaft = u + VGlobalValues::MYDERV_DIFF;
		VPoint dp = pcrv->GetPoint ( uaft );
		dp += (direct*v);
		uderv = dp - surfpoint;
		uderv.NormThis();
	}

	VPoint result = uderv ^ direct;
	result.NormThis();

	return result;*/
}

void TabulatedCylinderPointGenerator::GetUDefIntervals ( DoubleVec & defint ) const
{
	pcrv->GetDefIntervals ( defint );
}

void TabulatedCylinderPointGenerator::GetVDefIntervals ( DoubleVec & defint ) const
{
	defint.Addvreal (0.0);
	defint.Addvreal (1.0);
}

//VRefTriangleMesh * TabulatedCylinderPointGenerator::Digitise (const vreal &tol) const
//{
//	SequentialLineSet c(100);
//	PointSet tng (100);
//	DoubleVec p;
//	pcrv->Digitise (tol,c,tng,p);
//	int mysize = c.GetPointSize();
//	VRefTriangleMesh * result = new VRefTriangleMesh (mysize*2);
//	SurfPointsPack &sp = result->surfpack;
//	
//	const vreal &par1 = p[0];
//	const VPoint *p1 = c.Start();
//	const VPoint *t1 = tng[0];
//	VVector n = direct ^ *t1;
//	n.NormThis();
//	VPoint p1up = *p1 + direct;
//	VPoint2d vpar1 (par1,0.0);
//	VPoint2d vpar1up (par1,1.0);
//	sp.InsertPoint (vpar1,*p1,n);
//	sp.InsertPoint (vpar1up,p1up,n);
//
//	for (int i=1; i<mysize; i++)
//	{
//		const vreal &par2 = p[i];
//		const VPoint *p2 = c.GetPoint(i);
//		const VPoint *t2 = tng[i];
//		VPoint p2up = (*p2)+direct;
//		VVector n = direct ^ (*t2);
//		n.NormThis();
//		VPoint2d vpar2 (par2,0.0);
//		VPoint2d vpar2up (par2,1.0);
//		sp.InsertPoint (vpar2,*p2,n);
//		sp.InsertPoint (vpar2up,p2up,n);
//		int basei = i*2;
//		RefTriangle r1 (basei-2,basei,basei+1);
//		RefTriangle r2 (basei+1,basei-1,basei-2);
//		result->AddTriangle(r1);
//		result->AddTriangle(r2);
//	}
//	//FormRefTriangles (result);
//	return result;
//}

VRefTriangleMesh * TabulatedCylinderPointGenerator::Digitise (const vreal &tol) const
{
	return GenericDigitise (tol);
	//SequentialLineSet c(100);
	//PointSet t(100);
	//DoubleVec p;
	//pcrv->Digitise (tol,c,t,p);
	//int mysize = c.GetPointSize();
	//VRefTriangleMesh * result = new VRefTriangleMesh (mysize*2);
	//SurfPointsPack &sp = result->surfpack;
	//for (int i=0; i<mysize-1; i++)
	//{
	//	const vreal &par1 = p[i];
	//	const vreal &par2 = p[i+1];
	//	const VPoint *p1 = c.GetPoint(i);
	//	const VPoint *p2 = c.GetPoint(i+1);
	//	//const VVector *t1 = t[i];
	//	//const VVector *t2 = t[i+1];
	//	VPoint p3 = (*p1)+direct;
	//	VPoint p4 = (*p2)+direct;
	//	//VVector n1 = direct ^ *t1;
	//	//VVector n2 = direct ^ *t2;
	//	VVector e1 = *p2 - *p1;
	//	VVector e2 = p3 - *p1;
	//	VVector n = e1 ^ e2;
	//	n.NormThis();
	//	VPoint2d vpar1 (par1,0.0);
	//	VPoint2d vpar2 (par2,0.0);
	//	VPoint2d vpar3 (par1,1.0);
	//	VPoint2d vpar4 (par2,1.0);
	//	result->surfpack.InsertPoint (vpar1,*p1,n);
	//	result->surfpack.InsertPoint (vpar2,*p2,n);
	//	result->surfpack.InsertPoint (vpar3,p3,n);
	//	result->surfpack.InsertPoint (vpar4,p4,n);
	//	int basei = i*4;
	//	RefTriangle r1 (basei,basei+1,basei+2);
	//	RefTriangle r2 (basei+1,basei+3,basei+2);
	//	result->AddTriangle(r1);
	//	result->AddTriangle(r2);
	//}
	////FormRefTriangles (result);
	//return result;
}

void TabulatedCylinderPointGenerator::GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const
{
	m = GetPoint (u,v);
	n = GetNormal (u,v,m);
}


void TabulatedCylinderPointGenerator::ResetExtrudeLength(vreal newLen)
{
	VPoint normedDir = direct.Unit();
	direct = normedDir * newLen;
}

//bool TabulatedCylinderPointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return GenericUnproject(mp, u, v, tol);
//}

IgesEntity * TabulatedCylinderPointGenerator::GetIgesEntity()
{
	return (IgesEntity *)&m_surf;
}