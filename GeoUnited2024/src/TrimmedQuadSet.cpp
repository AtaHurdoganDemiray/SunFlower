// TrimmedQuadSet.cpp

#include "stdafx.h"
#include "TrimmedQuadSet.h"
#include "gmQuadSet.h"
#include "PreTrimmedQuadSet.h"
#include "TrimLoop2dSet.h"
#include "TrimLoop2d.h"
#include "TriangleMesh2d.h"
#include "QuadSplitter.h"
#include "SetMerger.h"
#include "TrimTriangleByLoop.h"
#include "TriangleMeshSplitter.h"
#include "VDelaunay2d.h"
#include "TrimQuad.h"

// for debug
//#include "UntrimmedSurface.h"
//#include "debughelper.h"


TrimmedQuadSet::TrimmedQuadSet ( const gmQuadSet & _quads , TrimLoop2dSet & _trimloops )
: quads ( _quads ) , trimloops ( _trimloops ) 
, prevquadno ( 0 ) , isprevquadsplitted ( false ) , isprevquadtrimmed ( false )
{
	//for ( long i = 0; i < quads.Size(); i++ )
	//	extratrianglesinfo.AddLong(i);


	//isquadvalid = new bool [quads.Size()];
}

TrimmedQuadSet::~TrimmedQuadSet()
{
	for ( unsigned int i = 0; i < extratriangles.size(); i++ )
	{
		TriangleMesh2d * currmesh = extratriangles[i];
		delete currmesh;
	}
	//delete [] isquadvalid;
}

void TrimmedQuadSet::RecordExtraTriangles ( int quadno , TriangleMesh2d * extratri )
{
	//(extratrianglesinfo.At(quadno)) = (extratriangles.size());
	extratriangles.push_back ( extratri );
}

void TrimmedQuadSet::SplitQuadWithExtraPoints ( int qno , int extrapointsno 
					, const PreTrimmedQuadSet & prequads , TriangleMesh2d & trimesh )
{
	gmQuad2d q = prequads.mquads.GetQuad2d ( qno );
	PointSet2d * extrapoints = prequads.extrapointsvec[extrapointsno]; 
	//TrimQuad tq;
	//tq.q = q;
	//SetMerger merge;
	//merge.Merge ( *extrapoints , tq.extrap );
	//MakeTriMesh ( tq , trimesh );
	//return;
	
	//QuadSplitter split ( q , *extrapoints , trimesh );
	//split.Split();
	
	if ( IsPointTrimmed ( q.GetCor1() ) == false )
		extrapoints->InsertPoint(q.GetCor1().GetArray());
	if ( IsPointTrimmed ( q.GetCor2() ) == false )
		extrapoints->InsertPoint(q.GetCor2().GetArray());
	if ( IsPointTrimmed ( q.GetCor3() ) == false )
		extrapoints->InsertPoint(q.GetCor3().GetArray());
	if ( IsPointTrimmed ( q.GetCor4() ) == false )
		extrapoints->InsertPoint(q.GetCor4().GetArray());
	
	if ( extrapoints->Size() > 3  )
	{
		VDelaunay2d vd;
		vd.Calculate (*extrapoints , trimesh); 
	}
	else if ( extrapoints->Size() == 3 )
		trimesh.AddTriangle ( *(*extrapoints)[0] , *(*extrapoints)[1] , *(*extrapoints)[2] );

}

TriangleMesh2d * TrimmedQuadSet::TrimTriMesh ( const TriangleMesh2d & source )
{
	TriangleMesh2d * result = new TriangleMesh2d (30);
	
	for ( int i=0; i < source.Size(); i++ )
	{
		VTriangle2dE * currtri = source[i];
		TrimSingleTri ( *currtri , *result );
		//if ( TrimSingleTri(*currtri)==false)
		//	result->AddTriangle(*currtri);	
		
		// for debug
/*		vreal f_u = static_cast<vreal>(currtri->GetCorner1().x());
		vreal f_v = static_cast<vreal>(currtri->GetCorner1().y());
		VPoint c1 = surf->GetPoint ( f_u , f_v );	
		f_u = static_cast<vreal>(currtri->GetCorner2().x());
		f_v = static_cast<vreal>(currtri->GetCorner2().y());		
		VPoint c2 = surf->GetPoint ( f_u , f_v );
		f_u = static_cast<vreal>(currtri->GetCorner3().x());
		f_v = static_cast<vreal>(currtri->GetCorner3().y());	
		VPoint c3 = surf->GetPoint ( f_u , f_v );
		//VPoint p ( 0.3627 , 0.3637 , 0.0 );

		if ( CheckTri ( c1,c2,c3) )
			int stp=1;
		//if ( CheckPwithP ( c1 , p ) || CheckPwithP ( c2 , p ) || CheckPwithP ( c3 , p ) )
		//	int stp = 1;
*/		// for debug
	
	}
	return result;
}

