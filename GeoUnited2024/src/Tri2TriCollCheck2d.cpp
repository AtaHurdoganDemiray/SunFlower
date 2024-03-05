// Tri2TriCollCheck2d.cpp

#include "stdafx.h"
#include "Tri2TriCollCheck2d.h"
#include "VTriangle2dE.h"
#include "LineIntersection2d.h"
#include "VLine2d.h"
#include "PointSet2d.h"

Tri2TriCollCheck2d::Tri2TriCollCheck2d()
{

}

//#############################################################################

Tri2TriCollCheck2d::~Tri2TriCollCheck2d()
{

}

//#############################################################################

bool Tri2TriCollCheck2d::DoesAnyEdgesIntersect( const VTriangle2dE &tri1 , const VTriangle2dE &tri2 )
{
	//LineIntersection2d myck;
	VPoint2d intp,parint;
	
	// tri1 edge1 vs tri2 edge1
	LineIntersection2d::intersectiontype2d ckinfo;
	
	ckinfo = LineIntersection2d::LinesIntersect2d ( tri1.GetCorner1() , tri1.GetEdge1()
								, tri2.GetCorner1() , tri2.GetEdge1() , intp , parint );

	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;

	// tri1 edge1 vs tri2 edge2
	ckinfo = LineIntersection2d::LinesIntersect2d ( tri1.GetCorner1() , tri1.GetEdge1()
							, tri2.GetCorner2() , tri2.GetEdge2() , intp , parint );
	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;
	

	// tri1 edge1 vs tri2 edge3
	ckinfo =  LineIntersection2d::LinesIntersect2d ( tri1.GetCorner1() , tri1.GetEdge1()
										, tri2.GetCorner3() , tri2.GetEdge3() , intp , parint );
	
	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;

	// tri1 edge2 vs tri2 edge1
	ckinfo =  LineIntersection2d::LinesIntersect2d ( tri1.GetCorner2() , tri1.GetEdge2()
							, tri2.GetCorner1() , tri2.GetEdge1() , intp , parint );
	
	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;


	// tri1 edge2 vs tri2 edge2
	ckinfo = LineIntersection2d::LinesIntersect2d ( tri1.GetCorner2() , tri1.GetEdge2()
										, tri2.GetCorner2() , tri2.GetEdge2() , intp , parint );

	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;


	// tri1 edge1 vs tri2 edge3
	ckinfo = LineIntersection2d::LinesIntersect2d ( tri1.GetCorner2() , tri1.GetEdge2()
										, tri2.GetCorner3() , tri2.GetEdge3() , intp , parint );

	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;

	// tri1 edge3 vs tri2 edge1
	ckinfo = LineIntersection2d::LinesIntersect2d ( tri1.GetCorner3() , tri1.GetEdge3()
										, tri2.GetCorner1() , tri2.GetEdge1() , intp , parint );

	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;

	// tri1 edge3 vs tri2 edge2
	ckinfo = LineIntersection2d::LinesIntersect2d ( tri1.GetCorner3() , tri1.GetEdge3()
									, tri2.GetCorner2() , tri2.GetEdge2() , intp , parint );

	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;

	// tri1 edge3 vs tri2 edge3
	ckinfo = LineIntersection2d::LinesIntersect2d ( tri1.GetCorner3() , tri1.GetEdge3()
									, tri2.GetCorner3() , tri2.GetEdge3() , intp , parint );

	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;

	return false;
}

//#############################################################################

bool Tri2TriCollCheck2d::IsAnyTriCompletelyInOther ( const VTriangle2dE &tri1 , const VTriangle2dE &tri2 )
{
	// is tri2 completely in tri1
	if ( tri1.IsInTriangle ( tri2.GetCorner1() ) 
		&& tri1.IsInTriangle ( tri2.GetCorner2() )
		&& tri1.IsInTriangle ( tri2.GetCorner3() ) )
		return true;

	// is tri1 completely in tri2
	if ( tri2.IsInTriangle ( tri1.GetCorner1() ) 
		&& tri2.IsInTriangle ( tri1.GetCorner2() )
		&& tri2.IsInTriangle ( tri1.GetCorner3() ) )
		return true;

	return false;// no triangle is completely inside other
}

//#############################################################################

bool Tri2TriCollCheck2d::CheckKoll ( const VTriangle2dE &tri1 , const VTriangle2dE &tri2 )
{
	if ( DoesAnyEdgesIntersect ( tri1 , tri2 ) )
		return true; // 2 triangles collide

	if ( IsAnyTriCompletelyInOther ( tri1 , tri2 ) )
		return true; // 2 triangles collide

	return false; // 2 triangles doesnot collide
}

//#############################################################################

