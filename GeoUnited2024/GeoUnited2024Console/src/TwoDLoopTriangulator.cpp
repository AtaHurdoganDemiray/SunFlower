// TwoDLoopTriangulator.cpp

#include "stdafx.h"
#include "TwoDLoopTriangulator.h"
#include "BspTree.h"
#include "BspTreeConstructor.h"
#include "LineSet2d.h"
#include "BspTreeBooleanOp.h"
#include "BspTreeToLineSet2d.h"
#include "LineSet2d2SequentialLineSet2d.h"
#include "SequentialLineSet2d.h"
#include "VRefTriangleMesh.h"
#include "VPolygon.h"
#include "DelRefMesh.h"
#include "IncDel.h"

TwoDLoopTriangulator::TwoDLoopTriangulator(const SequentialLineSet2d *outerLoop
						, const std::vector<SequentialLineSet2d*> &innerLoopVec
						,const std::vector<vreal> &ugrid
						,const std::vector<vreal> &vgrid)
:m_outerLoop (outerLoop),m_innerLoopVec (innerLoopVec),m_ugrid(ugrid),m_vgrid(vgrid)
{
	m_bspOuter = 0;
}

TwoDLoopTriangulator::~TwoDLoopTriangulator()
{
	delete m_bspOuter;
	int mysize=(int)m_bspInnerVec.size();
	for (int i=0;i<mysize;++i)
	{
		delete m_bspInnerVec[i];
	}
}

void TwoDLoopTriangulator::MakeBspTrees()
{
	m_bspOuter = MakeSingleBsp (m_outerLoop);
	int mysize = (int)m_innerLoopVec.size();
	for (int i=0;i<mysize;++i)
	{
		m_bspInnerVec.push_back ( MakeSingleBsp (m_innerLoopVec[i]));
	}
}

geo::BspTree * TwoDLoopTriangulator::MakeSingleBsp(const SequentialLineSet2d *loop)
{
	geo::BspTreeConstructor cons;
	geo::BspTree * newbsp = cons.ConstructTree (*loop);
	return newbsp;
}
VRefTriangleMesh * TwoDLoopTriangulator::TriangulateOuterLoop(const vreal step)
{
	m_res = new VRefTriangleMesh (100);
	int mysize3 = m_outerLoop->GetPointSize();
	std::vector<int>ccwp(mysize3-1);
	VPoint m,n;
	for (int w=0;w<mysize3-1;++w)
	{
		ccwp[w] = m_res->surfpack.InsertPoint (*m_outerLoop->GetPoint(w),m,n);
	}
	// and triangulate each VPolygon with Delaunay
	TriangulateLoopInsertPoints(ccwp,step);
	//TriangulateQuadLoop (ccwp);
	return m_res;
}
VRefTriangleMesh * TwoDLoopTriangulator::Triangulate()
{
	m_res = new VRefTriangleMesh (200);
	MakeBspTrees();
	MakeQuads();
	// for each quad 
	int mysize = (int)m_qv.size();
	for (int i=0;i<mysize;++i)
	{
		// calculate a bsptree 
		const Quad &q = m_qv[i];
		geo::BspTree *quadbsp = MakeQuadBsp (q);
		// find its intersection with loop bsp trees
		geo::BspTree * remainingBsp = QuadBspLoopsInt (quadbsp);
		delete quadbsp;
		// then explode intersection Bsp to sequential line sets
		geo::BspTreeToLineSet2d converter;
		LineSet2d * linemesh = converter.Make (*remainingBsp);
		LineSet2d2SequentialLineSet2d converter2;
		std::vector<SequentialLineSet2d *>explosion = converter2.Make (*linemesh);
		delete linemesh;
		// for each sequential line set
		int mysize2 = (int)explosion.size();
		for (int j=0;j<mysize2;++j)
		{
			// load sequentialline set points to result vrefmesh
			SequentialLineSet2d *currloop = explosion[j];
			//loops.push_back (currloop);
			// make VPolygons out of each sequential line set 
			
			int mysize3 = currloop->GetPointSize();
			std::vector<int>ccwp (mysize3-1);
			VPoint m,n;
			for (int w=0;w<mysize3-1;++w)
			{
				ccwp[3] = m_res->surfpack.InsertPoint (*currloop->GetPoint(w),m,n);
			}
			// and triangulate each VPolygon with Delaunay
			TriangulateQuadLoop(ccwp);
			delete currloop;	
		}
	}

	return m_res;
}

