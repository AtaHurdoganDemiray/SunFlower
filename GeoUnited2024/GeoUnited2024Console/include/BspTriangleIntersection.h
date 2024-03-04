// BspTriangleIntersection.h

#ifndef __BspTriangleIntersection_h__
#define __BspTriangleIntersection_h__

struct VTriangle2dE;
class TriangleMesh2d;

namespace geo
{
	class BspTree;

	class BspTriangleIntersection
	{
		static void Intersect (const VTriangle2dE &tri , const BspTree &bsp , TriangleMesh2d &intr);
	private:	
		BspTree * Intersect (const VTriangle2dE &tri , BspTree &bsp);
		//void PartitionAnd
	};

};//namespace geo
#endif