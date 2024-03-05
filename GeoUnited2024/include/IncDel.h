// IncDel.h

#ifndef __IncDel_h__
#define __IncDel_h__
#include "DelRefMesh.h"

struct VPoint2d;
class RefTriangleMesh;

class IncDel
{
public:
	static void IncDel2d (std::vector<VPoint2d> &points 
						, std::list<int> &pref
						, RefTriangleMesh &refmesh);
	typedef std::list<int>::iterator iitr;
	static void SwapTrianglesTill (std::list <DelRefMesh::tripair> &swapstack,
							DelRefMesh &mesh);

	static void BuildSwapStack (const DelRefMesh &mesh,std::list<DelRefMesh::tripair> &swapstack);
	static void CancelNeighborEdge (DelRefMesh::Tri &ct,int i);
};

#endif