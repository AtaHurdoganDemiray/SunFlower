// Voronoi.h

#ifndef __Voronoi_h__
#define __Voronoi_h__

#include "VPoint2d.h"
class RefTriangleMesh;

class Voronoi
{
public:
	struct P
	{
	public:
		int i; // reference to points vec
		std::list<int> e; // reference to polygon edges
	};

	struct E
	{
	public:
		E():deleted(false){};
		int pi[2]; // 2 points that edge seperates
		VPoint2d p; // point on edge line
		VPoint2d v; // vector of edge line
		VPoint2d n; // normal of edge vector
		bool deleted;
	};
	Voronoi (const std::vector<VPoint2d> points
			,const std::list<int> &pointref);
	
	void DelaunayTriangulation (RefTriangleMesh &refmesh);
	const std::vector<P> & GetVP()const;
	const std::vector<E> & GetVE()const;
private:
	const std::vector<VPoint2d> &m_points;
	std::vector<P> m_vp;
	std::vector<E> m_ve;

	const int GetNearestPoint (const int pi , std::list<int> &pref);//pi and pref refers to m_vp
	E & MakePolygonEdge (const int i1 , const int i2); //i1,i2 ref to m_vp
	void DivideList (std::list<int> &pref // reference to points in m_vp
					, E &e
					, std::list<int> &rightlist );
	void RecFill (std::list<int> &pref);// refs to m_vp
	typedef std::list<int>::iterator iitr;
};

#endif