// SphereSurfacePointGenerator.cpp

#include "stdafx.h"
#include "SphereSurfacePointGenerator.h"
#include "SphericalSurface.h"
#include "IgesEntitySet.h"
#include "Point.h"
#include "DirectionEntity.h"
#include "DoubleVec.h"

SphereSurfacePointGenerator::SphereSurfacePointGenerator(const SphericalSurface &e, IgesEntitySet &entset)
	:SurfacePointGenerator(0.0, 1.0, 0.0, 1.0), m_surf(e), m_entset(entset)
{
	try
	{
		IgesEntity *pe = m_entset.GetEntityPointer(m_surf.m_DELOC);
		if (pe == 0)
			throw VException("SphereSurfacePointGenerator constructor , iges entity center point null", 1);
		IgesEntity *ae = m_entset.GetEntityPointer(m_surf.m_DEAXIS);
		if (ae == 0)
			throw VException("SphereSurfacePointGenerator constructor , iges entity axis null", 1);
		IgesPoint *centere = static_cast<IgesPoint *> (pe);
		DirectionEntity *axise = static_cast<DirectionEntity *>(ae);
		VPoint center(centere->m_X, centere->m_Y, centere->m_Z);
		VPoint ax(axise->m_X, axise->m_Y, axise->m_Z);
		if (m_surf.DE.m_formNo == 1)
		{
			IgesEntity *re = m_entset.GetEntityPointer(m_surf.m_DEREFD);
			if (re == 0)
				throw VException("SphereSurfacePointGenerator constructor , iges entity center point null", 1);
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
			throw VException("SphereSurfacePointGenerator exception , radius is corrupted", 1);
		SetIsPeriodicInU(true);
		SetIsPeriodicInV(true);
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "SphereSurfacePointGenerator constructor");
		throw VException("SphereSurfacePointGenerator constructor failed", 1);
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "SphereSurfacePointGenerator constructor");
		throw VException("SphereSurfacePointGenerator constructor failed", 1);
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "SphereSurfacePointGenerator constructor");
		throw VException("SphereSurfacePointGenerator constructor failed", 1);
	}
}
VPoint SphereSurfacePointGenerator::GetPoint(const vreal & u, const vreal & v) const
{
	vreal vAng = PI * v - HALF_PI;
	VPoint p = m_cs.GetOrigin() + m_radius *cos(vAng) * (cos(TWO_PI * u) * m_cs.GetDirx() + sin(TWO_PI * u) * m_cs.GetDiry()) + m_radius * sin(vAng) * m_cs.GetDirz();
	return p;
}
VPoint SphereSurfacePointGenerator::GetNormal(const vreal & u, const vreal & v, const VPoint & surfpoint) const
{
	return SurfacePointGenerator::GetNormalAppr(u, v, surfpoint);
}
void SphereSurfacePointGenerator::GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const
{
	m = GetPoint(u, v);
	n = GetNormal(u, v, m);
}
void SphereSurfacePointGenerator::GetUDefIntervals(DoubleVec & defint) const
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
void SphereSurfacePointGenerator::GetVDefIntervals(DoubleVec & defint) const
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
VRefTriangleMesh * SphereSurfacePointGenerator::Digitise(const vreal &tol) const
{
	return SurfacePointGenerator::GenericDigitise(tol);
}

//bool SphereSurfacePointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return SurfacePointGenerator::GenericUnproject(mp, u, v, tol);
//}

IgesEntity * SphereSurfacePointGenerator::GetIgesEntity()
{
	return (IgesEntity *)&m_surf;
}