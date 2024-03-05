// SnapEntities.cpp

#include "stdafx.h"
#include "SnapEntities.h"
#include "SequentialLineSet.h"
#include "VLine.h"
#include "VArc.h"
//#include "SurfaceWireFrame.h"
//#include "PointsDisplay.h"
#include "VisualAttLoader.h"
//#include "LinesDisplay.h"
#include "Display.h"
#include "ViewPointDefaults.h"
#include "TransformData.h"

SnapEntities::SnapEntities ()
:snappoints(10),snaplines(10)
{
	//snappointsdisplay = 0;
	//snaplinesdisplay = 0;
	m_display = 0;
	m_dynamicTr = 0;
}

SnapEntities::SnapEntities ( const SnapEntities & Foo )
:snappoints(Foo.snappoints),snaplines(Foo.snaplines)
//,snappointsdisplay(Foo.snappointsdisplay),snaplinesdisplay(Foo.snaplinesdisplay)
{
	//m_display = Foo.m_display; // ????
	//m_dynamicTr = Foo.m_dynamicTr;
	m_display = 0;
	m_dynamicTr = 0;

	Display * snappointsdisplay = new Display(snappoints);//new PointsDisplay ( snappoints ); 
	snappointsdisplay->MakeDisplayList(snaplines);
	PointVisualAtt tmpva;
	tmpva.SetPointSize(VGlobalValues::viewpointdefaults.m_defaultSnapPointSize);
	tmpva.ChangePointColor(VGlobalValues::viewpointdefaults.m_defaultSnapPointColor);
	snappointsdisplay->SetVisualAttPoints(tmpva);
	//if (m_display)
	//{
	//	snappointsdisplay->SetVisualAtt(m_display->GetVisualAtt());
	//	delete m_display;
	//}
	m_display = snappointsdisplay;
}

SnapEntities & SnapEntities::operator =(const SnapEntities & Foo)
{
	snappoints=Foo.snappoints;
	snaplines=Foo.snaplines;
	//snappointsdisplay=Foo.snappointsdisplay;
	//snaplinesdisplay=Foo.snaplinesdisplay;
	//m_display = Foo.m_display;
	m_dynamicTr = Foo.m_dynamicTr;
	
	Display * snappointsdisplay = new Display(snappoints);//new PointsDisplay ( snappoints ); 
	snappointsdisplay->MakeDisplayList(snaplines);
	PointVisualAtt tmpva;
	tmpva.SetPointSize(VGlobalValues::viewpointdefaults.m_defaultSnapPointSize);
	tmpva.ChangePointColor(VGlobalValues::viewpointdefaults.m_defaultSnapPointColor);
	snappointsdisplay->SetVisualAttPoints(tmpva);
	if (m_display != 0)
	{
		snappointsdisplay->SetVisualAtt(m_display->GetVisualAtt());
		delete m_display;
	}
	m_display = snappointsdisplay;

	return (*this);
}

SnapEntities::~SnapEntities ()
{
	//delete snappointsdisplay;
	//delete snaplinesdisplay;
	delete m_display;
	for ( unsigned int i=0; i < arcset.size(); i++ )
	{
		VArc * currarc = arcset[i];
		delete currarc;
	}
}

//void SnapEntities::CheckAllLoops ( const SurfaceWireFrame & wireframe , const vreal & tol )
//{
//	tolerance = tol;
//	for ( int i=0; i < wireframe.Size(); i++ )
//	{
//		const SequentialLineSet * pls = wireframe[i];
//		VArc formedarc;
//		if ( IsArc ( pls , formedarc ) )
//			AddArc ( formedarc ); 
//
//	}
//}

