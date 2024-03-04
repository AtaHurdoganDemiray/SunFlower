// DelRefMesh.h

#ifndef __DelRefMesh_h__
#define __DelRefMesh_h__
#include "RefTriangleMesh.h"

struct VPoint2d;
struct RefTriangle;
struct VTriangle2dE;
struct VLine2d;
class RefTriangleMesh;

class DelRefMesh
{
public:
	struct Tri
	{
	public:
		Tri ()
		{
			v[0]=-1;
			v[1]=-1;
			v[2]=-1;
			n[0]=-1;
			n[1]=-1;
			n[2]=-1;
		};
		int EqualVertices (const Tri &tochk,int e1[] , int e2[])const;
		void UpdateNeighbor (const int oldne , const int newne);
		int v[3];
		int n[3];
	};
	
	struct tripair
	{
	public:
		tripair (const int _t1,const int _t2)
		:t1(_t1),t2(_t2)
		{};
		int t1,t2; // ref to delrefmesh
		bool operator == (const tripair &tochk)
		{
			if (t1 == tochk.t1 && t2 == tochk.t2)
				return true;
			if (t1 == tochk.t2 && t2 == tochk.t1)
				return true;
			return false;
		}
	};
	
	DelRefMesh (const std::vector<VPoint2d> &points);
	VTriangle2dE GetTri (int i)const;
	const VPoint2d & GetC (const int trino,const int i)const;
	const int GetRC (const int trino,const int i)const;
	const VLine2d GetE (const int trino,const int i)const;
	const VPoint2d GetEV (const int trino , const int i)const;
	int Size()const;
	void Insert (const int i[]);
	VTriangle2dE Back()const;
	std::list<int> Contains (const VPoint2d &p)const;
	const VTriangle2dE SuperTriangle()const;
	void Split (const int trino  // ref to m_refmesh 
				, const int pointno	// ref to m_points
				, std::list<tripair> &swapstack); 

	void Split (const int trino // ref to m_refmesh
				,const int e	// edge no that point lies on
				,const int pointno	// ref to m_points
				,std::list<tripair> &swapstack
				,int &t1ref); 

	void UpdateNeighborsIf (const int trino1 , const int trino2);
	void Swap ( Tri &t1 
			  , Tri &t2 
			  , const int trino1
			  , const int trino2
			  , const int sharedp1 
			  , const int sharedp2 
			  , const int t1unshared 
			  , const int t2unshared 
			  , std::list<tripair> &swapstack);
	Tri CCWTri (const int i[]);
	void SwapIf (const int trino1 , const int trino2 , std::list<tripair> &swapstack);
	void RemoveSuperTrianglePartners (const int r1,const int r2,const int r3);
	void GetResult (RefTriangleMesh &refmesh);
	void GetTriangle (const tripair &corners , int triref[]);
	void RemoveFrom (std::list<tripair> &swapstack , const tripair &toremove);
	const std::vector<Tri> & GetTriMesh()const;
private:
	const std::vector<VPoint2d> &m_points;
	std::vector<Tri> m_refmesh;
};

#endif