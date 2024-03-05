// PolygonToTriangles.cpp

#include "stdafx.h"
#include "PolygonToTriangles.h"
#include "BspTree.h"
#include "SequentialLineSet2d.h"
#include "TriangleMesh2d.h"
#include "PartitionLine.h"
#include "LineSet2d.h"

namespace geo
{
	void PolygonToTriangles::Triangulate (const SequentialLineSet2d &sl , const BspTree &T , TriangleMesh2d &tri)
	{
		int n = sl.Size();
		if (n==3)
		{
			VTriangle2dE newtri (*(sl.GetPoint(0)) , *(sl.GetPoint(1)) , *(sl.GetPoint(2)) );
			tri.AddTriangle(newtri);
			return;
		}

		for (int i0=0;i0<n+1;i0++)
		{
			VPoint2d p1(*(sl.GetPoint(i0)));		
			for (int i1=0;i1<n+1;i1++)
			{
				int d1 = abs(i1-i0);
				int d2 = n - d1;
				if (d1<2 || d2<2)
					continue;

				VPoint2d p2(*(sl.GetPoint(i1)));
				VLine2d currL (p1,p2);
				if(IsDiagonal (currL,T))
				{
					int j0,j1;
					if (i0>i1)
					{
						j0=i1;
						j1=i0;
					}
					else
					{
						j0=i0;
						j1=i1;
					}
					int d = j1-j0;
					SequentialLineSet2d sub1(d+1);
					SequentialLineSet2d sub2((n-d)+1);
					Split (sl,j0,j1,sub1,sub2);
					Triangulate (sub1,T,tri);
					Triangulate (sub2,T,tri);
					return;
				}
			}
		}
	}

	void PolygonToTriangles::Split (const SequentialLineSet2d &sl,int i0,int i1
									,SequentialLineSet2d &sub1 , SequentialLineSet2d &sub2)
	{
		int n=sl.Size();
		const VPoint2d *firstP = sl.GetPoint(i0);
		const VPoint2d *lastP = sl.GetPoint(i1);

		for (int i=i0;i<=i1;i++)
		{
			const VPoint2d *p = sl.GetPoint(i);
			sub1.InsertPoint (*p);
		}
		sub1.InsertPoint (*firstP);

		for (int i=i1;i<n;i++)
		{
			const VPoint2d *p = sl.GetPoint(i);
			sub2.InsertPoint(*p);
		}//n th point is not added it is equal to 0 th point
		for (int i=0;i<=i0;i++)
		{
			const VPoint2d *p = sl.GetPoint(i);
			sub2.InsertPoint(*p);
		}
		sub2.InsertPoint (*lastP);
	}
	bool PolygonToTriangles::IsDiagonal(const VLine2d &l, const geo::BspTree &T)
	{
		LineSet2d Pos(1),Neg(1),CoSame(1),CoDiff(1);
		GetPartition (T , l , Pos,Neg,CoSame,CoDiff);
		if (Neg.Size() == 0 && CoSame.Size()==0 &&CoDiff.Size()==0)
			return true;
		else
			return false;		
	}
};
