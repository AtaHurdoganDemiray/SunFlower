// SurfaceSpace.h

#ifndef __SurfaceSpace_h__
#define __SurfaceSpace_h__

#include "SpaceConverter.h"
class UntrimmedSurface;
class PointSet2d;
class PointSet;
class PosCubes;
struct VPoint2d;
class SequentialLineSet;
class SequentialLineSet2d;
class UntrimmedSurface;

class SurfaceSpace:public SpaceConverter
{
public:
	SurfaceSpace (UntrimmedSurface *surf,double tol,int pointCountInOneDirection = 50 , int refinePointCountInOneDirection = 10);
	SurfaceSpace (PointSet2d *parVec , PointSet *modVec,UntrimmedSurface *surf,double tol , int pointCountInOneDirection = 50 , int refinePointCountInOneDirection = 10);
	~SurfaceSpace();
	virtual VPoint Convert (const VPoint &p);
	virtual VPoint ConvertThrowIfOut(const VPoint &p , double tol);
	void SetParModVec (PointSet2d *parVec,PointSet *modVec);
	void SetPointCountInOneDirection (int pointCountInOneDirection);
	int GetPointCountInOneDirection ()const;
	void SetRefinePointCountInOneDirection (int refinePointCountInOneDirection);
	int GetRefinePointCountInOneDirection ()const;
	void ProjectModelSpaceTrimLoops (const SequentialLineSet &modelLoop 
									, SequentialLineSet2d &paramLoop
									, std::vector <VPoint> &witnessPoints
									, UntrimmedSurface *surf);
private:
	UntrimmedSurface *m_surf;
	PointSet2d *m_parVec;
	PointSet *m_modVec;
	bool m_ownerOfParModVec;
	void MakeParModVec (int size);
	void FillParModVec (int pointNoInOneDirection);
	PosCubes * MakePosCubes (PointSet *pset);
	PosCubes *m_pc;
	double m_tol;
	void RefineSolutionAround (const VPoint &p , VPoint2d &refinedPar , VPoint &refinedMod,double &dprev , double &mindist);
	int m_pointCountInOneDirection;
	int m_refinePointCountInOneDirection;
	static int m_maxRefineIterationCount;
	double GetDpAroundForTolerance (const VPoint2d &par,UntrimmedSurface *surf);
};
#endif