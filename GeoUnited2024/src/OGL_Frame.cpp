// OGL_Frame.cpp: implementation of the OGL_Frame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasicDefinitions.h"
#include "OGL_Frame.h"
#include "Matrix.h"
#include "LineSet.h"
#include "gmVectorUtility.h"
#include "VisualAttributes.h"
#include "TransformationMatrix.h"
#include "DoubleList.h"
#include "SelectionFlags.h"
#include "ModelList.h"
#include "Model.h"
#include "ViewingState.h"
#include "TriangleMesh2d.h"
//#include "VString.h"
#include "OglWindowData.h"
#include "MultiTextToPointsObject.h"
#include "DraftEntityVec.h"
//#include "FontDisp.h"
#include "Display.h"
#include "LabelDisplay.h"
#include "ViewPointItemTypes.h"
#include "HandleSet.h"
#include "OglMatrix.h"
#include "RotationMatrix.h"
#include "DoubleVec.h"
#include "OperationTransformList.h"
#include "VPlane.h"
#include "PolarCalc2d.h"
#include "SelectableTypes.h"
#include "dglLine.h"
#include "dglArc.h"
#include "VTransformable.h"
#include "GridDrawNew.h"
#include "ViewPointDefaults.h"
#include "gl\\glu.h"
#include "TextBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const VColor default_colupleft ( 0.1 , 0.5 , 0.8 );
const VColor default_colbottomleft ( 0.7803 , 0.9098 , 1.0 );
const VColor default_colbottomright ( 0.7803 , 0.9098 , 1.0 );
const VColor default_colupright ( 0.1 , 0.5 , 0.8 );

const vreal OGL_MAX_DRAWING_SPHERE_RAD = 1.e4;
double OGL_Frame::M_MAXWORKDIAMETER = 7000.0;
//std::vector<Display *> OGL_Frame::m_asciiDisp = std::vector<Display *>();

void (OGL_Frame::*funcPtrDrawPoints)(const ModelList &, const std::vector<VTransformable *> &)const = NULL; 
void (OGL_Frame::*funcPtrDrawShaded)(const ModelList &, const std::vector<VTransformable *> &)const = NULL; 
void (OGL_Frame::*funcPtrDrawWireFrame)(const ModelList &, const std::vector<VTransformable *> &) = NULL; 
void (OGL_Frame::*funcPtrDrawCoordSystems)(ModelList &, const std::vector<VTransformable *> &)const = NULL; 
void (OGL_Frame::*funcPtrDrawDraftEntities)(DraftEntityVec &, const std::vector<VTransformable *> &)const = NULL; 

OGL_Frame::OGL_Frame( ViewingState & _viewingstate)
: view_volume() //,hitr ( view_volume.corner_x , view_volume.corner_y )  
, selection ( view_volume , quat , rot_center) , selectioncolor (VGlobalValues::viewpointdefaults.m_selectionColor )
, viewingstate ( _viewingstate )
, clipplane ( VPoint (0,0,0),VPoint(1,0,0) )
, m_liftmag(0.5 * VGlobalValues::ONEMM_TO_VUNIT)
, m_rotationCenterCoord ()
, m_rotationCenterVisatt ()
, m_rotationCenterCoordDisp (0)
, m_selmodel (0)
, m_surfselmodel (0)
,m_dglLines(0)
,m_dglArcs(0)
,m_drawSelectionRectangle(false)
,m_gridDraw (0)
,m_isVpRenderContextNull (false)
{
	SetColorArray ( default_colupleft , default_colbottomleft , default_colbottomright , default_colupright );
	VPoint temp ( 0.0 , 0.0 , 0.0 );
	rot_center = temp;
	scale_factor = 1.0;
	m_drawingstate = shadedandwireframe;
	VColor newcolor (VGlobalValues::viewpointdefaults.m_defaultWireframeColor);
	m_rotationCenterVisatt.SetLineWidth (LineVisualAtt::thick);
	m_rotationCenterVisatt.ChangeWireColor (newcolor);
	//VColor tmp (1,1,1);
	MakeGridDraw();
	SetColorMode (VGlobalValues::viewpointdefaults.m_colorMode);
	m_statusText.resize(5);
	m_statusTextHeader.resize(5);
	m_statusTextHeader[0] = "";// sozluk("4 messages before:");
	m_statusText[0] = sozluk("Hello SolidCNC is initialising...");
	m_statusTextHeader[1] = "";// sozluk("3 messages before:");
	m_statusText[0] = sozluk("Hello SolidCNC is initialising...");
	m_statusTextHeader[2] = "";// sozluk("2 messages before:");
	m_statusText[0] = sozluk("Hello SolidCNC is initialising...");
	m_statusTextHeader[3] = "";// sozluk("Previous message:");
	m_statusText[1] = sozluk("These are information text");
	m_statusTextHeader[4] = "";//sozluk("Active message:"); 
	m_statusText[2] = sozluk("SolidCNC CAD CAM Software");
}

void OGL_Frame::InitOGL_Frame (ModelList *modellist,DraftEntityVec *draftvec,OperationTransformList *transformations,SelectionFlags *sel_flags
							   ,std::map<VStr , std::vector<dglLine *>> *dglLines , std::map<VStr , std::vector<dglArc *>> *dglArcs)
{
	m_transformations=transformations;
	m_modellist=modellist;
	m_draftvec=draftvec;
	selection.SetSelectionFlags(sel_flags);
	m_dglLines = dglLines;
	m_dglArcs = dglArcs;
}

OGL_Frame::~OGL_Frame()
{
	if (m_rotationCenterCoordDisp)
		delete m_rotationCenterCoordDisp;
}

void OGL_Frame::InitSize( int screenx , int screeny )
{
	Size ( screenx , screeny );
}
void OGL_Frame::FirstInit()
{
	glEnable (GL_DEPTH_TEST);	
	glShadeModel (GL_SMOOTH);
	if (m_rotationCenterCoordDisp == 0)
	{
		m_rotationCenterCoordDisp = new CoordSystemDisplay (&m_rotationCenterCoord , m_rotationCenterVisatt);
		VPoint amin (-10.,-10.,-10.);
		VPoint amax (10.,10.,10.);
		Fit (amin,amax);
	}
	glPolygonMode (GL_FRONT_AND_BACK,GL_FILL);
	Light alight;
	glLightModeli (GL_LIGHT_MODEL_TWO_SIDE , GL_TRUE);
	//alight.SetLight (GL_LIGHT1);
	alight.InitLightAsRedBookLitSphereExample();
	glEnable(GL_NORMALIZE);
	glFrontFace(GL_CCW);

	//bool lineAntialisingActive = false;
	//if (lineAntialisingActive)
	//{
	//	glEnable (GL_LINE_SMOOTH);
	//	glEnable (GL_BLEND);
	//	glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//	glHint (GL_LINE_SMOOTH_HINT , GL_DONT_CARE);
	//	//glLineWidth (1.5);
	//}
}

void OGL_Frame::Init()
{
	//glEnable(GL_DEPTH_TEST);        //Enable Depth Test
	//
	////glShadeModel (GL_FLAT);
	//glShadeModel (GL_SMOOTH);
	////For triangle seeing
	////glPolygonMode (GL_FRONT_AND_BACK,GL_LINE);
	////glDisable ( GL_POLYGON_STIPPLE );

	//glMatrixMode ( GL_MODELVIEW );
	//glPushMatrix ();
	//glLoadIdentity ();
	////lighting.light.SetLight(GL_LIGHT0);
	//
	//if (m_rotationCenterCoordDisp == 0)
	//{
	//	m_rotationCenterCoordDisp = new CoordSystemDisplay (m_rotationCenterCoord , m_rotationCenterVisatt);
	//	VPoint amin (-10.,-10.,-10.);
	//	VPoint amax (10.,10.,10.);
	//	Fit (amin,amax);
	//}
	//
	//glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE , GL_TRUE );
	//
	//glClearColor(0.4392f,0.62f,0.94f,1.0f);  
	//glClearDepth(1.0f);
 //   glFrontFace(GL_CCW);    
 //          //Vertices of the primitives are wound 
 //          //in counter clock direction
	glClearDepth (1.0f);
	glDisable (GL_LIGHTING);
	glPolygonMode (GL_FRONT_AND_BACK , GL_FILL);
}

//void OGL_Frame::Draw()
//{
//	glClear(//GL_COLOR_BUFFER_BIT | 
//		GL_DEPTH_BUFFER_BIT);
//	view_volume.SetViewVolume();
//	visualattloader.ResetPrev();
//	WavyScreen();
//	
//	//glLightModeli (GL_LIGHT_MODEL_TWO_SIDE , GL_TRUE);
//	//Light alight;
//	//alight.SetLight (GL_LIGHT1);
//	//glFrontFace(GL_CCW);
//
//	// Current matrix will be model view matrix
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();      //Initialize matrix	
//	glMultMatrixd(quat.QuatToMat()); 
//	ClipIf();
//	// Making view center the origin
//	glTranslated ( -rot_center.x() , -rot_center.y() , -rot_center.z() );
//
//	const std::vector<OperationTransformList::Mat_Mod *> &allmat = m_transformations->GetAllMat();
//	std::vector<OperationTransformList::Mat_Mod *>::const_iterator i;
//	std::vector<OperationTransformList::Mat_Mod *>::const_iterator e=allmat.end();
//
//	if (VGlobalValues::viewpointdefaults.m_doShowStatusText == TRUE)
//	{
//		DrawViewPointStatusText();
//	}
//
//	// draw geometry
//	//const HandleSet &hs = m_transformations->GetUntransformed();
//	const std::vector<VTransformable *> &hs = m_transformations->GetUntransformed();
//
//	if (m_drawingstate != OGL_Frame::wireframe)
//	{
//		glFrontFace (GL_CCW);
//		glDisable(GL_NORMALIZE);
//		//m_gridDraw->DrawAbs();
//		glMatrixMode(GL_MODELVIEW);
//		glPushMatrix();
//		glLoadIdentity();
//		GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };;// { 0.0f, 0.0f, 1000.0f, 0.0f };
//		GLfloat white_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//		GLfloat lmodel_ambient[] = { 0.50f, 0.50f, 0.50f, 1.0f };
//		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//		glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
//		glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
//		glLightfv(GL_LIGHT0, GL_AMBIENT, lmodel_ambient);
//		glPopMatrix();
//		(this->*funcPtrDrawShaded) (*m_modellist,hs);
//
//	}
//	if (m_drawingstate != OGL_Frame::shaded && m_drawingstate != pixelrenderBlackAndWhite) 
//	{
//		DrawDglLinesAndArcs();
//		m_gridDraw->Draw();
//		LiftWireFrame();
//		(this->*funcPtrDrawWireFrame) ( *m_modellist , hs);
//		Unlift();
//	}
//
//	if (m_drawingstate == pixelrenderBlackAndWhite)
//		return;		// the rest need not be drawn
//
//	(this->*funcPtrDrawPoints) ( *m_modellist , hs);
//	for (i=allmat.begin();i!=e;++i)
//	{
//		if ((*i)->m_isOpen)
//		{
//			SetModelViewMatrix ((*i)->m_tm.m_om);
//			const std::vector<VTransformable *> &hs2 = (*i)->m_mvec;
//			(this->*funcPtrDrawPoints) ( *m_modellist , hs2);
//			glMatrixMode(GL_MODELVIEW);
//			glPopMatrix();
//		}
//	}
//	UpdateRotCenterCoordPosition ();	
//
//	glDisable ( GL_DEPTH_TEST );	
//	glDisable ( GL_CLIP_PLANE1 );
//	DrawViewCoordinateSystem ();
//	glDisable ( GL_DEPTH_TEST );
//	
//	//LiftWireFrame();
//	(this->*funcPtrDrawCoordSystems) ( *m_modellist , hs);
//	DrawSelectionRectangle ();
//	
//	// Draw dynamic transformation(s)
//	if (m_transformations->IsThereDynamicTransformation())
//	{
//		for (i=allmat.begin();i!=e;++i)
//		{
//			if ((*i)->m_isOpen)
//			{
//				// Set ogl modelview matrix
//				SetModelViewMatrix ((*i)->m_tm.m_om);
//				const std::vector<VTransformable *> &hs2 = (*i)->m_mvec;
//				(this->*funcPtrDrawShaded) ( *m_modellist , hs2);
//				glMatrixMode(GL_MODELVIEW);
//				glPopMatrix();
//			}
//		}
//		LiftWireFrame();
//		for (i=allmat.begin();i!=e;++i)
//		{
//			if ((*i)->m_isOpen)
//			{
//				SetModelViewMatrix ((*i)->m_tm.m_om);
//				const std::vector<VTransformable *> &hs2 = (*i)->m_mvec;
//				(this->*funcPtrDrawWireFrame) ( *m_modellist , hs2);
//				glMatrixMode(GL_MODELVIEW);
//				glPopMatrix();
//			}
//		}
//		Unlift();
//		for (i=allmat.begin();i!=e;++i)
//		{
//			if ((*i)->m_isOpen)
//			{
//				SetModelViewMatrix ((*i)->m_tm.m_om);
//				const std::vector<VTransformable *> &hs2 = (*i)->m_mvec;
//				(this->*funcPtrDrawPoints) ( *m_modellist , hs2);
//				glMatrixMode(GL_MODELVIEW);
//				glPopMatrix();
//			}
//		}
//		for (i=allmat.begin();i!=e;++i)
//		{
//			if ((*i)->m_isOpen)
//			{
//				SetModelViewMatrix ((*i)->m_tm.m_om);
//				const std::vector<VTransformable *> &hs2 = (*i)->m_mvec;
//				(this->*funcPtrDrawCoordSystems) ( *m_modellist , hs2);
//				glMatrixMode(GL_MODELVIEW);
//				glPopMatrix();
//			}
//		}
//	}
//	(this->*funcPtrDrawDraftEntities) ( *m_draftvec , hs);	
//	//DrawFontDisp (*m_modellist , hs);
//	//Unlift();
//	glEnable(GL_DEPTH_TEST);
//}

