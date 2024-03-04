// LoopTriangleIntersection.h

#ifndef __LoopTriangleIntersection_h__
#define __LoopTriangleIntersection_h__

#include "LineIntersection2d.h"

struct RefTriangle;
class VRefTriangleMesh;
struct VLine2d;
class SequentialLineSet2d;
class LineSet2d;
class PosCubes2d;

class LoopTriangleIntersection
{
public:
	bool IsIntersecting (const VLine2d &l 
						, const SequentialLineSet2d &loop 
						, const PosCubes2d &cubes
						, LineSet2d &intLines);
	void SplitTriangle (RefTriangle &tri 
						, VRefTriangleMesh &mesh
						, VLine2d &l
						, std::vector<RefTriangle> &splitRes);
					

private:

};

#endif