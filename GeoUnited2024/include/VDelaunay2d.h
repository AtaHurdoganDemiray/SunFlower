// VDelaunay2d.h

#ifndef __VDelaunay2d_h__
#define __VDelaunay2d_h__

class PointSet2d;
class TriangleMesh2d;
class RefTriangleMesh;
struct VPoint2d;

class VDelaunay2d
{
public:
	VDelaunay2d ();
	void Calculate (PointSet2d &allpoints , TriangleMesh2d &result);
	void Calculate (PointSet2d &allpoints , RefTriangleMesh &result);
	static VPoint2d * Calculate (std::list<VPoint2d> &plist
								,RefTriangleMesh &result
								,int &arraysize);
	//void Calculate (std::vector<VPoint2d *> &vec , RefTriangleMesh &result);
	//VPoint2d * MakePointArray (PointSet2d &allpoints,int &addedPointNo);
	VPoint2d * MakePointArray (PointSet2d &allpoints);
};

#endif