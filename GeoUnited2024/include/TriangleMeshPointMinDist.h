// TriangleMeshPointMinDist.h

#ifndef __TriangleMeshPointMinDist_h__
#define __TriangleMeshPointMinDist_h__

#include "PosCubes.h"
class TriangleMesh;

struct VPoint;

class TriangleMeshPointMinDist
{
public:
	TriangleMeshPointMinDist ( const TriangleMesh & newmesh );
	vreal MinDist ( const VPoint & p , VPoint & pointonmesh , VPoint &cubeCenter) const;
	vreal MinDistWithAllTriangle ( const VPoint & p , VPoint & pointonmesh , VPoint &cubeCenter) const;

protected:
	const TriangleMesh & mesh;
	PosCubes cubes;
};

#endif