bool SnapEntities::IsArc ( const SequentialLineSet * currloop , VArc & formedarc ) const
{
	if ( currloop->Size() < 12 )
		return false;

	VPoint p[6];
		
	p[0] = (*currloop)[0].GetP1();
	p[1] = (*currloop)[4].GetP1();
	p[2] = (*currloop)[9].GetP1();

	p[3] = (*currloop)[1].GetP1();
	p[4] = (*currloop)[5].GetP1();
	p[5] = (*currloop)[8].GetP1();

	VArc a1 ( p[0] , p[1] , p[2] );
	VArc a2 ( p[3] , p[4] , p[5] );
	formedarc = a1;

	if ( a1.GetCenter().Compare ( a2.GetCenter() , /*tolerance*/VGlobalValues::ONEMICRON_TO_VUNIT * 0.01) == false  )
		return false;

	if ( fabs(a1.GetRadius() - a2.GetRadius() ) > /*tolerance*/ VGlobalValues::ONEMICRON_TO_VUNIT * 0.01)
		return false;

	return true;

}

void SnapEntities::AddArc ( const VArc & newarc )
{
	VArc * narc = new VArc ( newarc );
	arcset.push_back ( narc );
	
	AddPoint ( newarc.GetCenter() );
}

void SnapEntities::DrawPoints() const
{
	//if ( snappointsdisplay )
	if (m_display)
	{
		//if (snappointsdisplay->GetVisualAtt().GetDraw())
		if (m_display->GetVisualAttPoints().GetDraw())
		{
			VisualAttLoader ld;
			//VColor selcolor ( 1. , 0. , 0. );
			ld.Load ( /*snappointsdisplay->GetVisualAtt()*/ m_display->GetVisualAttPoints() , false , VGlobalValues::viewpointdefaults.m_selectionColor );
			//snappointsdisplay->Draw();
			m_display->DrawPoints();
		}
	}
}
void SnapEntities::DrawLines()const
{
	//if ( snaplinesdisplay )
	if (m_display)
	{
		VisualAttLoader ld;
		ld.Load ( /*snaplinesdisplay->GetVisualAtt()*/m_display->GetVisualAtt() , false , VGlobalValues::viewpointdefaults.m_selectionColor );
		//snaplinesdisplay->Draw();
		m_display->DrawLines(false);
	}
}

void SnapEntities::GetHitPoints
( PointSet & selpoints , DoubleVec & depthvalues ) const
{
	//if ( snappointsdisplay )
	if (m_display)
		//snappointsdisplay->GetHitPoints ( selpoints , depthvalues );
		m_display->GetHitPointsPoints( selpoints , depthvalues , m_dynamicTr);
}

void SnapEntities::GetHitLines 
( LineSet & sellines , DoubleVec & depthvalues ) const
{
	//if ( snaplinesdisplay )
	if (m_display)
		//snaplinesdisplay->GetHitLines ( sellines , depthvalues );
		m_display->GetHitLines (sellines , depthvalues);
}

void SnapEntities::AddPoint ( const VPoint & snapp )
{
	snappoints.InsertPoint ( snapp.GetArray() );
	
	//if ( snappointsdisplay )
		//delete snappointsdisplay;
	
	//snappointsdisplay = new PointsDisplay ( snappoints ); 
	Display * newDisplay = new Display (snappoints);
	newDisplay->MakeDisplayList (snaplines);
	PointVisualAtt tmpva;
	tmpva.SetPointSize(VGlobalValues::viewpointdefaults.m_defaultSnapPointSize);
	tmpva.ChangePointColor(VGlobalValues::viewpointdefaults.m_defaultSnapPointColor);
	//snappointsdisplay->SetVisualAtt(tmpva);
	newDisplay->SetVisualAttPoints (tmpva);
	if (m_display)
	{	
		newDisplay->SetVisualAtt (m_display->GetVisualAtt());
		delete m_display;
	}
	m_display = newDisplay;		
}

