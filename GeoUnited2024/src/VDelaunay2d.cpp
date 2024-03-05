// VDelaunay2d.cpp

#include "stdafx.h"
#include "VDelaunay2d.h"
#include "MgcDelaunay2d.h"
#include "PointSet2d.h"
#include "TriangleMesh2d.h"

#include "RefTriangleMesh.h"
#include "PosCubes2d.h"

VDelaunay2d::VDelaunay2d()
{
}

void VDelaunay2d::Calculate (PointSet2d & allpoints , TriangleMesh2d & result)
{
	int vertexno = allpoints.Size();
	VPoint2d *pa = MakePointArray (allpoints);
	Delaunay2D md( vertexno , pa );

	for ( int j=0; j<md.GetTriangleQuantity(); j++ )
	{
		int i1 = md.GetTriangle(j).m_aiVertex[0];
		int i2 = md.GetTriangle(j).m_aiVertex[1];
		int i3 = md.GetTriangle(j).m_aiVertex[2];
		
		if ( i1 < 0 || i1 >= vertexno
			|| i2 < 0 || i2 >= vertexno
			|| i3 < 0 || i3 >= vertexno )
			throw ( VException ( ( IDS_VDeExc),0));

		result.AddTriangle(pa[i1],pa[i2],pa[i3]);
	}
	delete [] pa;
}

void VDelaunay2d::Calculate (PointSet2d & allpoints , RefTriangleMesh &result)
{
	int vertexno = allpoints.Size();
	VPoint2d * pa = MakePointArray (allpoints);
	Delaunay2D md (vertexno , pa);

	for ( int j=0; j<md.GetTriangleQuantity(); j++ )
	{
		int i1 = md.GetTriangle(j).m_aiVertex[0];
		int i2 = md.GetTriangle(j).m_aiVertex[1];
		int i3 = md.GetTriangle(j).m_aiVertex[2];
		RefTriangle newref (i1,i2,i3);
		if ( i1 < 0 || i1 >= vertexno
			|| i2 < 0 || i2 >= vertexno
			|| i3 < 0 || i3 >= vertexno )
			throw ( VException ( ( IDS_VDeExc),0));

		result.AddTriangle(newref);
	}
	delete [] pa;
}

//void VDelaunay2d::Calculate (std::vector<VPoint2d *> &vec , RefTriangleMesh &result)
//{
//	int vertexno = (int)(vec.size());
//	VPoint2d * pa = new VPoint2d [vertexno];
//	for ( int i=0; i<vertexno; i++ )
//		pa[i] = (*(vec[i]));
//
//	Delaunay2D md( vertexno , pa );
//
//	//int m_iTriangleQuantity;
//    //Triangle* m_akTriangle;
//
//	for ( int j=0; j<md.GetTriangleQuantity(); j++ )
//	{
//		int i1 = md.GetTriangle(j).m_aiVertex[0];
//		int i2 = md.GetTriangle(j).m_aiVertex[1];
//		int i3 = md.GetTriangle(j).m_aiVertex[2];
//		RefTriangle newref (i1,i2,i3);
//		if ( i1 < 0 || i1 >= vertexno
//			|| i2 < 0 || i2 >= vertexno
//			|| i3 < 0 || i3 >= vertexno )
//			throw ( VException ( ("VDelaunay exception"),0));
//
//		result.AddTriangle(newref);
//	}
//	delete [] pa;
//}

//VPoint2d * VDelaunay2d::MakePointArray (PointSet2d &allpoints , int &addedPointNo)
//{
//	int vertexno = allpoints.Size();
//	VPoint2d * pa = new VPoint2d [vertexno];
//	
//	PosCubes2d cubes (allpoints);
//	addedPointNo = 0;
//	vreal comptol = 1.e-8;
//
//	for ( int i=0; i<vertexno; i++ )
//	{	
//		VPoint2d *currp = allpoints[i];
//		LongVec relind , nii;
//		cubes.SetRelatedIndiceListForPoint (*currp , relind , nii);
//		bool coinc = false;
//		int mysize = relind.Size();
//		for (int j=0;j<mysize;j++)
//		{
//			int currind = relind[j];
//			if (currind >= i)//deal only with added points
//				continue;
//			VPoint2d *p = allpoints[currind];
//			if (currp->Compare (*p , comptol))
//			{
//				coinc = true;
//				break;
//			}
//		}
//		if (coinc == false)
//		{
//			pa[addedPointNo] = (*currp);
//			addedPointNo++;
//		}
//		
//	}
//	return pa;
//}	

VPoint2d * VDelaunay2d::MakePointArray (PointSet2d &allpoints)
{
	int vertexno = allpoints.Size();
	VPoint2d * pa = new VPoint2d [vertexno];	
	
	for ( int i=0; i<vertexno; i++ )
	{	
		VPoint2d *currp = allpoints[i];
		pa[i] = (*currp);
	}
	return pa;
}
bool vdelaunay_pcompare (VPoint2d &p1,VPoint2d &p2)
{
	if (p1.x()<p2.x())
		return true;
	else
		return false;
}
const vreal vdelaunay_duplicatetol = 1.e-6;
VPoint2d * VDelaunay2d::Calculate(std::list<VPoint2d> &plist
								  , RefTriangleMesh &result
								  , int &arraysize)
{
	// Making point array
	int vertexno = (int)plist.size();
	VPoint2d *pa = new VPoint2d [vertexno];
	std::list<VPoint2d>::iterator ii=plist.begin();
	std::list<VPoint2d>::iterator ee=plist.end();
	int counter=0;
	for (ii;ii!=ee;ii++)
	{
		pa[counter]=*ii;	
		++counter;
	}

	// Removing duplicate
	std::sort (pa,&pa[vertexno-1],vdelaunay_pcompare);
	VPoint2d *pas = new VPoint2d [vertexno];
	int vertexnos = 0;
	pas[0]=pa[0];
	++vertexnos;
	for (int i=0;i<vertexno-1;i++)
	{
		if (pa[i].Compare(pa[i+1],vdelaunay_duplicatetol)==false)
		{
			pas[vertexnos]=pa[i+1];
			++vertexnos;
		}
	}
	//Delaunay working
	Delaunay2D md (vertexnos , pas);

	for ( int j=0; j<md.GetTriangleQuantity(); j++ )
	{
		int i1 = md.GetTriangle(j).m_aiVertex[0];
		int i2 = md.GetTriangle(j).m_aiVertex[1];
		int i3 = md.GetTriangle(j).m_aiVertex[2];
		RefTriangle newref (i1,i2,i3);
		if ( i1 < 0 || i1 >= vertexno
			|| i2 < 0 || i2 >= vertexno
			|| i3 < 0 || i3 >= vertexno )
			throw ( VException ( ( IDS_VDeExc),0));

		result.AddTriangle(newref);
	}
	delete [] pa;
	arraysize = vertexnos;
	return pas;
}
