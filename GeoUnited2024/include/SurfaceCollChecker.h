// SurfaceCollChecker.h

#ifndef __SurfaceCollChecker_h__
#define __SurfaceCollChecker_h__
#include "PosCubes.h"
#include "vTool.h"

struct VPoint;
class VSurface;
class MultiTriangles;
struct VTriangleE;

class SurfaceCollChecker
{
public:
	SurfaceCollChecker (const std::vector<VSurface *> &surfVec
						, const vTool<float> &tool );
	~SurfaceCollChecker();
	bool Check (VPoint &pos);
	//bool Check (VPoint &posb , VPoint &pose);
	
private:
	PosCubes *m_cubes;
	const vTool<float> &m_tool;
	MultiTriangles *m_tri;
	//bool Check (const VTriangleE &tri,const VPoint &pos,const ToolPrim<float> &prim);
	//bool Check (const VTriangleE &tri , const VPoint &posb , const VPoint &pose,const ToolPrim<float> &prim);
	std::vector<sVector<float,3>> m_pvec;
	//std::vector<sVector<float,3>> m_nvec;

};

#endif