#pragma once
#include "sVector.h"
#include "VPoint2d.h"
#include <vector>
class COpenGLControl;
class SequentialLineSet2d;
class VPolygon
{
public:
	enum vertexType{convex, collinear, reflex};

public:
	VPolygon(const std::vector<VPoint2d> &pointPool, std::vector<int> &polygonPoints);
	~VPolygon(void);
	bool VertexIsVisible(int v1, int v2) const;
	bool PolygonIsConvex() const;
	//void RemoveCollinear(VPolygon &vPol);
	vertexType GetType(int vertexNo) const;
	int GetVertexCount() const;
	sVector<double,2> GetVertex(int v);
	int IsVertex(const sVector<double, 2> &v);
    void TriangulateConvexPolygon(std::vector<sVector<int, 3> > TriPol);
	bool DivideToConvexPolygons(std::list<VPolygon> convexPolygons);


	VPolygon GetRightPolygon(int i0, int i1);
	VPolygon GetLeftPolygon(int i0, int i1);
	bool GetADiagonal(int &i0, int &i1);
	void Triangulate(std::vector<VPolygon> &triangles, VPolygon &left, VPolygon &right);
	void Triangulate(std::vector<VPolygon> &triangles);

	const VPoint2d &GetPoint( int i )const;
	bool IsDiagonal( int i0, int i1 )const;
	bool SegmentIntersect( const VPoint2d &a1, const VPoint2d &a2, const VPoint2d &b1, const VPoint2d &b2 )const;
	double Kross2d( VPoint2d &sv1, VPoint2d &sv2)const;
	bool SegmentInCone( const VPoint2d &v0, const VPoint2d &v1, const VPoint2d &vM, const VPoint2d &vP )const;
	bool IsCCWPolygonConvex () const;
	
	const std::vector<VPoint2d> * m_pointPool;
	std::vector<int> m_polygonPoints;
	SequentialLineSet2d * MakeSequentialLineSet ();
private:

	
};
