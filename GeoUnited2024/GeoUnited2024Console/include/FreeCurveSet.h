// FreeCurveSet.h

#ifndef __FreeCurveSet_h__
#define __FreeCurveSet_h__

#include "BSplineCurve.h"
#include <vector>

class IgesEntitySet;
class gmTrimmedSurfaceSet;
struct DoubleVec;
class LineSet;

class FreeCurveSet
{
public:
	FreeCurveSet ( const IgesEntitySet &allentities , const gmTrimmedSurfaceSet &alltrimmedsurf );
	long Size () const
	{
		return ( allsurfaces.size() );
	}
	BSplineCurve * operator [] ( const long &index ) const
	{
		if ( index < 0 || index >= Size() )
			return NULL;
		return allsurfaces[index];
	}
	void CalcLineSetForCurve 
		( const BSplineCurve &curve , LineSet &curvelines , DoubleVec &params ) const;
private:
	std::vector<BSplineCurve *> allsurfaces;

};

#endif