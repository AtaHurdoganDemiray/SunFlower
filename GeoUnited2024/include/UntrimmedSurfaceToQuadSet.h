// UntrimmedSurfaceToQuadSet.h

#ifndef __UntrimmedSurfaceToQuadSet_h__
#define __UntrimmedSurfaceToQuadSet_h__

class UntrimmedSurface;
class gmQuadSet;

class UntrimmedSurfaceToQuadSet
{
public:
	gmQuadSet * MakeQuads ( const double & tolerance , const UntrimmedSurface & surf ) const;

};

#endif