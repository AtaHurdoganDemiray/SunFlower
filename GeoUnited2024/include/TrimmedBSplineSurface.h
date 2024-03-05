// TrimmedBSplineSurface.h

#ifndef __TrimmedBSplineSurface_h__
#define __TrimmedBSplineSurface_h__

#include <vector>
#include "gmTrimLoopUtility.h"

class BSplineSurface;
class BSplineCurve;

class TrimmedBSplineSurface
{
public:
	TrimmedBSplineSurface( BSplineSurface * mybody );
	~TrimmedBSplineSurface();
	//long Size();
	//void operator + ( BSplineCurve * newtrimcurve );
	//BSplineCurve * operator [] ( long index );
private:
	BSplineSurface *thebody;
	//typedef std::vector <BSplineCurve *> TrimLoop;
	//typedef std::vector <TrimLoop *> AllTrimLoops;
	// These curves define boundary in the parameter space of thebody.
	AllTrimLoops alltrimloops;
	gmTrimLoop * currtrimloop;
};

#endif // __TrimmedBSplineSurface_h__