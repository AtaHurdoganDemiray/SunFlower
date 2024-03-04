// DelRefMeshTest.cpp

#include "stdafx.h"
#include "DelRefMeshTest.h"
#include "DelRefMesh.h"
#include "VPoint2d.h"

void DelRefMeshTest::TestCreatingAMesh()
{
	std::vector<VPoint2d> points;
	points.push_back (VPoint2d (0,0));
	points.push_back (VPoint2d (1,0));
	points.push_back (VPoint2d (1,1));
	points.push_back (VPoint2d (0,1));
	points.push_back (VPoint2d (0.2,0.3));

	DelRefMesh mymesh (points);
	int i1[3] = {0,4,1}; 
	mymesh.Insert (i1);// it should reorder as 0,1,4
	int i2[3] = {3,0,4};
	mymesh.Insert (i2);// order is ok ,t1.n[2]=2,t2.n[1]=1 
}
