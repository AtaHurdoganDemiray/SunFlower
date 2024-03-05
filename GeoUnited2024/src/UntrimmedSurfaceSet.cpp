// UntrimmedSurfaceSet.h

#include "stdafx.h"
#include "UntrimmedSurfaceSet.h"
#include "IgesEntitySet.h"
#include "gmTrimmedSurfaceSet.h"
#include "CalcMeshByDelaunay2D.h"
#include "gmNurbsTesselator.h"
#include "gmTriangleUtility.h"
#include "gmTrimmedSurface.h"
#include "gmBoundedSurface.h"
#include "VRefTriangleMesh.h"
#include "UntrimmedSurface.h"
#include "gmGeneralIgesInfo.h"
#include "UntrimmedSurface.h"
#include "SetMerger.h"
#include "SurfaceDigitiserWithTol.h"
#include "gmQuadSet.h"

UntrimmedSurfaceSet::UntrimmedSurfaceSet 
( const IgesEntitySet &allentities , const gmTrimmedSurfaceSet &alltrimmedsurf )
{
	gmGeneralIgesInfo gnrlinfo ( allentities );

	for ( long i=0; i<allentities.Size(); i++)
	{
		IgesEntity *pe = allentities[i];
		UntrimmedSurface tmp ( pe );

		if ( gnrlinfo.IsSurfSupported ( tmp ) )
		{
			bool record = true;
			for ( long j=0; j < alltrimmedsurf.gmTrimmedSurfaceSize(); j++ )
			{
				gmTrimmedSurface *pts = alltrimmedsurf.GetgmTrimmedSurface(j);
				
				bool ck = gnrlinfo.IsSurfaceIn 
					( pe->directorylineno , *(pts->GetData()->m_trimmedsurface) );
				
				if ( ck == true )
				{
					record = false;
					break;
				}
				
			}
			
			if ( record == false )
				continue;
			
			for ( j=0; j < alltrimmedsurf.gmBoundedSurfaceSize(); j++ )
			{
				gmBoundedSurface *pb = alltrimmedsurf.GetgmBoundedSurface(j);

				bool ck = gnrlinfo.IsSurfaceIn ( pe->directorylineno 
					, *(pb->GetData()->m_boundedsurface) );

				if ( ck == true )
				{
					record = false;
					break;
				}
			}
			
			if ( record == true )
				allsurfaces.push_back ( new UntrimmedSurface ( pe ) );
		}
	}
			
}				
					
//#############################################################################

UntrimmedSurfaceSet::~UntrimmedSurfaceSet()
{
	for ( int i = 0; i < Size(); i++ )
	{
		UntrimmedSurface * tmp = allsurfaces[i];
		delete tmp;
	}
}

//#############################################################################

