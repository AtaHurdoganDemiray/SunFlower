// TriangleIntersection.h

#ifndef __TriangleIntersection_h__
#define __TriangleIntersection_h__

struct VTriangleE;
struct VLine;
class VInterval;
struct VPlane;
struct VPoint;

class TriangleIntersection
{
public:
	enum IntType {NOINT , COINCIDENT , REGULAR};
	static IntType Int (const VTriangleE &t1 
				, const VTriangleE &t2 
				, bool &intrsct
				, VLine &intL );
	
	static bool Int (const VTriangleE &t 
			, const VPlane &p		
			, VLine &intL );

	static bool Int (const VTriangleE &t , const VLine &l , VPoint &intp);
	static bool IsTriCollidingToBox(const VTriangleE &tri , const VPoint &boxmin, const VPoint &boxmax);
	static void TriangulateBox(const VPoint &boxMin, const VPoint &boxMax, std::vector<VTriangleE> &tri);
};

#endif