void TrimmedQuadSet::TrimQuadSet ( const PreTrimmedQuadSet & prequads , bool * isquadvalid )
{
	for ( int i = 0; i < prequads.mquads.Size(); i++ )
		TrimSingleQuad ( i , prequads , isquadvalid );
}

void TrimmedQuadSet::TrimSingleQuad ( int qno , const PreTrimmedQuadSet & prequads , bool * isquadvalid )
{
	int extrapointsno =  prequads.extrapointsinfo[qno]; 
	if ( extrapointsno != -1 )
	{
		TriangleMesh2d  startmesh (20);
		SplitQuadWithExtraPoints ( qno , extrapointsno , prequads , startmesh );
		//TriangleMesh2d midmesh (20);
		//ForceMeshSplitting ( startmesh , midmesh ); 
		
		TriangleMesh2d * finalmesh = TrimTriMesh ( startmesh );
		
		RecordExtraTriangles ( qno , finalmesh );
		isquadvalid[qno] = false;
		isprevquadsplitted = true;
	}
	else
	{
		MarkIfTrimmed ( qno , isquadvalid );
		isprevquadsplitted = false;
	}

	prevquadno = qno;
}

void TrimmedQuadSet::MarkIfTrimmed ( int qno , bool * isquadvalid )
{
	gmQuad2d currquad = quads.GetQuad2d ( qno );

	bool isstartquad = false;
	VPoint2d p = currquad.GetCor1();
	if ( fabs(p.x()) < 1.0e-4 || fabs (p.y())<1.0e-4 )
		isstartquad = true;
	
	bool isprevquadneighbor = false;
	
	if ( isprevquadsplitted == false && isstartquad == false )
		isprevquadneighbor = IsPrevQuadNeighbor ( currquad , isstartquad );
	
	if ( 1 || isprevquadsplitted == true || isstartquad == true || isprevquadneighbor == false )
	{
		VPoint2d centerp = currquad.GetCenter();
		bool trimmed;
		for ( int i = 0; i < trimloops.Size(); i++ )
		{
			TrimLoop2d * currloop = trimloops[i];
			trimmed = currloop->IsPointTrimmed ( centerp );
			if ( trimmed )
				break;
		}
		isquadvalid[qno] = !trimmed;
		isprevquadtrimmed = trimmed;
	}
	else
		isquadvalid[qno] = !isprevquadtrimmed;
}

bool TrimmedQuadSet::IsPrevQuadNeighbor ( const gmQuad2d & currquad , bool isstartquad )
{
	bool result = false;
	gmQuad2d prevquad = quads.GetQuad2d ( prevquadno );
	if ( isstartquad == false )
	{
		VPoint2d cp[4];
		cp[0] = currquad.GetCor1();
		cp[1] = currquad.GetCor2();
		cp[2] = currquad.GetCor3();
		cp[3] = currquad.GetCor4();

		for ( int i=0; i < 4; i++ )
		{
			if ( cp[i] == prevquad.GetCor1() )
				return true;
			else if ( cp[i] == prevquad.GetCor2() )
				return true;
			else if ( cp[i] == prevquad.GetCor3() )
				return true;
			else if ( cp[i] == prevquad.GetCor4() )
				return true;
		}
	}

	return false;
}

void TrimmedQuadSet::TrimSingleTri ( const VTriangle2dE & tri , TriangleMesh2d & remainingtri )
{
	TrimTriangleByLoop tr;
	TriangleMesh2d * currmesh;
	TriangleMesh2d * splittedmesh;
	currmesh = new TriangleMesh2d ( 1 );
	currmesh->AddTriangle ( tri );

	for ( int i=0; i < trimloops.Size(); i++ )
	{
		TrimLoop2d * currloop = trimloops[i];
		
		splittedmesh = new TriangleMesh2d ( 6 );

		for ( int j=0; j<currmesh->Size(); j++ )
		{
			VTriangle2dE * currtri = (*currmesh)[j];
			tr.Trim ( *currtri , *currloop , *splittedmesh );
		}
		
		delete currmesh;
		currmesh = splittedmesh;
	}

	SetMerger merge;
	merge.Merge ( *splittedmesh , remainingtri );
	delete splittedmesh;

}

