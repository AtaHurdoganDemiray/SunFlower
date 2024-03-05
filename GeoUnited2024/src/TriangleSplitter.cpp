// TriangleSplitter.cpp

#include "stdafx.h"
#include "TriangleSplitter.h"
#include "BasicLineFunctions2d.h"
#include "VPoint2d.h"
#include "VTriangle2dE.h"
#include "RefTriangle.h"
#include "PointSet2d.h"
#include "TriangleMesh2d.h"




TriangleSplitter::TriangleSplitter ( const vreal & _tol )
{
	tol = _tol;
}

bool TriangleSplitter::SplitTriangleByPoint2d ( const VTriangle2dE &target
												, const RefTriangle &reftri
												, const VPoint2d &checkpoint
												, const long &pointref
												, bool onedge[]
												, RefTriangle e[3] ) const
{
	if ( target.IsInTriangle ( checkpoint ) )
	{	
		bool ck = BasicLineFunctions2d::IsPointOnInfiniteLine2d 
			( target.GetCorner1() , target.GetCorner2() , checkpoint );

		if ( ck==false )
		{
			e[0].ref0 = reftri.ref0;
			e[0].ref1 = reftri.ref1;
			e[0].ref2 = pointref;
			onedge[0] = false;
		}
		else
			onedge[0] = true;

		ck = BasicLineFunctions2d::IsPointOnInfiniteLine2d 
			( target.GetCorner2() , target.GetCorner3() , checkpoint );

		if ( ck==false )
		{
			e[1].ref0 = reftri.ref1;
			e[1].ref1 = reftri.ref2;
			e[1].ref2 = pointref;
			onedge[1] = false;
		}
		else 
			onedge[1] = true;

		ck = BasicLineFunctions2d::IsPointOnInfiniteLine2d
			 ( target.GetCorner3() , target.GetCorner1() , checkpoint );

		if ( ck==false )
		{
			e[2].ref0 = reftri.ref2;
			e[2].ref1 = reftri.ref0;
			e[2].ref2 = pointref;
			onedge[2] = false;
		}
		else
			onedge[2] = true;
	
		return true;
	}
	else
		return false;

};

bool TriangleSplitter::SplitTriangleByPoint2d ( const VTriangle2dE &target
												, const VPoint2d & checkpoint 
												, int & formedtrino
												, VTriangle2dE formedtri[] ) const
{
	formedtrino = 0;

	//vreal offsettol = 1.0e-6;
	if ( target.IsInTriangle ( checkpoint/*,offsettol*/ ) )
	{
		bool onedge[3];
		//BasicLineFunctions2d::ispointonline2dinfo info;
		//vreal mys;

		//bool ck = calc.IsPointOnLine2d 
		//	( target.GetCorner1() , target.GetEdge1() , checkpoint , mys , info /*, offsettol*/ );

		bool ck = BasicLineFunctions2d::IsPointOnInfiniteLine2d ( target.GetCorner1() 
												, target.GetEdge1() 
												, checkpoint , tol );

		//if ( ck )
		//	if ( fabs(mys) < EPSILON || fabs(mys-1.0) < EPSILON )
		//		return false;

		if ( ck==false )
			onedge[0] = false;
		else
			onedge[0] = true;

		//ck = calc.IsPointOnLine2d 
		//	( target.GetCorner1() , target.GetEdge2() , checkpoint , mys , info /*, offsettol*/ );
		
		ck = BasicLineFunctions2d::IsPointOnInfiniteLine2d ( target.GetCorner2() 
											, target.GetEdge2() 
											, checkpoint , tol );

		//if ( ck )
		//	if ( fabs(mys) < EPSILON || fabs(mys-1.0) < EPSILON )
		//		return false;
		
		if ( ck==false )
			onedge[1] = false;
		else 
			onedge[1] = true;

		//ck = calc.IsPointOnLine2d
		//	( target.GetCorner2() , target.GetEdge3() , checkpoint , mys , info /*, offsettol*/ );

		ck = BasicLineFunctions2d::IsPointOnInfiniteLine2d ( target.GetCorner3() 
											, target.GetEdge3() 
											, checkpoint , tol );

		//if ( ck )
		//	if ( fabs(mys) < EPSILON || fabs(mys-1.0) < EPSILON )
		//		return false;

		if ( ck==false )
			onedge[2] = false;
		else
			onedge[2] = true;
	
		if ( onedge[0] == false )
		{
			VTriangle2dE tmp ( target.GetCorner1() , target.GetCorner2() , checkpoint );
			formedtri[formedtrino] = tmp;
			formedtrino++;
		}
		if ( onedge[1] == false )
		{
			VTriangle2dE tmp ( target.GetCorner2() , target.GetCorner3() , checkpoint );
			formedtri[formedtrino] = tmp;
			formedtrino++;
		}
		if ( onedge[2] == false )
		{
			VTriangle2dE tmp ( target.GetCorner3() , target.GetCorner1() , checkpoint );
			formedtri[formedtrino] = tmp;
			formedtrino++;
		}

		return true;
	}
	else
		return false;

};

void TriangleSplitter::SplitTriangle ( const VTriangle2dE & tri , const PointSet2d & pointset , TriangleMesh2d & result ) const
{
	result.AddTriangle ( tri );
	for ( int i=0; i < result.Size(); i++)
	{
		VTriangle2dE * currtri = result[i];
		
		for ( int j=0; j < pointset.Size(); j++ )
		{
			int trino;
			VTriangle2dE formedtri[3];
			const VPoint2d * currpoint = pointset[j];
			bool ck = SplitTriangleByPoint2d ( *currtri , *currpoint , trino , formedtri );
			if ( ck )
			{
				for ( int w=0; w < trino; w++ )
				{
					if ( w==0 )
						(*currtri) = formedtri[w];
					else
						result.AddTriangle ( formedtri[w] );
				}
			}
		}
	}
}
