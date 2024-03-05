// DelRefMesh.cpp

#include "stdafx.h"
#include "DelRefMesh.h"
#include "VTriangle2dE.h"
#include "VPoint2d.h"
#include "VLine2d.h"

#include "VArc2d.h"
#include "RefTriangle.h"
#include "RefTriangleMesh.h"

DelRefMesh::DelRefMesh (const std::vector<VPoint2d> &points)
:m_points(points)
{
	
}
	
VTriangle2dE DelRefMesh::GetTri (int i)const
{
	VTriangle2dE res (m_points[m_refmesh[i].v[0]] 
					, m_points[m_refmesh[i].v[1]] 
					, m_points[m_refmesh[i].v[2]]);
	return res;
}
	
const VPoint2d & DelRefMesh::GetC (const int trino,const int i)const
{
	return m_points [m_refmesh[trino].v[i]];
}
	
const int DelRefMesh::GetRC (const int trino,const int i)const
{
	return m_refmesh[trino].v[i];
}
	
const VLine2d DelRefMesh::GetE (const int trino,const int i)const
{
	return VLine2d (m_points[m_refmesh[trino].v[(i+1)%3]] 
					, m_points[m_refmesh[trino].v[i%3]] );
}
const VPoint2d DelRefMesh::GetEV (const int trino , const int i)const
{
	return m_points[m_refmesh[trino].v[(i+1)%3]] - m_points[m_refmesh[trino].v[i%3]];
}
int DelRefMesh::Size()const
{
	return m_refmesh.size();
}
	
void DelRefMesh::Insert (const int i[])
{
	Tri newtri = CCWTri (i);

	int e1[2],e2[2];
	std::vector<Tri>::iterator ii = m_refmesh.begin();
	std::vector<Tri>::iterator ee = m_refmesh.end();
	int counter = 0;
	for(ii;ii!=ee;++ii)
	{
		int mycount = ii->EqualVertices (newtri , e1 , e2);
		if (mycount == 2)
		{
			for (int w=0;w<3;++w)
			{
				if ( (e1[0] == w && e1[1] == (w+1)%3)
					|| (e1[0] == (w+1)%3 && e1[1] == w) )
				{
					ii->n[w] = (int)m_refmesh.size();
				}
				if ( (e2[0] == w && e2[1] == (w+1)%3)
					|| (e2[0] == (w+1)%3 && e2[1] == w) )
				{
					newtri.n[w] = counter;
				}
			}
		}
		++counter;
	}
	m_refmesh.push_back(newtri);
}
	
VTriangle2dE DelRefMesh::Back()const
{
	const int *a = m_refmesh.back().v;
	return VTriangle2dE (m_points[a[0]],m_points[a[1]],m_points[a[2]]);
}

const vreal DELAUNAY_EDGE_TOL = 1.e-8;

std::list<int> DelRefMesh::Contains (const VPoint2d &p)const
{
	std::list<int> res;
	int mysize = (int)m_refmesh.size();
	for (int i=0;i<mysize;++i)
	{
		const VTriangle2dE currtri = GetTri (i);
		//int ck = currtri.IsInTriangleTol (p,DELAUNAY_EDGE_TOL);
		bool ck = currtri.IsInTriangle (p);
		if (ck)//(ck != 0)
		{
			res.push_back (i);
			res.push_back (ck);
		}
	}
	return res;
}
	
const VTriangle2dE DelRefMesh::SuperTriangle()const
{
	// Calculate min,max;
	VPoint2d mymin (MAX_NUMBER_LIMIT);
	VPoint2d mymax (MIN_NUMBER_LIMIT);
	int mysize = (int)m_points.size();
	for (int i=0;i<mysize;++i)
	{
		const VPoint2d &p1 = m_points[i];
		mymin.ReplaceMin(p1);
		mymax.ReplaceMax(p1);
	}
	VPoint2d cen ((mymin +mymax)*0.5);
	VPoint2d diag (mymax - cen);
	diag *= 10.;
	vreal r = diag.GetLength();
	vreal superr = 2. * r;
	VPoint2d o1 (UY2D);
	VPoint2d o2 (-0.866025 , -0.5);
	VPoint2d o3 (0.866025, -0.5);
	VPoint2d c1 = cen + o1 * superr;
	VPoint2d c2 = cen + o2 * superr;
	VPoint2d c3 = cen + o3 * superr;
	return VTriangle2dE (c1,c2,c3);
}

int DelRefMesh::Tri::EqualVertices (const DelRefMesh::Tri &tochk 
									, int e1[] 
									, int e2[])const
{
	int ecount = 0;
	for (int i=0;i<3;++i)
		for (int j=0;j<3;++j)
		{
			if (v[i] == tochk.v[j])
			{
				if (ecount==2)
					throw VException (( IDS_DelEquIdeTri),1);
				e1[ecount] = i;
				e2[ecount] = j;
				++ecount;
			}
		}
	return ecount;		
}

