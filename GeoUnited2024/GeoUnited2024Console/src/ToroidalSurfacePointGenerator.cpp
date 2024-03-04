// ToroidalSurfacePointGenerator.cpp

#include "stdafx.h"
#include "ToroidalSurfacePointGenerator.h"
#include "ToroidalSurface.h"
#include "IgesEntitySet.h"
#include "Point.h"
#include "DirectionEntity.h"
#include "DoubleVec.h"

ToroidalSurfacePointGenerator::ToroidalSurfacePointGenerator(const ToroidalSurface &e, IgesEntitySet &entset)
	:SurfacePointGenerator(0.0, 1.0, 0.0, 1.0), m_surf(e), m_entset(entset)
{
	try
	{
		IgesEntity *pe = m_entset.GetEntityPointer(m_surf.m_DELOC);
		if (pe == 0)
			throw VException("ToroidalSurfacePointGenerator constructor , iges entity center point null", 1);
		IgesEntity *ae = m_entset.GetEntityPointer(m_surf.m_DEAXIS);
		if (ae == 0)
			throw VException("ToroidalSurfacePointGenerator constructor , iges entity axis null", 1);
		IgesPoint *centere = static_cast<IgesPoint *> (pe);
		DirectionEntity *axise = static_cast<DirectionEntity *>(ae);
		VPoint center(centere->m_X, centere->m_Y, centere->m_Z);
		VPoint ax(axise->m_X, axise->m_Y, axise->m_Z);
		if (m_surf.DE.m_formNo == 1)
		{
			IgesEntity *re = m_entset.GetEntityPointer(m_surf.m_DEREFD);
			if (re == 0)
				throw VException("ToroidalSurfacePointGenerator constructor , iges entity center point null", 1);
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
		SetIsPeriodicInU(true);
		SetIsPeriodicInV(true);
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "ToroidalSurfacePointGenerator constructor");
		throw VException("ToroidalSurfacePointGenerator constructor failed", 1);
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "ToroidalSurfacePointGenerator constructor");
		throw VException("ToroidalSurfacePointGenerator constructor failed", 1);
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "ToroidalSurfacePointGenerator constructor");
		throw VException("ToroidalSurfacePointGenerator constructor failed", 1);
	}
}
VPoint ToroidalSurfacePointGenerator::GetPoint(const vreal & u, const vreal & v) const
{
	VPoint p = m_cs.GetOrigin() + (m_surf.m_MAJRAD + m_surf.m_MINRAD * cos(u * TWO_PI))*(cos(v*TWO_PI)*m_cs.GetDirx() - sin(v*TWO_PI)*m_cs.GetDiry()) + m_surf.m_MINRAD * sin(u*TWO_PI)*m_cs.GetDirz();
	return p;
}
VPoint ToroidalSurfacePointGenerator::GetNormal(const vreal & u, const vreal & v, const VPoint & surfpoint) const
{
	return SurfacePointGenerator::GetNormalAppr(u,v,surfpoint);
}
void ToroidalSurfacePointGenerator::GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const
{
	m = GetPoint(u, v);
	n = GetNormal(u, v, m);
}
void ToroidalSurfacePointGenerator::GetUDefIntervals(DoubleVec & defint) const
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
void ToroidalSurfacePointGenerator::GetVDefIntervals(DoubleVec & defint) const
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
VRefTriangleMesh * ToroidalSurfacePointGenerator::Digitise(const vreal &tol) const
{
	return SurfacePointGenerator::GenericDigitise(tol);
}

//bool ToroidalSurfacePointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return SurfacePointGenerator::GenericUnproject(mp, u, v, tol);
//}

IgesEntity * ToroidalSurfacePointGenerator::GetIgesEntity()
{
	return (IgesEntity *)&m_surf;
}