void SnapEntities::AddPointSet ( const PointSet & snapp )
{	
	//if ( snappointsdisplay )
	//	delete snappointsdisplay;
	if (snapp.Size())
	{
		snappoints.Clear();
		snappoints.AddBack (snapp);// .InsertPoint ( snapp.GetArray() );

		Display * snappointsdisplay = new Display (snappoints);//new PointsDisplay ( snappoints ); 
		snappointsdisplay->MakeDisplayList (snaplines);
		PointVisualAtt tmpva;
		tmpva.SetPointSize(VGlobalValues::viewpointdefaults.m_defaultSnapPointSize);
		tmpva.ChangePointColor(VGlobalValues::viewpointdefaults.m_defaultSnapPointColor);
		snappointsdisplay->SetVisualAttPoints(tmpva);
		if (m_display)
		{
			snappointsdisplay->SetVisualAtt (m_display->GetVisualAtt());
			delete m_display;
		}
		m_display = snappointsdisplay;
	}
}

void SnapEntities::AddLine ( const VLine & snapl )
{
	snaplines.AddLine ( snapl.GetP1() , snapl.GetP2() );
	//if ( snaplinesdisplay )
	//	delete snaplinesdisplay;
	Display * snaplinesdisplay = new Display (snaplines);//LinesDisplay ( snaplines ); 
	snaplinesdisplay->MakeDisplayList (snappoints);
	LineVisualAtt tmpva;
	tmpva.SetLineWidth(1.);
	tmpva.ChangeWireColor(VGlobalValues::viewpointdefaults.m_defaultWireframeColor);
	snaplinesdisplay->SetVisualAtt(tmpva);
	if (m_display)
	{
		snaplinesdisplay->SetVisualAttPoints (m_display->GetVisualAttPoints());
		delete m_display;
	}
	m_display = snaplinesdisplay;
}

void SnapEntities::AddLineSet ( const LineSet & snapl )
{
	if (snapl.Size())
	{
		snaplines.Clear();
		snaplines.AddBack ( snapl );
	
		//if ( snaplinesdisplay )
		//	delete snaplinesdisplay;

		Display * snaplinesdisplay = new Display (snaplines);//LinesDisplay ( snaplines ); 
		snaplinesdisplay->MakeDisplayList (snappoints);
		LineVisualAtt tmpva;
		tmpva.SetLineWidth(1.);
		tmpva.ChangeWireColor(VGlobalValues::viewpointdefaults.m_defaultWireframeColor);
		snaplinesdisplay->SetVisualAtt(tmpva);
		if (m_display)
		{
			snaplinesdisplay->SetVisualAttPoints (m_display->GetVisualAttPoints());
			delete m_display;
		}
		m_display = snaplinesdisplay;
	}
}

/*
void SnapEntities::SetXmatrix (vreal * xmatrix )
{
	if ( snappointsdisplay )
	{
		snappointsdisplay->Xmatrix = xmatrix;
	}

	if ( snaplinesdisplay )
	{
		snaplinesdisplay->Xmatrix = xmatrix;
	}
}
*/
//void SnapEntities::AddMatrix (TransformData *x)
//{
//	if ( snappointsdisplay )
//	{
//		snappointsdisplay->AddMatrix(x);
//	}
//
//	if ( snaplinesdisplay )
//	{
//		snaplinesdisplay->AddMatrix(x);
//	}
//}

//void SnapEntities::ClearAllMatrix()
//{
//	if ( snappointsdisplay )
//	{
//		snappointsdisplay->xmats.clear();
//	}
//
//	if ( snaplinesdisplay )
//	{
//		snaplinesdisplay->xmats.clear();
//	}
//}

void SnapEntities::Transform (const TransformationMatrix &tm)
{
	PointSet snappointsnew (snappoints);
	snappointsnew.Transform(tm);
	AddPointSet (snappointsnew);//Remaking the display
	LineSet snaplinesnew (snaplines);
	snaplinesnew.Transform(tm);
	AddLineSet (snaplinesnew);
	m_dynamicTr = 0;
}

void SnapEntities::SetDynamicTransformData (TransformData *trData)
{
	m_dynamicTr = trData;
}

const TransformData * SnapEntities::GetDynamicTransformData ()const
{
	return m_dynamicTr;
}