void DelRefMesh::Tri::UpdateNeighbor(const int oldne, const int newne)
{
	for (int i=0;i<3;++i)
	{
		if (n[i] == oldne)
		{
			n[i] = newne;
			break;
		}
	}
}

void DelRefMesh::Split (const int trino // ref to m_refmesh
						, const int pointno
						, std::list<tripair> &swapstack) // ref to m_points
{
	Tri &t0 = m_refmesh[trino];
	int t1ref = (int)m_refmesh.size();
	int t2ref = (int)m_refmesh.size() + 1;

	Tri t1;
	t1.v[0] = t0.v[1];
	t1.v[1] = t0.v[2];
	t1.v[2] = pointno;
	t1.n[0] = t0.n[1];		swapstack.push_back (tripair(t1.v[0],t1.v[1]));
	t1.n[1] = t2ref;
	t1.n[2] = trino;

	Tri t2;
	t2.v[0] = t0.v[2];
	t2.v[1] = t0.v[0];
	t2.v[2] = pointno;
	t2.n[0] = t0.n[2];		swapstack.push_back (tripair (t2.v[0],t2.v[1]));
	t2.n[1] = trino;
	t2.n[2] = t1ref;

	if (t0.n[2] !=-1)
		m_refmesh[t0.n[2]].UpdateNeighbor (trino,t2ref);
	if (t0.n[1] != -1)
		m_refmesh[t0.n[1]].UpdateNeighbor (trino,t1ref);

	t0.v[2] = pointno;
							swapstack.push_back (tripair (t0.v[0],t0.v[1]));
	t0.n[1] = t1ref;
	t0.n[2] = t2ref;
	m_refmesh.push_back (t1);
	m_refmesh.push_back (t2);
}

void DelRefMesh::Split(const int trino		//ref to m_refmesh
					   , const int e		//edgeno that point lies on
					   , const int pointno	//ref to m_points
					   , std::list<tripair> &swapstack
					   , int &t1ref)	
{
	Tri &t0 = m_refmesh[trino];
	t1ref = (int)m_refmesh.size();
	Tri t1;
	tripair toremove(-1,-1);
	if (e == 0)
	{
		toremove.t1 = t0.v[0]; toremove.t2 = t0.v[1];
		swapstack.push_back (tripair(pointno,t0.v[0]));
		swapstack.push_back (tripair(pointno,t0.v[1]));
		t1.v[0] = pointno;
		t1.v[1] = t0.v[1];
		t1.v[2] = t0.v[2];
		t1.n[0] = -1;
		t1.n[1] = t0.n[1];		swapstack.push_back (tripair(t1.v[1],t1.v[2]));
		t1.n[2] = trino;
		
		if (t0.n[1] != -1)
			m_refmesh[t0.n[1]].UpdateNeighbor (trino,t1ref);

		t0.v[1] = pointno;
		t0.n[1] = t1ref;		
		swapstack.push_back (tripair(t0.v[2],t0.v[0]));
	}
	else if (e == 1)
	{
		toremove.t1 = t0.v[1]; toremove.t2 = t0.v[2];
		swapstack.push_back (tripair(pointno,t0.v[1]));
		swapstack.push_back (tripair(pointno,t0.v[2]));
		t1.v[0] = pointno;
		t1.v[1] = t0.v[2];
		t1.v[2] = t0.v[0];
		t1.n[0] = -1;
		t1.n[1] = t0.n[2];		swapstack.push_back (tripair(t1.v[1],t1.v[2]));
		t1.n[2] = trino;

		if (t0.n[2] != -1)
			m_refmesh[t0.n[2]].UpdateNeighbor (trino,t1ref);

		t0.v[2] = pointno;
		t0.n[2] = t1ref;
								swapstack.push_back (tripair(t0.v[0],t0.v[1]));
	}
	else if (e == 2)
	{
		toremove.t1 = t0.v[2]; toremove.t2 = t0.v[0];
		swapstack.push_back (tripair(pointno,t0.v[2]));
		swapstack.push_back (tripair(pointno,t0.v[0]));
		t1.v[0] = pointno;
		t1.v[1] = t0.v[1];
		t1.v[2] = t0.v[2];
		t1.n[0] = trino;
		t1.n[1] = t0.n[1];		swapstack.push_back (tripair(t1.v[1],t1.v[2]));
		t1.n[2] = -1;

		if (t0.n[1] != -1)
			m_refmesh[t0.n[1]].UpdateNeighbor (trino,t1ref);

		t0.v[2] = pointno;
		t0.n[1] = t1ref;
								swapstack.push_back (tripair(t0.v[0],t0.v[1]));
	}
	RemoveFrom (swapstack , toremove);
	m_refmesh.push_back (t1);
}

