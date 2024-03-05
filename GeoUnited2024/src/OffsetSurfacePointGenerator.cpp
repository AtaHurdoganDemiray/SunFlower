// OffsetSurfacePointGenerator.cpp

#include "stdafx.h"
#include "OffsetSurfacePointGenerator.h"
#include "IgesEntitySet.h"
#include "IgesEntity.h"
#include "OffsetSurfacePointGenerator.h"
#include "VRefTriangleMesh.h"

OffsetSurfacePointGenerator::OffsetSurfacePointGenerator 
( const OffsetSurface &surf , const IgesEntity * newsurf , IgesEntitySet & allent 
 , const VPoint & offsetindicator 
 , const vreal & _offsetvalue )
 :SurfacePointGenerator(0.0, 1.0, 0.0, 1.0), m_surf(surf)
{
	allent.MakePartOfTrimmedSurface (  newsurf  );
	allent.MakeAllSubsetIndex0();
	m_ibasesurf = newsurf->MakeCopyAndAddToSet (allent,m_eSet);
	m_basesurf = new UntrimmedSurface (m_ibasesurf,m_eSet);
	//m_basesurf->SetTransformationMatrixIf ();
	SetU0 (m_basesurf->U0());
	SetU1 (m_basesurf->U1());
	SetV0 (m_basesurf->V0());
	SetV1 (m_basesurf->V1());

	VPoint midp = m_basesurf->GetPoint ( 0.5f , 0.5f );
	VPoint midnormal = m_basesurf->GetNormal ( 0.5f , 0.5f , midp );		
	
	vreal dotprd = midnormal % offsetindicator;
	if ( dotprd < 0.0 )
		offsetvalue =-_offsetvalue;
	else
		offsetvalue = _offsetvalue;

}

VPoint OffsetSurfacePointGenerator::GetPoint 
						( const vreal & u , const vreal & v ) const
{
	VPoint originalpoint = m_basesurf->GetPoint ( u , v );
	VPoint myoffset = m_basesurf->GetNormal ( u , v , originalpoint );
	myoffset *= offsetvalue;
	VPoint result = originalpoint + myoffset;
	return result;
}

VPoint OffsetSurfacePointGenerator::GetNormal 
( const vreal & u , const vreal & v , const VPoint & surfpoint ) const
{
	VPoint mypoint = m_basesurf->GetPoint ( u , v );
	VPoint mynormal = m_basesurf->GetNormal ( u , v , mypoint );
	return mynormal;
}

void OffsetSurfacePointGenerator::GetUDefIntervals ( DoubleVec & defint ) const
{
	m_basesurf->GetUDefIntervals ( defint );
}

void OffsetSurfacePointGenerator::GetVDefIntervals ( DoubleVec & defint ) const
{
	m_basesurf->GetVDefIntervals ( defint );
}

VRefTriangleMesh * OffsetSurfacePointGenerator::Digitise(const vreal &tol)const
{
	VRefTriangleMesh *res1 = m_basesurf->Digitise (tol);
	int mysize = res1->surfpack.parampoints.Size();
	VRefTriangleMesh *res2 = new VRefTriangleMesh (mysize);
	const PointSet2d &p = res1->surfpack.parampoints;
	const PointSet &m = res1->surfpack.modelpoints;
	const PointSet &n = res1->surfpack.normals;
	for (int i=0;i<mysize;i++)
	{
		const VPoint2d *currp = p[i];
		const VPoint *currm = m[i];
		const VVector *currn = n[i];
		VPoint newm = (*currm) + (*currn) * offsetvalue;
		res2->surfpack.InsertPoint(*currp,newm,*currn);
	}
	RefTriangleMesh &rm = res2->GetRefTriangleMesh();
	rm = res1->GetRefTriangleMesh();
	delete res1;
	return res2;
}

void OffsetSurfacePointGenerator::GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const
{
	VPoint basem;
	m_basesurf->GetPointAndNormal (u,v,basem,n);
	m = basem + n * offsetvalue;
}

//bool OffsetSurfacePointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return GenericUnproject(mp, u, v, tol);
//}

IgesEntity * OffsetSurfacePointGenerator::GetIgesEntity()
{
	return (IgesEntity *)&m_surf;
}