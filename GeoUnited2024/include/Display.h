// Display.h

#ifndef __Display_h__
#define __Display_h__
#include "OglConstValues.h"
#include "OglMatrix.h"

class TransformationMatrix;
struct TransformData;
#include "LineVisualAtt.h"
#include "OglConstValues.h"
//#include "Display.h"

class LineSet;

struct VPoint;
struct DoubleVec;
class SequentialLineSet;
//class SurfaceWireFrame;
class PointSet;

#include "FacetVisualAtt.h"
class VRefTriangleMesh;
class TriangleMesh;
class PointSet;

struct VPoint;
struct DoubleVec;
class OglRefMesh;
class FMesh;
//#include "Display.h"
struct texture;
#include "PointVisualAtt.h"
#include "OglConstValues.h"
//#include "Display.h"

class PointSet;

struct VPoint;
struct DoubleVec;

//#include "Display.h"
#include "CoordinateSystem.h"
//#include "NumberFonts.h"
#include "LineVisualAtt.h"
#include "TransformData.h"

struct OglMatrix;
struct VPoint;
class LineSet;
class VTexture;
class QuadPointGenerator;

class Display
{
public:
	Display();
	Display(const Display &tocopy);
	Display & operator=(const Display &toCopy);
	~Display();
	//void DestroyDisplayList();
	//void Draw()const;
	void DrawLines(bool isSelected)const;
	void DrawFacet()const;
	void DrawPoints()const;
	static bool IsScaled(const OglMatrix &m);
	static bool IsMirrored(const OglMatrix &m);
	//std::list<TransformData*> xmats;
	//void AddMatrix(TransformData *x);
	bool IsDefined() const;
	static GLfloat selectionbuff[SELECTION_BUFFER_SIZE];
	//TransformationMatrix GetOverallMat()const;
	void OnlineDraw(bool isSelected) const;
	static bool m_isGraphicViewChanging;
protected:
	unsigned int m_dispListLines;
	unsigned int m_dispListFacet;
	unsigned int m_dispListPoints;
	//int * usercount;
	//bool m_isScaled;
	//bool m_isMirrored;


	// From LinesDisplay
public:
	//Display ();
	Display ( const LineSet & lines );
	Display ( const SequentialLineSet & seqlines );
	//Display ( const Display & Foo );
	//virtual ~Display();
	void DestroyDisplayListLines();
	//Display & operator = (const Display &tocopy);
	void GetHitPoints ( PointSet & selpoints , DoubleVec & depthvalues ) const;
	void GetHitLines ( LineSet & sellines , DoubleVec & depthvalues ) const;
	bool IsHit ( vreal & depthvalue ) const;
	void MakeDisplayList ( const LineSet & lines );
	void MakeDisplayList ( const SequentialLineSet & seqlines );
	void MakeDisplayList (const std::vector<VPoint> *sl , const std::vector<std::pair<size_t,size_t>> &drwInt , const std::vector<LineVisualAtt> &visualAttVec);
	//void MakeDisplayList ( const SurfaceWireFrame & wiredata );
	LineVisualAtt & GetVisualAtt ();
	const LineVisualAtt & GetVisualAtt () const;
	void SetVisualAtt ( const LineVisualAtt &newvisatt );
	//virtual void OnlineDraw()const;
	size_t GetLimitBi ()const;
	void SetLimitBi (size_t limitbi);
	size_t GetLimitEi ()const;
	void SetLimitEi (size_t limitei);
	void RemoveLimitsForOnlineDraw ();
protected:
	LineVisualAtt visualatt;
	const std::vector<VPoint> * m_seqLine;
	std::vector<std::pair<size_t,size_t>> m_drwInt;
	std::vector<LineVisualAtt> m_visualAttVec;
	mutable size_t m_limitbi,m_limitei;
	// From LinesDisplay

