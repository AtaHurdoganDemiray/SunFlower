// CalcMeshByDelaunay2D.h

#ifndef __CalcMeshByDelaunay2D_h__
#define __CalcMeshByDelaunay2D_h__

class PointSet2d;
class PointSet;
class TriangleMesh2d;
class TriangleMesh;

class CalcMeshByDelaunay2D
{
public:
	 CalcMeshByDelaunay2D ( const PointSet2d &paramp , const PointSet &modelp , const PointSet &normalp );
	 void CalcMesh ( TriangleMesh2d &paramtri , TriangleMesh &modeltri ) const;		
private:

	const PointSet2d &parampoints;
	const PointSet &modelpoints;
	const PointSet &normals;
};

#endif //__CalcMeshByDelaunay2D_h__