void DelRefMesh::UpdateNeighborsIf (const int trino1 , const int trino2)
{
	if (trino1 == -1 || trino2 == -1 || trino1 == trino2)
		return;
	Tri & t1 = m_refmesh[trino1];
	Tri & t2 = m_refmesh[trino2];
	int e1[2] , e2[2];
	int ecount = t1.EqualVertices (t2,e1,e2);
	if (ecount != 2)
		return;
	for (int w=0;w<3;++w)
	{
		if ( (e1[0] == w && e1[1] == (w+1)%3)
			|| (e1[0] == (w+1)%3 && e1[1] == w) )
		{
			t1.n[w] = trino2;
		}
		if ( (e2[0] == w && e2[1] == (w+1)%3)
			|| (e2[0] == (w+1)%3 && e2[1] == w) )
		{
			t2.n[w] = trino1;
		}
	}
}

void DelRefMesh::Swap ( Tri &t1 
					  , Tri &t2 
					  , const int trino1
					  , const int trino2
					  , const int sharedp1 
					  , const int sharedp2 
					  , const int t1unshared 
					  , const int t2unshared 
					  , std::list<tripair> &swapstack)
{
	int na = t1.n[0];
	int nb = t1.n[1];
	int nc = t1.n[2];
	if (na == trino2)
	{
		std::swap (nc,na);
	}
	if (nb == trino2)
	{
		std::swap (nb,nc);
	}

	int nd = t2.n[0];
	int ne = t2.n[1];
	int nf = t2.n[2];
	if (nd == trino1)
	{//edge 1 is shared
		std::swap (nd,nf);
	}
	if (ne == trino1)
	{// edge2 is shared
		std::swap (ne,nf);
	}
	
	// swapping triangle edges
	int ia1[3] = {t1unshared,sharedp1,t2unshared};
	int ia2[3] = {t1unshared,sharedp2,t2unshared};
	t1 = CCWTri (ia1);
	t2 = CCWTri (ia2);
	
	// updating neighbor indices
	UpdateNeighborsIf (trino1,na);
	UpdateNeighborsIf (trino1,nb);
	UpdateNeighborsIf (trino1,nc); // trino1 and trino2
	UpdateNeighborsIf (trino1,nd);
	UpdateNeighborsIf (trino1,ne);
	UpdateNeighborsIf (trino2,na);
	UpdateNeighborsIf (trino2,nb);
	UpdateNeighborsIf (trino2,nd);
	UpdateNeighborsIf (trino2,ne);
	
	// updating swapstack
	if (t1.n[0] != trino2)
		swapstack.push_back (tripair(t1.v[0],t1.v[1]));
	if (t1.n[1] != trino2)
		swapstack.push_back (tripair(t1.v[1],t1.v[2]));
	if (t1.n[2] != trino2)
		swapstack.push_back (tripair(t1.v[2],t1.v[0]));
	if (t2.n[0] != trino1)
		swapstack.push_back (tripair(t2.v[0],t2.v[1]));
	if (t2.n[1] != trino1)
		swapstack.push_back (tripair (t2.v[1],t2.v[2]));
	if (t2.n[2] != trino1)
		swapstack.push_back (tripair(t2.v[2],t2.v[0]));
}


DelRefMesh::Tri DelRefMesh::CCWTri (const int i[])
{
	VPoint2d ed1 = m_points[i[1]] - m_points[i[0]];
	VPoint2d ed2 = m_points[i[2]] - m_points[i[1]];
	vreal ck = ed1 ^ ed2;

	Tri newtri;
	newtri.v[0] = i[0];
	if (ck > 0.0) // ensuring that loop is ccw
	{
		newtri.v[1] = i[1];
		newtri.v[2] = i[2];
	}
	else
	{
		newtri.v[1] = i[2];
		newtri.v[2] = i[1];
	}
	return newtri;
}

