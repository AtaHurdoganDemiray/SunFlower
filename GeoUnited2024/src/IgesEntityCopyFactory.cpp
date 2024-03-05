// IgesEntityCopyFactory.cpp

#include "stdafx.h"
#include "IgesEntityCopyFactory.h"

#include "IgesEntity.h"
#include "Point.h"
#include "Line.h"
#include "Arc.h"
#include "BSplineCurve.h"
#include "BSplineSurface.h"
#include "ConicArc.h"
#include "RuledSurface.h"
#include "ParametricSpline.h"
#include "RuledSurface.h"
#include "SurfaceOfRevolution.h"
#include "TabulatedCylinder.h"
#include "BoundedSurface.h"
#include "TrimmedSurface.h"
#include "CurveOnParametricSurface.h"
#include "CompositeCurve.h"
#include "Boundary.h"
#include "igesTransformationMatrix.h"
#include "OffsetSurface.h"
#include "CopiousData.h"
#include "PlaneEntity.h"
#include "ColorDefinition.h"
#include "NullEntity.h"
#include "Vertex.h"
#include "Edge.h"
#include "SubFigureDefinition.h"
#include "SingularSubFigureInstance.h"
#include "ParametricSplineSurface.h"
#include "PlaneSurface.h"
#include "RightCircularCylindricalSurface.h"
#include "RightCircularConicalSurface.h"
#include "SphericalSurface.h"
#include "ToroidalSurface.h"
#include "Face.h"
#include "GeneralNote.h"

IgesEntity  * IgesEntityCopyFactory::MakeCopy (const IgesEntity &e)
{
	m_entity = 0;
	e.AcceptVisitor (*this);
	m_entity->DE = e.DE;
	m_entity->directorylineno = e.directorylineno;
	//m_entity->parameterlineno = e.parameterlineno;
	return m_entity;
}

void IgesEntityCopyFactory::AcceptVirtual(const IgesEntity &e)
{
	throw VException (( IDS_IgeExc),1);
}
void IgesEntityCopyFactory::AcceptVirtual(const IgesPoint &e)
{
	m_entity = new IgesPoint(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const Line &e)
{
	m_entity = new Line(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const igesArc &e)
{
	m_entity = new igesArc(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const BSplineCurve &e)
{
	m_entity = new BSplineCurve(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const BSplineSurface &e)
{
	m_entity = new BSplineSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const RuledSurface &e)
{
	m_entity = new RuledSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const SurfaceOfRevolution &e)
{
	m_entity = new SurfaceOfRevolution(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const TabulatedCylinder &e)
{
	m_entity = new TabulatedCylinder(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const BoundedSurface &e)
{
	m_entity = new BoundedSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const TrimmedSurface &e)
{
	m_entity = new TrimmedSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const CurveOnParametricSurface &e)
{
	m_entity = new CurveOnParametricSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const CompositeCurve &e)
{
	m_entity = new CompositeCurve(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const Boundary &e)
{
	m_entity = new Boundary(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const igesTransformationMatrix &e)
{
	m_entity = new igesTransformationMatrix(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const OffsetSurface &e)
{
	m_entity = new OffsetSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const ConicArc &e)
{
	m_entity = new ConicArc(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const ParametricSpline &e)
{
	m_entity = new ParametricSpline(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const CopiousData &e)
{
	m_entity = new CopiousData(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const PlaneEntity &e)
{
	m_entity = new PlaneEntity(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const ColorDefinition &e)
{
	m_entity = new ColorDefinition(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const NullEntity &e)
{
	m_entity = new NullEntity(e);
}

void IgesEntityCopyFactory::AcceptVirtual(const Vertex &e)
{
	m_entity = new Vertex (e);
}
void IgesEntityCopyFactory::AcceptVirtual(const Edge &e)
{
	m_entity = new Edge (e);
}

void IgesEntityCopyFactory::AcceptVirtual (const SubFigureDefinition &e)
{
	m_entity = new SubFigureDefinition (e);
}
void IgesEntityCopyFactory::AcceptVirtual (const SingularSubFigureInstance &e)
{
	m_entity = new SingularSubFigureInstance (e);
}
void IgesEntityCopyFactory::AcceptVirtual (const ParametricSplineSurface &e)
{
	m_entity = new ParametricSplineSurface (e);
}

void IgesEntityCopyFactory::AcceptVirtual(const PlaneSurface &e)
{
	m_entity = new PlaneSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const RightCircularCylindricalSurface &e)
{
	m_entity = new RightCircularCylindricalSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const RightCircularConicalSurface &e)
{
	m_entity = new RightCircularConicalSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const SphericalSurface &e)
{
	m_entity = new SphericalSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const ToroidalSurface &e)
{
	m_entity = new ToroidalSurface(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const Face &e)
{
	m_entity = new Face(e);
}
void IgesEntityCopyFactory::AcceptVirtual(const GeneralNote &gn)
{
	m_entity = new GeneralNote(gn);
}