void TwoDLoopTriangulator::MakeQuads()
{
	int usize = (int)m_ugrid.size() - 1;
	int vsize = (int)m_vgrid.size() - 1;
	for (int u=0;u<usize;++u)
	{
		const vreal uleft = m_ugrid[u];
		const vreal uright = m_ugrid[u+1];
		for (int v=0;v<vsize;++v)
		{
			const vreal vtip= m_vgrid[v];
			const vreal vtop = m_vgrid[v+1];
			m_qv.push_back (Quad (uleft,uright,vtip,vtop));

		}
	}
}

TwoDLoopTriangulator::Quad::Quad(const vreal leftu, const vreal rightu, const vreal tipv, const vreal topv)
{
	c[0].x(rightu);
	c[0].y(topv);
	c[1].x(leftu);
	c[1].y(topv);
	c[2].x(leftu);
	c[2].y(tipv);
	c[3].x(rightu);
	c[3].y(tipv);
}

geo::BspTree * TwoDLoopTriangulator::MakeQuadBsp(const TwoDLoopTriangulator::Quad &q)
{
	LineSet2d ls (4);
	ls.AddLine (VLine2d (q.c[0],q.c[1]));
	ls.AddLine (VLine2d (q.c[1],q.c[2]));
	ls.AddLine (VLine2d (q.c[2],q.c[3]));
	ls.AddLine (VLine2d (q.c[3],q.c[0]));
	geo::BspTreeConstructor cons;
	return cons.ConstructTree (ls);
}

geo::BspTree * TwoDLoopTriangulator::QuadBspLoopsInt(geo::BspTree *quadbsp)
{
	geo::BspTree *bspi1 = geo::BspTreeBooleanOp::Intersect (*quadbsp,*m_bspOuter);		
	int mysize = (int)m_bspInnerVec.size();
	
	geo::BspTree * bspf = bspi1;
	for (int i=0;i<mysize;++i)
	{
		geo::BspTree *innerbsp = m_bspInnerVec[i];
		geo::BspTree * todelete = bspf;
		bspf = geo::BspTreeBooleanOp::Intersect (*bspf,*innerbsp);
		delete todelete;
	}
	return bspf;
}

void TwoDLoopTriangulator::TriangulateQuadLoop (std::vector<int> &polygonloop)
{
	// this function gets ccw sorted sequences of loop points 
	// which are considered to form a ccw polygon
	// first polygon is formed
	// then it is triangulated by ear clipping
	// and triangles are improved by delaunay method
	// and finaly results are recorded to main triangle mesh
	VPolygon accwP (m_res->surfpack.parampoints.GetVec(), polygonloop);
	// Triangulate polygon
	std::vector<VPolygon> clippedToTris;
	accwP.Triangulate (clippedToTris);
	// delaunay refinement
	std::vector<VPolygon>::iterator clippedi = clippedToTris.begin();
	std::vector<VPolygon>::iterator clippede = clippedToTris.end();
	DelRefMesh mydelmesh (m_res->surfpack.parampoints.GetVec());
	for (clippedi;clippedi!=clippede;++clippedi)
	{
		int triref[3] = {clippedi->m_polygonPoints[0],clippedi->m_polygonPoints[1],clippedi->m_polygonPoints[2]};
		mydelmesh.Insert (triref);
	}
	std::list<DelRefMesh::tripair> swapstak;
	IncDel::BuildSwapStack (mydelmesh,swapstak);
	IncDel::SwapTrianglesTill (swapstak,mydelmesh);
	// add triangles to final mesh
	const std::vector<DelRefMesh::Tri> &tv=mydelmesh.GetTriMesh();
	std::vector<DelRefMesh::Tri>::const_iterator tvi = tv.begin();
	std::vector<DelRefMesh::Tri>::const_iterator tve = tv.end();
	for (tvi;tvi!=tve;++tvi)
	{
		RefTriangle reft (tvi->v);
		m_res->AddTriangle (reft);
	}
}

