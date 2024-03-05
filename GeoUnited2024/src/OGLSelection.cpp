// OGLSelection.cpp

#include "stdafx.h"
#include "OGLSelection.h"
#include "Mouse.h"
#include "VPoint.h"
#include "Quat.h"
#include "ModelList.h"
//#include "FacetDisplay.h"
#include "Display.h"
#include "DoubleVec.h"
#include "LongVec.h"
#include "HandleSet.h"
#include "SelectionFlags.h"
#include "SelectableTypes.h"
//#include "LinesDisplay.h"
//#include "PointsDisplay.h"
#include "CoordSystemDisplay.h"
#include "SnapEntities.h"
//#include "FontDisp.h"
#include "DraftEntityVec.h"
#include "DraftEntity.h"
#include "ViewPointItemTypes.h"
#include "VTransformable.h"

const vreal HITRADIUSRATIO = 200;//150.0;

OGLSelection::OGLSelection ( const ViewVolume & _viewvolume 
							, Quat & _quat 
							, const VPoint & _rotcenter)
: viewvolume ( _viewvolume ) , quat ( _quat ) , rotcenter ( _rotcenter )
{
	ResetSelectionTime();	
}

vreal OGLSelection::GetHitRadiusOnWindow () const 
{
	vreal diagonal = sqrt ( static_cast <vreal> 
		( viewvolume.corner_x * viewvolume.corner_x 
		+ viewvolume.corner_y * viewvolume.corner_y ) );

	//vreal dia2 = (vreal)(viewvolume.m_right - viewvolume.m_left);

	vreal windowportion = diagonal / VGlobalValues::viewpointdefaults.m_hitRadiusRatio;// HITRADIUSRATIO;
	return windowportion;
}

vreal OGLSelection::GetHitRadiusOnVolume () const
{
	vreal windowportion = GetHitRadiusOnWindow();
	return (windowportion*viewvolume.ConvertOneUnitOnWindowToVolume());
}

void OGLSelection::SetSelectionVolume 
( Mouse & mouse , bool selectwithbox , bool overWriteHitRadius) 
{
	//ViewVolume selectionvolume;

	if ( selectwithbox )
	{
		VPoint p , downp;
		p = mouse.GetCurrentP();
		downp = mouse.GetLButtonDownP();
		vreal p1x , p1y , p2x , p2y;

		viewvolume.ScreenCoordToViewVolume ( p.x() , p.y() , p1x , p1y , rotcenter.x() , rotcenter.y());
		viewvolume.ScreenCoordToViewVolume ( downp.x() , downp.y() , p2x , p2y , rotcenter.x() , rotcenter.y() );
		
		vreal dx = fabs ( p2x - p1x );
		vreal dy = fabs ( p2y - p1y );

		vreal midx = ( p2x + p1x ) / 2.0;
		vreal midy = ( p2y + p1y ) / 2.0;

	//	vreal ar = viewvolume.GetAspectRatio();

	//	if ( ar > dx / dy )
	//		dy = dx / ar;
	//	else
	//		dx = ar * dy;

		dx = dx / 2.0;
		dy = dy / 2.0;

		if ( dx < 1e-6 || dy < 1e-6 )
			return;

		// set view volume
		selectionvolume.SetRight ( midx + dx );
		selectionvolume.SetLeft ( midx - dx );
		selectionvolume.SetTop ( midy + dy );
		selectionvolume.SetBottom ( midy - dy );
		selectionvolume.SetFar ( viewvolume.m_far );
		selectionvolume.SetNear ( viewvolume.m_near );

	}
	else
	{
		// set A NEW view volume by using mouse left button down point
		vreal scrx = mouse.GetLButtonDownP().x();
		vreal scry = mouse.GetLButtonDownP().y();

		vreal volx , voly;
		viewvolume.ScreenCoordToViewVolume ( scrx , scry , volx , voly , rotcenter.x(),rotcenter.y());

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		vreal hitrad = GetHitRadiusOnVolume();
		if (overWriteHitRadius)
			hitrad = 0.01;
		// a new view volume
		selectionvolume.SetLeft ( volx - hitrad );
		selectionvolume.SetRight ( volx + hitrad );	
		selectionvolume.SetBottom ( voly - hitrad );
		selectionvolume.SetTop ( voly + hitrad );
		selectionvolume.SetFar ( viewvolume.m_far );
		selectionvolume.SetNear ( viewvolume.m_near );
	}

	// set this new view volume by opengl commands
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();		//Initialize matrix

	selectionvolume.SetViewVolume ();

	// Current matrix will be model view matrix
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();      //Initialize matrix	
	
	
//	glMultMatrixd(quat.QuatToMat()); 
	// Making view center the origin
//	glTranslated ( -rotcenter.x() , -rotcenter.y() , -rotcenter.z() );	
}

