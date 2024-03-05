// SnapEntities.h

#ifndef __SnapEntities_h__
#define __SnapEntities_h__

//#include <vector>
#include "PointSet.h"
#include "LineSet.h"

class SequentialLineSet;
class VArc;
//class SurfaceWireFrame;
//class PointsDisplay;
struct DoubleVec;
class PointSet;
//class LinesDisplay;
struct OglMatrix;
struct TransformData;
class Display;

class SnapEntities
{
public:
	SnapEntities ();
	SnapEntities (const SnapEntities & Foo);	
	SnapEntities & operator =(const SnapEntities & Foo);	
	virtual ~SnapEntities();
	//void CheckAllLoops ( const SurfaceWireFrame & wireframe , const vreal & tol );
	bool IsArc ( const SequentialLineSet * currloop , VArc & formedarc ) const;
	void AddArc ( const VArc & newarc );
	void AddPoint ( const VPoint & snapp );
	void AddPointSet ( const PointSet & snapp );
	void AddLine ( const VLine & snapl );
	void AddLineSet ( const LineSet & snapl );
	void DrawPoints() const;
	void DrawLines()const;
	void GetHitPoints ( PointSet & selpoints , DoubleVec & depthvalues ) const;
	void GetHitLines ( LineSet & sellines , DoubleVec & depthvalues ) const;	
	//void SetXmatrix ( vreal * xmatrix );
	//void AddMatrix (TransformData *x);
	//void ClearAllMatrix();	
	std::vector<VArc *> arcset;
	PointSet snappoints;
	LineSet snaplines;
	//PointsDisplay * snappointsdisplay;
	//vreal tolerance;
	//LinesDisplay * snaplinesdisplay;
	Display * m_display;
	void Transform (const TransformationMatrix &tm);
private:
	TransformData * m_dynamicTr;
public:
	void SetDynamicTransformData (TransformData *dynamicTr); 
	const TransformData * GetDynamicTransformData ()const;
};


#endif
