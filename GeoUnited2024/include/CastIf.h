// CastIf.h

#ifndef __CastIf_h__
#define __CastIf_h__
#include "IgesEntity.h"
class Boundary;
class BoundedSurface;
class BSplineCurve;
class BSplineSurface;
class ColorDefinition;
class CompositeCurve;
class CopiousData;
class CurveOnParametricSurface;
class igesArc;
class IgesPoint;
class igesTransformationMatrix;
class Line;
class OffsetSurface;
class SurfaceOfRevolution;
class TabulatedCylinder;
class TrimmedSurface;

class CastIf 
{
public:
	static Boundary * ToBoundary (IgesEntity *ent);
	static BoundedSurface * ToBoundedSurface (IgesEntity *ent);
	static BSplineCurve * ToBSplineCurve (IgesEntity *ent);
	static BSplineSurface * ToBSplineSurface (IgesEntity *ent);
	static ColorDefinition * ToColorDefinition (IgesEntity *ent);
	static CompositeCurve * ToCompositeCurve (IgesEntity *ent);
	static CopiousData * ToCopiousData (IgesEntity *ent);
	static CurveOnParametricSurface * ToCurveOnParametricSurface (IgesEntity *ent);
	static igesArc * ToigesArc (IgesEntity *ent);
	static IgesPoint * ToIgesPoint (IgesEntity *ent);
	static igesTransformationMatrix * ToigesTransformationMatrix (IgesEntity *ent);
	static Line * ToLine (IgesEntity *ent);
	static OffsetSurface * ToOffsetSurface (IgesEntity *ent);
	static SurfaceOfRevolution * ToSurfaceOfRevolution (IgesEntity *ent);
	static TabulatedCylinder * ToTabulatedCylinder (IgesEntity *ent);
	static TrimmedSurface * ToTrimmedSurface (IgesEntity *ent);
};

#endif
