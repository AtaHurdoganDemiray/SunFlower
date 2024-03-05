// IgesEntityVisitor.h

#ifndef __IgesEntityVisitor_h__
#define __IgesEntityVisitor_h__

class IgesEntity;
class IgesPoint;
class Line;
class igesArc;
class BSplineCurve;
class BSplineSurface;
class ConicArc;
class ParametricSpline;
class RuledSurface;
class SurfaceOfRevolution;
class TabulatedCylinder;
class BoundedSurface;
class TrimmedSurface;
class CurveOnParametricSurface;
class CompositeCurve;
class Boundary;
class igesTransformationMatrix;
class OffsetSurface;
class CopiousData;
class PlaneEntity;
class ColorDefinition;
class NullEntity;
class Vertex;
class Edge;
class SubFigureDefinition;
class SingularSubFigureInstance;
class ParametricSplineSurface;
class PlaneSurface;
class RightCircularCylindricalSurface;
class RightCircularConicalSurface;
class SphericalSurface;
class ToroidalSurface;
class Face;
class GeneralNote;

class IgesEntityVisitor
{
public:
	virtual void AcceptVirtual (const IgesEntity &e)=0;
	virtual void AcceptVirtual (const IgesPoint &e)=0;
	virtual void AcceptVirtual (const Line &e)=0;
	virtual void AcceptVirtual (const igesArc &e)=0;
	virtual void AcceptVirtual (const BSplineCurve &e)=0;
	virtual void AcceptVirtual (const BSplineSurface &e)=0;
	virtual void AcceptVirtual (const RuledSurface &e)=0;
	virtual void AcceptVirtual (const SurfaceOfRevolution &e)=0;
	virtual void AcceptVirtual (const TabulatedCylinder &e)=0;
	virtual void AcceptVirtual (const BoundedSurface &e)=0;
	virtual void AcceptVirtual (const TrimmedSurface &e)=0;
	virtual void AcceptVirtual (const CurveOnParametricSurface &e)=0;
	virtual void AcceptVirtual (const CompositeCurve &e)=0;
	virtual void AcceptVirtual (const Boundary &e)=0;
	virtual void AcceptVirtual (const igesTransformationMatrix &e)=0;
	virtual void AcceptVirtual (const OffsetSurface &e)=0;
	virtual void AcceptVirtual (const ConicArc &e)=0;
	virtual void AcceptVirtual (const ParametricSpline &e)=0;
	virtual void AcceptVirtual (const CopiousData &e)=0;
	virtual void AcceptVirtual (const PlaneEntity &e)=0;
	virtual void AcceptVirtual (const ColorDefinition &e)=0;
	virtual void AcceptVirtual (const NullEntity &e)=0;
	virtual void AcceptVirtual(const Vertex &e)=0;
	virtual void AcceptVirtual(const Edge &e)=0;
	virtual void AcceptVirtual (const SubFigureDefinition &e) =0;
	virtual void AcceptVirtual (const SingularSubFigureInstance &e) =0;
	virtual void AcceptVirtual (const ParametricSplineSurface &e) = 0;
	virtual void AcceptVirtual(const PlaneSurface &e) = 0;
	virtual void AcceptVirtual(const RightCircularCylindricalSurface &e) = 0;
	virtual void AcceptVirtual(const RightCircularConicalSurface &e) = 0;
	virtual void AcceptVirtual(const SphericalSurface &e) = 0;
	virtual void AcceptVirtual(const ToroidalSurface &e) = 0;
	virtual void AcceptVirtual(const Face &e) = 0;
	virtual void AcceptVirtual(const GeneralNote &e) = 0;
};

#endif