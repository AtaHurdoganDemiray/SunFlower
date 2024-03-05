// PlaneEntityPointGenerator.cpp

#include "stdafx.h"
#include "PlaneEntityPointGenerator.h"
#include "VPoint.h"
#include "PlaneEntity.h"
#include "DoubleVec.h"
#include "VRefTriangleMesh.h"
#include "SequentialLineSet.h"

PlaneEntityPointGenerator::PlaneEntityPointGenerator ( const PlaneEntity &plnent , const IgesEntitySet &entset)
:SurfacePointGenerator (0.0,1.0,0.0,1.0) , m_plnent(plnent)
{
	VPoint zdir (m_plnent.m_A , m_plnent.m_B , m_plnent.m_C);
	vreal vecLenInv = 1. / zdir.GetLength();
	m_DNormed = m_plnent.m_D * vecLenInv;
	zdir *= vecLenInv;
	VVector xdir , ydir;
	if (fabs(zdir % UX) < fabs(zdir % UY))
	{
		xdir = zdir ^ UX;
		xdir.NormThis();
		ydir = zdir ^ xdir;
	}
	else
	{
		xdir = zdir ^ UY;
		xdir.NormThis();
		ydir = zdir ^ xdir;
	}
	VPoint o = m_DNormed * zdir;
	CoordinateSystem tmp (o,xdir,ydir,zdir);
	m_cs = tmp;
	m_normal = zdir;
	m_pointonplane = o;
	m_origin = o;
}

PlaneEntityPointGenerator::~PlaneEntityPointGenerator()
{

}

VPoint PlaneEntityPointGenerator::GetPoint ( const vreal & u , const vreal & v ) const
{
	vreal su,sv;
	ShiftParIf(u,v,su,sv);
	//return m_origin + GetDeltaU() * su * m_cs.GetDirx() + GetDeltaV() * sv * m_cs.GetDiry();	
	VPoint p = m_cs.RelativeCoordToAbsoluteCoord(VPoint(m_minInCs.x() + m_deltaMnMx.x() * su, m_minInCs.y() + m_deltaMnMx.y() * sv, 0.0));
	return p;
}

VPoint PlaneEntityPointGenerator::GetNormal ( const vreal & u , const vreal & v , const VPoint & surfpoint ) const
{
	return m_normal;
}	

void PlaneEntityPointGenerator::GetUDefIntervals ( DoubleVec & defint ) const
{
	defint.Addvreal (0.0);
	defint.Addvreal (1.0);
}

void PlaneEntityPointGenerator::GetVDefIntervals ( DoubleVec & defint ) const
{	
	defint.Addvreal (0.0);
	defint.Addvreal (1.0);
}

VRefTriangleMesh * PlaneEntityPointGenerator::Digitise (const vreal &tol) const
{
	VPoint2d par1 (0.0,0.0);
	VPoint2d par2 (1.0,0.0);
	VPoint2d par3 (1.0,1.0);
	VPoint2d par4 (0.0,1.0);

	VPoint p1 = GetPoint (0.0,0.0);
	VPoint p2 = GetPoint (1.0,0.0);
	VPoint p3 = GetPoint (1.0,1.0);
	VPoint p4 = GetPoint (0.0,1.0);
	
	VRefTriangleMesh * newmesh = new VRefTriangleMesh (4);
	newmesh->surfpack.InsertPoint (par1,p1,m_normal);
	newmesh->surfpack.InsertPoint (par2,p2,m_normal);
	newmesh->surfpack.InsertPoint (par3,p3,m_normal);
	newmesh->surfpack.InsertPoint (par4,p4,m_normal);
	
	RefTriangle r1 (0,1,2);
	RefTriangle r2 (2,3,0);
	newmesh->AddTriangle (r1);
	newmesh->AddTriangle (r2);
	return newmesh;
}

void PlaneEntityPointGenerator::ResetRectangle (const vreal &_u0 , const vreal & _u1 , const vreal &_v0 , const vreal & _v1)
{
	SetU0 (_u0);
	SetU1 (_u1);
	SetV0 (_v0);
	SetV1 (_v1);
	VPoint tmp (U0() , V0() , 0.0);
	m_origin = m_cs.RelativeCoordToAbsoluteCoord (tmp);
}

const CoordinateSystem & PlaneEntityPointGenerator::GetPlaneCS()const
{
	return m_cs;
}

CoordinateSystem & PlaneEntityPointGenerator::GetPlaneCS ()
{
	return m_cs;
}

VPoint2d PlaneEntityPointGenerator::GetNormalizedPlaneCoord(const VPoint &spaceCoord)const
{
	VPoint rp = m_cs.AbsoluteCoordToRelativeCoord (spaceCoord);
	VPoint2d res ( (rp.x() - U0()) / GetDeltaU() , (rp.y()-V0()) / GetDeltaV() ); 
	return res;
}

void PlaneEntityPointGenerator::GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const
{
	m = GetPoint (u,v);
	n = m_normal;
}

void PlaneEntityPointGenerator::SetMinMax(const SequentialLineSet &outerLoop)
{
	m_minInCs = outerLoop.GetMinPointInCs(m_cs);
	m_maxInCs = outerLoop.GetMaxPointInCs(m_cs);
	m_deltaMnMx = m_maxInCs - m_minInCs;
}

//bool PlaneEntityPointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return GenericUnproject(mp, u, v, tol);
//}

IgesEntity * PlaneEntityPointGenerator::GetIgesEntity()
{
	return (IgesEntity *)&m_plnent;
}