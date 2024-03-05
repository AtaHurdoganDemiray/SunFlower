/******************************************************************************
*               File: Model.h											  *
*******************************************************************************
*               Description:                                                  *
*                                                                             *
*******************************************************************************
*               History:                                                      *
*  02.07.2003 18:23:08 Created by:    Ata Hurdogan Demiray                    *
*******************************************************************************
*               (C) 2003 by Pozitif GmbH                                     *
******************************************************************************/

#ifndef __Model_h__
#define __Model_h__

#include "TriangleMesh.h"
#include "LineSet.h"
#include "PointSet.h"
#include "VisualAttributes.h"
#include <cstring>
//#include "FacetDisplay.h"
//#include "LinesDisplay.h"
//#include "PointsDisplay.h"
//#include "QuadsDisplay.h"
#include "Display.h"
#include "CoordinateSystem.h"
#include "SnapEntities.h"
#include "ViewPointItem.h"

class ModelSelectionFilters;
struct texture;
class TriangleMesh2d;
class BoundingBox;
class VRefTriangleMesh;
class QuadSet;
class SequentialLineSet;
class SurfaceWireFrame;
class CoordSystemDisplay;
struct CoordinateSystem;
//class FontDisp;
struct TransformData;
class NumberFonts;
class VTransformable;
class FMesh;
class VTexture;
class QuadPointGenerator;

class Model : public ViewPointItem
{
public:
	friend class SerializerModel;
	friend class Iges2ViewPoint;
	friend class ActionTransformModel;
	Model (const ModelID &id);
	Model (VStr modeltype,VStr name);
	Model (TriangleMesh * p_mesh,VStr modeltype,VStr name);
	Model (const Model & Foo);
	virtual ~Model();
	void SetTriMesh ( const TriangleMesh * p_mesh , const FacetVisualAtt *visatt=0 );
	void SetTriMesh ( const FMesh &fmesh , const FacetVisualAtt *visatt=0);
	void SetTriMesh ( std::vector<FMesh *> &triVec , const FacetVisualAtt &visatt);
	void SetLineSet ( const LineSet * p_lineset , const LineVisualAtt *visatt=0 );
	void SetLineSet ( const SequentialLineSet * p_lineset , const LineVisualAtt *visatt=0);
	void SetLineSet (const std::vector<VPoint> *p_lineset , const std::vector<std::pair<size_t,size_t>> &rangeVec , const std::vector<LineVisualAtt> &visAttVec);
	void SetPointSet ( const PointSet *p_points , const PointVisualAtt *visatt=0 );
	void SetSelectionPointSet ( PointSet *p_points );
	void SetRefTriMesh ( const VRefTriangleMesh *p_reftris , const FacetVisualAtt *visatt=0 );
	void SetCoordSystemAndDisplay ( const CoordinateSystem & cs );
	//void MakeTextureDisplayList (const VRefTriangleMesh *p_tri , const texture *p_texture , const FacetVisualAtt *visatt);
	void MakeTextureDisplayList (const VRefTriangleMesh *p_tri , const VTexture *p_texture , const FacetVisualAtt *visatt);
	//FontDisp * SetFontDisp (VStr txt , const vreal &fontsize , const CoordinateSystem &base,const NumberFonts &nf);
	//FacetDisplay & GetDisplayList ();
	//LinesDisplay & GetWireDisplayList ();
	//PointsDisplay & GetPointsDisplayList ();
	//QuadsDisplay & GetQuadsDisplayList ();
	CoordSystemDisplay * GetCoordSystemDisplay ();
	//FontDisp * GetFontDisp();
	Display & GetDisplay();
	void GetBothDisplay (Display ** quickDisplay , Display **disp);
	const Display & GetDisplay()const;
	CoordinateSystem GetCoordinateSystem () const; 
	void GetHitPoints ( PointSet & selpoints , DoubleVec & depthvalues , bool selectanyvertex) const;
	void GetHitPointsNew (PointSet &selpoints , DoubleVec &depthvalues , const VPoint &curserp,const VPoint &lookdir,double selr) const;
	void GetHitLines ( LineSet & sellines , DoubleVec & depthvalues ) const;
	void DrawSnapPoints() const;
	void DrawSnapLines()const;
	void Transform (TransformData &td);
	void AddSnapPoint ( const VPoint & snapp );
	void AddSnapPointSet ( const PointSet & snapp );
	void AddSnapLine ( const VLine & snapl );
	void AddSnapLineSet ( const LineSet & snapl );
	SnapEntities & GetSnapEnt ();
	virtual void OnTreeViewClicked();
	virtual void OnTreeViewDoubleClicked();
	virtual void OnTreeViewRightClicked(const VStr &commandcap);
	//FacetDisplay & GetFacetDisplay();
	//const FacetDisplay & GetFacetDisplay()const;
	//LinesDisplay & GetLinesDisplay();
	//const LinesDisplay & GetLinesDisplay()const;
	//PointsDisplay & GetPointsDisplay();
	//const PointsDisplay & GetPointsDisplay()const;
	void SetTransformable (VTransformable *tr);
	VTransformable * GetTransformable ();
	virtual const VStr GetModelTypeEng()const;
	int GetSelectionTime ()const;
	void SetSelectionTime (int selectionTime);
	bool GetDrawDuringChange ()const;
	void TransformSnapEnt (const TransformationMatrix &tm);
	//void SetTexture (texture *pTexture);
private:
	//FacetDisplay facetdisplay;
	//LinesDisplay linesdisplay;
	//PointsDisplay pointsdisplay;
	//QuadsDisplay quadsdisplay;
	Display m_display;
	CoordSystemDisplay * coorddisplay;
	//FontDisp *rootfont;
	CoordinateSystem mcoord;
	SnapEntities snapent;
	VTransformable * m_transform;
	int m_selectionTime;
public:
	void MakeDisplayList (std::vector<const Display *> &facetVec , std::vector<const Display*> &linesDisplay , std::vector<const Display* > &pointsDisplay
						, std::vector<OglMatrix> &curveTmVec);
	Display * m_quickDisplay;
	void MakeQuickDisplayList (std::vector<const Display *> &facetVec , std::vector<const Display*> &linesDisplay , std::vector<const Display *> &pointsDisplay
		, std::vector<OglMatrix> &curveTmVec);
	void MakeQuadDisplayList (std::vector<QuadPointGenerator *> &quadPoints , FacetVisualAtt &visAtt); 
	void SetCoord (const CoordinateSystem &cs);
	bool IsColorSelectable(COLORREF col)const;
	void ResetTypeName(const VStr &typeNameToSet);
};

#endif //__Model_h__