void OGLSelection::MakeSelectionVolumeHalf () 
{
	vreal dx = selectionvolume.m_right - selectionvolume.m_left;
	vreal dy = selectionvolume.m_top - selectionvolume.m_bottom;

	dx = dx / 4.0;
	dy = dy / 4.0;

	selectionvolume.SetRight ( selectionvolume.m_right - dx );
	selectionvolume.SetLeft ( selectionvolume.m_left + dx );
	selectionvolume.SetTop ( selectionvolume.m_top - dy );
	selectionvolume.SetBottom ( selectionvolume.m_bottom + dy );

	selectionvolume.SetViewVolume();
}

void OGLSelection::DoPointSelection  ( Mouse & mouse 
									 , bool selectwithbox 
									 , const ModelList &modellist 
									 , const std::vector<VTransformable *> &hs 
									 , PointSet &selpoints
									 , DoubleVec &depthvalues  
									 , const VPoint &curserp
									 , const VPoint &lookdir)
{
	SetSelectionVolume ( mouse , selectwithbox , false);
	vreal hitrad = GetHitRadiusOnVolume();

	Model *currmodel;
	int mysize=(int)hs.size();
	for ( int i=0;i<mysize;++i)
	{
		//currmodel = modellist.GetModel(hs.GetID(i));
		VTransformable *curtr = hs.at(i);
		currmodel = curtr->GetTrModel();
		if ( currmodel->IsVisible() && currmodel->GetDeleted() == false)
		{
			//vreal tmp;
			//if ( currmodel->GetSelectionFilter().IsHit(tmp) == true )
			{
				//currmodel->GetHitPoints ( selpoints , depthvalues ,m_selflags->GetSelectAnyVertex());
				currmodel->GetHitPointsNew ( selpoints , depthvalues , curserp , lookdir , hitrad);
			}
		}
	}
}

void OGLSelection::DoVectorSelection ( Mouse & mouse 
									  , bool selectwithbox 
									  , const ModelList &modellist 
									  , const std::vector<VTransformable *> &hs 
									  , PointSet &selpoints 
									  , DoubleVec &depthvalues )
{
	SetSelectionVolume ( mouse , selectwithbox , false);
	Model *currmodel;
	int mysize=(int)hs.size();
	for ( int i=0;i<mysize;++i)
	{
		VTransformable *curtr = hs.at(i);
		currmodel = curtr->GetTrModel();//modellist.GetModel(hs.GetID(i));
		if ( currmodel->IsVisible() && currmodel->GetDeleted()==false)
		{
			vreal tmp(MAX_NUMBER_LIMIT);
			//if ( currmodel->GetSelectionFilter().IsHit(tmp) == true )
			{
				//FacetDisplay & disp = ( currmodel->GetDisplayList() );
				//disp.GetHitPoints ( selpoints , depthvalues );
				currmodel->GetDisplay().GetHitPointsFacet (selpoints , depthvalues);
			}
		}
	}
}

