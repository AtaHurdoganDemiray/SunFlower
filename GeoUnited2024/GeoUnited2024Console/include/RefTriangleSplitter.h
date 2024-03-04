// RefTriangleSplitter.h

#ifndef __RefTriangleSplitter_h__
#define __RefTriangleSplitter_h__
#include "BasicDefinitions.h"
struct VPoint2d;
struct RefTriangle;
class RefTriangleMesh;

class RefTriangleSplitter
{
public:
	
	static void Split (const std::vector<VPoint2d> &points
						, std::list<int> &indvec
						, std::list<int> &shared
						, const RefTriangle &parent
						, RefTriangleMesh &result ); 

	typedef std::list<int>::iterator intitr;
	struct SS
	{
	public:
		intitr i;
		vreal d;
	};
	typedef std::vector<SS>::iterator ssitr;
	
};

#endif