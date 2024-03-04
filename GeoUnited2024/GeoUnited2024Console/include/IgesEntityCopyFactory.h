// IgesEntityCopyFactory.h

#ifndef __IgesEntityCopyFactory_h__
#define __IgesEntityCopyFactory_h__
#include "IgesEntityVisitor.h"

class IgesEntityCopyFactory:public IgesEntityVisitor
{
public:
	IgesEntity * MakeCopy (const IgesEntity &e);
	virtual void AcceptVirtual (const IgesEntity &e);
	virtual void AcceptVirtual (const IgesPoint &e);
	virtual void AcceptVirtual (const Line &e);
	virtual void AcceptVirtual (const igesArc &e);
	virtual void AcceptVirtual (const BSplineCurve &e);
	virtual void AcceptVirtual (const BSplineSurface &e);
	virtual void AcceptVirtual (const RuledSurface &e);
	virtual void AcceptVirtual (const SurfaceOfRevolution &e);
	virtual void AcceptVirtual (const TabulatedCylinder &e);
	virtual void AcceptVirtual (const BoundedSurface &e);
	virtual void AcceptVirtual (const TrimmedSurface &e);
	virtual void AcceptVirtual (const CurveOnParametricSurface &e);
	virtual void AcceptVirtual (const CompositeCurve &e);
	virtual void AcceptVirtual (const Boundary &e);
	virtual void AcceptVirtual (const igesTransformationMatrix &e);
	virtual void AcceptVirtual (const OffsetSurface &e);
	virtual void AcceptVirtual (const ConicArc &e);
	virtual void AcceptVirtual (const ParametricSpline &e);
	virtual void AcceptVirtual (const CopiousData &e);
	virtual void AcceptVirtual (const PlaneEntity &e);
	virtual void AcceptVirtual (const ColorDefinition &e);
	virtual void AcceptVirtual (const NullEntity &e);
	virtual void AcceptVirtual (const Vertex &e);
	virtual void AcceptVirtual (const Edge &e);
	virtual void AcceptVirtual (const SubFigureDefinition &e);
	virtual void AcceptVirtual (const SingularSubFigureInstance &e);
	virtual void AcceptVirtual (const ParametricSplineSurface &e);
	virtual void AcceptVirtual(const PlaneSurface &e);
	virtual void AcceptVirtual(const RightCircularCylindricalSurface &e);
	virtual void AcceptVirtual(const RightCircularConicalSurface &e);
	virtual void AcceptVirtual(const SphericalSurface &e);
	virtual void AcceptVirtual(const ToroidalSurface &e);
	virtual void AcceptVirtual(const Face &e);
	virtual void AcceptVirtual(const GeneralNote &gn);
private:
	IgesEntity *m_entity;
};

#endif