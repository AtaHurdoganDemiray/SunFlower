// gmTriangleUtility.h

#ifndef __gmTriangleUtility_h__
#define __gmTriangleUtility_h__

#include "TriangleMesh.h"
#include "TriangleMesh2d.h"
#include "VPoint.h"
#include "VPoint2d.h"

class DynamicPosCubes2d;
class SurfPointsPack;
class VRefTriangleMesh;
struct RefTriangle;

class gmTriangleUtility
{
public:
	gmTriangleUtility();
	~gmTriangleUtility();
	//void AddTriangle ( const VPoint &cor1 , const VPoint &cor2 , const VPoint &cor3 
	//				  , const VPoint& normal1 , const VPoint &normal2 , const VPoint &normal3 
	//				  , const VPoint2d &parcor1 , const VPoint2d &parcor2 , const VPoint2d &parcor3					  
	//				  , TriangleMesh &theresult 
	//				  , DynamicPosCubes2d &dynamicposcubes ) const;

	void AddTriangle ( const VPoint &cor1 , const VPoint &cor2 , const VPoint &cor3 
					  , const VPoint& normal1 , const VPoint &normal2 , const VPoint &normal3 
					  , const VPoint2d &parcor1 , const VPoint2d &parcor2 , const VPoint2d &parcor3					  
					  , TriangleMesh &theresult 
					  , TriangleMesh2d &paramtris ) const;

	void gmTriangleUtility::AddTriangle ( const SurfPointsPack &surfpack 
		, const RefTriangle &tri , VRefTriangleMesh &trimesh ) const;

	bool IsVertexOrderCorrect ( const VPoint & cor1 , const VPoint & cor2 , const VPoint & cor3 , const VPoint & normal )const;

private:

};

#endif //__gmTriangleUtility_h__