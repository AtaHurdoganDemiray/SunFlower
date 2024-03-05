// UntrimmedSurfaceToQuadSet.cpp

#include "stdafx.h"
#include "UntrimmedSurfaceToQuadSet.h"
#include "SurfPointsPack.h"
#include "gmQuadSet.h"
#include "SurfaceDigitiserWithTol.h"

gmQuadSet * UntrimmedSurfaceToQuadSet::MakeQuads ( const double & tolerance , const UntrimmedSurface & surf ) const 
{
	int initialcapacity = 600;
	SurfPointsPack * surfpack = new SurfPointsPack ( initialcapacity );
	SurfaceDigitiserWithTol digitiser ( surf );
	gmQuadSet * quadset = digitiser.DigitiseSurface ( tolerance , *surfpack );
	return quadset;
}
