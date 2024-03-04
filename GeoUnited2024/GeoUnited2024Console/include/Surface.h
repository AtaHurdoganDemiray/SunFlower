// Surface.h

#ifndef __Surface_h__
#define __Surface_h__

#include "UntrimmedSurface.h"
#include "QuadSet.h"
#include "VRefTriangleMesh.h"
#include "TrimLoop2dSet.h"
#include <vector>

class BoundedSurface;
class TrimmedSurface;
class IgesEntity;

class Surface
{
public:
	Surface ( const BoundedSurface & bsurf );
	Surface ( const TrimmedSurface & tsurf );
	Surface ( IgesEntity * surf );

	~Surface();
		
	UntrimmedSurface surf;
	QuadSet quads;
	bool * isquadvalid;
	VRefTriangleMesh extratri;
	TrimLoop2dSet trimloop;
	std::vector<SequentialLineSet*> allboundary;
};

#endif