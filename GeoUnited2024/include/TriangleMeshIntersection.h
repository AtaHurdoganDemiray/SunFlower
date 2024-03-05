// TriangleMeshIntersection.h

#ifndef __TriangleMeshIntersection_h__
#define __TriangleMeshIntersection_h__

class TriangleMesh;
class LineSet;
struct VPlane;
class MultiTriangles;

class TriangleMeshIntersection
{
public:
	void Intersect (const TriangleMesh &m1 , const TriangleMesh &m2 , LineSet &res);
	void Intersect (const TriangleMesh &m1 
					, const VPlane &p 
					, const vreal &offs
					, LineSet &res);
};

#endif