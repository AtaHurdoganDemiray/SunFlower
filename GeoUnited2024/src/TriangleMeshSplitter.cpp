// TriangleMeshSplitter.cpp
#include "stdafx.h"


/*
#include "stdafx.h"
#include "TriangleMeshSplitter.h"
#include "TriangleMesh2d.h"
#include "TriangleSplitter.h"
#include "Tri2TriCollCheck2d.h"
#include "SetMerger.h"
#include "PointSet2d.h"
#include "SetMerger.h"


const int SAFESIZE = 10000;

TriangleMeshSplitter::TriangleMeshSplitter 
( const TriangleMesh2d & _q , const PointSet2d & _extrapoints , TriangleMesh2d & _result )
: q ( _q ) , extrapoints ( _extrapoints ) , result ( _result )
{
	SetMerger merge;
	merge.Merge ( q,result );
	
	for ( int i=0; i < q.Size(); i++ )
	{
		startpointindex.AddLong ( 0 );
	}
}

TriangleMeshSplitter::~TriangleMeshSplitter()
{

}

void TriangleMeshSplitter::Split()
{
	TriangleSplitter trisplit(1.e-5);

	for ( int trii = 0; trii < result.Size(); trii++ )
	{
		VTriangle2dE * currtri = result [trii];
		int startpi = startpointindex[trii];

		for ( int pointi = startpi; pointi < extrapoints.Size(); pointi++ )
		{
			if ( result.Size() > SAFESIZE )
				throw ( VException ( ( IDS_TriMesSpl) , 0  ));
			
			VPoint2d * currp = extrapoints[pointi];
			int formedtrino;
			VTriangle2dE formedtri[3];
			bool ck = trisplit.SplitTriangleByPoint2d 
					( *currtri , *currp , formedtrino , formedtri );
			
			if ( ck )
			{
				//if ( formedtrino > 3 )
				//	throw ( VException (("triangle splitter exception 2"),0));

				for ( int j = 0; j < formedtrino; j++ )
				{
					if ( j==0 )
						(*currtri) = formedtri[0];
					else
						AddTriangle ( formedtri[j] , (pointi+1) );
				
				}// for all splitted triangles
			
			}// if currtri splitted
		
		}// for all extra points
	
	}// for all result triangles
//	SplitByCollPoints ();
}


void TriangleMeshSplitter::AddTriangle ( const VTriangle2dE & newtri , int startpi )
{
	//AddCollPoints ( newtri );
	startpointindex.AddLong ( startpi );	
	result.AddTriangle ( newtri );
}
*/
