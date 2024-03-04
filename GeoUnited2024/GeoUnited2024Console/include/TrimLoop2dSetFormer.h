// TrimLoop2dSetFormer.h

#ifndef __TrimLoop2dSetFormer_h__
#define __TrimLoop2dSetFormer_h__

#include "TrimLoop2dSet.h"

class ProcessedBoundedSurface;
class ProcessedTrimmedSurface;

class TrimLoop2dSetFormer
{
public:
	TrimLoop2dSetFormer ( ProcessedBoundedSurface & boundedsurf );
	TrimLoop2dSetFormer ( ProcessedTrimmedSurface & trimmedsurf );
	TrimLoop2dSet trimloops;
};

#endif