bool Tri2TriCollCheck2d::DoesAnyEdgeIntersectToLine 
( const VTriangle2dE &tri1 , const VLine2d &newline )
{
	VPoint2d intp,parint;
	LineIntersection2d::intersectiontype2d ckinfo;
	// tri1 edge1 vs line
	ckinfo = LineIntersection2d::LinesIntersect2d ( tri1.GetCorner1() , tri1.GetEdge1()
									, newline.GetP1() , newline.GetV() , intp , parint );

	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;


	// tri1 edge2 vs line
	ckinfo = LineIntersection2d::LinesIntersect2d ( tri1.GetCorner2() , tri1.GetEdge2()
									, newline.GetP1() , newline.GetV() , intp , parint );
		
	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;

	// tri1 edge3 vs line
	ckinfo = LineIntersection2d::LinesIntersect2d ( tri1.GetCorner3() , tri1.GetEdge3()
							, newline.GetP1() , newline.GetV() , intp , parint );

	if ( ckinfo == LineIntersection2d::LINESINTERSECT )
		return true;

	return false;
}

//#############################################################################

bool Tri2TriCollCheck2d::GetIntersectionPoints 
( const VTriangle2dE & tri1 , const VTriangle2dE & tri2 , PointSet2d & intp )
{
	VLine2d t1e1 ( tri1.GetCorner1() , tri1.GetCorner2() );
	VLine2d t1e2 ( tri1.GetCorner2() , tri1.GetCorner3() );
	VLine2d t1e3 ( tri1.GetCorner3() , tri1.GetCorner1() );

	VLine2d t2e1 ( tri2.GetCorner1() , tri2.GetCorner2() );
	VLine2d t2e2 ( tri2.GetCorner2() , tri2.GetCorner3() );
	VLine2d t2e3 ( tri2.GetCorner3() , tri2.GetCorner1() );	
	
	// t1e1 t2e1
	LineIntersection2d::LineIntersectionData2d ck1 = 
		LineIntersection2d::CalcInt ( t1e1 , t2e1 );
	if ( ck1.intinfo == LineIntersection2d::LINESINTERSECT )
		intp.InsertPoint ( ck1.intpoint[0].GetArray() );
	// t1e1 t2e2	
	LineIntersection2d::LineIntersectionData2d ck2 = 
		LineIntersection2d::CalcInt ( t1e1 , t2e2 );
	if ( ck2.intinfo == LineIntersection2d::LINESINTERSECT )
		intp.InsertPoint ( ck2.intpoint[0].GetArray() );	
	// t1e1 t2e3	
	LineIntersection2d::LineIntersectionData2d ck3 = 
		LineIntersection2d::CalcInt ( t1e1 , t2e3 );
	if ( ck3.intinfo == LineIntersection2d::LINESINTERSECT )
		intp.InsertPoint ( ck3.intpoint[0].GetArray() );	
	
	// t1e2 t2e1	
	LineIntersection2d::LineIntersectionData2d ck4 = 
		LineIntersection2d::CalcInt ( t1e2 , t2e1 );
	if ( ck4.intinfo == LineIntersection2d::LINESINTERSECT )
		intp.InsertPoint ( ck4.intpoint[0].GetArray() );	
	// t1e2 t2e2	
	LineIntersection2d::LineIntersectionData2d ck5 = 
		LineIntersection2d::CalcInt ( t1e2 , t2e2 );
	if ( ck5.intinfo == LineIntersection2d::LINESINTERSECT )
		intp.InsertPoint ( ck5.intpoint[0].GetArray() );
	// t1e2 t2e3	
	LineIntersection2d::LineIntersectionData2d ck6 = 
		LineIntersection2d::CalcInt ( t1e2 , t2e3 );
	if ( ck6.intinfo == LineIntersection2d::LINESINTERSECT )
		intp.InsertPoint ( ck6.intpoint[0].GetArray() );

	// t1e3 t2e1	
	LineIntersection2d::LineIntersectionData2d ck7 = 
		LineIntersection2d::CalcInt ( t1e3 , t2e1 );
	if ( ck7.intinfo == LineIntersection2d::LINESINTERSECT )
		intp.InsertPoint ( ck7.intpoint[0].GetArray() );	
	// t1e3 t2e2	
	LineIntersection2d::LineIntersectionData2d ck8 = 
		LineIntersection2d::CalcInt ( t1e3 , t2e2 );
	if ( ck8.intinfo == LineIntersection2d::LINESINTERSECT )
		intp.InsertPoint ( ck8.intpoint[0].GetArray() );		
	// t1e3 t2e3	
	LineIntersection2d::LineIntersectionData2d ck9 = 
		LineIntersection2d::CalcInt ( t1e3 , t2e3 );
	if ( ck9.intinfo == LineIntersection2d::LINESINTERSECT )
		intp.InsertPoint ( ck9.intpoint[0].GetArray() );	

	if ( intp.Size() > 0 )
		return true;
	else
		return false;

}

//#############################################################################
