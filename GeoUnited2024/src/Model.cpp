/******************************************************************************
*               File: Model.cpp											      *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                      *
******************************************************************************/

#include "stdafx.h"
#include "BasicDefinitions.h"
#include "Model.h"
#include "VisualAttributes.h"
//#include "texture.h"
#include "TriangleMesh2d.h"
#include "BoundingBox.h"
#include "BoundingBox2TriMesh.h"
#include "VRefTriangleMesh.h"
//#include "FacetDisplay.h"
#include "DoubleList.h"
#include "gmQuadSet.h"
#include "SequentialLineSet.h"
#include "CoordSystemDisplay.h"
#include "CoordinateSystem.h"
#include "TransformationMatrix.h"
//#include "FontDisp.h"
#include "ViewPointItemTypes.h"
#include "LinePointMinDist.h"
#include "ThinTransformShell.h"
#include "QuadPointGenerator.h"

Model::Model (const ModelID &id)
:ViewPointItem (( IDS_Undef),( IDS_Undef)),coorddisplay(0)/*,rootfont(0)*/,m_transform(0),m_selectionTime(-1)
{
	m_transform = new ThinTransformShell (VPoint(-10.0),VPoint(10.0),this);
	m_quickDisplay = & m_display;
}
Model::Model (VStr modeltype , VStr name)
:ViewPointItem (modeltype,name),coorddisplay(0)/*,rootfont(0)*/,m_transform(0),m_selectionTime(-1)
{
	m_transform = new ThinTransformShell (VPoint(-10.0),VPoint(10.0),this);
	m_quickDisplay = & m_display;
}
Model::Model( TriangleMesh * p_mesh , VStr modeltype , VStr name)
:ViewPointItem (modeltype,name),coorddisplay(0)/*,rootfont(0)*/,m_transform(0),m_selectionTime(-1)
{
	SetTriMesh ( p_mesh );
	m_transform = new ThinTransformShell (VPoint(-10.0),VPoint(10.0),this);
	m_quickDisplay = & m_display;
}

Model::Model ( const Model & Foo )
:ViewPointItem ( Foo )
//,ModelBounds(Foo)
,mcoord(Foo.mcoord)
//,facetdisplay(Foo.facetdisplay)
//,quadsdisplay(Foo.quadsdisplay)
//,linesdisplay(Foo.linesdisplay)
//,pointsdisplay(Foo.pointsdisplay)
,m_display(Foo.m_display)
,snapent(Foo.snapent)
,coorddisplay(0)
//,rootfont(0)
,m_transform(Foo.m_transform)
,m_selectionTime(Foo.m_selectionTime)
{
	//if ( Foo.coorddisplay )
	//	coorddisplay = new CoordSystemDisplay ( *(Foo.coorddisplay) );
	SetCoordSystemAndDisplay (Foo.mcoord);
	m_quickDisplay = &m_display;
}

Model::~Model()
{
	delete coorddisplay;
	//delete rootfont;
}

//#############################################################################


void Model::SetTriMesh ( const TriangleMesh *p_mesh , const FacetVisualAtt *visatt )
{
	if ( visatt )
	{
		//facetdisplay.SetVisualAtt ( *visatt );
		m_display.SetVisualAttFacet(*visatt);
	}

	//facetdisplay.MakeDisplayList ( *p_mesh );
	m_display.MakeDisplayList (*p_mesh);
}

void Model::SetTriMesh ( const FMesh &fmesh , const FacetVisualAtt *visatt)
{
	if ( visatt )
	{
		//facetdisplay.SetVisualAtt ( *visatt );
		m_display.SetVisualAttFacet (*visatt);
	}

	//facetdisplay.MakeDisplayList ( fmesh );
	m_display.MakeDisplayList (fmesh);
}

void Model::SetTriMesh(std::vector<FMesh *> &triVec, const FacetVisualAtt &visatt)
{
	//facetdisplay.SetVisualAtt (visatt);
	m_display.SetVisualAttFacet(visatt);
	//facetdisplay.MakeDisplayList (triVec);
	m_display.MakeDisplayList (triVec);
}

