// IncDel.cpp

#include "stdafx.h"
#include "IncDel.h"
#include "VPoint2d.h"
#include "VTriangle2dE.h"

void IncDel::IncDel2d(std::vector<VPoint2d> &points
					  , std::list<int> &pref
					  , RefTriangleMesh &refmesh)
{
	DelRefMesh mesh(points);
	std::list<DelRefMesh::tripair> swapstack;
	VTriangle2dE mysupertri = mesh.SuperTriangle ();
	int mysize = points.size();
	points.push_back (mysupertri.GetCorner1());
	points.push_back (mysupertri.GetCorner2());
	points.push_back (mysupertri.GetCorner3());
	int rii[3] = {mysize,mysize+1,mysize+2};
	mesh.Insert (rii);

	iitr i=pref.begin();
	iitr e =pref.end();
	for (i;i!=e;++i)
	{
		std::list<int> c = mesh.Contains (points[*i]);
		int mysize = (int)c.size();
		if (mysize == 2)
		{
			int triref = c.front();
			c.pop_front();
			int cktype = c.front();
			c.pop_front();
			if (cktype == 4)
			{
				//inside one triangle
				mesh.Split (triref,*i,swapstack);
				//SwapTrianglesTill (swapstack,mesh);
			}
			else if (cktype == 1 || cktype == 2 || cktype==3)
			{
				//on the edge of one triangle
				int tref;
				mesh.Split (triref,cktype-1,*i,swapstack,tref);
				//SwapTrianglesTill (swapstack,mesh);
			}
		}
		else if(mysize == 4)
		{
			//on edges of 2 triangle
			int trino1 = c.front(); 
			c.pop_front();
			int cktype1 = c.front(); 
			c.pop_front();
			if (cktype1 == 1 || cktype1 == 2 || cktype1 == 3)
			{
				int t1ref;
				int mye1;// edge orders of VTriangle2dE & DelRefMesh::Tri are different				
				if (cktype1 == 1)
					mye1 = 0;
				if (cktype1 == 2)
					mye1 = 2;
				if (cktype1 == 3)
					mye1 = 1;
				mesh.Split (trino1,mye1,*i,swapstack,t1ref);			
				int trino2 = c.front();
				c.pop_front();
				int cktype2 = c.front();
				c.pop_front();
				if (cktype2 == 1 || cktype2 == 2 || cktype2 == 3)
				{
					int mye2;// edge orders of VTriangle2dE & DelRefMesh::Tri are different	
					if (cktype2 == 1)
						mye2 = 0;
					if (cktype2 == 2)
						mye2 = 2;
					if (cktype2 == 3)
						mye2 = 1;
					int t2ref;
					mesh.Split (trino2,mye2,*i,swapstack,t2ref);
					mesh.UpdateNeighborsIf (trino1,trino2);
					mesh.UpdateNeighborsIf (trino1,t2ref);
					mesh.UpdateNeighborsIf (t1ref,trino2);
					mesh.UpdateNeighborsIf (t1ref,t2ref);
				}
				//SwapTrianglesTill (swapstack,mesh);
			}
		} 
	}
	
	SwapTrianglesTill (swapstack,mesh);

	//// remove super triangle points
	points.pop_back();
	points.pop_back();
	points.pop_back();
	// remove super triangle point refs
	mesh.RemoveSuperTrianglePartners(mysize,mysize+1,mysize+2);
	mesh.GetResult (refmesh);
}

void IncDel::SwapTrianglesTill (std::list<DelRefMesh::tripair> &swapstack
								,DelRefMesh &mesh)
{
	int mycounter = 0;
	while (swapstack.size())
	{
		DelRefMesh::tripair mypair = swapstack.front ();
		swapstack.pop_front();
		if (mypair.t1 == -1 || mypair.t2 == -1)
			continue;
		int triref[2] ={-1,-1};
		mesh.GetTriangle (mypair,triref);
		if (triref[0] != -1 && triref[1] != -1)
			mesh.SwapIf (triref[0],triref[1],swapstack);
		++mycounter;
		if (mycounter == 500000)
			break;
	}
}

void IncDel::BuildSwapStack(const DelRefMesh &mesh
							, std::list<DelRefMesh::tripair> &swapstack)
{
	std::vector<DelRefMesh::Tri> vec (mesh.GetTriMesh());

	int mysize = (int)vec.size();
	for (int i=0;i<mysize;++i)
	{
		DelRefMesh::Tri &t1 = vec[i];
		int e1n = t1.n[0];
		int e2n = t1.n[1];
		int e3n = t1.n[2];
		if (e1n!=-1)
		{
			swapstack.push_back (DelRefMesh::tripair ( t1.v[0],t1.v[1]));
			DelRefMesh::Tri &ct = vec[e1n];
			CancelNeighborEdge (ct,i);
		}
		if (e2n!=-1)
		{
			swapstack.push_back (DelRefMesh::tripair ( t1.v[1] , t1.v[2]));
			DelRefMesh::Tri &ct = vec[e2n];
			CancelNeighborEdge (ct,i);
		}
		if (e3n != -1)
		{
			swapstack.push_back (DelRefMesh::tripair ( t1.v[2] , t1.v[0]));
			DelRefMesh::Tri &ct = vec[e3n];
			CancelNeighborEdge (ct,i);
		}
	}
}

void IncDel::CancelNeighborEdge (DelRefMesh::Tri &ct , int i)
{
	if (ct.n[0] == i)
	{
		ct.n[0] = -1;
	}
	if (ct.n[1] == i)
	{
		ct.n[1] = -1;
	}
	if (ct.n[2] == i)
	{
		ct.n[2] = -1;
	}
}
