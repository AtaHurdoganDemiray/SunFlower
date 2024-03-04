// PolygonToTriangles.h

#ifndef __PolygonToTriangles_h__
#define __PolygonToTriangles_h__

class SequentialLineSet2d;
class TriangleMesh2d;
struct VLine2d;

namespace geo
{
	class BspTree;

	class PolygonToTriangles
	{
	public:
		void Triangulate (const SequentialLineSet2d &sl , const BspTree &T 
							, TriangleMesh2d &tri);
	private:
		void Split (const SequentialLineSet2d &sl,int i0,int i1
					,SequentialLineSet2d &sub1 , SequentialLineSet2d &sub2);
		bool IsDiagonal (const VLine2d &l , const BspTree &T);
	};
};
#endif