	// From FacetDisplay
public:
	//Display ();
	Display ( const VRefTriangleMesh & tri );
	Display ( const TriangleMesh & tri );
	//Display ( const FacetDisplay & Foo );
	//Display & operator = ( const Display & Foo );
	//virtual ~FacetDisplay ();
	void DestroyDisplayListFacet();
	void GetHitPointsFacet ( PointSet & selpoints , DoubleVec & depthvalues )const;
	bool IsHitFacet ( vreal & depthvalue ) const;
	void MakeDisplayList ( const VRefTriangleMesh & tri );
	void MakeDisplayList ( const TriangleMesh & tri );
	void MakeDisplayList (const FMesh &tri);
	void MakeDisplayList (std::vector<FMesh *> &triVec);
	//void MakeDisplayList (const std::list<float> &tri);
	void MakeDisplayListByVertexArray ( const VRefTriangleMesh & tri );
	void MakeDisplayListByVertexArray ( OglRefMesh & oglrefmesh );
	FacetVisualAtt & GetVisualAttFacet ();
	const FacetVisualAtt & GetVisualAttFacet () const;
	bool IsReversed ( int index , const VRefTriangleMesh & trimesh );
	bool IsDefinedFacet() const;
	void SetVisualAttFacet ( const FacetVisualAtt & visatt );
	virtual void OnlineDrawFacet()const;
	//void MakeTextureDisplayList (const VRefTriangleMesh *p_tri , const texture *p_texture);
	void MakeTextureDisplayList (const VRefTriangleMesh *p_tri , const VTexture *p_texture);
protected:	
	FacetVisualAtt visualattFacet;
	const FMesh *m_ftri;
	// From FacetDisplay

	// From PointsDisplay
public:
	//PointsDisplay();
	Display ( const PointSet & points );
	//PointsDisplay (const PointsDisplay & Foo);
	//PointsDisplay & operator=(const PointsDisplay & Foo);
	//virtual ~PointsDisplay ();
	void DestroyDisplayListPoints();
	void GetHitPointsPoints ( PointSet & selpoints , DoubleVec & depthvalues , TransformData *dynamicTr) const;
	bool IsHitPoints ( vreal & depthvalue ) const;
	void MakeDisplayList ( const PointSet & points );
	PointVisualAtt & GetVisualAttPoints ();
	const PointVisualAtt & GetVisualAttPoints () const;
	void SetVisualAttPoints ( const PointVisualAtt &newvisatt );
	virtual void OnlineDrawPoints()const;
protected:
	PointVisualAtt visualattPoints;
	// From PointsDisplay
	std::vector<Display *> m_attachedDisplay;
	std::vector<Display *> m_attachedDisplayFacet;
	std::vector<Display *> m_attachedDisplayPoints;
public:
	static size_t QUICK_STEP;
	void MakeDisplayList (std::vector<const Display *> &facetDisplays , std::vector<const Display *> &lineDisplays , std::vector<const Display *> &pointDisplays
		,std::vector<OglMatrix> &curveTmVec);
	const std::vector<VPoint> * GetSeqLine()const;
	unsigned int GetDispListIdLines()const;
	void SetDispListIdLines (unsigned int dispListIdLines);
	unsigned int GetDispListIdFacet ()const;
	void SetDispListIdFacet (unsigned int dispListIdFacet);
	unsigned int GetDispListIdPoints()const;
	void SetDispListIdPoints (unsigned int dispListIdPoints);
	void MakeQuadDisplay (std::vector<QuadPointGenerator *> &quadPoints);
	static int m_maxOnLineItemDrawLimit;
	std::vector<Display *> & GetAttachedDisplay();
	std::vector<Display *> & GetAttachedDisplayFacet();
	std::vector<Display *> & GetAttachedDisplayPoints();
	void DrawAttachedDisplay(bool isSelected)const;
	void DrawAttachedDisplayFacet()const;
	void DrawAttachedDisplayPoints()const;

	unsigned int m_dispListSnapPoints;
	unsigned int m_dispListSnapLines;
	unsigned int m_dispListCustomGeometry;// as cylinder plane box etc.
};

//typedef Display FacetDisplay;
//typedef Display LinesDisplay;
//typedef Display PointsDisplay;

#endif