void Model::SetRefTriMesh ( const VRefTriangleMesh *p_reftris , const FacetVisualAtt *visatt )
{		
	if (visatt)
	{
		//facetdisplay.SetVisualAtt ( *visatt );
		m_display.SetVisualAttFacet (*visatt);
	}

	//facetdisplay.MakeDisplayListByVertexArray ( *p_reftris );
	m_display.MakeDisplayListByVertexArray ( *p_reftris );
}

//#############################################################################

//void Model::SetQuadSet ( const QuadSet & qset , const FacetVisualAtt *visatt )
//{
//	//mymin.ReplaceMin ( qset.GetMin() );
//	//mymax.ReplaceMax ( qset.GetMax() );
//	//tmin=mymin;
//	//tmax=mymax;	
//	//
//	//MakeBoundingBox();
//	ModifMinMax(qset.GetMin(),qset.GetMax());
//
//	if ( visatt )
//	{
//		quadsdisplay.SetVisualAtt ( *visatt );
//	}
//
//	quadsdisplay.MakeDisplayListByVertexArray ( *(qset.quadset) );
//	//m_area += newArea;
//}

//void Model::SetQuadSet ( const QuadSet & qset , bool isquadvalid[] , const FacetVisualAtt *visatt )
//{
//	gmQuadSet tqs ( *(qset.quadset) , isquadvalid );
//	
//	//mymin.ReplaceMin ( tqs.GetMin() );
//	//mymax.ReplaceMax ( tqs.GetMax() );
//	//tmin=mymin;
//	//tmax=mymax;
//	//MakeBoundingBox();
//	ModifMinMax(tqs.GetMin(),tqs.GetMax());
//
//	if ( visatt )
//	{
//		quadsdisplay.SetVisualAtt ( *visatt );
//	}
//
//	quadsdisplay.MakeDisplayListByVertexArray ( tqs );
//	//m_area += newArea;
//}

//#############################################################################

void Model::SetPointSet ( const PointSet *p_points , const PointVisualAtt *visatt )
{
	if ( visatt )
	{
		//pointsdisplay.SetVisualAtt (*visatt);
		m_display.SetVisualAttPoints (*visatt);
	}
	//pointsdisplay.MakeDisplayList ( * p_points );
	m_display.MakeDisplayList ( *p_points );
}

//#############################################################################

void Model::SetLineSet ( const LineSet * p_lineset , const LineVisualAtt *visatt )
{
	if ( visatt )
	{
		//linesdisplay.SetVisualAtt (*visatt);
		m_display.SetVisualAtt (*visatt);
	}
	//linesdisplay.MakeDisplayList ( *p_lineset );
	m_display.MakeDisplayList (*p_lineset);
}

//#############################################################################

void Model::SetLineSet ( const SequentialLineSet * p_lineset , const LineVisualAtt *visatt )
{
	if ( visatt )
	{
		//linesdisplay.SetVisualAtt (*visatt);
		m_display.SetVisualAtt (*visatt);
	}
	//linesdisplay.MakeDisplayList ( *p_lineset );
	m_display.MakeDisplayList ( *p_lineset );
}

//#############################################################################

void Model::SetLineSet(const std::vector<VPoint> *p_lineset, const std::vector<std::pair<size_t,size_t>> &rangeVec , const std::vector<LineVisualAtt> &visAttVec)
{
	if (visAttVec.size())
	{
		//linesdisplay.SetVisualAtt(visAttVec.front());
		m_display.SetVisualAtt (visAttVec.front());
	}
	//linesdisplay.MakeDisplayList(p_lineset , rangeVec , visAttVec);
	m_display.MakeDisplayList (p_lineset , rangeVec , visAttVec );
}
//void Model::SetLineSet ( const SurfaceWireFrame & wiredata , const LineVisualAtt *visatt)
//{
//	for ( int i=0;i<wiredata.Size();i++)
//	{
//		const SequentialLineSet * p_lineset = wiredata[i];
//		//mymin.ReplaceMin ( p_lineset->GetMin () );
//		//mymax.ReplaceMax ( p_lineset->GetMax () );
//		//tmin=mymin;
//		//tmax=mymax;	
//		ModifMinMax(p_lineset->GetMin(),p_lineset->GetMax());
//	}
//	
//	//MakeBoundingBox ();
//
//	if ( visatt )
//	{
//		linesdisplay.SetVisualAtt (*visatt);
//	}
//	linesdisplay.MakeDisplayList ( wiredata );
//
//}

