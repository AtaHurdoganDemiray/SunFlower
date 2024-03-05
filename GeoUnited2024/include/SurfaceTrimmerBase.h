// SurfaceTrimmerBase.h

#ifndef __SurfaceTrimmerBase_h__
#define __SurfaceTrimmerBase_h__

class IgesTrimLoop2d;
#include "IgesEntitySet.h"
class UntrimmedSurface;
class SequentialLineSet2d;
class IgesEntity;
class PosCubes2d;
struct VPoint;
class CurvePointGenerator;

class SurfaceTrimmerBase
{
public:
	SurfaceTrimmerBase (double tol);
	virtual ~SurfaceTrimmerBase();
	void MakeLoops2d (const IgesTrimLoop2d &outerloop 
					, const std::vector<IgesTrimLoop2d *> &innerloop 
					, const IgesEntitySet &allent ,const UntrimmedSurface &surf);

	void DigitiseLoop (const IgesEntity *ent 
						, const IgesEntitySet &allEnt
						, const UntrimmedSurface &surf
						, SequentialLineSet2d &loop
						, bool isReversed);
	bool ShouldIReverse (const CurvePointGenerator &crv , const SequentialLineSet2d &sl , const UntrimmedSurface &surf);
	
	//void MakeLoopsForBoundedSurf (const ProcessedBoundedSurface &pbs);


	SequentialLineSet2d *p_outerLoop;
	std::vector <SequentialLineSet2d*> p_innerLoopVec;
	double m_tol;
	IgesEntitySet m_eSet;
};

#endif