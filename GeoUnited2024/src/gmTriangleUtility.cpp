// gmTriangleUtility.cpp

#include "stdafx.h"
#include "gmTriangleUtility.h"
//#include "DynamicPosCubes2d.h"
//#include "DebugHelper.h"
#include "VRefTriangleMesh.h"
#include "RefTriangle.h"
#include "SurfPointsPack.h"




gmTriangleUtility::gmTriangleUtility()//( gmTriangle &mytri ):m_tri ( mytri )
{

}

//#############################################################################

gmTriangleUtility::~gmTriangleUtility()
{

}

//#############################################################################
/*
void gmTriangleUtility::AddTriangle 
( const VPoint &cor1 , const VPoint &cor2 , const VPoint &cor3 
 , const VPoint& normal1 , const VPoint &normal2 , const VPoint &normal3 
 , const VPoint2d &parcor1 , const VPoint2d &parcor2 , const VPoint2d &parcor3
 , TriangleMesh &theresult 
 , DynamicPosCubes2d &dynamicposcubes ) const

{
//	if ( CheckTri ( cor1,cor2,cor3 ) )
//		int stp=1;

	// checking if points are ordered clockwise or counter clockwise
	VPoint edge1 = cor2 - cor1;
	VPoint edge3 = cor3 - cor1;

	VPoint cross = edge1 ^ edge3;

	double dotp = cross % normal1;

	if ( dotp >= 0 )
	{
		VTriangle2dE newtri( parcor1 , parcor2 , parcor3 );
		theresult.AddTriangle ( cor1 , cor2 , cor3 );
		dynamicposcubes.AddTriangle2d( newtri );
		theresult.GetNormalSet().InsertPoint(normal1.GetArray());
		theresult.GetNormalSet().InsertPoint(normal2.GetArray());
		theresult.GetNormalSet().InsertPoint(normal3.GetArray()); 
	}
	else
	{
		VTriangle2dE newtri( parcor3 , parcor2 , parcor1 );
		theresult.AddTriangle ( cor3 , cor2 , cor1 );
		dynamicposcubes.AddTriangle2d ( newtri );
		theresult.GetNormalSet().InsertPoint(normal3.GetArray());
		theresult.GetNormalSet().InsertPoint(normal2.GetArray());
		theresult.GetNormalSet().InsertPoint(normal1.GetArray()); 
	}

}
*/
//#############################################################################

void gmTriangleUtility::AddTriangle 
					( const VPoint &cor1 , const VPoint &cor2 , const VPoint &cor3 
					  , const VPoint& normal1 , const VPoint &normal2 , const VPoint &normal3 
					  , const VPoint2d &parcor1 , const VPoint2d &parcor2 , const VPoint2d &parcor3					  
					  , TriangleMesh &theresult 
					  , TriangleMesh2d &paramtris ) const
{	
	// checking if points are ordered clockwise or counter clockwise
	VPoint edge1 = cor2 - cor1;
	VPoint edge3 = cor3 - cor1;

	VPoint cross = edge1 ^ edge3;

	double dotp = cross % normal1;

	if ( dotp >= 0 )
	{
		bool ck = theresult.AddTriangle ( cor1 , cor2 , cor3 );
		if ( ck )	
		{
			paramtris.AddTriangle ( parcor1 , parcor2 , parcor3 );
			//theresult.GetNormalSet().InsertPoint(normal1.GetArray());
			//theresult.GetNormalSet().InsertPoint(normal2.GetArray());
			//theresult.GetNormalSet().InsertPoint(normal3.GetArray()); 
		}
	}
	else
	{
		bool ck = theresult.AddTriangle ( cor3 , cor2 , cor1 );
		if ( ck )
		{
			paramtris.AddTriangle ( parcor3 , parcor2 , parcor1 );
			//theresult.GetNormalSet().InsertPoint(normal3.GetArray());
			//theresult.GetNormalSet().InsertPoint(normal2.GetArray());
			//theresult.GetNormalSet().InsertPoint(normal1.GetArray()); 
		}
	}

}

//#############################################################################

void  gmTriangleUtility::AddTriangle ( const SurfPointsPack &surfpack 
		, const RefTriangle &tri , VRefTriangleMesh &trimesh ) const 
{	
	const VPoint &cor1 = surfpack.GetModelPoint( tri.ref0 );
	const VPoint &cor2 = surfpack.GetModelPoint( tri.ref1 );
	const VPoint &cor3 = surfpack.GetModelPoint( tri.ref2 );
	const VPoint &normal1 = surfpack.GetNormal ( tri.ref0 );	

	// checking if points are ordered clockwise or counter clockwise
	VPoint edge1 = cor2 - cor1;
	VPoint edge3 = cor3 - cor1;

	VPoint cross = edge1 ^ edge3;

	double dotp = cross % normal1;

	if ( dotp >= 0 )
	{
		trimesh.AddTriangle ( tri );
	}
	else
	{
		RefTriangle tmp ( tri.ref2 , tri.ref1 , tri.ref0 );
		trimesh.AddTriangle ( tmp );
	}

}

//#############################################################################

bool gmTriangleUtility::IsVertexOrderCorrect ( const VPoint & cor1 , const VPoint & cor2 , const VPoint & cor3 , const VPoint & normal )const
{	
	// checking if points are ordered clockwise or counter clockwise
	VPoint edge1 = cor2 - cor1;
	VPoint edge3 = cor3 - cor1;

	VPoint cross = edge1 ^ edge3;

	double dotp = cross % normal;

	if ( dotp >= 0 )
		return true;
	else
		return false;

}
