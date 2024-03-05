// TriangleSplitter.h

#ifndef __TriangleSplitter_h__
#define __TriangleSplitter_h__

struct VTriangle2dE;
struct VPoint2d;
struct RefTriangle;
class TriangleMesh2d;
class PointSet2d;

class TriangleSplitter
{
public:
	TriangleSplitter ( const vreal & _tol );
	bool SplitTriangleByPoint2d ( const VTriangle2dE &target
								, const RefTriangle &reftri
								, const VPoint2d &checkpoint
								, const long &pointref
								, bool onedge[]
								, RefTriangle e[3] ) const;

	bool SplitTriangleByPoint2d ( const VTriangle2dE &target
								, const VPoint2d & checkpoint 
								, int & formedtrino
								, VTriangle2dE formedtri[] ) const;

	void SplitTriangle ( const VTriangle2dE & tri , const PointSet2d & pointset , TriangleMesh2d & result ) const;
	
	vreal tol;
};

#endif //__TriangleSplitter_h__
