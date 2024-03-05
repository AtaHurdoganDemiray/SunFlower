// SurfaceQuadBspTrimmer.h

#ifndef __SurfaceQuadBspTrimmer_h__
#define __SurfaceQuadBspTrimmer_h__
#include "BspTree.h"

class SequentialLineSet2d;
class UntrimmedSurface;
class VRefTriangleMesh; 

class SurfaceQuadBspTrimmer
{
public:

	SurfaceQuadBspTrimmer(const std::vector<vreal> &ugrid
						,const std::vector<vreal> &vgrid
						,const SequentialLineSet2d *outerLoop
						,const std::vector<SequentialLineSet2d *> &innerLoopVec
						,const UntrimmedSurface &surf);
	~SurfaceQuadBspTrimmer();
	void TrimAndTriangulateSingleQuad (SequentialLineSet2d &q);
	void MakeBsp ();
	VRefTriangleMesh * GetResult();
private:
	const SequentialLineSet2d *m_outerLoop;
	const std::vector<SequentialLineSet2d *> &m_innerLoopVec;
	const UntrimmedSurface &m_surf;
	geo::BspTree *m_bspouter;
	std::vector<geo::BspTree *> m_bspinnervec;
	static std::vector<VPoint2d> m_pointpool;
	static int m_pointcount;
	static SequentialLineSet2d m_unitSquare;
	VRefTriangleMesh *m_result;
	void AddTriToPool (const geo::BspTree *myloop , const VPoint2d &c1 ,const VPoint2d &c2,const VPoint2d &c3);
};

#endif