//#############################################################################

//QuadsDisplay & Model::GetQuadsDisplayList ()
//{
//	return quadsdisplay;
//}

//FacetDisplay & Model::GetDisplayList ()
//{
//	return facetdisplay;
//}


//LinesDisplay & Model::GetWireDisplayList ()
//{
//	return linesdisplay;
//}


//PointsDisplay & Model::GetPointsDisplayList ()
//{
//	return pointsdisplay;
//}

CoordSystemDisplay * Model::GetCoordSystemDisplay ()
{
	return coorddisplay;
}

CoordinateSystem Model::GetCoordinateSystem () const
{
	CoordinateSystem res(mcoord);
	//if (coorddisplay)
	//{
		//TransformationMatrix mytrans = coorddisplay->GetOverallMat();
		//res.Transform(mytrans);	
	//}
	return res;
}

void Model::SetCoordSystemAndDisplay ( const CoordinateSystem & cs )
{
	//if ( coorddisplay )
	//	delete coorddisplay;
	VPoint delta ( 1 , 1 , 1 );
	VPoint cmin = cs.GetOrigin() - delta;
	VPoint cmax = cs.GetOrigin() + delta;
	mcoord = cs;
	if (coorddisplay == 0)
	{
		LineVisualAtt tmpva;
		tmpva.ChangeWireColor (VColor (1,0,0));
		tmpva.SetLineWidth(LineVisualAtt::mid);		
		coorddisplay = new CoordSystemDisplay ( &mcoord , tmpva );
	}
	
	snapent.snaplines.Clear();
	//adding lines for direction selection
	VLine lx(cs.GetOrigin(),cs.GetOrigin()+cs.GetDirx()*VGlobalValues::ONEMM_TO_VUNIT*10.);
	VLine ly(cs.GetOrigin(),cs.GetOrigin()+cs.GetDiry()*VGlobalValues::ONEMM_TO_VUNIT*10.);
	VLine lz(cs.GetOrigin(),cs.GetOrigin()+cs.GetDirz()*VGlobalValues::ONEMM_TO_VUNIT*10.);

	snapent.AddLine(lx);
	snapent.AddLine(ly);
	snapent.AddLine(lz);
	snapent.AddPoint(cs.GetOrigin());
}

void Model::AddSnapPoint ( const VPoint & snapp )
{
	snapent.AddPoint ( snapp );
}

void Model::AddSnapPointSet (const PointSet &snapp)
{
	snapent.AddPointSet (snapp);
}

void Model::AddSnapLine ( const VLine & snapl )
{
	snapent.AddLine ( snapl );
}

void Model::AddSnapLineSet ( const LineSet & snapl )
{
	snapent.AddLineSet ( snapl );
}

