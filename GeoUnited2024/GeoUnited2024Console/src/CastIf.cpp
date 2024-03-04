// CastIf.cpp

#include "stdafx.h"
#include "CastIf.h"

Boundary * CastIf::ToBoundary (IgesEntity *ent)
{
	if (ent->entitytype == 141)
	{
		return ((Boundary*)(ent));
	}
	else
		return 0;
}
BoundedSurface * CastIf::ToBoundedSurface (IgesEntity *ent)
{
	if (ent->entitytype == 143)
	{
		return ((BoundedSurface*)(ent));
	}
	else
		return 0;
}
BSplineCurve * CastIf::ToBSplineCurve (IgesEntity *ent)
{
	if (ent->entitytype == 126)
	{
		return ((BSplineCurve*)(ent));
	}
	else
		return 0;
}
BSplineSurface * CastIf::ToBSplineSurface (IgesEntity *ent)
{
	if (ent->entitytype == 128)
	{
		return ((BSplineSurface*)(ent));
	}
	else
		return 0;
}
ColorDefinition * CastIf::ToColorDefinition (IgesEntity *ent)
{
	if (ent->entitytype == 314)
	{
		return ((ColorDefinition*)(ent));
	}
	else
		return 0;
}
CompositeCurve * CastIf::ToCompositeCurve (IgesEntity *ent)
{
	if (ent->entitytype == 102)
	{
		return ((CompositeCurve*)(ent));
	}
	else
		return 0;
}
CopiousData * CastIf::ToCopiousData (IgesEntity *ent)
{
	if (ent->entitytype == 106)
	{
		return ((CopiousData*)(ent));
	}
	else
		return 0;
}
CurveOnParametricSurface * CastIf::ToCurveOnParametricSurface (IgesEntity *ent)
{
	if (ent->entitytype == 142)
	{
		return ((CurveOnParametricSurface*)(ent));
	}
	else
		return 0;
}
igesArc * CastIf::ToigesArc (IgesEntity *ent)
{
	if (ent->entitytype == 100)
	{
		return ((igesArc*)(ent));
	}
	else
		return 0;
}
IgesPoint * CastIf::ToIgesPoint (IgesEntity *ent)
{
	if (ent->entitytype == 116)
	{
		return ((IgesPoint*)(ent));
	}
	else
		return 0;
}
igesTransformationMatrix * CastIf::ToigesTransformationMatrix (IgesEntity *ent)
{
	if (ent->entitytype == 124)
	{
		return ((igesTransformationMatrix*)(ent));
	}
	else
		return 0;
}
Line * CastIf::ToLine (IgesEntity *ent)
{
	if (ent->entitytype == 110)
	{
		return ((Line*)(ent));
	}
	else
		return 0;
}
OffsetSurface * CastIf::ToOffsetSurface (IgesEntity *ent)
{
	if (ent->entitytype == 140)
	{
		return ((OffsetSurface*)(ent));
	}
	else
		return 0;
}
SurfaceOfRevolution * CastIf::ToSurfaceOfRevolution (IgesEntity *ent)
{
	if (ent->entitytype == 120)
	{
		return ((SurfaceOfRevolution*)(ent));
	}
	else
		return 0;
}
TabulatedCylinder * CastIf::ToTabulatedCylinder (IgesEntity *ent)
{
	if (ent->entitytype == 122)
	{
		return ((TabulatedCylinder*)(ent));
	}
	else
		return 0;
}
TrimmedSurface * CastIf::ToTrimmedSurface (IgesEntity *ent)
{
	if (ent->entitytype == 144)
	{
		return ((TrimmedSurface*)(ent));
	}
	else
		return 0;
}
