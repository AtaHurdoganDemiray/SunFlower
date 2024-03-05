// OGL_Frame.h: interface for the OGL_Frame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __OGL_Frame_h__
#define __OGL_Frame_h__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ViewVolume.h"
#include "Lighting.h"
#include "Quat.h"
#include "VPoint.h"
#include "Mouse.h"
#include "HitRadius.h"
#include "DrawShapes.h"
#include "Perspective.h"
#include "Camera.h"
#include "OGLSelection.h"
#include "VisualAttLoader.h"
#include "VColor.h"
//#include "texture.h"
#include "VPlane.h"
#include "CoordinateSystem.h"
#include "CoordSystemDisplay.h"
#include "LineVisualAtt.h"
#include "PointVisualAtt.h"
#include <map>
#include "VVoxel.h"

class LineSet;
struct DoubleList;
class HandleSet;
class SelectionFlags;
class SelectableTypes;
class ModelList;
class ViewingState;
class Model;
class DraftEntityVec;
class SelectionFlags;
struct OglMatrix;
class OperationTransformList;
class ModelList;
class dglLine;
class dglArc;
struct VLine;
class VArc;
class GridDrawNew;
class SelectableTypes;
class NumberFonts;

class OGL_Frame 
{
public:
		enum DrawingState {shadedandwireframe 
						, shaded 
						, wireframe 
						, hiddenline 
						, hiddenlinegray
						, triangles
						, pixelrenderBlackAndWhite};
	OGL_Frame( ViewingState & _viewingstate);
	void InitOGL_Frame (ModelList *modellist,DraftEntityVec *draftvec,OperationTransformList *transformations,SelectionFlags *sel_flags
		,std::map<VStr , std::vector<dglLine *>> *dglLines , std::map<VStr , std::vector<dglArc *>> *dglArcs);
	virtual ~OGL_Frame();
	void InitSize( int screenx , int screeny );

	ViewVolume view_volume;
	ViewVolume selectionvolume;
	DrawingState m_drawingstate;
	texture * mybackground;
	VPoint rot_center;
//	Lighting lighting;
	mutable Quat quat;
	VPoint rot_angles;
	VPoint lookdir;
	VPoint lift;
	vreal adx , ady , adz;
	int m_i;
	vreal scale_factor;
	//HitRadius hitr;
	vreal hitrad;

	VColor color1_up_left;
	VColor color2_bottom_left;
	VColor color3_bottom_right;
	VColor color4_up_right;
	DrawShapes myshapes;
	//bool navigationmode;
	Perspective perspective;
	Camera m_camera;
	ViewingState & viewingstate;
	void FirstInit();
	void Init();
	void Draw();
	void DrawAllOnce(const ModelList &modellist, const std::vector<VTransformable *> &hs);
	void DrawPoints ( const ModelList & modellist , const std::vector<VTransformable *> &hs) const;
	void DrawWireFrame ( const ModelList & modellist , const std::vector<VTransformable *> &hs);
	void DrawShaded ( const ModelList & modellist , const std::vector<VTransformable *> &hs) const;
	//void DrawFontDisp (const ModelList &modellist , const std::vector<VTransformable *> &hs);
	void DrawDraftEntities ( DraftEntityVec & draftvec ,const std::vector<VTransformable *> &hs) const;
	void DrawCoordSystems ( ModelList & modellist , const std::vector<VTransformable *> &hs) const;
	void Size(int cx, int cy);
	void WavyScreen ();
	void Rotate ( Mouse& mouse );
	void Roll ( Mouse & mouse );
	void SetRotationAngles ();
	void PanView( Mouse & mouse );
	void ScreenCoordToViewVolume ( vreal x , vreal y , vreal & vx , vreal & vy )const;
	void ZoomToBox( Mouse &mouse );
	void ZoomPan( Mouse &mouse , const vreal & scalef = 1.1);
	VPoint CalcRotAxis ( Mouse & mouse );
	void RotConst ( VPoint rot_axis );
	void Top ( VPoint & min , VPoint & max );
	void Iso ( VPoint & min , VPoint & max );
	void Bottom ( VPoint & min , VPoint & max );
	void Back ( VPoint & min , VPoint & max );
	void Left ( VPoint & min , VPoint & max );
	void Fit ( VPoint & min , VPoint & max );
	void Right ( VPoint & min , VPoint & max );
	void Front ( VPoint & min , VPoint & max );
	void SetNewViewVolume (vreal x , vreal y , vreal vx , vreal vy);
	void DoSelection ( Mouse &mouse , bool selectwithbox , bool verysmallhitrad );
	void DoPointSelection 
	( Mouse &mouse , bool doSketchPoint , const ModelList & modellist , DoubleList & selpoints );
	void DoVectorSelection 
	( Mouse &mouse , bool selectwithbox , const ModelList & modellist , DoubleList & selpoints );
	void DoEdgeSelection 
	( Mouse & mouse , const ModelList & modellist , DoubleList & selpoints );	
	Model * DoModelSelection ( Mouse &mouse , bool deselect , SelectionFlags & selflags , const SelectableTypes & filter , const ModelList & modellist , bool ctrlActive , bool disableBoxSel = false , Model ** pSelModel = 0);
	Model * DoModelSelection2(Mouse &mouse, bool deselect, SelectionFlags & selflags, const SelectableTypes &filter, const ModelList &modellist, bool ctrlActive, bool disableBoxSel = false, Model ** pSelModel = 0);
	void EndModelSelection ( ModelList & modellist , HandleSet * handles = 0 );
	void SetRotationCenter ( VPoint newcenter );
	VPoint GetLookDir ();
	void LiftWireFrame ();
	void LiftPointMax ( VPoint poi );
	void Unlift();
	void DrawVector ( VLine &vect );
	void DrawAllSelectedVectors ( LineSet * myset );
	vreal GetScaleFactorForWindowPortion ( vreal windowportion );
	void ChangeViewDirection ( VPoint dir );
	void DrawPlanes();
	void SetNavigationMode ();
	void RotateNavigateCamera ( Mouse& mouse );
	void MoveNavigateCamera ( Camera::movedirection movedir );
	TransformationMatrix GetOglModelTransformMatrix () const;
	TransformationMatrix GetOglProjectionMatrix () const;
	void GetOglViewport ( int viewport[4] );
	//void GetOglModelTransformMatrix ( vreal modelMatrix[16] );
	//void GetOglProjectionMatrix ( vreal projMatrix[16] );
	void SetColorArray ( const VColor & colupleft 
						   , const VColor & colbottomleft 
						   , const VColor & colbottomright
						   , const VColor & colupright );
	void DrawSnapPoints ();
	void DrawSnapLines ();
	void DrawSnapPoints ( const ModelList & modellist , const std::vector<VTransformable *> &hs);
	void DrawSnapLines ( const ModelList &modellist , const std::vector<VTransformable *> &hs);
	void ClipIf ();
	//void DrawDimension( const ModelList & modellist );
	void DrawViewCoordinateSystem ()const;
	VPoint GetSketchPointOnScreen(const vreal xyz[]);
	VPoint GetAPointOnPrincipalPlanes(VLine &l);
	CoordinateSystem GetLookCoordinateSystem();
	VPoint FreeClickToModelPoint (Mouse &mouse);
	VPoint FreeClickToModelPoint(int screenx , int screeny , const VPlane &prjPlane);
	VLine ScreenPointToModelSpaceLine (Mouse &mouse);

