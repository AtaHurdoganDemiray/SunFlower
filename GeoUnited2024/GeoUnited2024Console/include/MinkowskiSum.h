// MinkowskiSum.h

#ifndef __MinkowskiSum_h__
#define __MinkowskiSum_h__

struct VTriangle;
class TriangleMesh;

class MinkowskiSum
{
public:
	// moving triangle is already rotated and mirrored along cs Z
	static void Tri2TriAllInABS(const VTriangle &tmoving , const VTriangle &tstatic , TriangleMesh &sum);
	static void Add2TriToMinkowskiSumIf(const VPoint &tmoveV0, const VPoint &tmoveV1, const VPoint &lvecmoving ,const VPoint &tstaticV0, const VPoint &tstaticV1 , TriangleMesh &minkowskiSum);
	static void Mesh2MeshAllInABS(TriangleMesh &toolMesh, const TriangleMesh &partMesh, TriangleMesh &sum);
};

#endif