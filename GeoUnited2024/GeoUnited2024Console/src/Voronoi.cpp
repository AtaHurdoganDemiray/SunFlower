// Voronoi.cpp

#include "stdafx.h"
#include "Voronoi.h"
#include "RefTriangleMesh.h"

Voronoi::Voronoi(const std::vector<VPoint2d> points
				 ,const std::list<int> &pointref)
:m_points(points)
{
	std::list<int>::const_iterator i = pointref.begin();
	std::list<int>::const_iterator e = pointref.end();
	for (i;i!=e;++i)
	{
		P np;
		np.i = *i;
		m_vp.push_back (np);
	}

	int mysize = (int)m_vp.size();
	std::list<int> pref;
	for (int i=0;i<mysize;++i)
	{
		pref.push_back(i);
	}
	while(pref.size())
	{
		RecFill (pref);
		pref.pop_front();
	}
}

const int Voronoi::GetNearestPoint (const int pi 
									, std::list<int> &pref)
{
	P &p = m_vp[pi];
	iitr i = pref.begin();
	iitr e = pref.end();
	vreal mind2 (MAX_NUMBER_LIMIT);
	int nearestpi = -1;
	for (i;i!=e;)
	{
		if (*i==pi)
		{
			++i;
			continue;
		}
		
		//// check if currp is already neighbor
		//iitr ii = p.e.begin();
		//iitr ee = p.e.end();
		//bool isneighbor = false;
		//for (ii;ii!=ee;++ii)
		//{
		//	E &curre = m_ve[*ii];
		//	if (curre.pi[0] == *i || curre.pi[1] == *i)
		//	{
		//		isneighbor = true; // points has been recorded as neighbors
		//		break;
		//	}
		//}
		//if (isneighbor)
		//{
		//	++i;
		//	continue;
		//}
		const P &currp = m_vp[*i];
		const vreal d2 = (m_points[p.i] - m_points[currp.i]).GetLength2();
		
		if (d2 < EPSILON)
		{// duplicate point remove it
			i = pref.erase (i);
			continue;
		}
		
		if (d2 < mind2)
		{
			nearestpi = *i;
			mind2 = d2;
		}
		++i;
	}
	return nearestpi;
}

Voronoi::E & Voronoi::MakePolygonEdge(const int i1, const int i2)//i1,i2 ref to m_vp
{
	P &p1 = m_vp[i1];
	P &p2 = m_vp[i2];
	E ne;
	ne.p = (m_points[p1.i] + m_points[p2.i]) * 0.5;
	ne.n = m_points[p2.i] - m_points[p1.i];
	ne.n.NormThis();
	ne.v.x (-ne.n.y());
	ne.v.y (ne.n.x());
	ne.pi[0] = i1;
	ne.pi[1] = i2;
	m_ve.push_back (ne);
	int er = (int)m_ve.size()-1;
	p1.e.push_back(er);
	p2.e.push_back(er);
	return m_ve.back();
}

void Voronoi::DivideList (std::list<int> &pref
						  , Voronoi::E &e
						  , std::list<int> &rightlist)
{
	rightlist.push_back (e.pi[0]);
	iitr i=pref.begin(); //iterator to refs to m_vp
	iitr ee=pref.end();
	
	for (i;i!=ee;++i)
	{
		if (*i == e.pi[0] || *i == e.pi[1])
			continue;

		P &currp = m_vp[*i];
		vreal dp = (m_points[currp.i] - e.p) % e.n;
		if (dp <= 0.0)
			rightlist.push_back (*i);
	}
}

void Voronoi::RecFill(std::list<int> &pref)
{
	if (pref.size() == 1)
		return; //recursive leaf

	int currpi = pref.front();
	int nearpi = GetNearestPoint (currpi,pref);
	if (nearpi == -1)
		return; //recursive leaf
	E &e = MakePolygonEdge (currpi,nearpi);
	std::list<int> rightlist;
	DivideList (pref,e,rightlist);
	RecFill (rightlist);
}

void Voronoi::DelaunayTriangulation(RefTriangleMesh &refmesh)
{
	int mysize = (int)m_ve.size();
	for(int i=0;i<mysize;++i)// for all edges
	{//e1
		E &e1 = m_ve[i];
		if (e1.deleted)
			continue;
		P &p1 = m_vp[e1.pi[0]];
		std::list<int>::iterator ii = p1.e.begin();
		std::list<int>::iterator ee = p1.e.end();
		for (ii;ii!=ee;)// for all edges of p1 of e1
		{//e2
			if (*ii == i)// e1 == e2
			{
				++ii;
				continue;
			}
			E &e2 = m_ve[*ii];
			int p3 = e2.pi[0];
			if (p3 == e1.pi[0])
				p3 = e2.pi[1];
			if (e1.pi[1] == p3)
			{
				RefTriangle rt(e1.pi[0],e1.pi[1],p3);
				refmesh.AddTriangle(rt);
				e1.deleted = true;
				e2.deleted = true;
				ii = p1.e.erase (ii);
				continue;
			}
			++ii;
		}
	}
}

const std::vector<Voronoi::P> & Voronoi::GetVP()const
{
	return m_vp;
}

const std::vector<Voronoi::E> & Voronoi::GetVE()const
{
	return m_ve;
}
