// RightConicalSurfacePointGenerator.cpp

#include "stdafx.h"
#include "RightConicalSurfacePointGenerator.h"
#include "RightCircularConicalSurface.h"
#include "IgesEntitySet.h"
#include "Point.h"
#include "DirectionEntity.h"
#include "SequentialLineSet.h"
#include "DoubleVec.h"

RightConicalSurfacePointGenerator::RightConicalSurfacePointGenerator(const RightCircularConicalSurface &e, IgesEntitySet &entset)
	:SurfacePointGenerator(0.0, 1.0, 0.0, 1.0), m_surf(e), m_entset(entset)
{
	try
	{
		IgesEntity *pe = m_entset.GetEntityPointer(m_surf.m_DELOC);
		if (pe == 0)
			throw VException("RightConicalSurfacePointGenerator constructor , iges entity center point null", 1);
		IgesEntity *ae = m_entset.GetEntityPointer(m_surf.m_DEAXIS);
		if (ae == 0)
			throw VException("RightConicalSurfacePointGenerator constructor , iges entity axis null", 1);
		IgesPoint *centere = static_cast<IgesPoint *> (pe);
		DirectionEntity *axise = static_cast<DirectionEntity *>(ae);
		VPoint center(centere->m_X, centere->m_Y, centere->m_Z);
		VPoint ax(axise->m_X, axise->m_Y, axise->m_Z);
		if (m_surf.DE.m_formNo == 1)
		{
			IgesEntity *re = m_entset.GetEntityPointer(m_surf.m_DEREFD);
			if (re == 0)
				throw VException("RightConicalSurfacePointGenerator constructor , iges entity center point null", 1);
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
			throw VException("RightConicalSurfacePointGenerator constructor exception , radius is corrupted", 1);
		m_angleInDegrees = m_surf.m_SANGLE;
		if (m_angleInDegrees <= 0 || m_angleInDegrees > 90.0 + EPSILON)
			throw VException("RightConicalSurfacePointGenerator constructor exception , conic angle is wrong",1);
		m_angleInRad = m_angleInDegrees * DEGREE_TO_RAD;
		SetIsPeriodicInU(true);
		SetIsPeriodicInV(false);
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e, "RightConicalSurfacePointGenerator constructor");
		throw VException("RightConicalSurfacePointGenerator constructor failed", 1);
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "RightConicalSurfacePointGenerator constructor");
		throw VException("RightConicalSurfacePointGenerator constructor failed", 1);
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE ", "RightConicalSurfacePointGenerator constructor");
		throw VException("RightConicalSurfacePointGenerator constructor failed", 1);
	}
}
VPoint RightConicalSurfacePointGenerator::GetPoint(const vreal & u, const vreal & v) const
{
	vreal curH = m_H * v;
	VPoint p = m_cs.GetOrigin() + ((m_mnr+ curH*tan(m_angleInRad)) * (m_cs.GetDirx() * cos(u * TWO_PI) + m_cs.GetDiry() * sin(u*TWO_PI))) + m_cs.GetDirz() * curH;
	return p;
}
VPoint RightConicalSurfacePointGenerator::GetNormal(const vreal & u, const vreal & v, const VPoint & surfpoint) const
{
	return GetNormalAppr(u, v, surfpoint);
}
void RightConicalSurfacePointGenerator::GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const
{
	m = GetPoint(u, v);
	n = GetNormal(u, v, m);
}
void RightConicalSurfacePointGenerator::GetUDefIntervals(DoubleVec & defint) const
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
void RightConicalSurfacePointGenerator::GetVDefIntervals(DoubleVec & defint) const
{
	defint.Addvrealif(0.0);
	defint.Addvrealif(1.0);
}
VRefTriangleMesh * RightConicalSurfacePointGenerator::Digitise(const vreal &tol) const
{
	return SurfacePointGenerator::GenericDigitise(tol);
}

//bool RightConicalSurfacePointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return SurfacePointGenerator::GenericUnproject(mp, u, v, tol);
//}

void RightConicalSurfacePointGenerator::DecideH(const SequentialLineSet &outerLoop)
{
	VPoint mn = outerLoop.GetMinPointInCs(m_cs);
	VPoint mx = outerLoop.GetMaxPointInCs(m_cs);
	vreal H = mx.z() - mn.z();
	if (fabs(H) < EPSILON)
		throw VException("RightCylindricalSurfacePointGenerator::DecideH outer loop margins along H is 0 in length", 1);
	m_H = H;
	VPoint o = m_cs.GetOrigin();
	o.z(mn.z());
	m_cs.SetOrigin(o);
	m_mnr = m_radius + mn.z() * tan(m_angleInRad);
}

IgesEntity * RightConicalSurfacePointGenerator::GetIgesEntity()
{
	return (IgesEntity *)&m_surf;
}