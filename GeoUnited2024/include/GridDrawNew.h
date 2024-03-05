// GridDrawNew.h

#ifndef __GridDrawNew_h__
#define __GridDrawNew_h__

#include "CoordinateSystem.h"
#include "LineVisualAtt.h"
#include "VColor.h"
#include "FacetVisualAtt.h"
#include "PointSet.h"
//#include "FacetDisplay.h"

class VisualAttLoader;
class PointSet;
struct DoubleVec;

class GridDrawNew
{
public:
	GridDrawNew(const CoordinateSystem &cs,const VisualAttLoader *visAttLoader);

	const CoordinateSystem & GetCs()const;
	double GetUnitLength()const;
	void SetUnitLength (double unitLength);
	int GetXCellCount ()const;
	void SetXCellCount (int xCellCount);
	int GetYCellCount ()const;
	void SetYCellCount (int yCellCount);
	double GetXShift ()const;
	void SetXShift (double xShift);
	double GetYShift ()const;
	void SetYShift (double yShift);
	double GetZShift ()const;
	void SetZShift (double zShift);
	bool GetIsVisible ()const;
	void SetIsVisible (bool isVisible);

	void Draw ();
	//void DrawAbs ();
	void GetHitPointsNew (PointSet &selpoints 
							, DoubleVec &depthvalues
							,const VPoint &curserp
							,const VPoint &lookdir
							,double selr)const;
	const LineVisualAtt & GetVisAtt()const;
	void SetVisAtt (const LineVisualAtt &visAtt);
	const LineVisualAtt & GetAbsLineVisAtt()const;
	void SetAbsLinesVisAtt (const LineVisualAtt &absLineVisAtt);
	double GetAbsLinesLength()const;
	void SetAbsLinesLength (double absLinesLength);
private:
	const CoordinateSystem &m_cs;
	double m_unitLength;
	int m_xCellCount;
	int m_yCellCount;
	double m_xShift;
	double m_yShift;
	double m_zShift;
	bool m_isVisible;
	LineVisualAtt m_visAtt;
	const VisualAttLoader *m_visAttLoader;
	VColor m_selcolor;

	// Absolute coordinates drawing
	bool m_drawAbsCoords;
	//FacetVisualAtt m_whiteVisAtt;
	//FacetVisualAtt m_blackVisAtt;
	//PointSet m_blackMesh;
	//PointSet m_whiteMesh;
	void BuildAbsCoordGraphics();
	LineVisualAtt m_absLineVisAtt;
	PointSet m_absLinePoints;
	double m_absLinesLength;
public:
	void GetMinMax(VPoint &mn , VPoint &mx)const;
};

#endif