void OGL_Frame::Draw()
{
	glClear(//GL_COLOR_BUFFER_BIT | 
		GL_DEPTH_BUFFER_BIT);
	view_volume.SetViewVolume();
	visualattloader.ResetPrev();
	WavyScreen();

	//glLightModeli (GL_LIGHT_MODEL_TWO_SIDE , GL_TRUE);
	//Light alight;
	//alight.SetLight (GL_LIGHT1);
	//glFrontFace(GL_CCW);

	// Current matrix will be model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();      //Initialize matrix	
	glMultMatrixd(quat.QuatToMat());
	ClipIf();
	// Making view center the origin
	glTranslated(-rot_center.x(), -rot_center.y(), -rot_center.z());

	const std::vector<OperationTransformList::Mat_Mod *> &allmat = m_transformations->GetAllMat();
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator i;
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator e = allmat.end();

	if (VGlobalValues::viewpointdefaults.m_doShowStatusText == TRUE)
	{
		DrawViewPointStatusText();
	}

	// draw geometry
	//const HandleSet &hs = m_transformations->GetUntransformed();
	const std::vector<VTransformable *> &hs = m_transformations->GetUntransformed();

	if (m_drawingstate != OGL_Frame::wireframe)
	{
		glFrontFace(GL_CCW);
		glDisable(GL_NORMALIZE);
		//m_gridDraw->DrawAbs();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };;// { 0.0f, 0.0f, 1000.0f, 0.0f };
		GLfloat white_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat lmodel_ambient[] = { 0.50f, 0.50f, 0.50f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
		glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lmodel_ambient);
		glPopMatrix();
		(this->*funcPtrDrawShaded) (*m_modellist, hs);

	}
	if (m_drawingstate != OGL_Frame::shaded && m_drawingstate != pixelrenderBlackAndWhite)
	{
		DrawDglLinesAndArcs();
		m_gridDraw->Draw();
		LiftWireFrame();
		(this->*funcPtrDrawWireFrame) (*m_modellist, hs);
		Unlift();
	}

	if (m_drawingstate == pixelrenderBlackAndWhite)
		return;		// the rest need not be drawn

	(this->*funcPtrDrawPoints) (*m_modellist, hs);
	for (i = allmat.begin(); i != e; ++i)
	{
		if ((*i)->m_isOpen)
		{
			SetModelViewMatrix((*i)->m_tm.m_om);
			const std::vector<VTransformable *> &hs2 = (*i)->m_mvec;
			(this->*funcPtrDrawPoints) (*m_modellist, hs2);
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}
	UpdateRotCenterCoordPosition();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CLIP_PLANE1);
	DrawViewCoordinateSystem();
	glDisable(GL_DEPTH_TEST);

	//LiftWireFrame();
	(this->*funcPtrDrawCoordSystems) (*m_modellist, hs);
	DrawSelectionRectangle();

	// Draw dynamic transformation(s)
	if (m_transformations->IsThereDynamicTransformation())
	{
		for (i = allmat.begin(); i != e; ++i)
		{
			if ((*i)->m_isOpen)
			{
				// Set ogl modelview matrix
				SetModelViewMatrix((*i)->m_tm.m_om);
				const std::vector<VTransformable *> &hs2 = (*i)->m_mvec;
				(this->*funcPtrDrawShaded) (*m_modellist, hs2);
				glMatrixMode(GL_MODELVIEW);
				glPopMatrix();
			}
		}
		LiftWireFrame();
		for (i = allmat.begin(); i != e; ++i)
		{
			if ((*i)->m_isOpen)
			{
				SetModelViewMatrix((*i)->m_tm.m_om);
				const std::vector<VTransformable *> &hs2 = (*i)->m_mvec;
				(this->*funcPtrDrawWireFrame) (*m_modellist, hs2);
				glMatrixMode(GL_MODELVIEW);
				glPopMatrix();
			}
		}
		Unlift();
		for (i = allmat.begin(); i != e; ++i)
		{
			if ((*i)->m_isOpen)
			{
				SetModelViewMatrix((*i)->m_tm.m_om);
				const std::vector<VTransformable *> &hs2 = (*i)->m_mvec;
				(this->*funcPtrDrawPoints) (*m_modellist, hs2);
				glMatrixMode(GL_MODELVIEW);
				glPopMatrix();
			}
		}
		for (i = allmat.begin(); i != e; ++i)
		{
			if ((*i)->m_isOpen)
			{
				SetModelViewMatrix((*i)->m_tm.m_om);
				const std::vector<VTransformable *> &hs2 = (*i)->m_mvec;
				(this->*funcPtrDrawCoordSystems) (*m_modellist, hs2);
				glMatrixMode(GL_MODELVIEW);
				glPopMatrix();
			}
		}
	}
	(this->*funcPtrDrawDraftEntities) (*m_draftvec, hs);
	//DrawFontDisp (*m_modellist , hs);
	//Unlift();
	glEnable(GL_DEPTH_TEST);
}

void OGL_Frame::DrawPoints (const ModelList & modellist , const std::vector<VTransformable *> &hs) const
{
	//glDisable ( GL_LIGHTING );

	if ( viewingstate.IsClipped () )
		glEnable ( GL_CLIP_PLANE1 );

	Model * currmodel;

	int mysize = (int)hs.size();
	
	for (int i=0; i<mysize; ++i)
	{
		//const ModelID &id = hs.GetID(i);
		VTransformable *curtr = hs.at(i);
		currmodel = curtr->GetTrModel();//modellist.GetModel(id);

		//bool forcedrawing = false;
		
		//if ( currmodel->/*GetDisplayList()*/GetDisplay().IsDefined() == false
		//	//&& currmodel->GetWireDisplayList().IsDefined() == false 
		//	&& currmodel->GetCoordSystemDisplay() == 0  )
		//		forcedrawing = true;

		if ( currmodel->IsVisible()==false )
			continue;
		if(currmodel->GetDeleted()==true)
			continue;

		//const PointsDisplay & pointdisp = currmodel->GetPointsDisplayList();
		Display &disp = currmodel->GetDisplay();

		//if ( pointdisp.GetVisualAtt().GetDraw() || forcedrawing )
		//if (disp.GetVisualAttPoints().GetDraw() || forcedrawing)
		//{
			visualattloader.Load ( /*pointdisp.*/disp.GetVisualAttPoints() , currmodel->IsSelected() , selectioncolor );
			//pointdisp.Draw();
			disp.DrawPoints();
		//}
	}
	//glEnable (GL_LIGHTING);
}

void OGL_Frame::DrawWireFrame ( const ModelList & modellist , const std::vector<VTransformable *> &hs)
{
	//glDisable ( GL_LIGHTING );
	//LiftWireFrame ();
	
	if ( viewingstate.IsClipped () )
		glEnable ( GL_CLIP_PLANE1 );

	Model * currmodel;
	int mysize=(int)hs.size();

	for (int i = 0; i < mysize; ++i)
	{
		//if (Display::m_isGraphicViewChanging && i % 2 == 0)
		//	continue;
		//const ModelID &id = hs.GetID(i);
		VTransformable * curtr = hs.at(i);
		currmodel = curtr->GetTrModel();//modellist.GetModel(id);
		if ( currmodel->IsVisible()==false )
			continue;
		if(currmodel->GetDeleted()==true)
			continue;
		//const LinesDisplay & wiredisp = currmodel->GetWireDisplayList();
		Display &disp = currmodel->GetDisplay();
		//bool forcedrawing = false;
		//if ( currmodel->GetDisplayList().IsDefined() == false 
		//	&& currmodel->GetCoordSystemDisplay() == 0 
		//	&& currmodel->GetPointsDisplayList().IsDefined() == false   )
		//	forcedrawing = true;

		//if ( wiredisp.GetVisualAtt().GetDraw()) //|| forcedrawing )
		//if (disp.GetVisualAtt().GetDraw())
		//{
			//visualattloader.Load ( wiredisp.GetVisualAtt() , currmodel->IsSelected() , selectioncolor );
			visualattloader.Load (disp.GetVisualAtt(), currmodel->IsSelected() , selectioncolor);
			//wiredisp.Draw();
			disp.DrawLines(currmodel->IsSelected());
		//}
	}
	//Unlift();
	//glEnable (GL_LIGHTING);
}

void OGL_Frame::DrawShaded ( const ModelList & modellist ,const std::vector<VTransformable *> &hs) const
{
	if (m_drawingstate != OGL_Frame::triangles )//&& m_drawingstate != OGL_Frame::hiddenline && m_drawingstate!= hiddenlinegray)
		glEnable (GL_LIGHTING);
	if ( viewingstate.IsClipped () )
		glEnable ( GL_CLIP_PLANE1 );
	
	//FacetVisualAtt tmp;
	//tmp.ChangeShadeColor(VGlobalValues::viewpointdefaults.m_defaultSurfaceColor);

	//if ( m_drawingstate != hiddenline && m_drawingstate != hiddenlinegray && m_drawingstate != pixelrenderBlackAndWhite)
	//{
	//	//glEnable ( GL_LIGHTING );
	//}
	//else
	//{
		//glDisable ( GL_LIGHTING );
		//glColor3d ( hiddenlinecolor.R() , hiddenlinecolor.G() , hiddenlinecolor.B() );
		//if (m_drawingstate != pixelrenderBlackAndWhite)
		//{
		//	//tmp.ChangeShadeColor (hiddenlinecolor);
		//	if (m_drawingstate == hiddenlinegray)
		//		tmp.SetTranslucent(true);
		//}
		//else
		//{

		//}

		//visualattloader.Load ( tmp , false , selectioncolor );
	//}
	if (m_drawingstate == triangles)
	{
		glPolygonMode (GL_FRONT_AND_BACK , GL_LINE);
	}

	Model * currmodel;
	int mysize = (int)hs.size();
	for (int i = 0; i <mysize; ++i)
	{
		//const ModelID &id = hs.GetID(i);
		VTransformable *curtr = hs.at(i);
		currmodel = curtr->GetTrModel();//modellist.GetModel(id);

		//bool forcedrawing = false;
		
		//if ( currmodel->GetPointsDisplayList().IsDefined() == false
		//	&& currmodel->GetWireDisplayList().IsDefined() == false 
		//	&& currmodel->GetCoordSystemDisplay() == 0  )
		//		forcedrawing = true;
		
		if ( currmodel->IsVisible()==false )
			continue;
		if (currmodel->GetDeleted()==true)
			continue;
	
		//const FacetDisplay & facetdisp = currmodel->GetDisplayList();
		const Display & disp = currmodel->GetDisplay();
		//if ( facetdisp.GetVisualAtt().GetDraw() )//|| forcedrawing )
		//if (disp.GetVisualAttFacet().GetDraw())
		//{
			if (m_drawingstate != hiddenline && m_drawingstate != hiddenlinegray && m_drawingstate != pixelrenderBlackAndWhite)
				//visualattloader.Load ( facetdisp.GetVisualAtt() , currmodel->IsSelected() , selectioncolor );
				visualattloader.Load ( disp.GetVisualAttFacet() , currmodel->IsSelected() , selectioncolor);
			else
			{
				FacetVisualAtt tmp(disp.GetVisualAttFacet());
				//tmp.ChangeShadeColor (disp.GetVisualAttFacet().GetShadeColor());
				tmp.SetTranslucent(true);
				visualattloader.Load (tmp,currmodel->IsSelected(),selectioncolor);
			}
			//facetdisp.Draw();
			disp.DrawFacet();
		//}

		//const QuadsDisplay & quadsdisp = currmodel->GetQuadsDisplayList();
		//if ( quadsdisp.GetVisualAtt().GetDraw() )
		//{
		//	if ( hiddenline == false )
		//		visualattloader.Load ( quadsdisp.GetVisualAtt() , currmodel->IsSelected() , selectioncolor );
			
		//	quadsdisp.Draw();
		//}
					
	}
	glDisable (GL_LIGHTING);
}

void OGL_Frame::Size (int cx, int cy)
{
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity(); 
	view_volume.corner_x = cx;
	view_volume.corner_y = cy;

		
	
	//The aspect ratio should be applied to view_volume
	vreal dx = (view_volume.m_right - view_volume.m_left)/2.0;
	vreal dy = view_volume.m_top - view_volume.m_bottom/2.0;
	
	vreal ar ;
	ar = (vreal)view_volume.corner_x / (vreal)view_volume.corner_y;
	
	perspective.SetAspect ( ar );
	
	if ( viewingstate.IsPerspectiveNavigate() )
		perspective.SetPerspective();

	dy = dx/ar;

	view_volume.m_right = dx;
	view_volume.m_left = -dx;
	
	view_volume.m_top = dy;
	view_volume.m_bottom = -dy;

	glViewport (0,0,cx,cy);
	//hitr.CalcDiagonal ( view_volume.corner_x , view_volume.corner_y );

//	int anim = animation_stepno;
//	animation_stepno = 1;
//	FitViewVolume(cdc , model_list);
//	animation_stepno = anim;

}