bool TrimmedQuadSet::TrimSingleTri ( const VTriangle2dE & tri )
{
	VPoint2d cm = tri.GetCenterOfMass();

	for ( int i=0; i < trimloops.Size(); i++ )
	{
		TrimLoop2d * currloop = trimloops[i];
		
		if ( currloop->IsPointTrimmed(cm) )
			return true;
	}
	return false;
}

void TrimmedQuadSet::ForceMeshSplitting ( const TriangleMesh2d & source , TriangleMesh2d & result ) const
{
	PointSet2d forcedextrapoints(10);
	SetMerger merge;

	for ( int i = 0; i < trimloops.Size(); i++ )
	{
		TrimLoop2d * currloop = trimloops[i];
		for ( int j=0; j < source.Size(); j++ )
		{
			VTriangle2dE * currtri = source[j];
			PointSet2d tmp(6);
			currloop->GetIntersectionPoints ( *currtri , tmp );
			if ( tmp.Size() > 1 )
			{
				tmp.InsertPoint(currtri->GetCorner1().GetArray());
				tmp.InsertPoint(currtri->GetCorner2().GetArray());
				tmp.InsertPoint(currtri->GetCorner3().GetArray());
				TriangleMesh2d tmesh(6);
				VDelaunay2d deln;
				deln.Calculate (tmp , tmesh); 
				merge.Merge ( tmesh , result );
				//merge.MergeNoRepeat ( tmp , forcedextrapoints );
			}
			else
				result.AddTriangle(*currtri);
		}
	}
	//TriangleMeshSplitter split ( source , forcedextrapoints , result );
	//split.Split();
	
}

void TrimmedQuadSet::TrimPoints ( const PointSet2d & firstset , PointSet2d & trimmedset ) const
{
	for ( int i=0; i < firstset.Size(); i++ )
	{
		const VPoint2d * currpoint = firstset[i];
		if ( IsPointTrimmed ( *currpoint ) == false )
			trimmedset.InsertPoint(currpoint->GetArray());
	}
}

bool TrimmedQuadSet::IsPointTrimmed ( const VPoint2d & p ) const
{
	for ( int i=0; i < trimloops.Size(); i++ )
	{
		TrimLoop2d * currloop = trimloops[i];
		if ( currloop->IsPointTrimmed(p))	
			return true;
	}
	return false;
}

void TrimmedQuadSet::MakeTriMesh ( TrimQuad & tq , TriangleMesh2d & result ) const
{
	if ( tq.extrap.Size() <= 10 )
	{
		TriangleMesh2d tmp(25);
		//QuadSplitter split ( tq.q , tq.extrap , tmp );
		//split.Split();
		//if ( IsPointTrimmed ( tq.q.GetCor1() ) == false )
		//	tq.extrap.InsertPoint(tq.q.GetCor1().GetArray());
		//if ( IsPointTrimmed ( tq.q.GetCor2() ) == false )
		//	tq.extrap.InsertPoint(tq.q.GetCor2().GetArray());
		//if ( IsPointTrimmed ( tq.q.GetCor3() ) == false )
		//	tq.extrap.InsertPoint(tq.q.GetCor3().GetArray());
		//if ( IsPointTrimmed ( tq.q.GetCor4() ) == false )
		//	tq.extrap.InsertPoint(tq.q.GetCor4().GetArray());
	
		if ( tq.extrap.Size() > 3  )
		{
			VDelaunay2d vd; 
			vd.Calculate (tq.extrap , tmp);
		}
		else if ( tq.extrap.Size() == 3 )
			tmp.AddTriangle ( * tq.extrap[0] , * tq.extrap[1] , *tq.extrap[2] );

		SetMerger merge;
		merge.Merge ( tmp , result );
		return;
	}
	else
	{
		TrimQuad firsthalf , secondhalf;
		tq.DivideTrimQuad ( firsthalf , secondhalf );
		MakeTriMesh ( firsthalf , result );
		MakeTriMesh ( secondhalf , result );
	}
	
}