void OGLSelection::DoEdgeSelection ( Mouse & mouse 
									, const ModelList &modellist 
									, const std::vector<VTransformable *> &hs 
									, LineSet &sellines 
									, DoubleVec & depthvalues )
{
	bool selectwithbox = false;
	SetSelectionVolume ( mouse , selectwithbox , false);
	Model *currmodel;
	int mysize=(int)hs.size();
	for ( int i=0;i<mysize;++i )
	{
		VTransformable *curtr = hs.at(i);
		currmodel = curtr->GetTrModel();//modellist.GetModel(hs.GetID(i));
		if ( currmodel->IsVisible() && currmodel->GetDeleted()==false)
		{
			//vreal tmp;
			//if ( currmodel->GetSelectionFilter().IsHit(tmp) == true )
			{
				currmodel->GetHitLines ( sellines , depthvalues );
			}
		}
	}
}

bool OGLSelection::GetTopLine (const LineSet &sellines , const DoubleVec &depthvalues , VPoint &basepoint , VPoint &topvector)
{
	vreal nearest = 1. / EPSILON;

	for ( int i=0; i<sellines.Size(); i++ )
	{
		const VLine * currline = sellines[i];
		vreal currdepth = depthvalues[i];

		if ( currdepth < nearest )
		{
			nearest = currdepth;
			basepoint = currline->GetP1();
			topvector = currline->GetV();
		}
	}

	if ( sellines.Size() > 0 )
		return true;
	else
		return false;
}

void OGLSelection::DoModelSelection ( Mouse & mouse 
									 , bool deselect 
									 , SelectionFlags & selflags 
									 , const SelectableTypes & filter 
									 , const ModelList &modellist 
									 , const std::vector<VTransformable *> &hs 
									 , std::map<Model *,double> &selModelMap
									 , bool curveChainingActive)
{
	//*selmodel = 0;
	vreal setdepth = MAX_NUMBER_LIMIT;
	bool overWriteHitRadius = false;
	if (filter.Size() == 1)
		if (filter.operator [] (0) == ViewPointItemTypes::m_surfacetype)
			overWriteHitRadius = true;
	SetSelectionVolume ( mouse , selflags.GetSelectWithBox() , overWriteHitRadius);
	
	int mysize=(int)hs.size();
	for (int i=0;i<mysize;++i)
	{
		//const ModelID &myid = hs.GetID(i);
		VTransformable *curtr = hs.at(i);
		Model *currmodel = curtr->GetTrModel();//modellist.GetModel(myid);
		
		if ( currmodel->IsSelected() != deselect )
			continue;

		if ( currmodel->IsVisible() == false || currmodel->GetDeleted() == true)
			continue;

		if ( filter.GetColorFilter() == true  
			//&& !filter.IsColorSelectable ( currmodel->GetDisplayList().GetVisualAtt().GetShadeColor() )	)
			&& !filter.IsColorSelectable ( currmodel->GetDisplay().GetVisualAttFacet().GetShadeColor(),currmodel->GetDisplay().GetVisualAtt().GetWireColor() ) )
		{
			continue;
		}

		//bool forcedSurf = false;
		//if ( !filter.IsTypeSelectable ( currmodel->GetModelType() ))
		//{
		//	if (curveChainingActive)
		//	{
		//		if (currmodel->GetModelType() == ViewPointItemTypes::m_surfacetype)
		//			forcedSurf = true;
		//	}
		//	
		//	if (!forcedSurf)
		//		continue;
		//}
		if (filter.IsTypeSelectable(currmodel->GetModelType())==false)
			continue;
				
		vreal tmp(MAX_NUMBER_LIMIT);
		//if ( currmodel->GetSelectionFilter().IsHit(tmp) == false )
		//	continue;// model is far away from selection volume

		vreal currdepth;
		//FacetDisplay & disp = ( currmodel->GetDisplayList() );
		Display &disp = currmodel->GetDisplay();
		bool trick = disp.IsHitFacet ( currdepth );
		
		//QuadsDisplay & quaddisp = ( currmodel->GetQuadsDisplayList () );
		//bool quadck = quaddisp.IsHit ( currdepth );

		//LinesDisplay & linesdisp = ( currmodel->GetWireDisplayList() );
		bool wireck = disp.IsHit(currdepth);//linesdisp.IsHit( currdepth );

		//PointsDisplay & pointsdisp = ( currmodel->GetPointsDisplayList() );
		bool pointsck = disp.IsHitPoints (currdepth);//pointsdisp.IsHit ( currdepth );

		bool coordck = false;
		CoordSystemDisplay * coorddisp = currmodel->GetCoordSystemDisplay();
		if ( coorddisp != 0 )
			coordck = coorddisp->IsHit ( currdepth );
		//bool fontck = false;
		//FontDisp * fontdisp = currmodel->GetFontDisp();
		//while ( fontdisp != 0 )
		//{
		//	fontck = fontdisp->IsHit ( currdepth );
		//	if (fontck)
		//		break;
		//	fontdisp = fontdisp->next;
		//}

		if ( trick /*|| quadck */ || wireck || pointsck || coordck /*|| fontck*/) // is model hit
		{
			if ( selflags.GetSelectWithBox())
			{
				//if (forcedSurf)
				//	continue;
				selModelMap.insert (std::pair<Model*,double> (currmodel,currdepth));
			}
			else
			{
				if ( currdepth < setdepth )
				{
					selModelMap.insert (std::pair<Model*,double> (currmodel,currdepth));
					setdepth = currdepth;
					//*selmodel = currmodel;
				}
			
			}// if box selection or else
		
		}// if model has been hit

	}// for all models
}