void UntrimmedSurfaceSet::CalcMeshForSurf 
( const BSplineSurface &surf , TriangleMesh2d &paramtri , TriangleMesh &modeltri ) const
{
	gmNurbsTesselator calc;
	
	long initialcapacity = ( NURBS_SURFACE_TESSELATION_POINTS_PER_DIR * NURBS_SURFACE_TESSELATION_POINTS_PER_DIR )+ 5;
	
	PointSet modelpoints(initialcapacity) , normals(initialcapacity);
	PointSet2d parampoints(initialcapacity);
	
	calc.BSplineSurface2PointSet ( surf , modelpoints , normals , parampoints );

//	CalcMeshByDelaunay2D buildmesh ( parampoints , modelpoints , normals );
//	buildmesh.CalcMesh ( paramtri , modeltri );
	gmTriangleUtility help;
	long mysize = parampoints.Size();

	for ( long i=0; i < mysize; i++ )
	{
		// form a corner ref for the quad and record it
		long cor1ref = i;
		long cor2ref = i + 1;
		long cor3ref = i + NURBS_SURFACE_TESSELATION_POINTS_PER_DIR+1;
		long cor4ref = i + NURBS_SURFACE_TESSELATION_POINTS_PER_DIR;

		if ( cor3ref < mysize 
			&& (i+1)%(NURBS_SURFACE_TESSELATION_POINTS_PER_DIR) != 0 )
		{
			//AddItem ( cor1ref , cor2ref , cor3ref , cor4ref );
			VPoint2d * pc1 = parampoints[cor1ref];
			VPoint2d * pc2 = parampoints[cor2ref];
			VPoint2d * pc3 = parampoints[cor3ref];
			VPoint2d * pc4 = parampoints[cor4ref];

			VPoint * mc1 = modelpoints[cor1ref]; 
			VPoint * mc2 = modelpoints[cor2ref]; 
			VPoint * mc3 = modelpoints[cor3ref]; 
			VPoint * mc4 = modelpoints[cor4ref]; 
		
			VPoint *n1 = normals[cor1ref];
			VPoint *n2 = normals[cor2ref];
			VPoint *n3 = normals[cor3ref];
			VPoint *n4 = normals[cor4ref];
	
			help.AddTriangle 
			( *mc1 , *mc2 , *mc3 , *n1 , *n2 , *n3 , *pc1 , *pc2 , *pc3 , modeltri , paramtri);	
	
			help.AddTriangle 
			( *mc3 , *mc4 , *mc1 , *n3 , *n4 , *n1 , *pc3 , *pc4 , *pc1 , modeltri , paramtri);	
		}
	}
}

//#############################################################################

void UntrimmedSurfaceSet::CalcRefMeshForSurf 
( BSplineSurface &surf , VRefTriangleMesh &refmesh ) const
{/*
	gmNurbsTesselator calc;
	
	long initialcapacity = ( NURBS_SURFACE_TESSELATION_POINTS_PER_DIR * NURBS_SURFACE_TESSELATION_POINTS_PER_DIR )+ 5;
	
	calc.BSplineSurface2PointSet ( surf , refmesh.surfpack.modelpoints , refmesh.surfpack.normals , refmesh.surfpack.parampoints );
	gmTriangleUtility help;
	long mysize = refmesh.surfpack.parampoints.Size();

	for ( long i=0; i < mysize; i++ )
	{
		// form a corner ref for the quad and record it
		long cor1ref = i;
		long cor2ref = i + 1;
		long cor3ref = i + NURBS_SURFACE_TESSELATION_POINTS_PER_DIR+1;
		long cor4ref = i + NURBS_SURFACE_TESSELATION_POINTS_PER_DIR;

		RefTriangle reftri1 ( cor1ref , cor2ref , cor3ref );
		RefTriangle reftri2 ( cor3ref , cor4ref , cor1ref );

		if ( cor3ref < mysize 
			&& (i+1)%(NURBS_SURFACE_TESSELATION_POINTS_PER_DIR) != 0 )
		{
			help.AddTriangle ( refmesh.surfpack , reftri1 , refmesh );
			help.AddTriangle ( refmesh.surfpack , reftri2 , refmesh );
		}
	}
*/
	double tolerance = 0.04;
	gmTriangleUtility help;	
	UntrimmedSurface surface ( &surf );
	SurfaceDigitiserWithTol digitiser ( surface );
	gmQuadSet * quadset = digitiser.DigitiseSurface ( tolerance , refmesh.surfpack );

	for ( int i=0; i < quadset->Size(); i++ )
	{
		const CornerRef * currref = (*quadset)[i];
		RefTriangle reftri1 ( currref->cor1ref , currref->cor2ref , currref->cor3ref );
		RefTriangle reftri2 ( currref->cor3ref , currref->cor4ref , currref->cor1ref );
		help.AddTriangle ( refmesh.surfpack , reftri1 , refmesh );
		help.AddTriangle ( refmesh.surfpack , reftri2 , refmesh );
	}
}

//#############################################################################
