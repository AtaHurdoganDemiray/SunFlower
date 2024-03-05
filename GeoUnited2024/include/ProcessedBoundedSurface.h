// ProcessedBoundedSurface.h

#ifndef __ProcessedBoundedSurface_h__
#define __ProcessedBoundedSurface_h__

//#include <vector>
#include "IgesTrimLoop2d.h"
#include "IgesTrimLoop.h"

class BoundedSurface;
class IgesEntitySet;
class BSplineCurve;
class Boundary;
class IgesEntity;

class ProcessedBoundedSurface
{
public:
	ProcessedBoundedSurface ( BoundedSurface & _bsurf , IgesEntitySet & _allent );
	virtual ~ProcessedBoundedSurface ();

	std::vector<IgesTrimLoop2d *> m_loop2d;
	std::vector<IgesTrimLoop *>	m_loop3d;

	const IgesEntity * GetSurfaceIgesEntity ()const;
	IgesEntity * GetSurfaceIgesEntity ();
	IgesEntitySet & allent;
	const BoundedSurface & GetBoundedSurface()const;
	BoundedSurface & GetBoundedSurface();
protected:
	void FormAllTrimLoops ();
	void FormLoop ( const Boundary *pB );
	void FillLoopByCurve ( IgesEntity * ptmp , IgesTrimLoop2d *loop);
	void FillLoopByCurve3d ( IgesEntity * ptmp , IgesTrimLoop *loop);

	//bool IsCurveSupported ( IgesEntity * pigs ) const;

	BoundedSurface & bsurf;
	IgesEntity * surf;
};

#endif