void OGLSelection::DoModelSelection2(Mouse &mouse
	, bool deselect
	, SelectionFlags &selflags
	, const SelectableTypes &filter
	//, const std::vector<VTransformable *> &hs
	, std::multimap<vreal, Model *> &depthModelMap
	, bool curveChainingActive)
{
	//*selmodel = 0;
	vreal setdepth = MAX_NUMBER_LIMIT;
	bool overWriteHitRadius = false;
	if (filter.Size() == 1)
		if (filter.operator [] (0) == ViewPointItemTypes::m_surfacetype)
			overWriteHitRadius = true;
	SetSelectionVolume(mouse, selflags.GetSelectWithBox(), overWriteHitRadius);

	int mysize = (int)m_boxPotHits.size();
	for (int i = 0; i<mysize; ++i)
	{
		//const ModelID &myid = hs.GetID(i);
		VTransformable *curtr = m_boxPotHits.at(i);
		Model *currmodel = curtr->GetTrModel();//modellist.GetModel(myid);

		if (currmodel->IsSelected() != deselect)
			continue;

		if (currmodel->IsVisible() == false || currmodel->GetDeleted() == true)
			continue;

		if (filter.GetColorFilter() == true
			//&& !filter.IsColorSelectable ( currmodel->GetDisplayList().GetVisualAtt().GetShadeColor() )	)
			&& !filter.IsColorSelectable(currmodel->GetDisplay().GetVisualAttFacet().GetShadeColor(), currmodel->GetDisplay().GetVisualAtt().GetWireColor()))
		{
			continue;
		}

		//bool forcedSurf = false;
		//if ( !filter.IsTypeSelectable ( currmodel->GetModelType() ))
		//{
		//	if (curveChainingActive)
		//	{
		//		if (currmodel->GetModelType() == ViewPointItemTypes::m_surfacetype)
		//			forcedSurf = true;
		//	}
		//	
		//	if (!forcedSurf)
		//		continue;
		//}
		if (filter.IsTypeSelectable(currmodel->GetModelType()) == false)
			continue;

		vreal tmp(MAX_NUMBER_LIMIT);
		//if ( currmodel->GetSelectionFilter().IsHit(tmp) == false )
		//	continue;// model is far away from selection volume

		vreal currdepth;
		//FacetDisplay & disp = ( currmodel->GetDisplayList() );
		Display &disp = currmodel->GetDisplay();
		bool trick = disp.IsHitFacet(currdepth);

		//QuadsDisplay & quaddisp = ( currmodel->GetQuadsDisplayList () );
		//bool quadck = quaddisp.IsHit ( currdepth );

		//LinesDisplay & linesdisp = ( currmodel->GetWireDisplayList() );
		bool wireck = disp.IsHit(currdepth);//linesdisp.IsHit( currdepth );

		//PointsDisplay & pointsdisp = ( currmodel->GetPointsDisplayList() );
		bool pointsck = disp.IsHitPoints(currdepth);//pointsdisp.IsHit ( currdepth );

		bool coordck = false;
		CoordSystemDisplay * coorddisp = currmodel->GetCoordSystemDisplay();
		if (coorddisp != 0)
			coordck = coorddisp->IsHit(currdepth);
		//bool fontck = false;
		//FontDisp * fontdisp = currmodel->GetFontDisp();
		//while ( fontdisp != 0 )
		//{
		//	fontck = fontdisp->IsHit ( currdepth );
		//	if (fontck)
		//		break;
		//	fontdisp = fontdisp->next;
		//}

		if (trick /*|| quadck */ || wireck || pointsck || coordck /*|| fontck*/) // is model hit
		{
			if (selflags.GetSelectWithBox())
			{
				//if (forcedSurf)
				//	continue;
				depthModelMap.insert(std::pair<vreal,Model*>(currdepth,currmodel));
			}
			else
			{
				//if (currdepth < setdepth)
				//{
					depthModelMap.insert(std::pair<vreal,Model*>(currdepth,currmodel));
					setdepth = currdepth;
					//*selmodel = currmodel;
				//}

			}// if box selection or else

		}// if model has been hit

	}// for all models
}