void DelRefMesh::SwapIf(const int trino1, const int trino2, std::list<tripair> &swapstack)
{
	Tri &t1 = m_refmesh[trino1];
	Tri &t2 = m_refmesh[trino2];
	int e1[2] = {-1,-1},e2[2]={-1,1};
	t1.EqualVertices (t2 , e1,e2);
	
	if (e1[0] == -1 || e1[1] == -1 || e2[0] == -1 || e2[1] == -1)
		throw VException (( IDS_DelSwaExc),1);

	int sharedp1 = t1.v[e1[0]]; // ref to m_points
	int sharedp2 = t1.v[e1[1]]; // ref to m_points
	
	if ( (sharedp1 != t2.v[e2[0]] && sharedp1 != t2.v[e2[1]])
		|| (sharedp2 != t2.v[e2[0]] && sharedp2 != t2.v[e2[1]]) )
		throw VException (( IDS_DelSwaExc1),2);

	int t1unshared,t2unshared;
	if (e1[0] != 0 && e1[1] != 0)
		t1unshared = t1.v[0];
	else if (e1[0] != 1 && e1[1] != 1)
		t1unshared = t1.v[1];
	else if (e1[0] != 2 && e1[1] != 2)
		t1unshared = t1.v[2];
	if (e2[0] != 0 && e2[1] != 0)
		t2unshared = t2.v[0];
	else if (e2[0] != 1 && e2[1] != 1)
		t2unshared = t2.v[1];
	else if (e2[0] != 2 && e2[1] != 2)
		t2unshared = t2.v[2];

	const VPoint2d &p1 = m_points[t1unshared];
	const VPoint2d &p2 = m_points[sharedp1]; 
	const VPoint2d &p3 = m_points[sharedp2];
	const VPoint2d &p4 = m_points[t2unshared];

	vreal solutionx , solutiony;
	bool ck = VArc2d::Calculate (p1.x(),p1.y(),p2.x(),p2.y(),p3.x(),p3.y(),solutionx,solutiony);
	if (ck)
	{
		VPoint2d center (solutionx,solutiony);
		VPoint2d dif = center - p4;
		VPoint2d myr=p1 - center;
		if (dif.GetLength2() <  myr.GetLength2())
		{
			Swap (t1,t2,trino1,trino2,sharedp1,sharedp2,t1unshared,t2unshared,swapstack);
			// for debug
			// see if other combination also do no satisfy delaunay
			//vreal solx,soly;
			//bool ck2 = VArc2d::Calculate (p1.x(),p1.y(),p4.x(),p4.y(),p2.x(),p2.y(),solx,soly);
			//VPoint2d tmpcen (solx,soly);
			//if ((tmpcen - p3).GetLength2() < (p2 - tmpcen).GetLength2())
			//	int stp = 1;
		}
	}
}

void DelRefMesh::RemoveSuperTrianglePartners(const int r1, const int r2, const int r3)
{
	std::vector<Tri> newrefmesh;
	int mysize = (int)m_refmesh.size();
	for (int i=0;i<mysize;++i)
	{
		Tri &t = m_refmesh[i];
		if (t.v[0] == r1 || t.v[0] == r2 || t.v[0] == r3
			|| t.v[1]== r1 || t.v[1] == r2 || t.v[1] == r3
			|| t.v[2] == r1 || t.v[2] == r2 || t.v[2] == r3)
		{
			// partner of supertriangle 
			// do not add to refmesh
		}
		else
		{
			newrefmesh.push_back (t);
		}
	}
	m_refmesh = newrefmesh;
}

void DelRefMesh::GetResult(RefTriangleMesh &refmesh)
{
	std::vector<Tri>::iterator i = m_refmesh.begin();
	std::vector<Tri>::iterator e = m_refmesh.end();
	for (i;i!=e;++i)
	{
		RefTriangle rt (i->v[0],i->v[1],i->v[2]);
		refmesh.AddTriangle (rt);
	}
}

void DelRefMesh::GetTriangle (const tripair &corners , int triref[])
{
	std::vector<Tri>::iterator i = m_refmesh.begin();
	std::vector<Tri>::iterator e = m_refmesh.end();
	int counter = 0;
	int resno = 0;
	for (i;i!=e;++i)
	{
		int ck=0;
		if (i->v[0] == corners.t1 || i->v[0] == corners.t2)
			++ck;
		if (i->v[1] == corners.t1 || i->v[1] == corners.t2)
			++ck;
		if (i->v[2] == corners.t1 || i->v[2] == corners.t2)
			++ck;
		if (ck == 2)
		{
			triref[resno] = counter;
			++resno;
			if (resno == 2)
				break;
		}
		++counter;
	}
}

void DelRefMesh::RemoveFrom(std::list<tripair> &swapstack, const DelRefMesh::tripair &toremove)
{
	std::list<tripair>::iterator i=swapstack.begin();
	std::list<tripair>::iterator e=swapstack.end();
	for (i;i!=e;)
	{
		if (*i == toremove)
		{
			i = swapstack.erase (i);
			continue;
		}
		++i;
	}
}

const std::vector<DelRefMesh::Tri> & DelRefMesh::GetTriMesh ()const
{
	return m_refmesh;
}
