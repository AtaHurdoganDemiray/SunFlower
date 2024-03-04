// ProcessedFace.h

#ifndef __ProcessedFace_h__
#define __ProcessedFace_h__

#include "IgesTrimLoop2d.h"
#include "IgesTrimLoop.h"

class Face;
class IgesEntitySet;
class BSplineCurve;
class IgesEntity;
class dCurve;

class ProcessedFace
{
public:
public:
	ProcessedFace(Face & _tsurf, IgesEntitySet & _allent , std::map<int,std::map<int,dCurve*>>&edgeLines);
	virtual ~ProcessedFace();
	IgesTrimLoop2d outerloop;
	IgesTrimLoop m_outerloop;
	std::vector<IgesTrimLoop2d *>innerloop;
	std::vector<IgesTrimLoop *>m_innerloop;
	const IgesEntity * GetSurfaceIgesEntity()const;
	IgesEntity * GetSurfaceIgesEntity();
	IgesEntitySet & allent;
	bool Is2dLoopsGiven()const;
	const Face & GetFace()const;
	Face & GetFace();
protected:
	void FormAllTrimLoops();
	void FormOuterLoop();
	void FormInnerLoops();
	void FormLoop(IgesEntity * ptmp, bool triminside);
	void FormLoop3d(IgesEntity * ptmp, bool triminside);
	//bool IsCurveSupported ( IgesEntity * pigs ) const;

	Face & tsurf;
	IgesEntity * surf;
	bool m_is2dLoopsGiven;
public:
	std::map<int,std::map<int,dCurve *>> &m_edgeLines;
	std::vector<dCurve *> m_outerLoop;
	std::vector<std::vector<dCurve *>> m_innerLoops;
};

#endif