//void OGLSelection::SearchClosedCurveLoop(Model *curcurve , const HandleSet &hs)
//{
//	std::vector<VCurve *> allCurves;
//	int mysize ;
//}

void OGLSelection::DoDraftEntitySelection 
( Mouse & mouse , bool deselect , SelectionFlags & selflags , const SelectableTypes & filter , const DraftEntityVec &modellist , const HandleSet &hs)
{
	SetSelectionVolume ( mouse , selflags.GetSelectWithBox() , false);
	vreal setdepth = 1 / EPSILON;
	vreal currdepth;
	DraftEntity *currmodel, *selmodel = 0;
	DraftEntityVec::pos i = modellist.Begin();
	DraftEntityVec::pos e = modellist.End();
	for ( i ; i != e; i++ )
	{
		currmodel = *i;
		
		if ( currmodel->IsSelected() != deselect )
			continue;

		if ( currmodel->IsVisible() == false || currmodel->GetDeleted() == true)
			continue;

		if ( filter.GetColorFilter() == true  
			&& !filter.IsColorSelectable ( currmodel->GetColor() , currmodel->GetColor())	)
			continue;

		if ( !filter.IsTypeSelectable ( currmodel->GetModelType() ))
			continue;
				
		bool myck = currmodel->IsHit (currdepth);

		if (myck)
		{
			if ( selflags.GetSelectWithBox() )
			{
				currmodel->SetSelected (!deselect);
				if ( !deselect ) 
				{
					selflags.IncSelModelNoByOne();
				}
				else
				{	
					selflags.DecSelModelNoByOne();
				}
			}
			else
			{
				if ( currdepth < setdepth )
				{
					setdepth = currdepth;
					selmodel = currmodel;
				}
			
			}// if box selection or else
		
		}// if model has been hit

	}// for all models
	
	if ( selmodel != 0 )
	{
		selmodel->SetSelected(!deselect);
		if ( !deselect )
			selflags.IncSelModelNoByOne();
		else
			selflags.DecSelModelNoByOne();
	}
}