void Model::GetHitPoints ( PointSet & selpoints , DoubleVec & depthvalues , bool selectanyvertex) const
{
	if (selectanyvertex)
	{
		//facetdisplay.GetHitPoints ( selpoints , depthvalues );	
		m_display.GetHitPointsFacet (selpoints , depthvalues);
		//quadsdisplay.GetHitPoints ( selpoints , depthvalues );
		//linesdisplay.GetHitPoints ( selpoints , depthvalues );	
		m_display.GetHitPoints (selpoints , depthvalues);
	}
	//pointsdisplay.GetHitPoints ( selpoints , depthvalues );
	m_display.GetHitPointsPoints (selpoints , depthvalues ,0);
	if ( coorddisplay )
		coorddisplay->GetHitPoints ( selpoints , depthvalues );	
				
	snapent.GetHitPoints ( selpoints , depthvalues );
}
void Model::GetHitPointsNew (PointSet &selpoints , DoubleVec &depthvalues,const VPoint &curserp,const VPoint &lookdir,double selr)const
{
	int mysize = snapent.snappoints.Size();
	LinePointMinDist calc;
	for (int i=0;i<mysize;++i)
	{
		VPoint curp = *snapent.snappoints.operator [] (i);
		if (snapent.GetDynamicTransformData() != 0)
			curp = snapent.GetDynamicTransformData()->m_tm.Transform (curp);
		
		double curdist = calc.InfiniteLinePointMinDist (curserp , lookdir , curp);
		if (curdist < selr)
		{
			selpoints.InsertPoint (curp);
			VPoint diff = ((curp) - curserp);
			double depthlen2 =  (diff.GetLength2() - curdist*curdist);
			if (diff % lookdir < 0.0)
				depthlen2 *= -1.;
			depthvalues.Addvreal (depthlen2);
		}
	}
}

void Model::GetHitLines ( LineSet & sellines , DoubleVec & depthvalues ) const
{
	//linesdisplay.GetHitLines(sellines , depthvalues );
	m_display.GetHitLines (sellines , depthvalues );
	snapent.GetHitLines( sellines , depthvalues );
}

void Model::DrawSnapPoints () const
{
	snapent.DrawPoints();
}
void Model::DrawSnapLines() const
{
	snapent.DrawLines();
}
SnapEntities & Model::GetSnapEnt()
{
	return snapent;
}

void Model::OnTreeViewClicked()
{
	visible = !visible;
}

void Model::OnTreeViewDoubleClicked()
{

}

void Model::Transform(TransformData &td)
{

}

//FontDisp * Model::SetFontDisp (VStr txt , const vreal &fontsize , const CoordinateSystem &base,const NumberFonts &nf)
//{
//	FontDisp *fontdsp = new FontDisp (txt , fontsize , base,nf);
//	if (rootfont == 0)
//	{
//		rootfont = fontdsp;
//	}
//	else
//	{
//		rootfont->next = fontdsp;
//	}
//	VPoint newmin = base.GetOrigin() - (base.GetDirx()+base.GetDiry()) * fontsize * txt.length();
//	VPoint newmax =  base.GetOrigin() + (base.GetDirx()+base.GetDiry()) * fontsize * txt.length();
//	return fontdsp;
//}
//
//FontDisp * Model::GetFontDisp()
//{
//	return rootfont;
//}

//FacetDisplay & Model::GetFacetDisplay()
//{
//	return facetdisplay;
//}
//const FacetDisplay & Model::GetFacetDisplay()const
//{
//	return facetdisplay;
//}
//LinesDisplay & Model::GetLinesDisplay()
//{
//	return linesdisplay;
//}
//const LinesDisplay & Model::GetLinesDisplay()const
//{
//	return linesdisplay;
//}
//PointsDisplay & Model::GetPointsDisplay()
//{
//	return pointsdisplay;
//}
//const PointsDisplay & Model::GetPointsDisplay()const
//{
//	return pointsdisplay;
//}
Display & Model::GetDisplay ()
{
	return Display::m_isGraphicViewChanging ? *m_quickDisplay : m_display;
	//return m_display;
}
void Model::GetBothDisplay(Display **quickDisplay , Display **disp)
{
	*quickDisplay = m_quickDisplay;
	*disp = &m_display;
}
const Display & Model::GetDisplay ()const
{
	//Display::m_isGraphicViewChanging = true;
	return Display::m_isGraphicViewChanging ? *m_quickDisplay : m_display;
	//return m_display;
}

void Model::OnTreeViewRightClicked(const VStr &commandcap)
{
	if (commandcap == TREEVIEWCONTEXTMENU_HIDE)
		SetVisible(false);
	else if (commandcap == TREEVIEWCONTEXTMENU_UNHIDE)
		SetVisible(true);
}

