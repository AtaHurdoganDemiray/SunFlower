// PlaneSurfacePointGenerator.cpp

#include "stdafx.h"
#include "PlaneSurfacePointGenerator.h"
#include "PlaneSurface.h"
#include "IgesEntitySet.h"
#include "Point.h"
#include "DoubleVec.h"
#include "SequentialLineSet.h"
#include "DirectionEntity.h"

PlaneSurfacePointGenerator::PlaneSurfacePointGenerator(const PlaneSurface &surf,IgesEntitySet &entset)
	:m_surf(surf), SurfacePointGenerator(-100.0, 100.0, -100.0, 100.0), m_entset(entset)
{
	try
	{
		IgesEntity *e1 = m_entset.GetEntityPointer(m_surf.m_DELOC);
		if (e1 == 0)
		{
			throw VException("PlaneSurfacePointGenerator::Constructor , location point null", 1);
		}
		IgesPoint *locpoint = static_cast<IgesPoint*>(e1);
		VPoint planePoint (locpoint->m_X, locpoint->m_Y, locpoint->m_Z);
		IgesEntity *e2 = m_entset.GetEntityPointer(m_surf.m_DENRML);
		if (e2 == 0)
		{
			throw VException("PlaneSurfacePointGenerator::Constructor , normal vector null", 1);
		}
		DirectionEntity *norvec = static_cast<DirectionEntity *> (e2);
		VPoint planeNormal (norvec->m_X, norvec->m_Y, norvec->m_Z);
		planeNormal.NormThis();
		if (m_surf.DE.m_formNo == 1)
		{
			IgesEntity *e3 = m_entset.GetEntityPointer(m_surf.m_DEREFD);
			if (e3 == 0)
				throw VException("PlaneSurfacePointGenerator::Constructor , refdirection null",1);
			DirectionEntity *refd = static_cast<DirectionEntity *>(e3);
			VPoint refDirection = VPoint(refd->m_X, refd->m_Y, refd->m_Z);
			refDirection = refDirection - (refDirection%planeNormal) * planeNormal;
			refDirection.NormThis();
			VPoint otherDirection = planeNormal ^ refDirection;
			otherDirection.NormThis();
			m_cs = CoordinateSystem(planePoint, refDirection, otherDirection, planeNormal);
		}
		else
		{
			m_cs = CoordinateSystem::ArbitraryAxisAlg(planeNormal);
			m_cs.SetOrigin(planePoint);
		}
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog(e,"PlaneSurface::constructor");
		throw VException("PlaneSurfacePointGenerator::Construction failed", 1);
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog(e, "PlaneSurface::constructor");
		throw VException("PlaneSurfacePointGenerator::Construction failed", 1);
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog("UE", "unknown exception");
		throw VException("PlaneSurfacePointGenerator::Construction failed",1);
	}
}
VPoint PlaneSurfacePointGenerator::GetPoint(const vreal & u, const vreal & v) const
{
	vreal uu = u * GetDeltaU();
	vreal vv = v * GetDeltaV();
	VPoint p = m_cs.GetOrigin() + m_cs.GetDirx() * uu + m_cs.GetDiry() * vv;
	return p;
}
VPoint PlaneSurfacePointGenerator::GetNormal(const vreal & u, const vreal & v, const VPoint & surfpoint) const
{
	return m_cs.GetDirz();
}
void PlaneSurfacePointGenerator::GetPointAndNormal(const vreal &u, const vreal &v, VPoint &m, VPoint &n) const
{
	m = GetPoint(u,v);
	n = m_cs.GetDirz();
}
void PlaneSurfacePointGenerator::GetUDefIntervals(DoubleVec & defint) const
{
	defint.Addvreal(0.0);
	defint.Addvreal(1.0);
}
void PlaneSurfacePointGenerator::GetVDefIntervals(DoubleVec & defint) const
{
	defint.Addvreal(0.0);
	defint.Addvreal(1.0);
}
VRefTriangleMesh * PlaneSurfacePointGenerator::Digitise(const vreal &tol) const
{
	return  GenericDigitise(tol);
}
//bool PlaneSurfacePointGenerator::Unproject(const VPoint &mp, vreal &u, vreal &v, vreal tol)
//{
//	return GenericUnproject(mp, u, v, tol);
//}

void PlaneSurfacePointGenerator::SetMinMax(const SequentialLineSet &outerLoop)
{
	VPoint mncs = outerLoop.GetMinPointInCs(m_cs);
	VPoint mxcs = outerLoop.GetMaxPointInCs(m_cs);
	VPoint deltamnmx = mxcs - mncs;
	m_cs.SetOrigin(outerLoop.GetMin());
	SetU0(0.0);
	SetU1(deltamnmx.x());
	SetV0(0.0);
	SetV1(deltamnmx.y());
}

IgesEntity * PlaneSurfacePointGenerator::GetIgesEntity()
{
	return (IgesEntity *)&m_surf;
}