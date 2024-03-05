// ProcessedTrimmedSurface.h

#ifndef __ProcessedTrimmedSurface_h__
#define __ProcessedTrimmedSurface_h__

//#include <vector>
#include "IgesTrimLoop2d.h"
#include "IgesTrimLoop.h"

class TrimmedSurface;
class IgesEntitySet;
class BSplineCurve;
class IgesEntity;

class ProcessedTrimmedSurface
{
public:
	ProcessedTrimmedSurface ( TrimmedSurface & _tsurf , IgesEntitySet & _allent );
	virtual ~ProcessedTrimmedSurface();
	IgesTrimLoop2d outerloop;
	IgesTrimLoop m_outerloop;
	std::vector<IgesTrimLoop2d *>innerloop;
	std::vector<IgesTrimLoop *>m_innerloop;
	const IgesEntity * GetSurfaceIgesEntity ()const;
	IgesEntity * GetSurfaceIgesEntity ();
	IgesEntitySet & allent;
	bool Is2dLoopsGiven()const;
	const TrimmedSurface & GetTrimmedSurface()const;
	TrimmedSurface & GetTrimmedSurface();
protected:
	void FormAllTrimLoops ();
	void FormOuterLoop();
	void FormInnerLoops();	
	void FormLoop ( IgesEntity * ptmp , bool triminside );
	void FormLoop3d( IgesEntity * ptmp , bool triminside );
	//bool IsCurveSupported ( IgesEntity * pigs ) const;

	TrimmedSurface & tsurf;
	IgesEntity * surf;
	bool m_is2dLoopsGiven;
};

#endif