void TwoDLoopTriangulator::TriangulateLoopInsertPoints (std::vector<int> &polygonloop,const vreal step)
{
	// this function gets ccw sorted sequences of loop points 
	// which are considered to form a ccw polygon
	// first polygon is formed
	// then it is triangulated by ear clipping
	// and triangles are improved by delaunay method
	// and finaly results are recorded to main triangle mesh
	VPolygon accwP (m_res->surfpack.parampoints.GetVec(), polygonloop);
	// Triangulate polygon
	std::vector<VPolygon> clippedToTris;
	accwP.Triangulate (clippedToTris);
	// delaunay refinement
	std::vector<VPolygon>::iterator clippedi = clippedToTris.begin();
	std::vector<VPolygon>::iterator clippede = clippedToTris.end();
	DelRefMesh mydelmesh (m_res->surfpack.parampoints.GetVec());
	for (clippedi;clippedi!=clippede;++clippedi)
	{
		int triref[3] = {clippedi->m_polygonPoints[0],clippedi->m_polygonPoints[1],clippedi->m_polygonPoints[2]};
		std::vector <int>trisdivided;
		int recCount = 0;
		DivideTriTill (triref,step,trisdivided,recCount);
		int mysize4=(int)trisdivided.size();
		for (int j=0;j<mysize4;++j)
		{
			int temp[3] = {trisdivided[j],trisdivided[j+1],trisdivided[j+2]};
			++j;++j;
			mydelmesh.Insert (temp);
		}
	}
	std::list<DelRefMesh::tripair> swapstak;
	IncDel::BuildSwapStack (mydelmesh,swapstak);
	IncDel::SwapTrianglesTill (swapstak,mydelmesh);
	// add triangles to final mesh
	const std::vector<DelRefMesh::Tri> &tv=mydelmesh.GetTriMesh();
	std::vector<DelRefMesh::Tri>::const_iterator tvi = tv.begin();
	std::vector<DelRefMesh::Tri>::const_iterator tve = tv.end();
	for (tvi;tvi!=tve;++tvi)
	{
		RefTriangle reft (tvi->v);
		m_res->AddTriangle (reft);
	}
}

const int TWODLOOPTRIANGULATORRECLIMIT = 1200;

void TwoDLoopTriangulator::DivideTriTill (const int i[]
										, const vreal step
										, std::vector<int> &tris
										,int &recCount)
{
	recCount++;
	if (recCount > TWODLOOPTRIANGULATORRECLIMIT)
		return;
	vreal step2 = step * step;
	VTriangle2dE tri (*m_res->surfpack.parampoints[i[0]]
					,*m_res->surfpack.parampoints[i[1]]
					,*m_res->surfpack.parampoints[i[2]]);
	const vreal e1l = tri.GetEdge1().GetLength2();
	const vreal e2l = tri.GetEdge2().GetLength2();
	const vreal e3l = tri.GetEdge3().GetLength2();
	if (e1l <= step2 && e2l <= step2 && e3l <= step2)
	{
		// add tri
		tris.push_back (i[0]);
		tris.push_back (i[1]);
		tris.push_back (i[2]);
	}
	else if (e1l >= e2l && e1l >= e3l)
	{
		// divide tri
		VPoint2d mp = tri.GetCorner1() + tri.GetEdge1() * 0.5;
		int ci = m_res->surfpack.InsertPoint (mp,VPoint (),VPoint());
		int t1[3] = {i[0],ci,i[2]};
		int t2[3] = {ci,i[1],i[2]};		
		DivideTriTill (t1,step,tris,recCount);
		DivideTriTill (t2,step,tris,recCount);
	}
	else if (e2l >= e1l && e2l >= e3l)
	{
		VPoint2d mp = tri.GetCorner1() + tri.GetEdge2() * 0.5;
		int ci = m_res->surfpack.InsertPoint (mp,VPoint(),VPoint());
		int t1[3] = {i[2],ci,i[1]};
		int t2[3] = {ci,i[0],i[1]};
		DivideTriTill (t1,step,tris,recCount);
		DivideTriTill (t2,step,tris,recCount);
	}
	else if (e3l >= e2l && e3l >= e1l)
	{
		VPoint2d mp = tri.GetCorner2() + tri.GetEdge3() * 0.5;
		int ci = m_res->surfpack.InsertPoint (mp,VPoint(),VPoint());
		int t1[3] = {i[1],ci,i[0]};
		int t2[3] = {ci,i[2],i[0]};
		DivideTriTill (t1,step,tris,recCount);
		DivideTriTill (t2,step,tris,recCount);
	}
}