	void DoDragSelection (Mouse &mouse,DraftEntityVec & draftvec);
	void DoDrag (Mouse &mouse, DraftEntityVec &draftvec);
	void EndDrag (DraftEntityVec &draftvec);
	//void GotoView (const Quat &destQuat , const ViewVolume &destVol , const VPoint &destRotCenter , const int stepNo);
	void UpdateRotCenterCoordinate();
	void SetModelViewMatrix (const OglMatrix &om);
	bool GetMargins (const ModelList &modellist , const std::vector<VTransformable *> &hs , const VPoint &alongVec , vreal &zmin , vreal &zmax);
	void AnalyzeCurve (Mouse &m , const ModelList &modellist);
	void AnalyzeSurface (Mouse &m,const ModelList &modellist);
	void DrawDglLinesAndArcs ();
	GLubyte halftone[128];
	OGLSelection selection;
	VisualAttLoader visualattloader;
	VColor selectioncolor;
	VPlane clipplane;
	VColor hiddenlinecolor;
	vreal m_liftmag;	
	
	CoordinateSystem m_rotationCenterCoord;
	LineVisualAtt m_rotationCenterVisatt;
	CoordSystemDisplay *m_rotationCenterCoordDisp;	
	OperationTransformList *m_transformations;
	ModelList *m_modellist;
	DraftEntityVec *m_draftvec;
	CoordinateSystem m_workingCoordSys;
	void UpdateRotCenterCoordPosition ();
	Model *m_selmodel;
	Model *m_surfselmodel;
	std::map<VStr,std::vector<dglLine *>> * m_dglLines;
	std::map<VStr,std::vector<dglArc *>> * m_dglArcs;
	bool m_drawSelectionRectangle;
	void DrawSelectionRectangle ();
	vreal m_selRectX0,m_selRectY0,m_selRectX1,m_selRectY1;
	VArc GetSelCircle (Mouse &mouse);
	std::vector <Model *> m_oneDoSelectionResult;
	const GridDrawNew * GetGridDraw()const;	
	GridDrawNew * GetGridDraw();
	void MakeGridDraw();
	HGLRC & GetHRC();
	void SetHRC (HGLRC &hRC);
	bool IsVpRenderContextNull();
	void SetIsVpRenderContextNull(bool isVpRenderContextNull);
private:
	GridDrawNew *m_gridDraw;
	HGLRC m_hRC;
	bool m_isVpRenderContextNull;
	public:
	static double M_MAXWORKDIAMETER;
	void DrawPoints2 ( const ModelList & modellist , const std::vector<VTransformable *> &hs) const;	
	void DrawWireFrame2 ( const ModelList & modellist , const std::vector<VTransformable *> &hs);
	void DrawShaded2 ( const ModelList & modellist , const std::vector<VTransformable *> &hs) const;
	//void DrawFontDisp (const ModelList &modellist , const std::vector<VTransformable *> &hs);
	void DrawDraftEntities2 ( DraftEntityVec & draftvec ,const std::vector<VTransformable *> &hs) const;
	void DrawCoordSystems2 ( ModelList & modellist , const std::vector<VTransformable *> &hs) const;
public:
	void SetColorMode (int colorMode);//0 for variable , 1 for standard
	int DeselectOthers (ModelList &modellist,const SelectableTypes &types);// returns selected count
	void FitTo (const VPoint &mn , const VPoint &mx);
	std::vector<Display *> m_asciiDisp;
	void FillAsciiDispMap();
	NumberFonts * VTEXTBOXFONTS = 0;
	VVoxel m_voxel;
	void CollectModelSelectionPotentialHits(std::vector<VTransformable *> &hs , Mouse &mouse);
	void QuickModelSelection(std::vector<VTransformable *> &pothits,Mouse &mouse);
	void DrawViewPointStatusText();
	std::vector<VStr> m_statusText;
	std::vector<VStr> m_statusTextHeader;
	void GetScreenCoordinatesForPoint(const VPoint &p, vreal &fromLeftToPointScx, vreal &fromBottomToPointScy, vreal &fromTopToPointScy);

};

#endif