void OGL_Frame::WavyScreen ()
{
	//Draw a quad and smooth shade them!***Ata!
	glDisable (GL_POLYGON_STIPPLE);
	glMatrixMode ( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity ();
				
	VPoint normal ( 0.0 , 0.0 , 1.0 );
	
	VPoint qcor1 ( view_volume.m_left , view_volume.m_top , -view_volume.m_far*0.999 );

	VPoint qcor2 ( view_volume.m_left , view_volume.m_bottom , -view_volume.m_far*0.999 );
	
	VPoint qcor3 ( view_volume.m_right , view_volume.m_bottom , -view_volume.m_far*0.999 );
		
	VPoint qcor4 ( view_volume.m_right , view_volume.m_top , -view_volume.m_far*0.999 );

	//glDisable (GL_LIGHTING);
	glDisable (GL_CLIP_PLANE1);
/*
	TriangleMesh bg;
	TriangleMesh2d pbg;
	pbg.AddTriangle ( VPoint2d(0.,1.),VPoint2d(0.,0.),VPoint2d(1.,0.));
	pbg.AddTriangle ( VPoint2d(1.,0.),VPoint2d(1.,1.),VPoint2d(0.,1.));
	bg.AddTriangle ( qcor1 , qcor2 , qcor3 );
	bg.AddTriangle ( qcor3 , qcor4 , qcor1 );
	glColor3d ( 0.9,0.9,1. );
	bg.DrawWithTexture(mybackground,pbg);	
*/

	//if ( viewingstate.IsPerspectiveNavigate() == false )
	{
		glBegin ( GL_QUADS );
		glNormal3dv(normal.GetArray());
	
		//if ( m_drawingstate == hiddenline || m_drawingstate == hiddenlinegray )
		//{
		//	glColor3d ( hiddenlinecolor.R() , hiddenlinecolor.G() , hiddenlinecolor.B() );
		//}
		//else
		//{
			glColor3d ( color1_up_left.R() , color1_up_left.G() , color1_up_left.B() );
		//}

		glVertex3dv ( qcor1.GetArray() );

		//if ( m_drawingstate != hiddenline && m_drawingstate != hiddenlinegray )
		//{
			glColor3d ( color2_bottom_left.R() , color2_bottom_left.G() , color2_bottom_left.B() );
		//}
		glVertex3dv ( qcor2.GetArray() ); 

		//if ( m_drawingstate != hiddenline && m_drawingstate != hiddenlinegray )
		//{
			glColor3d ( color3_bottom_right.R() , color3_bottom_right.G() , color3_bottom_right.B() );
		//}
		glVertex3dv ( qcor3.GetArray() );

		//if ( m_drawingstate != hiddenline && m_drawingstate != hiddenlinegray )
		//{
			glColor3d ( color4_up_right.R() , color4_up_right.G() , color4_up_right.B() );
		//}
		glVertex3dv ( qcor4.GetArray() );
	
		glEnd();
	}


	glPopMatrix();
	//glEnable(GL_LIGHTING);

}

void OGL_Frame::Rotate ( Mouse& mouse)
{
		GLdouble delta_x, delta_y;

		VPoint delta_p;

	//	delta_p = mouse.GetDeltaP_Current_Prev();

		delta_p = mouse.GetCurrentP() - mouse.GetPrevP();//.GetLButtonDownP();

		delta_x = (int)delta_p.x();
		delta_y = (int)delta_p.y();

		//delta_x = (int)delta_x % 10800;
		//delta_y = (int)delta_y % 10800;
			
		delta_x /= 120;
		delta_y /= 120;

		Quat q;
		q.EulerToQuat(-delta_y,-delta_x,0);
		quat.Multip(q);
}

void OGL_Frame::Roll ( Mouse & mouse )
{
	GLdouble delta , delta_x, delta_y;

	VPoint delta_p;

	delta_p = mouse.GetCurrentP() - mouse.GetPrevP();//.GetLButtonDownP();

	delta_x = (int)delta_p.x();
	delta_y = (int)delta_p.y();

	delta_x /= 2*120;
	delta_y /= 2*120;
	delta = delta_x + delta_y;

	Quat q;
	q.EulerToQuat(0,0,delta);
	quat.Multip(q);
}

void OGL_Frame::SetRotationAngles ()
{
	GLfloat den[16];
	
 	glGetFloatv ( GL_MODELVIEW_MATRIX , den );
	
	
	rot_angles.y( vp_asin( (vreal)den[8] ) * 180.0/PI );

	vreal cosbeta = cos ( rot_angles.x() *PI/180.0 );
	// Special case cosbeta = 0.0 must be handled!
	
	vreal sinteta = - den[4] / cosbeta;
	vreal costeta = den[0] / cosbeta;
	rot_angles.z( vp_asin ( sinteta )* 180.0/PI );
	if ( costeta < 0.0 )
		rot_angles.z( 180 - rot_angles[2] );
	
	vreal singama = -den[9] / cosbeta;
	vreal cosgama = den[10] / cosbeta;
	rot_angles.x( vp_asin ( singama ) * 180.0/PI );
	if ( cosgama < 0.0 )
		rot_angles.x( 180.0 - rot_angles[0] );

}

//#############################################################################

TransformationMatrix OGL_Frame::GetOglModelTransformMatrix () const
{
	// works only when render context is active
	vreal mtm[16];	
 	glGetDoublev ( GL_MODELVIEW_MATRIX , mtm );	
	TransformationMatrix tm;
	tm.SetElement (mtm[15],4,4);
	tm.SetElement(mtm[14],3,4);
	tm.SetElement(mtm[13],2,4);
	tm.SetElement(mtm[12],1,4);
	tm.SetElement(mtm[11],4,3);
	tm.SetElement(mtm[10],3,3);
	tm.SetElement(mtm[9],2,3);
	tm.SetElement(mtm[8],1,3);
	tm.SetElement(mtm[7],4,2);
	tm.SetElement(mtm[6],3,2);
	tm.SetElement(mtm[5],2,2);
	tm.SetElement(mtm[4],1,2);
	tm.SetElement(mtm[3],4,1);
	tm.SetElement(mtm[2],3,1);
	tm.SetElement(mtm[1],2,1);
	tm.SetElement(mtm[0],1,1);
	return tm;
}

//void OGL_Frame::GetOglModelTransformMatrix ( vreal modelMatrix[16] )
//{
//	glGetDoublev ( GL_MODELVIEW_MATRIX, modelMatrix );
//}

//void OGL_Frame::GetOglProjectionMatrix ( vreal projMatrix[16] )
//{
//	glGetDoublev ( GL_PROJECTION_MATRIX , projMatrix );
//}

//#############################################################################

TransformationMatrix OGL_Frame::GetOglProjectionMatrix () const
{
	// works only when render context is active
	vreal mtm[16];	
 	glGetDoublev ( GL_PROJECTION_MATRIX , mtm );	
	TransformationMatrix tm;
	tm.SetElement(mtm[15],4,4);
	tm.SetElement(mtm[14],3,4);
	tm.SetElement(mtm[13],2,4);
	tm.SetElement(mtm[12],1,4);
	tm.SetElement(mtm[11],4,3);
	tm.SetElement(mtm[10],3,3);
	tm.SetElement(mtm[9],2,3);
	tm.SetElement(mtm[8],1,3);
	tm.SetElement(mtm[7],4,2);
	tm.SetElement(mtm[6],3,2);
	tm.SetElement(mtm[5],2,2);
	tm.SetElement(mtm[4],1,2);
	tm.SetElement(mtm[3],4,1);
	tm.SetElement(mtm[2],3,1);
	tm.SetElement(mtm[1],2,1);
	tm.SetElement(mtm[0],1,1);
	return tm;
}

//#############################################################################
void OGL_Frame::GetOglViewport ( int viewport[4] )
{
	glGetIntegerv ( GL_VIEWPORT , viewport );
	return;
}


/*
void ViewFrame::GoToView ( VStr orientation , VPoint vmin , VPoint vmax )
{
	//for fitting screen
	
	UpdateFitRadiusForFinalView ( vmin , vmax , orientation );
	vreal ar , dx , dy;
	ar = (vreal)corner_x /(vreal)corner_y;
	if ( ar > 1.0 )
	{
		dx = fitradius*ar;
		dy = fitradius;
	}
	else
	{
		dx = fitradius;
		dy = fitradius/ar;
	}

	vreal centerx , centery;
	centerx = ( view_volume.m_left + view_volume.m_right ) / 2.0;
	centery = ( view_volume.m_top + view_volume.m_bottom ) / 2.0;
	
	vreal dxc = ( view_volume.m_right - view_volume.m_left ) / 2.0;
	vreal dyc = ( view_volume.m_top - view_volume.m_bottom ) / 2.0;
	
	//int stepno = 20;
	vreal newcenterx , newcentery , deltax , deltay;
	// The viewcenter must also travel from initial state to fitcenter.
	p_3d diff , curr_diff;
	v_sub_3d ( fitcenter , view_center , diff );
	vreal mag = v_length_3d ( diff );
	v_norm_3d ( diff , diff );
	vreal curr_mag;
	p_3d init_view_center , curr_view_center;
	copy_p_3d ( init_view_center , view_center );

	//for fitting^
	
	if ( orientation == ( IDS_Front) ) {
		adx = (90.0+rot_angles[0])/(vreal)animation_stepno;
		ady = rot_angles[1]/(vreal)animation_stepno;
		adz = rot_angles[2]/(vreal)animation_stepno;
	} else if ( orientation == ( IDS_Right) ) {
		adx = ( 90.0 + rot_angles[0] ) / (vreal)animation_stepno;
		ady =  rot_angles[1]  / (vreal)animation_stepno;
		adz = ( 90.0 + rot_angles[2] ) / (vreal)animation_stepno;
	} else if ( orientation == ( IDS_Top) ) {
		adx = rot_angles[0]/animation_stepno;
		ady = rot_angles[1]/animation_stepno;
		adz = rot_angles[2]/animation_stepno;
	}

	callbygotoview = true;

	for (m_i = 0; m_i < animation_stepno; m_i++ )
	{	
		//Setting view_center
		curr_mag = mag * (vreal)( m_i + 1 ) / (vreal)animation_stepno;
		v_scale_3d ( curr_mag , diff , curr_diff );
		v_add_3d ( init_view_center , curr_diff , curr_view_center );
		copy_p_3d ( view_center , curr_view_center );
		
		newcenterx = centerx - (vreal)( m_i + 1 ) * centerx / (vreal)animation_stepno;
		newcentery = centery - (vreal)( m_i + 1 ) * centery / (vreal)animation_stepno;
			
		deltax = dxc - (vreal)( m_i + 1 ) * ( dxc - dx ) / (vreal)animation_stepno;
		deltay = dyc - (vreal)( m_i + 1 ) * ( dyc - dy ) / (vreal)animation_stepno;

		view_volume.m_left = newcenterx - deltax;
		view_volume.m_right = newcenterx + deltax;
		view_volume.m_top = newcentery + deltay;
		view_volume.m_bottom = newcentery - deltay;
//		view_volume.m_far =  fitradius*5;
//		view_volume.m_near = -fitradius*5;	
	
		DrawScene( cdc , model_list );
	}

	callbygotoview = false;
	
	quat.x = 0.0; 
	quat.y = 0.0; 
	quat.z = 0.0;
	quat.w = 0.0;
	for ( int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			quat.m[i][j] = 0;
		}
	}
	
	if ( orientation == ( IDS_Front) ) {
		quat.EulerToQuat( 90.0*PI/180.0 , 0.0 , 0.0 );
		rot_angles[0] = -90.0;
		rot_angles[1] = 0.0;
		rot_angles[2] = 0.0;
	} else if ( orientation == ( IDS_Right) ) {
		quat.EulerToQuat( 90.0*PI/180.0 , 0.0 , 90.0*PI/180.0 );
		rot_angles[0] = -90.0;
		rot_angles[1] = 0.0;
		rot_angles[2] = -90.0;
	} else if ( orientation == ( IDS_Top) ) {
		quat.EulerToQuat(0.0 , 0.0 , 0.0 );
		rot_angles[0] = 0.0;
		rot_angles[1] = 0.0;
		rot_angles[2] = 0.0;
	}

}

//#############################################################################

void ViewFrame::UpdateFitRadiusForFinalView ( VPoint vmin , VPoint vmax , VStr orientation )
{
	// This function updates the member variable fitradius for a given orientation,
	//it calculates the projected distance of the models enclosing box center point
	//and enclosing box corner points on the screen. This distance is used for fitting the 
	//scene on screen.

	p_3d min , max;
	copy_p_3d ( min , vmin.GetArray() );
	copy_p_3d ( max , vmax.GetArray() );
	
	//p_3d final_view_vec;
	VPoint vfinal_view_vec;

	if ( orientation == ( IDS_Current) )
	{
		// Calculating the normal vector of final view.
		//p_3d origin = { 0.0 , 0.0 , 0.0 };
		VPoint vorigin ( 0.0 , 0.0 , 0.0 );
		//p_3d x_axis = { 1.0 , 0.0 , 0.0 };
		VPoint vx_axis ( 1.0 , 0.0 , 0.0 );
		//p_3d y_axis = { 0.0 , 1.0 , 0.0 };
		VPoint vy_axis ( 0.0 , 1.0 , 0.0 );
		//p_3d z_axis = { 0.0 , 0.0 , 1.0 };
		

		p_3d view_vec = { 0.0 , 0.0 , 1.0 };
		p_3d temp1 , temp2;	
		RotatePointAroundLine ( view_vec , x_axis , origin , temp1 , rot_angles[0]*PI/180.0 );
		RotatePointAroundLine ( temp1 , y_axis , origin , temp2 , rot_angles[1]*PI/180.0 );
		RotatePointAroundLine ( temp2 , z_axis , origin , final_view_vec , rot_angles[2]*PI/180.0 );
	}
	else if ( orientation == ( IDS_Top) )
	{
		final_view_vec[X] = 0.0;
		final_view_vec[Y] = 0.0;
		final_view_vec[Z] = 1.0;
	}
	else if ( orientation == ( IDS_Front) )
	{
		final_view_vec[X] = 0.0;
		final_view_vec[Y] = 1.0;
		final_view_vec[Z] = 0.0;
	}
	else if ( orientation == ( IDS_Right) )
	{
		final_view_vec[X] = 1.0;
		final_view_vec[Y] = 0.0;
		final_view_vec[Z] = 0.0;
	}

	p_3d rad_vec , rad_vec2 , center;
	// Calculating the center point of enclosing box.
	v_add_3d ( max , min , center );
	v_scale_3d ( 0.5 , center , center );

	// calculating max - center
	v_sub_3d ( max , center , rad_vec );
	p_3d low_left = { max[X] , min[Y] , min[Z] };
	v_sub_3d ( low_left , center , rad_vec2 );

	// Calculating a direction perp to view_vec
	p_3d dir1 , dir2;

	v_cross_3d ( rad_vec , final_view_vec , dir1 );
	v_norm_3d ( dir1 , dir1 );

	if ( dir1[X] < MTOL && dir1[Y] < MTOL && dir1[Z] < MTOL )
	{
		// The special case.
		// we will recalculate rad_vec
		
		p_3d temp = { max[X] , max[Y] , min[Z] };
		v_sub_3d ( temp , center , rad_vec );

		v_cross_3d ( rad_vec , final_view_vec , dir1 );
		v_norm_3d ( dir1 , dir1 );
		
	}

	v_cross_3d ( final_view_vec , dir1 , dir2 );
	v_norm_3d ( dir2 , dir2 );

	vreal px = v_dot_3d ( rad_vec , dir1 );
	vreal py = v_dot_3d ( rad_vec , dir2 );

	vreal px2 = v_dot_3d ( rad_vec2 , dir1 );
	vreal py2 = v_dot_3d ( rad_vec2 , dir2 );


	fitradius = sqrt ( px * px + py * py );
	vreal tmp = sqrt ( px2 * px2 + py2 * py2 );

	// calculating fitradius_max
	p_3d avect;
	v_sub_3d ( max , center , avect );
	fitradius_max = v_length_3d ( avect );
	
	// to prevent clipping.
	view_volume.m_far = fitradius_max*50;
	view_volume.m_near = -fitradius_max*50;	

	// updating the fitcenter
	copy_p_3d ( fitcenter , center );

	if ( fitradius < tmp )
		fitradius = tmp;

	if ( fitradius > fitradius_max )
		fitradius = fitradius_max;

	return;
}

//#############################################################################
*/

void OGL_Frame::PanView( Mouse & mouse )
{
	vreal start_x_viewvolume , start_y_viewvolume;
	vreal curr_x_viewvolume , curr_y_viewvolume;
	vreal panx , pany;

	VPoint downp = mouse.GetPrevP();
	VPoint p = mouse.GetCurrentP();

	ScreenCoordToViewVolume ( downp.x() , downp.y()
			, start_x_viewvolume , start_y_viewvolume );
		
	ScreenCoordToViewVolume ( p.x() , p.y()
								, curr_x_viewvolume , curr_y_viewvolume );
			
	panx = curr_x_viewvolume - start_x_viewvolume;
	pany = curr_y_viewvolume - start_y_viewvolume;

	view_volume.m_left -= panx;
	view_volume.m_right -= panx;
	view_volume.m_top -= pany;
	view_volume.m_bottom -= pany;
					
}

//#############################################################################

void OGL_Frame::ScreenCoordToViewVolume ( vreal x , vreal y , vreal & vx
										 , vreal & vy )const
{
	
	vreal ym = -y + view_volume.corner_y;
	
	vreal deltax , deltay;

	deltax = view_volume.m_right - view_volume.m_left;
	deltay = view_volume.m_top - view_volume.m_bottom;

	vreal dx = x * deltax / static_cast <vreal> (view_volume.corner_x);
	vreal dy = ym * deltay / static_cast <vreal> (view_volume.corner_y);

	vx = view_volume.m_left + dx;
	vy = view_volume.m_bottom + dy;
	return;
}

//#############################################################################

void OGL_Frame::ZoomToBox( Mouse &mouse )
{
	VPoint p , downp;
	p = mouse.GetCurrentP();
	downp = mouse.GetLButtonDownP();

	vreal p1x , p1y , p2x , p2y;

	ScreenCoordToViewVolume ( p.x() , p.y() , p1x , p1y );
	ScreenCoordToViewVolume ( downp.x() , downp.y() , p2x , p2y );
	
	vreal dx = fabs ( p2x - p1x );
	vreal dy = fabs ( p2y - p1y );

	vreal midx = ( p2x + p1x ) * 0.5;
	vreal midy = ( p2y + p1y ) * 0.5;

	vreal ar = (vreal)view_volume.corner_x / (vreal)view_volume.corner_y;

	if ( ar > dx / dy )
		dy = dx / ar;
	else
		dx = ar * dy;

	dx = dx * 0.5;
	dy = dy * 0.5;

	if ( dx < 1e-4 || dy < 1e-4 )
		return;

	// set view volume
	view_volume.SetRight ( midx + dx );
	view_volume.SetLeft ( midx - dx );
	view_volume.SetTop ( midy + dy );
	view_volume.SetBottom ( midy - dy );
}

//#############################################################################

void OGL_Frame::ZoomPan( Mouse &mouse , const vreal & scalef)
{
	if ( mouse.IsGoingDown())
		scale_factor = scalef;
	else
		scale_factor = (1.0 / scalef);

	
	vreal myx = mouse.GetLButtonDownP().x();
	vreal myy = mouse.GetLButtonDownP().y();

	vreal vx , vy;
	
	ScreenCoordToViewVolume ( myx , myy	, vx , vy );
	SetNewViewVolume ( myx , myy , vx , vy );
}

/*
//#############################################################################

void OGL_Frame::Rotate( Mouse & mouse )
{
	// Find rotation axis
	VPoint rot_axis;

	rot_axis = CalcRotAxis ( mouse );
	
	// Rotate with a constant increment in rot axis
	RotConst ( rot_axis );

	return;
}

//#############################################################################

VPoint OGL_Frame::CalcRotAxis ( Mouse & mouse )
{
	// Calc dx , dy
	int dx , dy;
	VPoint prevp , p;
	
	p = mouse.GetCurrentP ();
	prevp = mouse.GetLButtonDownP();//.GetPrevP();
	dx = p.x() - prevp.x();
	dy = p.y() - prevp.y();

	// rotate line
	vreal temp = dx;
	dx = -dy;
	dy = -temp;

	// norm this
	VPoint vec ( dx , dy , 0.0 );
	
	return ( vec.Unit() );

}

//#############################################################################

void OGL_Frame::RotConst ( VPoint rot_axis )
{
	vreal const_angle = 2.0;
	
	GLfloat den[16];
	
 	glGetGLfloatv ( GL_MODELVIEW_MATRIX , den );

	glMatrixMode ( GL_MODELVIEW );
	glRotated ( const_angle , rot_axis.x() , rot_axis.y() , rot_axis.z() );

	glGetGLfloatv ( GL_MODELVIEW_MATRIX , den );

}

//#############################################################################
*/

void OGL_Frame::Fit ( VPoint & min , VPoint & max )
{
	rot_center = ( max + min ) / 2.0;

	vreal dx = max.x() - min.x();
	vreal dy = max.y() - min.y();
	vreal dz = max.z() - min.z();

	vreal r = (sqrt ( dx*dx + dy*dy + dz*dz ))/2.0;

	vreal ar = (vreal)view_volume.corner_x / (vreal)view_volume.corner_y;
	
	vreal cx , cy;

	if ( ar > 1 )
	{
		cx = r * ar;
		cy = r;
	}
	else
	{
		cx = r;
		cy = r / ar;
	}

	// set view volume
	view_volume.SetRight ( cx );
	view_volume.SetLeft (-cx );
	view_volume.SetTop ( cy );
	view_volume.SetBottom ( -cy );
	view_volume.SetNear ( (-2*r));// - OGL_Frame::M_MAXWORKDIAMETER);
	view_volume.SetFar ( 2*r ); //+ OGL_Frame::M_MAXWORKDIAMETER);

	CoordinateSystem tmp (rot_center);
	m_rotationCenterCoord = tmp;
	//if (m_rotationCenterCoordDisp)
	//	m_rotationCenterCoordDisp->UpdateCoordSystemData (tmp);
	Draw();
}

void OGL_Frame::FitTo (const VPoint &min , const VPoint &max)
{
	rot_center = ( max + min ) / 2.0;

	vreal dx = max.x() - min.x();
	vreal dy = max.y() - min.y();
	vreal dz = max.z() - min.z();

	vreal r = (sqrt ( dx*dx + dy*dy + dz*dz ))/2.0;

	vreal ar = (vreal)view_volume.corner_x / (vreal)view_volume.corner_y;
	vreal cx,cy;	

	if ( ar > 1 )
	{
		cx = r * ar;
		cy = r;
	}
	else
	{
		cx = r;
		cy = r / ar;
	}

	// set view volume
	view_volume.SetRight ( cx );
	view_volume.SetLeft (-cx );
	view_volume.SetTop ( cy );
	view_volume.SetBottom ( -cy );
	//view_volume.SetNear ( (-2*r));// - OGL_Frame::M_MAXWORKDIAMETER);
	//view_volume.SetFar ( 2*r ); //+ OGL_Frame::M_MAXWORKDIAMETER);

	CoordinateSystem tmp (rot_center);
	m_rotationCenterCoord = tmp;
	//if (m_rotationCenterCoordDisp)
	//	m_rotationCenterCoordDisp->UpdateCoordSystemData (tmp);
	Draw();
}

//#############################################################################

void OGL_Frame::Top( VPoint &min , VPoint &max )
{
	quat.EulerToQuat(0.0 , 0.0 , 0.0 );
	Fit ( min , max );
}

//#############################################################################
void OGL_Frame::Iso( VPoint &min , VPoint &max )
{
	quat.EulerToQuat(45.0 * DEGREE_TO_RAD , 0, -45.0 * DEGREE_TO_RAD);
	Fit ( min , max );
}

//#############################################################################

void OGL_Frame::Bottom( VPoint &min , VPoint &max )
{
	quat.EulerToQuat(PI , 0.0 , 0.0 );
	Fit ( min , max );
}

//#############################################################################

void OGL_Frame::Right ( VPoint &min , VPoint &max )
{
	quat.EulerToQuat( 90.0*PI/180.0 , 0.0 , 90.0*PI/180.0 );
	Fit ( min , max );
}

//#############################################################################

void OGL_Frame::Left ( VPoint &min , VPoint &max )
{
	quat.EulerToQuat( PI/2. , 0.0 , -PI/2. );
	Fit ( min , max );
}

//#############################################################################

void OGL_Frame::Front ( VPoint &min , VPoint &max )
{
	quat.EulerToQuat( 90.0*PI/180.0 , 0.0 , 0.0 );
	Fit ( min , max );
}

//#############################################################################

void OGL_Frame::Back ( VPoint &min , VPoint &max )
{
	quat.EulerToQuat( -PI/2.0 , PI , 0.0 );
	Fit ( min , max );
}

//#############################################################################

void OGL_Frame::SetNewViewVolume (vreal x , vreal y , vreal vx , vreal vy)
{
	vreal ym = -y + view_volume.corner_y;
	vreal dx , dy;

	dx = view_volume.m_right - view_volume.m_left;
	dy = view_volume.m_top - view_volume.m_bottom;

	dx = dx * scale_factor;
	dy = dy * scale_factor;

	vreal newleft = vx - ( dx / view_volume.corner_x ) * x;
	vreal newbottom = vy - ( dy / view_volume.corner_y ) * ym;

	view_volume.m_left = newleft;
	view_volume.m_bottom = newbottom;
	view_volume.m_right = newleft + dx;
	view_volume.m_top = newbottom + dy;
	//m_liftmag=view_volume.GetDiaogonalRadius()*0.01;

	return;

}

//#############################################################################          

void OGL_Frame::DoSelection ( Mouse & mouse , bool selectwithbox , bool verysmallhitrad )
{
	//ViewVolume selectionvolume;

	if ( selectwithbox )
	{
		VPoint p , downp;
		p = mouse.GetCurrentP();
		downp = mouse.GetLButtonDownP();
		vreal p1x , p1y , p2x , p2y;

		ScreenCoordToViewVolume ( p.x() , p.y() , p1x , p1y );
		ScreenCoordToViewVolume ( downp.x() , downp.y() , p2x , p2y );
		
		vreal dx = fabs ( p2x - p1x );
		vreal dy = fabs ( p2y - p1y );

		vreal midx = ( p2x + p1x ) / 2.0;
		vreal midy = ( p2y + p1y ) / 2.0;

		vreal ar = (vreal)view_volume.corner_x / (vreal)view_volume.corner_y;

		if ( ar > dx / dy )
			dy = dx / ar;
		else
			dx = ar * dy;

		dx = dx / 2.0;
		dy = dy / 2.0;

		if ( dx < 1e-4 || dy < 1e-4 )
			return;

		// set view volume
		selectionvolume.SetRight ( midx + dx );
		selectionvolume.SetLeft ( midx - dx );
		selectionvolume.SetTop ( midy + dy );
		selectionvolume.SetBottom ( midy - dy );
		selectionvolume.SetFar ( view_volume.m_far );
		selectionvolume.SetNear ( view_volume.m_near );

	}
	else
	{
		// set A NEW view volume by using mouse left button down point
		vreal scrx = mouse.GetLButtonDownP().x();
		vreal scry = mouse.GetLButtonDownP().y();

		vreal volx , voly;
		ScreenCoordToViewVolume ( scrx , scry , volx , voly );

		// create an hitradius object by screen cornerx and cornery
		//hitr.CalcDiagonal ( view_volume.corner_x , view_volume.corner_y );
		//hitr.CalcR();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		//vreal offs;

		if ( verysmallhitrad )
			hitrad = VGlobalValues::viewpointdefaults.m_verySmallHitRadius;
		else
			hitrad = selection.GetHitRadiusOnVolume();

	  	// a new view volume
		selectionvolume.SetLeft ( volx - hitrad );
		selectionvolume.SetRight ( volx + hitrad );	
		selectionvolume.SetBottom ( voly - hitrad );
		selectionvolume.SetTop ( voly + hitrad );
		selectionvolume.SetFar ( view_volume.m_far );
		selectionvolume.SetNear ( view_volume.m_near );
	}

	// set this new view volume by opengl commands
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();		//Initialize matrix

	selectionvolume.SetViewVolume ();

	// Current matrix will be model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();      //Initialize matrix	
	
	
	glMultMatrixd(quat.QuatToMat()); 
	// Making view center the origin
	glTranslated ( -rot_center.x() , -rot_center.y() , -rot_center.z() );
	
	
}

//#############################################################################

void OGL_Frame::SetRotationCenter ( VPoint newcenter )
{
	// increase far near distance by change of rot center
	VPoint deltarot = rot_center - newcenter;
	vreal increase = deltarot.GetLength();

	rot_center = newcenter;
	// view_volume should be updated
	vreal dx = ( view_volume.m_right - view_volume.m_left ) / 2.0;
	vreal dy = ( view_volume.m_top - view_volume.m_bottom ) / 2.0;
	
	view_volume.SetLeft ( - dx );
	view_volume.SetRight ( dx );
	view_volume.SetBottom ( - dy );
	view_volume.SetTop ( dy );
	view_volume.SetFar ( view_volume.m_far + increase );
	view_volume.SetNear ( view_volume.m_near - increase );

	//if ( navigationmode == false )
	view_volume.SetViewVolume();
	UpdateRotCenterCoordinate ();

}

//#############################################################################

VPoint OGL_Frame::GetLookDir ()
{
	vreal lookDir[3];
	quat.GetLookDir(lookDir);
	VPoint res(lookDir);
	return res;
}

//#############################################################################

CoordinateSystem OGL_Frame::GetLookCoordinateSystem()
{
	CoordinateSystem tmp=quat.GetLookCoordinateSystem();
	VPoint dirx=tmp.GetDirx();
	VPoint diry=tmp.GetDiry();
	VPoint dirz=tmp.GetDirz();
	dirx.NormThis();
	diry.NormThis();
	dirz.NormThis();
	CoordinateSystem res (rot_center,dirx,diry,dirz);
	return res;
}

//#############################################################################

void OGL_Frame::LiftWireFrame ()
{
	//// Cancel lifting
	//if ( viewingstate.IsPerspectiveNavigate() )
	//	return;

	lift = GetLookDir();
	lift = lift*m_liftmag;

	glTranslated ( lift.x() , lift.y() , lift.z() );
}

//#############################################################################

void OGL_Frame::Unlift ()
{
	//if ( viewingstate.IsPerspectiveNavigate() )
	//	return;
	glTranslated ( -lift.x() , -lift.y() , -lift.z() );
}

//#############################################################################

void OGL_Frame::DrawVector ( VLine &vect )
{
	// Decide for the scalefactor
	vreal scalefactor = GetScaleFactorForWindowPortion (12);
	// basepoint
	VPoint bp=vect.GetP1();

	//LiftWireFrame ();
	myshapes.DrawVector ( vect.GetV() , bp , scalefactor );
	//Unlift();
}

//#############################################################################

void OGL_Frame::DrawAllSelectedVectors ( LineSet * myset )
{
	for ( long i = 0; i < myset->Size(); i++ )
	{
		DrawVector ( *((*myset)[i]) );
	}
}

//#############################################################################

void OGL_Frame::LiftPointMax ( VPoint poi )
{
	lift = GetLookDir().Unit();

	VPoint diff = poi - rot_center;

	vreal height = diff % lift;

	vreal maxval = (view_volume.m_far - view_volume.m_near)/2.0;
	vreal liftscale = (maxval*0.98 - height);

	lift = lift * liftscale;

	glTranslated ( lift.x() , lift.y() , lift.z() );
}

//#############################################################################

vreal OGL_Frame::GetScaleFactorForWindowPortion ( vreal windowportion )
{
	vreal diagonal = view_volume.corner_x * view_volume.corner_x 
						+ view_volume.corner_y * view_volume.corner_y;
	diagonal = sqrt ( diagonal );

	diagonal = diagonal / windowportion;

	vreal vorx , vory , vx , vy;

	ScreenCoordToViewVolume ( 0.0 , 0.0 , vorx , vory );
	ScreenCoordToViewVolume ( diagonal , 0.0 , vx , vy );

	vreal scalefactor = (vx - vorx);

	return scalefactor;
}

//#############################################################################

void OGL_Frame::ChangeViewDirection ( VPoint dir )
{
	// Make orientation top first
	quat.EulerToQuat(0.0 , 0.0 , 0.0 );

	VPoint zdir ( 0.0 , 0.0 , 1.0 );
	VPoint ydir ( 0.0 , 1.0 , 0.0 );
	VPoint xdir ( 1.0 , 0.0 , 0.0 );
	VPoint temp ( 0.0 , dir.y() , dir.z() );

	VPoint newv;
	
	if ( fabs ( temp%temp ) > EPSILON )
	{
		vreal rad_angle = gmVectorUtility::GetAngleBetweenVectors( temp , zdir );
		// decide the sign 
		VPoint anxdir = temp ^ zdir;
		if ( anxdir.x() < 0.0 )
			rad_angle = rad_angle * -1.0;

		// first rotation
		Quat q1;
		q1.EulerToQuat ( -rad_angle , 0.0 , 0.0 );
		quat.Multip(q1);
		//now the direction is in x-z plane
		
		// find new vector
		vreal newvect[3];
		vreal origin[3] = {0.0 , 0.0 , 0.0 };
		gmVectorUtility::RotatePointAroundLine 
			( dir.GetArray() , xdir.GetArray() , origin , newvect , rad_angle );
		
		newv.x( newvect[0] ); newv.y(newvect[1]); newv.z(newvect[2] );
	}
	else
		newv = dir;
	

	vreal newangle = gmVectorUtility::GetAngleBetweenVectors ( newv , zdir );
	 // decide the sign
	 VPoint dec = newv ^ zdir;

	 if ( dec.y() < 0.0 )
		 newangle = newangle * -1.0;

	 //second rotation
	 Quat q2;
	 q2.EulerToQuat ( 0.0 , -newangle , 0.0 );
	 quat.Multip(q2);

	 //tmp.RotatePointAroundLine 
	//	( newv.GetArray() , ydir.GetArray() , origin , newvect , newangle * PI / 180.0 );
}

//#############################################################################

void OGL_Frame::DrawPlanes()
{
//	if ( viewingstate.IsClipped() )
//		myshapes.DrawPlane ( clipplane , view_volume );

/*
	VPoint basepoint ( 10.0 , 100.0 , 0.0 );
	VPoint normal ( 0.0 , 1.0 , 0.0 );
	static bool onetime = false;
	
	if ( onetime == false )
	{
		myshapes.CreateTriOfPlane ( basepoint , normal , 100.0 );
		onetime = true;
	}

	VisualAttributes inst;
	vreal newcol[3] = { 1.0 , 0.0 , 0.0 };
	inst.ChangeShadeColor ( newcol );

	glDisable ( GL_LIGHTING );
	inst.SetShadeColor( false );
	glEnable ( GL_POLYGON_STIPPLE );
	glPolygonStipple ( halftone );
	myshapes.planetri.Draw();
	glDisable ( GL_POLYGON_STIPPLE );
*/
}

//#############################################################################

void OGL_Frame::SetNavigationMode ()
{
	//navigationmode = newval;

	if ( viewingstate.IsPerspectiveNavigate() )
	{
		perspective.SetPerspective();
	}

}

//#############################################################################

void OGL_Frame::RotateNavigateCamera ( Mouse& mouse )
{
	VPoint mydelta = mouse.GetDeltaP_Current_Prev();
	if ( fabs(mydelta.x()) > fabs(mydelta.y()) )
	{
		if ( mydelta.x() > 0.0 )
			m_camera.StepTiltAroundZNegative();
		else
			m_camera.StepTiltAroundZPositive();
	}
	else
	{
		if ( mydelta.y() > 0.0 )
			m_camera.StepHeadUpDownNegative();
		else
			m_camera.StepHeadUpDownPositive();
	}

}

//#############################################################################

void OGL_Frame::MoveNavigateCamera ( Camera::movedirection movedir )
{
	switch ( movedir )
	{
	case ( Camera::GOFORWARD ):
		m_camera.WalkForward();
		break;
	case ( Camera::GOBACKWARD ):
		m_camera.WalkBackward();
		break;
	case ( Camera::GORIGHT ):
		m_camera.WalkRight();
		break;
	case ( Camera::GOLEFT ):
		m_camera.WalkLeft();
		break;	
	case ( Camera::GOUP ):
		m_camera.FlyUp();
		break;
	case ( Camera::GODOWN ):
		m_camera.FlyDown();
		break;
	}
}

//#############################################################################



//#############################################################################

void OGL_Frame::DoPointSelection 
( Mouse &mouse , bool doSketchPoint , const ModelList & modellist , DoubleList & selpoints )
{
	VPoint result; 
	PointSet selset(200);
	DoubleVec depthvalues;	
	

	//const HandleSet &hs=m_transformations->GetUntransformed();
	const std::vector<VTransformable *> &hs = m_transformations->GetUntransformed();
	VPoint screenp = FreeClickToModelPoint (mouse);
	VVector lookdir = GetLookDir ();
	lookdir *= -1.;
	
	// for grid first
	vreal hitrad = selection.GetHitRadiusOnVolume();
	m_gridDraw->GetHitPointsNew (selset,depthvalues,screenp , lookdir,hitrad);
	
	// for untransformed
	selection.DoPointSelection 
		( mouse , false , modellist , hs , selset , depthvalues , screenp , lookdir);
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator i = m_transformations->GetAllMat().begin();
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator e = m_transformations->GetAllMat().end();
	for (i;i!=e;++i)
	{
		SetModelViewMatrix ((*i)->m_tm.m_om);
		PointSet pointset(2);
		selection.DoPointSelection (mouse,false,modellist,(*i)->m_mvec,pointset,depthvalues,screenp , lookdir);
		//pointset.Transform (i->m_tm.m_tm);
		selset.AddBack (pointset);
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	//selection.MakeSelectionVolumeHalf();
	bool ck =  selection.GetTopPoint(selset,depthvalues,result);

	
	if ( ck )
		selpoints.AddPoint3D ( result.GetArray() );
	
	if (ck ==false && doSketchPoint)
	{
		selpoints.AddPoint3D ( screenp.GetArray() );
	}

}

void OGL_Frame::DoVectorSelection 
( Mouse &mouse , bool selectwithbox , const ModelList & modellist , DoubleList & selpoints )
{
	VPoint basepoint,topvector; 
	PointSet selset(200);
	DoubleVec depthvalues;
	//const HandleSet &hs=m_transformations->GetUntransformed();
	const std::vector<VTransformable *> &hs = m_transformations->GetUntransformed();
	selection.DoVectorSelection ( mouse , selectwithbox , modellist , hs , selset , depthvalues );
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator i=m_transformations->GetAllMat().begin();
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator e=m_transformations->GetAllMat().end();
	for (i;i!=e;++i)
	{
		SetModelViewMatrix ((*i)->m_tm.m_om);
		PointSet temp (2);
		selection.DoVectorSelection (mouse,selectwithbox,modellist,(*i)->m_mvec,temp,depthvalues);
		temp.Transform ((*i)->m_tm.m_tm);
		selset.AddBack(temp);
		glMatrixMode (GL_MODELVIEW);
		glPopMatrix();
	}
	bool ck = selection.GetTopVector (selset , depthvalues , basepoint , topvector );
	if ( ck )
	{
		selpoints.AddPoint3D ( basepoint.GetArray() );
		selpoints.AddPoint3D ( topvector.GetArray() );
	}
	
}

void OGL_Frame::DoEdgeSelection 
( Mouse & mouse , const ModelList & modellist , DoubleList & selpoints )
{
	VPoint basepoint,topvector; 
	LineSet sellines(20);
	DoubleVec depthvalues;
	
	//const HandleSet &hs = m_transformations->GetUntransformed();
	const std::vector<VTransformable *> &hs = m_transformations->GetUntransformed();
	selection.DoEdgeSelection 
		( mouse , modellist , hs , sellines , depthvalues );
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator i=m_transformations->GetAllMat().begin();
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator e=m_transformations->GetAllMat().end();
	for (i;i!=e;++i)
	{
		SetModelViewMatrix ((*i)->m_tm.m_om);
		LineSet temp (10);
		selection.DoEdgeSelection (mouse,modellist,(*i)->m_mvec,temp,depthvalues);
		temp.Transform ((*i)->m_tm.m_tm);
		sellines.AddBack (temp);
		glMatrixMode (GL_MODELVIEW);
		glPopMatrix();
	}

	bool ck = selection.GetTopLine (sellines,depthvalues,basepoint,topvector);
	if ( ck )
	{
		VPoint secp = basepoint + topvector;
		selpoints.AddPoint3D ( basepoint.GetArray() );
		selpoints.AddPoint3D ( secp.GetArray() );
	}
	
}

Model * OGL_Frame::DoModelSelection ( Mouse &mouse , bool deselect , SelectionFlags & selflags , const SelectableTypes & filter , const ModelList & modellist , bool ctrlActive , bool disableBoxSel , Model **pSelModel)
{
	m_oneDoSelectionResult.clear();
	VPoint delta = mouse.GetLButtonDownP() - mouse.GetLButtonUpP();
		
	vreal hitr = selection.GetHitRadiusOnWindow();
	
	if ( delta.GetLength2() > hitr*hitr )
	{
		if (disableBoxSel)
		{
			if (pSelModel != 0)
				*pSelModel = 0;
			return 0;
		}
		else
			selflags.SetSelectWithBox (true);
	}
	else
		selflags.SetSelectWithBox(false);

	Model *selmodel(0);
	vreal setdepth = MAX_NUMBER_LIMIT;
	std::map<Model *,double> selModelMap;
	
	const std::vector<VTransformable *> &hs = m_transformations->GetUntransformed();
	//std::vector<VTransformable  *> hs2;
	//CollectModelSelectionPotentialHits(hs2,mouse);
	bool curveChainingActive = false;
	
	if (ctrlActive && filter.GetChainingCurvesDisabledFlag() == false)
	//if (ctrlActive && filter.Size() == 1)
	{
		if (filter.IsTypeSelectable (ViewPointItemTypes::m_curvetype))
		{
			curveChainingActive = true;
		}
	}

	int filSize = filter.Size();
	for (int fili = 0;fili < filSize;++fili)
	{
		SelectableTypes subFilter;
		subFilter.SetColorFilter(filter.GetColorFilter());
		subFilter.SetSelectableColor(filter.GetSelectableColor());
		subFilter.AddSelectableType (filter.operator [] (fili));
		selection.DoModelSelection 
			( mouse , deselect , selflags , subFilter , modellist , hs , selModelMap , curveChainingActive);
	}
	// Select transformed
	const std::vector<OperationTransformList::Mat_Mod *> &allmat = m_transformations->GetAllMat();
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator i = allmat.begin();
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator e = allmat.end();
	for (i;i!=e;++i)
	{
		// Set ogl modelview matrix
		
		//SetModelViewMatrix ((*i)->m_tm.m_om);
		const std::vector<VTransformable *> &hs = (*i)->m_mvec;
		int filSize = filter.Size();
		for (int fili = 0;fili < filSize;++fili)
		{
			SelectableTypes subFilter;
			subFilter.SetColorFilter(filter.GetColorFilter());
			subFilter.SetSelectableColor(filter.GetSelectableColor());
			subFilter.AddSelectableType (filter.operator [](fili));
			selection.DoModelSelection 
			( mouse , deselect , selflags , subFilter , modellist , hs , selModelMap , curveChainingActive);
		}
		//glMatrixMode(GL_MODELVIEW);
		//glPopMatrix();
	}

	if (selflags.GetSelectWithBox())
	{
		if (pSelModel != 0)
			*pSelModel = 0;
		std::map<Model *,double>::iterator mi = selModelMap.begin();
		std::map<Model *,double>::iterator me = selModelMap.end();
		for (mi;mi!=me;++mi)
		{
			mi->first->SetSelected (!deselect);
			if ( !deselect ) 
			{
				mi->first->SetSelectionTime(selection.GetSelectionTime());
				selection.IncrementSelectionTime();
				selflags.IncSelModelNoByOne();
				//m_oneDoSelectionResult.push_back (mi->first);
			}
			else
			{
				mi->first->SetSelectionTime(-1);
				selflags.DecSelModelNoByOne();
			}
		}
	}
	else
	{
		// iterate through the map and find the smallest depth for top most model selected
	
		std::map<Model*,double>::iterator mi = selModelMap.begin();
		std::map<Model*,double>::iterator me = selModelMap.end();
		for (mi;mi!=me;++mi)
		{
			if (mi->second < setdepth)
			{
				setdepth = mi->second;
				selmodel = mi->first;
			}
		}
		if (pSelModel != 0)
			*pSelModel = selmodel;
		if ( selmodel != 0 )
		{
			selmodel->SetSelected(!deselect);
			if ( !deselect )
			{
				selmodel->SetSelectionTime(selection.GetSelectionTime());
				selection.IncrementSelectionTime();
				selflags.IncSelModelNoByOne();
			}
			else
			{
				selmodel->SetSelectionTime(-1);
				selflags.DecSelModelNoByOne();
			}
			m_oneDoSelectionResult.push_back (selmodel);	
		}
	}


	return selmodel;
}

void OGL_Frame::QuickModelSelection(std::vector<VTransformable *> &pothits,Mouse &mouse)
{
	VPoint trp = FreeClickToModelPoint(mouse);
	VPoint viewDir = GetLookDir();
	VLine hitline(trp, trp + viewDir);
	//mouse.SetLButtonUpP(mouse.GetLButtonDownP());
	m_voxel.CollectVTransformable(hitline, pothits);
}

Model * OGL_Frame::DoModelSelection2(Mouse &mouse, bool deselect, SelectionFlags & selflags, const SelectableTypes &filter, const ModelList &modellist, bool ctrlActive, bool disableBoxSel, Model ** pSelModel)
{
	m_oneDoSelectionResult.clear();
	VPoint delta = mouse.GetLButtonDownP() - mouse.GetLButtonUpP();

	vreal hitr = selection.GetHitRadiusOnWindow();

	if (delta.GetLength2() > hitr*hitr)
	{
		if (disableBoxSel)
		{
			if (pSelModel != 0)
				*pSelModel = 0;
			return 0;
		}
		else
			selflags.SetSelectWithBox(true);
	}
	else
		selflags.SetSelectWithBox(false);

	Model *selmodel(0);
	vreal setdepth = MAX_NUMBER_LIMIT;
	std::multimap<vreal,Model *> depthModelMap;

	//const std::vector<VTransformable *> &hs = m_transformations->GetUntransformed();
	//std::vector<VTransformable *> hs;
	//QuickModelSelection(hs, mouse);
	//std::vector<VTransformable  *> hs2;
	//CollectModelSelectionPotentialHits(hs2,mouse);
	bool curveChainingActive = false;

	if (ctrlActive && filter.GetChainingCurvesDisabledFlag() == false)
		//if (ctrlActive && filter.Size() == 1)
	{
		if (filter.IsTypeSelectable(ViewPointItemTypes::m_curvetype))
		{
			curveChainingActive = true;
		}
	}

	int filSize = filter.Size();
	for (int fili = 0; fili < filSize; ++fili)
	{
		SelectableTypes subFilter;
		subFilter.SetColorFilter(filter.GetColorFilter());
		subFilter.SetSelectableColor(filter.GetSelectableColor());
		subFilter.AddSelectableType(filter.operator [] (fili));
		selection.DoModelSelection2
			(mouse, deselect, selflags, subFilter, depthModelMap, curveChainingActive);
	}
	// Select transformed
	//const std::vector<OperationTransformList::Mat_Mod *> &allmat = m_transformations->GetAllMat();
	//std::vector<OperationTransformList::Mat_Mod *>::const_iterator i = allmat.begin();
	//std::vector<OperationTransformList::Mat_Mod *>::const_iterator e = allmat.end();
	//for (i; i != e; ++i)
	//{
	//	// Set ogl modelview matrix

	//	//SetModelViewMatrix ((*i)->m_tm.m_om);
	//	const std::vector<VTransformable *> &hs = (*i)->m_mvec;
	//	int filSize = filter.Size();
	//	for (int fili = 0; fili < filSize; ++fili)
	//	{
	//		SelectableTypes subFilter;
	//		subFilter.SetColorFilter(filter.GetColorFilter());
	//		subFilter.SetSelectableColor(filter.GetSelectableColor());
	//		subFilter.AddSelectableType(filter.operator [](fili));
	//		selection.DoModelSelection
	//			(mouse, deselect, selflags, subFilter, modellist, hs, selModelMap, curveChainingActive);
	//	}
	//	//glMatrixMode(GL_MODELVIEW);
	//	//glPopMatrix();
	//}

	if (selflags.GetSelectWithBox())
	{
		if (pSelModel != 0)
			*pSelModel = 0;
		std::multimap<vreal,Model *>::iterator mi = depthModelMap.begin();
		std::multimap<vreal,Model *>::iterator me = depthModelMap.end();
		for (mi; mi != me; ++mi)
		{
			mi->second->SetSelected(!deselect);
			if (!deselect)
			{
				mi->second->SetSelectionTime(selection.GetSelectionTime());
				selection.IncrementSelectionTime();
				selflags.IncSelModelNoByOne();
				//m_oneDoSelectionResult.push_back (mi->first);
			}
			else
			{
				mi->second->SetSelectionTime(-1);
				selflags.DecSelModelNoByOne();
			}
		}
	}
	else
	{
		// iterate through the map and find the smallest depth for top most model selected

		//std::map<Model*, double>::iterator mi = selModelMap.begin();
		//std::map<Model*, double>::iterator me = selModelMap.end();
		//for (mi; mi != me; ++mi)
		//{
		//	if (mi->second < setdepth)
		//	{
		//		setdepth = mi->second;
		//		selmodel = mi->first;
		//	}
		//}
		std::multimap<vreal, Model*>::iterator dmi = depthModelMap.begin();

		if (dmi != depthModelMap.end())
		{
			setdepth = dmi->first;
			selmodel = dmi->second;
		}
		if (pSelModel != 0)
			*pSelModel = selmodel;
		if (selmodel != 0)
		{
			selmodel->SetSelected(!deselect);
			if (!deselect)
			{
				selmodel->SetSelectionTime(selection.GetSelectionTime());
				selection.IncrementSelectionTime();
				selflags.IncSelModelNoByOne();
			}
			else
			{
				selmodel->SetSelectionTime(-1);
				selflags.DecSelModelNoByOne();
			}
			m_oneDoSelectionResult.push_back(selmodel);
		}
	}

	return selmodel;
}

void OGL_Frame::EndModelSelection ( ModelList & modellist , HandleSet * handles )
{
	if ( handles )
		selection.EndModelSelection ( modellist , handles );
	else
		selection.EndModelSelection ( modellist );
}
	
void OGL_Frame::DrawCoordSystems ( ModelList & modellist , const std::vector<VTransformable *> &hs) const
{
	
	glDisable ( GL_DEPTH_TEST );	
	glDisable ( GL_CLIP_PLANE1 );	
	
	//DrawViewCoordinateSystem ();

	Model * pmodel ;
	int mysize = (int)hs.size();

	for (int i=0;i<mysize;++i)
	{
		//const ModelID &id = hs.GetID(i);
		//pmodel = modellist.GetModel(id);
		VTransformable *curtr = hs.at(i);
		pmodel = curtr->GetTrModel();
		if (pmodel == 0)
			continue;
		CoordSystemDisplay * cd = pmodel->GetCoordSystemDisplay();
		bool forcedrawing = false;
		if (cd == 0)
			continue;
		//if ( pmodel->GetDisplayList().IsDefined() == false
		//	&& pmodel->GetWireDisplayList().IsDefined() == false 
		//	&& pmodel->GetPointsDisplayList().IsDefined() == false )
		//		forcedrawing = true;
		if ( pmodel->GetDisplay().IsDefined() == false)
			forcedrawing = true;
		if (pmodel->IsVisible()&&pmodel->GetDeleted()==false)
		{
			if ( cd->GetVisualAtt().GetDraw() || forcedrawing )
			{
				visualattloader.Load ( cd->GetVisualAtt() 
										, false//pmodel->IsSelected() 
										, selectioncolor );
			
				cd->Draw ( view_volume , &m_asciiDisp);
			}
		}
		
		//pmodel = modellist.Iterate();
	}
	
	glEnable ( GL_DEPTH_TEST );
}

void OGL_Frame::SetColorArray ( const VColor & colupleft 
							   , const VColor & colbottomleft 
							   , const VColor & colbottomright
							   , const VColor & colupright )
{
	color1_up_left = colupleft;
	color2_bottom_left	= colbottomleft;
	color3_bottom_right = colbottomright;
	color4_up_right		= colupright;
	hiddenlinecolor = VGlobalValues::viewpointdefaults.default_colupleft;
}
void OGL_Frame::DrawSnapPoints ()
{
	const std::vector<VTransformable *> &hs = m_transformations->GetUntransformed();
	DrawSnapPoints (*m_modellist,hs);
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator i=m_transformations->GetAllMat().begin();
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator e=m_transformations->GetAllMat().end();
	for(i;i!=e;++i)
	{
		if ((*i)->m_isOpen)
		{
			SetModelViewMatrix ((*i)->m_tm.m_om);
			//DrawSnapPoints (*m_modellist,(*i)->m_mvec);
			
			Model * currmodel;
			int mysize = (int)(*i)->m_mvec.size();
			for (int j = 0; j<mysize; ++j)
			{
				VTransformable *curtr = (*i)->m_mvec.at(j);
				currmodel = curtr->GetTrModel();//modellist.GetModel(hs.GetID(i));
				if (currmodel->GetDeleted() == true || currmodel->IsVisible() == false)
					continue;
				currmodel->DrawSnapPoints();
			}
			
			
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}
}
void OGL_Frame::DrawSnapLines()
{
	const std::vector<VTransformable *> &hs = m_transformations->GetUntransformed();
	DrawSnapLines (*m_modellist,hs);
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator i=m_transformations->GetAllMat().begin();
	std::vector<OperationTransformList::Mat_Mod *>::const_iterator e=m_transformations->GetAllMat().end();
	for(i;i!=e;++i)
	{
		if ((*i)->m_isOpen)
		{
			SetModelViewMatrix ((*i)->m_tm.m_om);
			DrawSnapLines (*m_modellist,(*i)->m_mvec);
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	}
}
void OGL_Frame::DrawSnapPoints( const ModelList & modellist , const std::vector<VTransformable *> &hs)
{
	glEnable ( GL_CLIP_PLANE1 );
	//LiftWireFrame();
	Model * currmodel;
	int mysize=(int)hs.size();
	for ( int i=0;i<mysize;++i )
	{
		VTransformable *curtr = hs.at(i);
		currmodel = curtr->GetTrModel();//modellist.GetModel(hs.GetID(i));
		if (currmodel->GetDeleted() ==true || currmodel->IsVisible() == false)
			continue;
		currmodel->DrawSnapPoints();
	}
	//Unlift();
	int mysize2 = (int)m_oneDoSelectionResult.size();
	for (int j = 0; j < mysize2; ++j)
	{
		Model *curM = m_oneDoSelectionResult.at(j);
		if (curM->GetDeleted() == true || curM->IsVisible() == false)
			continue;
		curM->DrawSnapPoints();
	}
}
void OGL_Frame::DrawSnapLines( const ModelList & modellist , const std::vector<VTransformable *> &hs)
{
	glEnable ( GL_CLIP_PLANE1 );
	//LiftWireFrame();
	Model * currmodel;
	int mysize=(int)hs.size();
	for ( int i=0;i<mysize;++i )
	{
		VTransformable *curtr = hs.at(i);
		currmodel = curtr->GetTrModel();//modellist.GetModel(hs.GetID(i));
		if (currmodel->GetDeleted() ==true || currmodel->IsVisible() == false)
			continue;
		currmodel->DrawSnapLines();
	}
	//Unlift();
}
//void OGL_Frame::DrawFontDisp (const ModelList & modellist , const std::vector<VTransformable *> &hs)
//{
//	glEnable ( GL_CLIP_PLANE1 );
//	glDisable(GL_LIGHTING);
//	//LiftWireFrame();
//	Model * currmodel;
//	int mysize=(int)hs.size();
//	for ( int i = 0; i <mysize; ++i)
//	{
//		//const ModelID &id = hs.GetID(i);
//		VTransformable *curtr = hs.at(i);
//		currmodel = curtr->GetTrModel();//modellist.GetModel(id);
//		if(currmodel->IsVisible() && currmodel->GetDeleted() == false)
//		{			
//			FontDisp *f = currmodel->GetFontDisp();
//			if (f != 0)
//			visualattloader.Load ( f->GetVisualAtt() 
//									, currmodel->IsSelected() 
//									, selectioncolor );
//			while (f != 0)
//			{
//				f->Draw();
//				f=f->next;
//			}
//		}
//	}
//	//Unlift();
//	//glEnable (GL_LIGHTING);
//}

void OGL_Frame::ClipIf ()
{
	glDisable ( GL_CLIP_PLANE1 );
	
	if ( viewingstate.IsClipped () )
	{
		// Draw plane
		if ( viewingstate.DrawClipPlane() )
			myshapes.DrawPlane ( clipplane , view_volume );

		glEnable ( GL_CLIP_PLANE1 );
		vreal peqn[4] = { clipplane.normal.x()
						, clipplane.normal.y()	
						, clipplane.normal.z()
						, -(clipplane.normal%clipplane.pointonplane) };
	
		glClipPlane ( GL_CLIP_PLANE1 , peqn );

	}
}

void OGL_Frame::DrawDraftEntities ( DraftEntityVec & draftvec , const std::vector<VTransformable *> &hs) const
{
	int mysize = draftvec.Size();
	for ( int i=0; i < mysize; i++ )
	{	
		DraftEntity * currdraft = draftvec[i];
		if ( currdraft->IsVisible() && currdraft->GetDeleted()==false)
		{
			visualattloader.Load (currdraft->GetLineVisualAtt() , currdraft->IsSelected() , selectioncolor);
			currdraft->Draw();
		}
	}
}

VPoint OGL_Frame::GetSketchPointOnScreen(const vreal xyz[])
{
	vreal projMatrix[16];
	vreal modelMatrix[16];
	int viewPort[4];
	glGetDoublev ( GL_MODELVIEW_MATRIX , modelMatrix );
	glGetDoublev ( GL_PROJECTION_MATRIX , projMatrix );
	glGetIntegerv ( GL_VIEWPORT , viewPort );
		
	vreal result[3];
	gluUnProject ( xyz[0] , xyz[1] , xyz[2] , modelMatrix , projMatrix , viewPort , 
					&(result[0]) , &(result[1]) , &(result[2]) );
			
	VPoint sketchp(result);
	return sketchp;
}

VPoint OGL_Frame::GetAPointOnPrincipalPlanes(VLine &l)
{
	VPoint o(0,0,0);
	VPoint dx(1,0,0);
	VPoint dy(0,1,0);
	VPoint dz(0,0,1);
	VPoint diff=o-l.GetP1();
	vreal d1=(diff%dx)/(l.GetV()%dx);
	vreal d2=(diff%dy)/(l.GetV()%dy);
	vreal d3=(diff%dz)/(l.GetV()%dz);

	VPoint p;
	if (d1<d2 && d1<d3)
	{
		p=l.GetP1()+(d1*l.GetV());
	}
	else if (d2<d1 && d2<d3)
	{
		p=l.GetP1()+(d2*l.GetV());
	}
	else if (d3<d2 && d3<d1)
	{
		p=l.GetP1()+(d3*l.GetV());
	}	

	return p;
}

void OGL_Frame::DrawViewCoordinateSystem ()const
{	
	visualattloader.Load (m_rotationCenterVisatt , false , selectioncolor);
	if (m_rotationCenterCoordDisp)
		m_rotationCenterCoordDisp->Draw(view_volume , &m_asciiDisp);
}

VLine OGL_Frame::ScreenPointToModelSpaceLine (Mouse &mouse)
{
	vreal scrx = mouse.GetLButtonDownP().x();
	vreal scry = mouse.GetLButtonDownP().y();
	vreal volx , voly;
	view_volume.ScreenCoordToViewVolume ( scrx , scry , volx , voly , rot_center.x() , rot_center.y());
	CoordinateSystem cs = GetLookCoordinateSystem();
	VPoint tmp(volx,voly,0.);
	VPoint tmp1 (volx,voly,1.);
	VPoint l1 = cs.RelativeCoordToAbsoluteCoord(tmp);
	VPoint l2 = cs.RelativeCoordToAbsoluteCoord(tmp1);
	VLine myline (l1,l2);
	return myline;
}

VPoint OGL_Frame::FreeClickToModelPoint (Mouse &mouse)
{
	vreal scrx = mouse.GetLButtonDownP().x();
	vreal scry = mouse.GetLButtonDownP().y();
	vreal volx , voly;
	view_volume.ScreenCoordToViewVolume ( scrx , scry , volx , voly ,rot_center.x() , rot_center.y());
	CoordinateSystem cs = GetLookCoordinateSystem();
	
	VPoint tmp(volx,voly,0.);
	VPoint tmp1 (volx,voly,1.);
	VPoint l1 = cs.RelativeCoordToAbsoluteCoord(tmp);
	VPoint l2 = cs.RelativeCoordToAbsoluteCoord(tmp1);
	VLine myline (l1,l2);
	VPlane myplane (m_workingCoordSys.GetOrigin() , m_workingCoordSys.GetDirz());
	double t;
	VPlane::IntType ck = myplane.LineInt(myline , t);
	VPoint res;
	if (ck == VPlane::REGULAR)
		res = myline.GetP(t);
	else
		res = cs.RelativeCoordToAbsoluteCoord (tmp);
	//CString s;s.Format (("%f %f %f"),cs.GetDirz().x(),cs.GetDirz().y(),cs.GetDirz().z());VpAfxMessageBox(s);
	return res;
}
VPoint OGL_Frame::FreeClickToModelPoint(int screenx , int screeny , const VPlane &prjPlane)
{
	vreal scrx = screenx;
	vreal scry = screeny;
	vreal volx,voly;
	view_volume.ScreenCoordToViewVolume(scrx , scry , volx , voly,rot_center.x() , rot_center.y());
	CoordinateSystem cs = GetLookCoordinateSystem();
	VPoint tmp (volx , voly ,0.0);
	VPoint tmp1 (volx , voly , 1.0);
	VPoint l1 = cs.RelativeCoordToAbsoluteCoord(tmp);
	VPoint l2 = cs.RelativeCoordToAbsoluteCoord(tmp1);
	VLine myline (l1,l2);
	double t;
	VPlane::IntType ck = prjPlane.LineInt(myline , t);
	VPoint res;
	if (ck == VPlane::REGULAR)
		res = myline.GetP(t);
	else
		res = cs.RelativeCoordToAbsoluteCoord(tmp);
	return res;
}

void OGL_Frame::DoDragSelection (Mouse &mouse , DraftEntityVec & draftvec )
{
	int mysize = draftvec.Size();
	for ( int i=0; i < mysize; i++ )
	{	
		DraftEntity * currdraft = draftvec[i];
		if ( currdraft->IsVisible() && currdraft->GetDeleted()==false)
		{
			if (currdraft->GetModelTypeEng() == ViewPointItemTypes::LabelTextEng() )
			{
				LabelDisplay *lt = (LabelDisplay *)(currdraft);
				if (lt->IsHit (mouse))
				{
					lt->SetIsOnRun (true);
					return;
				}
			}
		}	
	}
}

void OGL_Frame::DoDrag (Mouse &mouse, DraftEntityVec &draftvec)
{
	int mysize = draftvec.Size();
	for ( int i=0; i < mysize; i++ )
	{	
		DraftEntity * currdraft = draftvec[i];
		if ( currdraft->IsVisible() && currdraft->GetDeleted()==false)
		{
			if (currdraft->GetModelTypeEng() == ViewPointItemTypes::LabelTextEng() )
			{
				LabelDisplay *lt = (LabelDisplay *)(currdraft);
				if (lt->IsOnRun())
				{
					lt->Run (mouse);
				}
			}
		}	
	}	
}

void OGL_Frame::EndDrag (DraftEntityVec &draftvec)
{
	int mysize = draftvec.Size();
	for ( int i=0; i < mysize; i++ )
	{	
		DraftEntity * currdraft = draftvec[i];
		if ( currdraft->IsVisible() && currdraft->GetDeleted()==false)
		{
			if (currdraft->GetModelTypeEng() == ViewPointItemTypes::LabelTextEng() )
			{
				LabelDisplay *lt = (LabelDisplay *)(currdraft);
				if (lt->IsOnRun())
				{
					lt->SetIsOnRun (false);
				}
			}
		}	
	}	
}

//void OGL_Frame::GotoView(const Quat &destquat, const ViewVolume &destVol, const VPoint &destRotCenter, const int stepNo)
//{
//	VPoint dR = destRotCenter - rot_center;
//	Quat dQ = destquat - quat;
//	ViewVolume dV = destVol - view_volume;
//	double f = 1. / stepNo;
//	dR *= f;
//	dQ = dQ * f;
//	dV = dV * f;
//
//	int snp1 = stepNo;
//	for (int i=0;i<snp1;++i)
//	{
//		quat = quat + dQ;
//		rot_center += dR;
//		view_volume = view_volume + dV;
//	}
//}

void OGL_Frame::UpdateRotCenterCoordinate()
{
	CoordinateSystem tmp (rot_center);
	m_rotationCenterCoord = tmp;
	//if (m_rotationCenterCoordDisp)
	//	m_rotationCenterCoordDisp->UpdateCoordSystemData (tmp);
}
	
void OGL_Frame::SetModelViewMatrix(const OglMatrix &om)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixd(om.GetArray()); 	
	//if (Display::IsMirrored(om))
	//{
	//	glFrontFace (GL_CW);
	//}
	//else
	//{
		glFrontFace (GL_CCW);
	//}
	//if (Display::IsScaled (om))
	//{
	//	glEnable ( GL_NORMALIZE );
	//}
	//else
	//{
	//	glDisable ( GL_NORMALIZE );
	//}
}

bool OGL_Frame::GetMargins(const ModelList & modellist , const std::vector<VTransformable *> &hs , const VPoint &alongVec, vreal &zmin, vreal &zmax)
{
	// Calculate Transformation Matrix
	TransformationMatrix T;
	if ( alongVec.Compare (UZ,1.e-9))
	{
		// do no transformation currviewing direction is ok
	}
	else if (alongVec.Compare (-UZ,1.e-9))
	{
		RotationMatrix R (VPoint(0.),UX,180.);
		T=R;
	}
	else 
	{
		VPoint rotax = UZ ^ alongVec;
		rotax.NormThis();
		vreal myangle = gmVectorUtility::GetAngleBetweenVectors (UZ,alongVec) * RAD_TO_DEGREE;
		RotationMatrix R(VPoint(0.),rotax,myangle);
		T=R;
	}
	// Set ogl model view matrix
	OglMatrix O(T);
	glMatrixMode (GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd( O.GetArray());
	
	// Set view volume so that all drawn objects cause an hit
	ViewVolume bigone(OGL_MAX_DRAWING_SPHERE_RAD,100,70);
	bigone.SetViewVolume();
	// enable selection mode
	int mysize=10;
	unsigned int mybuffer[10];
	glSelectBuffer(mysize,mybuffer);
	glRenderMode (GL_SELECT);
	glPushName(1);
	// draw all model
	DrawShaded(modellist,hs);

	// process hits
	int hitno=mybuffer[0];
	bool res (false);
	if (hitno==1)
	{

		zmin = (double)mybuffer[1] / OGL_SEL_ZCONST;
		zmax = (double)mybuffer[2] / OGL_SEL_ZCONST;
		zmin*=(2*OGL_MAX_DRAWING_SPHERE_RAD);
		zmin-=OGL_MAX_DRAWING_SPHERE_RAD;
		zmax*=(2*OGL_MAX_DRAWING_SPHERE_RAD);
		zmax-=OGL_MAX_DRAWING_SPHERE_RAD;
		res =true;
	}
	// go back to drawing state
	glPopMatrix();
	glRenderMode (GL_RENDER);
	return res;
}

void OGL_Frame::UpdateRotCenterCoordPosition()
{
	if (m_rotationCenterCoordDisp)
	{
		vreal scrx = view_volume.corner_x*0.9;
		vreal scry = view_volume.corner_y*0.9;
		vreal volx , voly;
		view_volume.ScreenCoordToViewVolume ( scrx , scry , volx , voly , rot_center.x() , rot_center.y() );
		CoordinateSystem cs = GetLookCoordinateSystem();
		VPoint tmp(volx,voly,0.);
		VPoint myorg = cs.RelativeCoordToAbsoluteCoord (tmp);
		m_rotationCenterCoord.SetOrigin (myorg);
		//m_rotationCenterCoordDisp->origin[0] = myorg.x();
		//m_rotationCenterCoordDisp->origin[1] = myorg.y();
		//m_rotationCenterCoordDisp->origin[2] = myorg.z();
	}
}

void OGL_Frame::AnalyzeCurve(Mouse &m,const ModelList &modellist)
{
	SelectionFlags tmp;
	tmp.SetSelectWithBox(false);
	SelectableTypes myfilter;
	myfilter.SetColorFilter (false);
	myfilter.AddSelectableType(ViewPointItemTypes::m_curvetype);
	DoSelection(m,false,false);
	Model *selm = DoModelSelection (m,false,tmp,myfilter,modellist,false,true);
	if (selm != 0)
	{
		if (m_selmodel != 0)
			m_selmodel->SetSelected(false);
		m_selmodel = selm;
	}
}

void OGL_Frame::AnalyzeSurface(Mouse &m , const ModelList &modellist)
{
	SelectionFlags tmp;
	tmp.SetSelectWithBox(false);
	SelectableTypes myfilter;
	myfilter.AddSelectableType(ViewPointItemTypes::m_surfacetype);
	myfilter.SetColorFilter (false);
	DoSelection(m,false,false);
	Model *selm = DoModelSelection (m,false,tmp,myfilter,modellist,false,true);
	if (selm != 0)
	{
		if (m_surfselmodel != 0)
			m_surfselmodel->SetSelected(false);
		m_surfselmodel = selm;
	}
}

void OGL_Frame::DrawDglLinesAndArcs()
{
	PointVisualAtt pva;
	pva.SetPointSize(5.0);
	visualattloader.Load(pva,false,selectioncolor);
	std::map<VStr , std::vector<dglLine *>>::iterator li = m_dglLines->begin();
	std::map<VStr , std::vector<dglLine *>>::iterator le = m_dglLines->end();
	for (li;li!=le;++li)
	{
		std::vector<dglLine *>::iterator cli = li->second.begin();
		std::vector<dglLine *>::iterator cle = li->second.end();
		for (cli;cli!=cle;++cli)
		{
			if ((*cli)->GetVisAtt().GetDraw())
			{	
				visualattloader.Load ((*cli)->GetVisAtt() , false , selectioncolor);
				(*cli)->Draw();
			}
		}
	}
	// Arcs
	std::map<VStr , std::vector<dglArc *>>::iterator ai = m_dglArcs->begin();
	std::map<VStr , std::vector<dglArc *>>::iterator ae = m_dglArcs->end();
	for (ai;ai!=ae;++ai)
	{
		std::vector<dglArc *>::iterator cai = ai->second.begin();
		std::vector<dglArc *>::iterator cae = ai->second.end();
		for (cai;cai!=cae;++cai)
		{
			if ((*cai)->GetVisAtt().GetDraw())
			{
				visualattloader.Load ((*cai)->GetVisAtt() , false , selectioncolor);
				(*cai)->Draw();
			}
		}
	}
}

void OGL_Frame::DrawSelectionRectangle ()
{
	if (m_drawSelectionRectangle)
	{
		vreal vx0,vy0,vx1,vy1;
		ScreenCoordToViewVolume (m_selRectX0,m_selRectY0,vx0,vy0);
		ScreenCoordToViewVolume(m_selRectX1,m_selRectY1,vx1,vy1);
		vreal myz = view_volume.m_far * 0.999;
		glColor3d (VGlobalValues::viewpointdefaults.m_defaultWireframeColor.R(),VGlobalValues::viewpointdefaults.m_defaultWireframeColor.G(),VGlobalValues::viewpointdefaults.m_defaultWireframeColor.B());
		glLineWidth (1.f);
		glMatrixMode(GL_MODELVIEW_MATRIX);
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_LINES);
		glVertex3d(vx0,vy0,myz);
		glVertex3d(vx1,vy0,myz);
		glVertex3d(vx1,vy0,myz);
		glVertex3d(vx1,vy1,myz);
		glVertex3d(vx1,vy1,myz);
		glVertex3d(vx0,vy1,myz);
		glVertex3d(vx0,vy1,myz);
		glVertex3d(vx0,vy0,myz);
		glEnd();
		glPopMatrix();
	}
}

VArc OGL_Frame::GetSelCircle (Mouse &mouse)
{
	VPoint mp = FreeClickToModelPoint (mouse);
	VPoint dirz = GetLookDir();
	vreal hitrad = selection.GetHitRadiusOnWindow();
	VVector dirx1 = dirz ^ UX;
	VVector dirx2 = UY ^ dirz;
	VPoint dirx = dirx1.GetLength2() > dirx2.GetLength2() ? dirx1 : dirx2;
	dirx.NormThis();
	VPoint diry = dirz ^ dirx;
	VArc selcircle (mp , hitrad , dirz , mp + dirx * hitrad , 360.0);
	return selcircle;
}

void OGL_Frame::MakeGridDraw()
{
	m_gridDraw = new GridDrawNew(m_workingCoordSys,&visualattloader);
}

const GridDrawNew * OGL_Frame::GetGridDraw() const
{
	return m_gridDraw;
}

GridDrawNew * OGL_Frame::GetGridDraw()
{
	return m_gridDraw;
}

HGLRC & OGL_Frame::GetHRC()
{
	return m_hRC;
}
void OGL_Frame::SetHRC (HGLRC &hRC)
{
	m_hRC = hRC;
}

bool OGL_Frame::IsVpRenderContextNull()
{
	return m_isVpRenderContextNull;
}
	
void OGL_Frame::SetIsVpRenderContextNull(bool isVpRenderContextNull)
{
	m_isVpRenderContextNull = isVpRenderContextNull;
}

void OGL_Frame::DrawWireFrame2 ( const ModelList & modellist , const std::vector<VTransformable *> &hs)
{
	//glDisable ( GL_LIGHTING );
	//LiftWireFrame ();
	
	if ( viewingstate.IsClipped () )
		glEnable ( GL_CLIP_PLANE1 );

	Model * currmodel;
	int mysize=(int)hs.size();
	std::vector<Model*> selectedVec;
	std::vector<Model*> onLineVec;
	std::vector<Model*> surfVec;
	LineVisualAtt temp;
	temp.ChangeWireColor (VGlobalValues::viewpointdefaults.m_defaultWireframeColor);
	temp.SetLineWidth(LineVisualAtt::mid);
	temp.SetLineFont (LineVisualAtt::LINEFONT_NORMAL);
	visualattloader.Load(temp,false,selectioncolor);
	glFlush();
	for (int i = 0; i < mysize; ++i)
	{
		//const ModelID &id = hs.GetID(i);
		VTransformable * curtr = hs.at(i);
		currmodel = curtr->GetTrModel();//modellist.GetModel(id);
		Display &disp = currmodel->GetDisplay();
		if ( currmodel->IsVisible()==false || currmodel->GetDeleted()==true  || disp.GetVisualAtt().GetDraw()==false)
			continue;	
		//const LinesDisplay & wiredisp = currmodel->GetWireDisplayList();
		//bool forcedrawing = false;
		//if ( currmodel->GetDisplayList().IsDefined() == false 
		//	&& currmodel->GetCoordSystemDisplay() == 0 
		//	&& currmodel->GetPointsDisplayList().IsDefined() == false   )
		//	forcedrawing = true;

		//if ( wiredisp.GetVisualAtt().GetDraw()) //|| forcedrawing )
			//visualattloader.Load ( wiredisp.GetVisualAtt() , currmodel->IsSelected() , selectioncolor );
		//visualattloader.Load (disp.GetVisualAtt(), currmodel->IsSelected() , selectioncolor);
		//wiredisp.Draw();
		if (currmodel->IsSelected())
			selectedVec.push_back(currmodel);
		else if (currmodel->GetModelType() == ViewPointItemTypes::m_surfacetype)
			surfVec.push_back(currmodel);
		else if (currmodel->GetDisplay().GetSeqLine() != 0)
			onLineVec.push_back(currmodel);
		else
			disp.DrawLines(false);
	}
	//Unlift();
	//glEnable (GL_LIGHTING);
	//if (selection.m_selflags->GetSelectionMode() == false)
	//{
		size_t surfSize = surfVec.size();
		temp.ChangeWireColor (VGlobalValues::viewpointdefaults.m_defaultSurfaceWireColor);
		temp.SetLineWidth (LineVisualAtt::thin);
		temp.SetLineFont (LineVisualAtt::LINEFONT_NORMAL);
		visualattloader.Load(temp,false,selectioncolor);
		glFlush();
		for (size_t i=0;i<surfSize;++i)
		{
			surfVec.at(i)->GetDisplay().DrawLines(false);	
		}
		size_t onLineSize = onLineVec.size();
		for (size_t i=0;i<onLineSize;++i)
		{
			onLineVec.at(i)->GetDisplay().DrawLines(false);	
		}
	//}
	size_t selectedSize = selectedVec.size();
	temp.SetLineWidth(LineVisualAtt::mid);
	visualattloader.Load(temp,true,selectioncolor);
	glFlush();
	for (size_t i=0;i<selectedSize;++i)
	{
		selectedVec.at(i)->GetDisplay().DrawLines(true);	
	}
	//visualattloader.Load(temp,false,selectioncolor);
}

void OGL_Frame::DrawShaded2 ( const ModelList & modellist ,const std::vector<VTransformable *> &hs) const
{
	if (m_drawingstate != OGL_Frame::triangles )//&& m_drawingstate != OGL_Frame::hiddenline && m_drawingstate!= hiddenlinegray)
		glEnable (GL_LIGHTING);
	if ( viewingstate.IsClipped () )
		glEnable ( GL_CLIP_PLANE1 );
	
	FacetVisualAtt tmp;
	tmp.ChangeShadeColor(VGlobalValues::viewpointdefaults.m_defaultSurfaceColor);
	//LineVisualAtt tmpl;
	//tmpl.ChangeWireColor (VGlobalValues::viewpointdefaults.m_defaultSurfaceWireColor);
	//tmpl.SetLineWidth (LineVisualAtt::thin);
	if ( m_drawingstate != hiddenline && m_drawingstate != hiddenlinegray && m_drawingstate != pixelrenderBlackAndWhite)
	{
		//glEnable ( GL_LIGHTING );
	}
	else
	{
		//glDisable ( GL_LIGHTING );
		//glColor3d ( hiddenlinecolor.R() , hiddenlinecolor.G() , hiddenlinecolor.B() );
		if (m_drawingstate != pixelrenderBlackAndWhite)
		{
			//tmp.ChangeShadeColor (hiddenlinecolor);
			if (m_drawingstate == hiddenlinegray)
				tmp.SetTranslucent(true);
		}
		else
		{
			tmp.SetTranslucent(false);
		}
	}
	visualattloader.Load ( tmp , false , selectioncolor );
	//visualattloader.Load (tmpl,false,selectioncolor);
	if (m_drawingstate == triangles)
	{
		glPolygonMode (GL_FRONT_AND_BACK , GL_LINE);
	}

	Model * currmodel;
	int mysize = (int)hs.size();
	std::vector<Model *>selectedVec;
	for (int i = 0; i <mysize; ++i)
	{
		//const ModelID &id = hs.GetID(i);
		VTransformable *curtr = hs.at(i);
		currmodel = curtr->GetTrModel();//modellist.GetModel(id);

		//bool forcedrawing = false;
		
		//if ( currmodel->GetPointsDisplayList().IsDefined() == false
		//	&& currmodel->GetWireDisplayList().IsDefined() == false 
		//	&& currmodel->GetCoordSystemDisplay() == 0  )
		//		forcedrawing = true;
		const Display & disp = currmodel->GetDisplay();		
		if ( currmodel->IsVisible()==false || currmodel->GetDeleted()==true || disp.GetVisualAttFacet().GetDraw()==false)
			continue;
	
		//const FacetDisplay & facetdisp = currmodel->GetDisplayList();
		//if ( facetdisp.GetVisualAtt().GetDraw() )//|| forcedrawing )
		//if (disp.GetVisualAttFacet().GetDraw())
		//{
			//if (m_drawingstate != hiddenline && m_drawingstate != hiddenlinegray && m_drawingstate != pixelrenderBlackAndWhite)
				//visualattloader.Load ( facetdisp.GetVisualAtt() , currmodel->IsSelected() , selectioncolor );
			//	visualattloader.Load ( disp.GetVisualAttFacet() , currmodel->IsSelected() , selectioncolor);
			//else
			//{
			//	tmp.ChangeShadeColor (disp.GetVisualAttFacet().GetShadeColor());
			//	visualattloader.Load (tmp,currmodel->IsSelected(),selectioncolor);
			//}
			//facetdisp.Draw();
			if (currmodel->IsSelected() == false)
			{
				disp.DrawFacet();
			}
			else
				selectedVec.push_back (currmodel);
		//}

		//const QuadsDisplay & quadsdisp = currmodel->GetQuadsDisplayList();
		//if ( quadsdisp.GetVisualAtt().GetDraw() )
		//{
		//	if ( hiddenline == false )
		//		visualattloader.Load ( quadsdisp.GetVisualAtt() , currmodel->IsSelected() , selectioncolor );
			
		//	quadsdisp.Draw();
		//}
	}
	size_t selectedSize = selectedVec.size();
	visualattloader.Load (tmp,true,selectioncolor);
	//visualattloader.Load(tmpl,true,selectioncolor);
	for (size_t i=0;i<selectedSize;++i)
	{
		selectedVec.at(i)->GetDisplay().DrawFacet();
	}
	visualattloader.Load(tmp,false,selectioncolor);
	//visualattloader.Load(tmpl,false,selectioncolor);
	glDisable (GL_LIGHTING);
}

void OGL_Frame::SetColorMode (int mode)
{
	// 0 variable 
	if (mode == 0)
	{
		funcPtrDrawPoints = &OGL_Frame::DrawPoints;
		funcPtrDrawShaded = &OGL_Frame::DrawShaded;
		funcPtrDrawWireFrame = &OGL_Frame::DrawWireFrame;
		funcPtrDrawCoordSystems = &OGL_Frame::DrawCoordSystems;
		funcPtrDrawDraftEntities = &OGL_Frame::DrawDraftEntities;
	}
	else if (mode == 1)// 1 standard
	{
		funcPtrDrawPoints = &OGL_Frame::DrawPoints;
		funcPtrDrawShaded = &OGL_Frame::DrawShaded2;
		funcPtrDrawWireFrame = &OGL_Frame::DrawWireFrame2;
		funcPtrDrawCoordSystems = &OGL_Frame::DrawCoordSystems;
		funcPtrDrawDraftEntities = &OGL_Frame::DrawDraftEntities;
	}
}

int OGL_Frame::DeselectOthers (ModelList &modellist,const SelectableTypes &types)// returns selected count
{
	int selcount = 0;
	Model * currmodel;
	ModelList::pos e = modellist.End();
	ModelList::pos i = modellist.Begin();
	for (i; i != e; i++ )
	{
		currmodel = *i;
		
		if ( currmodel->IsSelected () )
		{
			if (types.IsTypeSelectable(currmodel->GetModelType()))
			{
				++selcount;//model selectable
			}
			else
			{
				currmodel->SetSelected(false);//deselect that
			}
		}
	}
	return selcount;
}

void OGL_Frame::CollectModelSelectionPotentialHits(std::vector<VTransformable *> &hs , Mouse &mouse)
{
	VPoint trp = FreeClickToModelPoint(mouse);
	VPoint viewDir = GetLookDir();
	VLine infoline(trp, trp + viewDir);
	m_voxel.CollectVTransformable(infoline, hs);
	int mysize = (int)hs.size();
	for (int i = 0; i < mysize; ++i)
	{
		VTransformable *tr = hs.at(i);
		tr->GetTrModel()->SetSelected(true);
	}
}

void OGL_Frame::DrawViewPointStatusText()
{

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, (GLfloat)view_volume.corner_x, 0.0, (GLfloat)view_volume.corner_y
		, view_volume.m_near, view_volume.m_far);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glLineWidth(0.01f);

	vreal depth = view_volume.m_far;// (view_volume.m_far + view_volume.m_near) *0.5;
	int txtCount = (int)m_statusText.size();
	GLfloat fontw = (GLfloat)VGlobalValues::viewpointdefaults.m_defaultFontWidth;
	GLfloat fonth = (GLfloat)VGlobalValues::viewpointdefaults.m_defaultFontHeight;
	GLfloat basex = 3 * fontw;
	GLfloat basey = 4 * txtCount * fonth;//view_volume.corner_y - 2 * xlet.fonth;
	OglWindowData wd(view_volume.corner_x, view_volume.corner_y, view_volume.m_near, depth);
	TextBoxDisplayer d1(wd);

	for (int ti = 0; ti < txtCount; ++ti)
	{
		VStr curMsg = m_statusTextHeader.at (ti) + m_statusText.at(ti);
		TextBox xlet(fontw , fonth , curMsg.c_str() , &m_asciiDisp);
		//TextBox ylet(6., 6., m_statusText[1].c_str(), &m_asciiDisp);
		//TextBox zlet(6., 6., m_statusText[2].c_str(), &m_asciiDisp);
		xlet.fontw = fontw;
		xlet.fonth = fonth;
		//ylet.fontw = 8;
		//ylet.fonth = 10.0;
		//zlet.fontw = 8;
		//zlet.fonth = 10.0;
		GLfloat b1[2] = { (GLfloat)basex, (GLfloat)(basey - 4.0 * ti * fonth)};
		//GLfloat b2[2] = { basex, basey - 2 * xlet.fonth };
		//GLfloat b3[2] = { basex, basey - 4 * xlet.fonth };
		TextBoxPosData xpos(b1, b1, b1);
		//TextBoxPosData ypos(b2, b2, b2);
		//TextBoxPosData zpos(b3, b3, b3);
		//d3.DisplayOnlyText(zlet, zpos);
		//d2.DisplayOnlyText(ylet, ypos);
		d1.DisplayOnlyText(xlet, xpos);
		//d1.DisplayOnlyText(ylet, ypos);
		//d1.DisplayOnlyText(zlet, zpos);
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void OGL_Frame::GetScreenCoordinatesForPoint(const VPoint &p, vreal &fromLeftToPointScx, vreal &fromBottomToPointScy, vreal &fromTopToPointScy)
{
	CoordinateSystem lookcs = GetLookCoordinateSystem();
	VPoint trp = lookcs.AbsoluteCoordToRelativeCoord(p);
	fromLeftToPointScx = (trp.x() - view_volume.m_left) * view_volume.corner_x / (view_volume.m_right - view_volume.m_left);
	fromBottomToPointScy = (trp.y() + -view_volume.m_bottom) * view_volume.corner_y / (view_volume.m_top - view_volume.m_bottom);
	fromTopToPointScy = view_volume.corner_y - fromBottomToPointScy;
}