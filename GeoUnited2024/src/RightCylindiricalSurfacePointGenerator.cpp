// RightCylindiricalSurfacePointGenerator.cpp

#include "stdafx.h"
#include "RightCylindricalSurfacePointGenerator.h"
#include "RightCircularCylindricalSurface.h"
#include "IgesEntitySet.h"
#include "SequentialLineSet.h"
#include "DoubleVec.h"
#include "Point.h"
#include "DirectionEntity.h"

RightCylindricalSurfacePointGenerator::RightCylindricalSurfacePointGenerator(const RightCircularCylindricalSurface &e, IgesEntitySet &entset)
	:SurfacePointGenerator(0.0, 1.0, 0.0, 1.0), m_surf(e), m_entset(entset), m_H(1.0)
{
	try
	{
		IgesEntity *pe = m_entset.GetEntityPointer(m_surf.m_DELOC);
		if (pe == 0)
			throw VException("RightCylindricalSurfacePointGenerator constructor , iges entity center point null",1);
		IgesEntity *ae = m_entset.GetEntityPointer(m_surf.m_DEAXIS);
		if (ae == 0)
			throw VException("RightCylindricalSurfacePointGenerator constructor , iges entity axis null", 1);
		IgesPoint *centere = static_cast<IgesPoint *> (pe);
		DirectionEntity *axise = static_cast<DirectionEntity *>(ae);
		VPoint center(centere->m_X, centere->m_Y, centere->m_Z);
		VPoint ax(axise->m_X, axise->m_Y, axise->m_Z);
		if (m_surf.DE.m_formNo == 1)
		{
			IgesEntity *re = m_entset.GetEntityPointer(m_surf.m_DEREFD);
			if (re == 0)
				throw VException("RightCylindricalSurfacePointGenerator constructor , iges entity center point null", 1);
			DirectionEntity *refdire = static_cast<DirectionEntity *>(re);
			VPoint xdir(refdire->m_X, refdire->m_Y, refdire->m_Z);
			xdir -= ax*(ax%xdir);
			xdir.NormThis();
			ax.NormThis();
			VPoint ydir = ax ^ xdir;
			m_cs = CoordinateSystem(center, xdir, ydir, ax);
		}
		else
		{
			m_cs = CoordinateSystem::ArbitraryAxisAlg(ax);
			m_cs.SetOrigin(center);
		}
		m_radius = m_surf.m_RADIUS;
		if (m_radius < 0.0 || m_radius > MAX_NUMBER_LIMIT)
			throw VException("RightCylindricalSurfacePointGenerator exception , radius is corrupted",1);
		SetIsPeriodicInU(true);
		SetIsPeriodicInV(false);
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "RightCylindricalSurfacePointGenerator constructor");
		throw VException("RightCylindricalSurfacePointGenerator constructor failed",1);
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "RightCylindricalSurfacePointGenerator constructor");
		throw VException("RightCylindricalSurfacePointGenerator constructor failed", 1);
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "RightCylindricalSurfacePointGenerator constructor");
		throw VException("RightCylindricalSurfacePointGenerator constructor failed", 1);
	}
}
VPoint RightCylindricalSurfacePointGenerator::GetPoint(const vreal & u, const vreal & v) const
{
	VPoint p = m_cs.GetOrigin() + (m_radius* (m_cs.GetDirx() * cos(u * TWO_PI) + m_cs.GetDiry() * sin(u*TWO_PI))) + m_cs.GetDirz() * m_H * v;
	return p;
}
VPoint RightCylindricalSurfacePointGenerator::GetNormal(const vreal & u, const vreal & v, const VPoint & surfpoint) const
{
	VPoint n = m_cs.GetDirx() * cos(u*TWO_PI) + m_cs.GetDiry() * sin(u*TWO_PI);
	return n;
}
void RightCylindricalSurfacePointGenerator::GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const
{
	m = GetPoint(u, v);
	n = GetNormal(u, v, m);
}
void RightCylindricalSurfacePointGenerator::GetUDefIntervals(DoubleVec & defint) const
{
	defint.Addvrealif(0.0);
	defint.Addvrealif(0.125);
	defint.Addvrealif(0.25);
	defint.Addvrealif(0.375);
	defint.Addvrealif(0.5);
	defint.Addvrealif(0.625);
	defint.Addvrealif(0.75);
	defint.Addvrealif(0.875);
	defint.Addvrealif(1.0);
}
void RightCylindricalSurfacePointGenerator::GetVDefIntervals(DoubleVec & defint) const
{
	defint.Addvrealif(0.0);
	defint.Addvrealif(1.0);
}
VRefTriangleMesh * RightCylindricalSurfacePointGenerator::Digitise(const vreal &tol) const
{
	return SurfacePointGenerator::GenericDigitise(tol);
}
	
//bool RightCylindricalSurfacePointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return SurfacePointGenerator::GenericUnproject(mp, u, v, tol);
//}
vreal RightCylindricalSurfacePointGenerator::GetH()const
{
	return m_H;
}
void RightCylindricalSurfacePointGenerator::SetH(vreal H)
{
	m_H = H;
}

void RightCylindricalSurfacePointGenerator::DecideH(const SequentialLineSet &sl)
{
	VPoint mn = sl.GetMinPointInCs(m_cs);
	VPoint mx = sl.GetMaxPointInCs(m_cs);
	vreal H = mx.z() - mn.z();
	if (fabs(H) < EPSILON)
		throw VException("RightCylindricalSurfacePointGenerator::DecideH outer loop margins along H is 0 in length",1);
	SetH(H);
	VPoint o = m_cs.GetOrigin();
	vreal deltaz = (mn - m_cs.AbsoluteCoordToRelativeCoord(o)).z();
	o += (m_cs.GetDirz() * deltaz);
	m_cs.SetOrigin(o);
}

IgesEntity * RightCylindricalSurfacePointGenerator::GetIgesEntity()
{
	return (IgesEntity *)&m_surf;
}