bool OGLSelection::GetTopPoint ( const PointSet & selpoints , const DoubleVec & depthvalues , VPoint &toppoint ) const
{
	if (selpoints.Size()==0)
		return false;
	vreal minz = MAX_NUMBER_LIMIT;
	int mysize=depthvalues.Size();
	for (int i=0;i<mysize;++i)
	{
		const vreal &currz=depthvalues.operator [](i);
		if (currz<minz)
		{
			minz=currz;
			toppoint=*(selpoints[i]);
		}
	}
	return true;
	//vreal setdepth = 1.0/EPSILON;
	//const VPoint * result = 0;
	//unsigned int sel_ind[500];	
	//glSelectBuffer ( 500 , sel_ind );
	//glRenderMode ( GL_SELECT );
	//glInitNames();
	//glPushName(0);
	//
	//int i;
	//for ( i=0; i < selpoints.Size(); i++ )
	//{
	//	// Draw all points 
	//	glLoadName(i);
	//	glBegin ( GL_POINTS );
	//	glVertex3dv ( selpoints[i]->GetArray() );
	//	glEnd();
	//}

	//// get the selected points from record
	//int hits = glRenderMode ( GL_RENDER );
	//int offset = 0;

	//for ( i = 0; i < hits; i++ )
	//{
	//	int nameno = sel_ind[offset++];
	//
	//	offset++;//vreal z1 =static_cast<vreal> (sel_ind[offset++]/0x7fffffff);
	//	offset++;//vreal z2 =static_cast<vreal> (sel_ind[offset++]/0x7fffffff);
	//		
	//	for ( int j = 0; j < nameno; j++  )
	//	{
	//		int name = sel_ind[offset++];
	//		vreal currdepth = depthvalues[name];
	//		if ( currdepth < setdepth )
	//		{
	//			setdepth = currdepth;
	//			result = selpoints[name];
	//		}
	//	}
	//}
	//
	//if ( result == 0 )
	//	return false;
	//else
	//{
	//	toppoint = (*result);
	//	return true;
	//}

}

bool OGLSelection::GetTopVector 
( const PointSet & selpoints , const DoubleVec & depthvalues , VPoint & basepoint , VPoint & topvector ) const
{
	vreal setdepth = 1.0/EPSILON;
	int result = -1;
	unsigned int sel_ind[500];	
	glSelectBuffer ( 500 , sel_ind );
	glRenderMode ( GL_SELECT );
	glInitNames();
	glPushName(0);
	int i;
	for (i=0; i < selpoints.Size(); i++ )
	{
		// Draw all points 
		glLoadName(i);
		glBegin ( GL_POINTS );
		glVertex3dv ( selpoints[i]->GetArray() );
		glEnd();
	}

	// get the selected points from record
	int hits = glRenderMode ( GL_RENDER );
	int offset = 0;

	for ( i = 0; i < hits; i++ )
	{
		int nameno = sel_ind[offset++];
	
		offset++;//vreal z1 =static_cast<vreal> (sel_ind[offset++]/0x7fffffff);
		offset++;//vreal z2 =static_cast<vreal> (sel_ind[offset++]/0x7fffffff);
			
		for ( int j = 0; j < nameno; j++  )
		{
			int name = sel_ind[offset++];
			vreal currdepth = depthvalues[name];
			
			if ( currdepth < setdepth )
			{
				setdepth = currdepth;
				int remainder = name%3;
				result = name-remainder;
			}
		}
	}
	
	if ( result == -1 )
		return false;
	else
	{
		const VPoint & c1 = *(selpoints[result]);
		const VPoint & c2 = *(selpoints[result+1]);
		const VPoint & c3 = *(selpoints[result+2]);
		basepoint = c1;

		VPoint e1 = c2 - c1;
		VPoint e2 = c3 - c1;
		topvector = e1 ^ e2;
		topvector.NormThis();
		topvector += basepoint;
		return true;
	}
}

void OGLSelection::EndModelSelection ( ModelList & modellist , HandleSet * handles )
{
	Model * currmodel;
	ModelList::pos e = modellist.End();
	ModelList::pos i = modellist.Begin();
	for ( i; i != e; i++ )
	{
		currmodel = *i;
		
		if ( currmodel->IsSelected () )
		{
			if ( handles )
			{
				ModelID myid(currmodel->GetModelID());
				handles->AddItem (myid);
			}
			currmodel->SetSelected ( false );
		}
	}
}

void OGLSelection::SetSelectionFlags (SelectionFlags *selflags)
{
	m_selflags =selflags;
}

void OGLSelection::ResetSelectionTime()
{
	m_selectionTime = 0;
}

int OGLSelection::GetSelectionTime()const
{
	return m_selectionTime;
}

void OGLSelection::IncrementSelectionTime()
{
	++m_selectionTime;
}