void Model::SetTransformable(VTransformable *tr)
{
	m_transform = tr;
}

VTransformable * Model::GetTransformable ()
{
	return m_transform;
}
const VStr Model::GetModelTypeEng() const
{
	return ViewPointItemTypes::VpModelEng();
}

int Model::GetSelectionTime()const
{
	return m_selectionTime;
}

void Model::SetSelectionTime(int selectionTime)
{
	m_selectionTime = selectionTime;
}

void Model::TransformSnapEnt(const TransformationMatrix &tm)
{
	snapent.Transform (tm);
}

//void Model::MakeTextureDisplayList(const VRefTriangleMesh *p_tri, const texture *p_texture, const FacetVisualAtt *visatt)
//{
//	try
//	{
//		if (visatt)
//		{
//			//facetdisplay.SetVisualAtt ( *visatt );
//			m_display.SetVisualAttFacet(*visatt);
//		}
//		//facetdisplay.MakeTextureDisplayList ( p_tri , p_texture);	
//		m_display.MakeTextureDisplayList (p_tri , p_texture);
//	}
//	catch (VException &e)
//	{
//		LogFileWriter::WriteUnknownExceptionLog (( IDS_ModMakVpExc));
//		LogFileWriter::WriteExceptionLog (e);	
//	}
//	catch (std::exception &e)
//	{
//		LogFileWriter::WriteUnknownExceptionLog (( IDS_ModMakStdExc));
//		LogFileWriter::WriteExceptionLog (e);
//	}
//	catch (...)
//	{
//		LogFileWriter::WriteUnknownExceptionLog (( IDS_ModMakUnkExc));
//	}
//}
void Model::MakeTextureDisplayList(const VRefTriangleMesh *p_tri, const VTexture *p_texture, const FacetVisualAtt *visatt)
{
	try
	{
		if (visatt)
		{
			//facetdisplay.SetVisualAtt ( *visatt );
			m_display.SetVisualAttFacet(*visatt);
		}
		//facetdisplay.MakeTextureDisplayList ( p_tri , p_texture);	
		m_display.MakeTextureDisplayList (p_tri , p_texture);
	}
	catch (VException &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ModMakVpExc));
		LogFileWriter::WriteExceptionLog (e);	
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ModMakStdExc));
		LogFileWriter::WriteExceptionLog (e);
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_ModMakUnkExc));
	}
}
void Model::MakeDisplayList (std::vector<const Display *> &facetVec , std::vector<const Display*> &linesDisplay , std::vector<const Display* > &pointsDisplay
	,std::vector<OglMatrix> &curveTmVec)
{
	m_display.MakeDisplayList(facetVec,linesDisplay,pointsDisplay,curveTmVec);
}

void Model::MakeQuickDisplayList (std::vector<const Display *> &facetVec , std::vector<const Display*> &linesDisplay , std::vector<const Display *> &pointsDisplay
		, std::vector<OglMatrix> &curveTmVec)
{
	m_quickDisplay = new Display();
	m_quickDisplay->MakeDisplayList (facetVec,linesDisplay,pointsDisplay,curveTmVec);
}

void Model::MakeQuadDisplayList (std::vector<QuadPointGenerator *> &quadPoints , FacetVisualAtt &visAtt) 
{
	m_display.SetVisualAttFacet(visAtt);
	m_display.MakeQuadDisplay (quadPoints);	
}

void Model::SetCoord(const CoordinateSystem &cs)
{
	mcoord = cs;
}

bool Model::IsColorSelectable(COLORREF col)const
{
	VColor tempcol(col);
	if (m_display.GetVisualAttFacet().GetShadeColor() == tempcol || m_display.GetVisualAtt().GetWireColor() == tempcol)
		return true;
	else
		return false;
}

void Model::ResetTypeName(const VStr &typeNameToSet)
{
	m_modeltype = typeNameToSet;
}
