// TestGeometryContainers.cpp

#include "stdafx.h"
#include "TestGeometryContainers.h"
#include "PointSet.h"
#include "PointSet2d.h"
#include "SequentialLineSet2d.h"
#include "SequentialLineSet.h"
#include "LineSet.h"
#include "LineSet2d.h"
#include "TriangleMesh2d.h"
#include "TriangleMesh.h"
#include "SurfPointsPack.h"
#include "RefTriangleMesh.h"
#include "RefTriangle.h"
#include "VRefTriangleMesh.h"
#include "PointList.h"
#include "PointList2d.h"





TestGeometryContainers::TestGeometryContainers()
{
	m_errorno=0;
	m_correctno=0;
}

void TestGeometryContainers::Testeqeq()
{
	TesteqeqPointSet();
	TesteqeqPointSet2d();
	TesteqeqSequentialLineSet2d();
	TesteqeqSequentialLineSet();
	TesteqeqLineSet2d();
	TesteqeqLineSet();
	TesteqeqTriangleMesh2d();
	TesteqeqTriangleMesh();
	TesteqeqSurfPointsPack();
	TesteqeqRefTriangleMesh();
	TesteqeqVRefTriangleMesh();
}

void TestGeometryContainers::TesteqeqPointSet()
{
	PointSet ps1(4),ps2(5),ps3(4),ps4(5),ps6(4);
	if(ps1==ps1)
	{
		CorrectNoPlusPlus();;
	}
	else
	{
		ErrorNoPlusPlus();;
	}
	if(ps1!=ps1)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
		
	VPoint p0(0,0,0);
	VPoint p1(1,1,1);
	VPoint p2(2,2,2);
	VPoint p3(3,3,3);
	VPoint p4(4,4,4);

	//ps1==ps3,ps1!=ps6
	ps1.InsertPoint(p0);
	ps3.InsertPoint(p0);
	
	ps1.InsertPoint(p1);
	ps3.InsertPoint(p1);
	
	ps1.InsertPoint(p2);
	ps3.InsertPoint(p2);
	
	ps1.InsertPoint(p3);
	ps3.InsertPoint(p3);

	if(ps1!=ps3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1==ps3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}


	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);

	if(ps1==ps6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1!=ps6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	//ps2==ps4,ps2!=ps1
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);

	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);

	if(ps2==ps4)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2!=ps4)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps2!=ps1)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2==ps1)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
}

void TestGeometryContainers::TesteqeqPointSet2d()
{
	PointSet2d ps1(4),ps2(5),ps3(4),ps4(5),ps6(4);
	VPoint2d p0(0,0);
	VPoint2d p1(1,1);
	VPoint2d p2(2,2);
	VPoint2d p3(3,3);
	VPoint2d p4(4,4);

	//ps1==ps3,ps1!=ps6
	ps1.InsertPoint(p0);
	ps3.InsertPoint(p0);
	
	ps1.InsertPoint(p1);
	ps3.InsertPoint(p1);
	
	ps1.InsertPoint(p2);
	ps3.InsertPoint(p2);
	
	ps1.InsertPoint(p3);
	ps3.InsertPoint(p3);

	if(ps1!=ps3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1==ps3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}


	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);

	if(ps1==ps6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1!=ps6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	//ps2==ps4,ps2!=ps1
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);

	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	
	if(ps2==ps4)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2!=ps4)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps2!=ps1)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2==ps1)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
}

void TestGeometryContainers::TesteqeqSequentialLineSet2d()
{
	SequentialLineSet2d ps1(4),ps2(5),ps3(4),ps4(5),ps6(4);
	VPoint2d p0(0,0);
	VPoint2d p1(1,1);
	VPoint2d p2(2,2);
	VPoint2d p3(3,3);
	VPoint2d p4(4,4);

	//ps1==ps3,ps1!=ps6
	ps1.InsertPoint(p0);
	ps3.InsertPoint(p0);
	
	ps1.InsertPoint(p1);
	ps3.InsertPoint(p1);
	
	ps1.InsertPoint(p2);
	ps3.InsertPoint(p2);
	
	ps1.InsertPoint(p3);
	ps3.InsertPoint(p3);

	if(ps1!=ps3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1==ps3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}


	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);

	if(ps1==ps6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1!=ps6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	//ps2==ps4,ps2!=ps1
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);

	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	if(ps2==ps4)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2!=ps4)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps2!=ps1)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2==ps1)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
}

void TestGeometryContainers::TesteqeqSequentialLineSet()
{
	SequentialLineSet ps1(4),ps2(5),ps3(4),ps4(5),ps6(4);
	VPoint p0(0,0,0);
	VPoint p1(1,1,1);
	VPoint p2(2,2,2);
	VPoint p3(3,3,3);
	VPoint p4(4,4,4);

	//ps1==ps3,ps1!=ps6
	ps1.InsertPoint(p0);
	ps3.InsertPoint(p0);
	
	ps1.InsertPoint(p1);
	ps3.InsertPoint(p1);
	
	ps1.InsertPoint(p2);
	ps3.InsertPoint(p2);
	
	ps1.InsertPoint(p3);
	ps3.InsertPoint(p3);

	if(ps1!=ps3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1==ps3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}


	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);
	ps6.InsertPoint(p1);

	if(ps1==ps6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1!=ps6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	//ps2==ps4,ps2!=ps1
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);
	ps2.InsertPoint(p0);

	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	ps4.InsertPoint(p0);
	if(ps2==ps4)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2!=ps4)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps2!=ps1)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2==ps1)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
}

void TestGeometryContainers::TesteqeqLineSet()
{
	LineSet ps1(4),ps2(5),ps3(4),ps4(5),ps6(4);
	vreal a0[3]={0,0,0};
	vreal a1[3]={1,1,1};
	vreal a2[3]={2,2,2};
	vreal a3[3]={3,3,3};
	vreal a4[3]={4,4,4};

	VLine p0(a0,a1);
	VLine p1(a1,a2);
	VLine p2(a2,a3);
	VLine p3(a3,a4);
	VLine p4(a4,a0);

	//ps1==ps3,ps1!=ps6
	ps1.AddLine(p0);
	ps3.AddLine(p0);
	
	ps1.AddLine(p1);
	ps3.AddLine(p1);
	
	ps1.AddLine(p2);
	ps3.AddLine(p2);
	
	ps1.AddLine(p3);
	ps3.AddLine(p3);

	if(ps1!=ps3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1==ps3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}


	ps6.AddLine(p1);
	ps6.AddLine(p1);
	ps6.AddLine(p1);
	ps6.AddLine(p1);

	if(ps1==ps6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1!=ps6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	//ps2==ps4,ps2!=ps1
	ps2.AddLine(p0);
	ps2.AddLine(p0);
	ps2.AddLine(p0);
	ps2.AddLine(p0);
	ps2.AddLine(p0);

	ps4.AddLine(p0);
	ps4.AddLine(p0);
	ps4.AddLine(p0);
	ps4.AddLine(p0);
	ps4.AddLine(p0);
	if(ps2==ps4)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2!=ps4)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps2!=ps1)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2==ps1)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
}

void TestGeometryContainers::TesteqeqLineSet2d()
{
	LineSet2d ps1(4),ps2(5),ps3(4),ps4(5),ps6(4);
	vreal a0[2]={0,0};
	vreal a1[2]={1,1};
	vreal a2[2]={2,2};
	vreal a3[2]={3,3};
	vreal a4[2]={4,4};

	VLine2d p0(a0,a1);
	VLine2d p1(a1,a2);
	VLine2d p2(a2,a3);
	VLine2d p3(a3,a4);
	VLine2d p4(a4,a0);

	//ps1==ps3,ps1!=ps6
	ps1.AddLine(p0);
	ps3.AddLine(p0);
	
	ps1.AddLine(p1);
	ps3.AddLine(p1);
	
	ps1.AddLine(p2);
	ps3.AddLine(p2);
	
	ps1.AddLine(p3);
	ps3.AddLine(p3);

	if(ps1!=ps3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1==ps3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}


	ps6.AddLine(p1);
	ps6.AddLine(p1);
	ps6.AddLine(p1);
	ps6.AddLine(p1);

	if(ps1==ps6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1!=ps6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	//ps2==ps4,ps2!=ps1
	ps2.AddLine(p0);
	ps2.AddLine(p0);
	ps2.AddLine(p0);
	ps2.AddLine(p0);
	ps2.AddLine(p0);

	ps4.AddLine(p0);
	ps4.AddLine(p0);
	ps4.AddLine(p0);
	ps4.AddLine(p0);
	ps4.AddLine(p0);
	if(ps2==ps4)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2!=ps4)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps2!=ps1)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2==ps1)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
}

void TestGeometryContainers::TesteqeqTriangleMesh2d()
{
	TriangleMesh2d ps1(4),ps2(5),ps3(4),ps4(5),ps6(4);
	vreal a0[2]={0,0};
	vreal a1[2]={1,1};
	vreal a2[2]={2,2};
	vreal a3[2]={3,3};
	vreal a4[2]={4,4};

	VTriangle2dE p0(a0,a1,a2);
	VTriangle2dE p1(a1,a2,a3);
	VTriangle2dE p2(a2,a3,a4);
	VTriangle2dE p3(a3,a4,a1);
	VTriangle2dE p4(a4,a0,a1);

	//ps1==ps3,ps1!=ps6
	ps1.AddTriangle(p0);
	ps3.AddTriangle(p0);
	
	ps1.AddTriangle(p1);
	ps3.AddTriangle(p1);
	
	ps1.AddTriangle(p2);
	ps3.AddTriangle(p2);
	
	ps1.AddTriangle(p3);
	ps3.AddTriangle(p3);

	if(ps1!=ps3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1==ps3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}


	ps6.AddTriangle(p1);
	ps6.AddTriangle(p1);
	ps6.AddTriangle(p1);
	ps6.AddTriangle(p1);

	if(ps1==ps6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1!=ps6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	//ps2==ps4,ps2!=ps1
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);

	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);
	if(ps2==ps4)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2!=ps4)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps2!=ps1)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(ps2==ps1)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
}

void TestGeometryContainers::TesteqeqTriangleMesh()
{
	TriangleMesh ps1(4),ps2(5),ps3(4),ps4(5),ps6(4);
	vreal a0[3]={0,0,0};
	vreal a1[3]={1,2,1};
	vreal a2[3]={2,3,2};
	vreal a3[3]={3,4,3};
	vreal a4[3]={4,5,4};

	VTriangleE p0(a0,a1,a2);
	VTriangleE p1(a1,a2,a3);
	VTriangleE p2(a2,a3,a4);
	VTriangleE p3(a3,a4,a1);
	VTriangleE p4(a4,a0,a1);

	//ps1==ps3,ps1!=ps6
	ps1.AddTriangle(p0);
	ps3.AddTriangle(p0);
	
	ps1.AddTriangle(p1);
	ps3.AddTriangle(p1);
	
	ps1.AddTriangle(p2);
	ps3.AddTriangle(p2);
	
	ps1.AddTriangle(p3);
	ps3.AddTriangle(p3);

	if(ps1!=ps3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1==ps3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}


	ps6.AddTriangle(p1);
	ps6.AddTriangle(p1);
	ps6.AddTriangle(p1);
	ps6.AddTriangle(p1);

	if(ps1==ps6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1!=ps6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	//ps2==ps4,ps2!=ps1
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);

	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);

}

void TestGeometryContainers::TesteqeqSurfPointsPack()
{
	SurfPointsPack ps1(4),ps2(5),ps3(4),ps4(5),ps6(4);
	VPoint2d pt0(0,0);
	VPoint2d pt1(1,1);
	VPoint2d pt2(2,2);
	VPoint2d pt3(3,3);
	VPoint2d pt4(4,4);	
	
	VPoint p0(0,0,0);
	VPoint p1(1,1,1);
	VPoint p2(2,2,2);
	VPoint p3(3,3,3);
	VPoint p4(4,4,4);

	//ps1==ps3,ps1!=ps6
	ps1.InsertPoint(pt0,p0,p0);
	ps3.InsertPoint(pt0,p0,p0);
	
	ps1.InsertPoint(pt1,p1,p1);
	ps3.InsertPoint(pt1,p1,p1);
	
	ps1.InsertPoint(pt2,p2,p2);
	ps3.InsertPoint(pt2,p2,p2);
	
	ps1.InsertPoint(pt3,p3,p3);
	ps3.InsertPoint(pt3,p3,p3);

	if(ps1!=ps3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1==ps3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}


	ps6.InsertPoint(pt1,p1,p1);
	ps6.InsertPoint(pt1,p1,p1);
	ps6.InsertPoint(pt1,p1,p1);
	ps6.InsertPoint(pt1,p1,p1);

	if(ps1==ps6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1!=ps6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	//ps2==ps4,ps2!=ps1
	ps2.InsertPoint(pt0,p0,p0);
	ps2.InsertPoint(pt0,p0,p0);
	ps2.InsertPoint(pt0,p0,p0);
	ps2.InsertPoint(pt0,p0,p0);
	ps2.InsertPoint(pt0,p0,p0);

	ps4.InsertPoint(pt0,p0,p0);
	ps4.InsertPoint(pt0,p0,p0);
	ps4.InsertPoint(pt0,p0,p0);
	ps4.InsertPoint(pt0,p0,p0);
	ps4.InsertPoint(pt0,p0,p0);

}

void TestGeometryContainers::TesteqeqRefTriangleMesh()
{
	RefTriangleMesh ps1(4),ps2(5),ps3(4),ps4(5),ps6(4);

	RefTriangle p0(0,1,2);
	RefTriangle p1(1,2,3);
	RefTriangle p2(2,3,4);
	RefTriangle p3(3,4,1);
	RefTriangle p4(4,0,1);

	//ps1==ps3,ps1!=ps6
	ps1.AddTriangle(p0);
	ps3.AddTriangle(p0);
	
	ps1.AddTriangle(p1);
	ps3.AddTriangle(p1);
	
	ps1.AddTriangle(p2);
	ps3.AddTriangle(p2);
	
	ps1.AddTriangle(p3);
	ps3.AddTriangle(p3);

	if(ps1!=ps3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1==ps3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}

	ps6.AddTriangle(p1);
	ps6.AddTriangle(p1);
	ps6.AddTriangle(p1);
	ps6.AddTriangle(p1);

	if(ps1==ps6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(ps1!=ps6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	//ps2==ps4,ps2!=ps1
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);
	ps2.AddTriangle(p0);

	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);
	ps4.AddTriangle(p0);

}

void TestGeometryContainers::TesteqeqVRefTriangleMesh()
{
	VRefTriangleMesh pr1(4),pr2(5),pr3(4),pr4(5),pr6(4);

	//filling surfpointspack
	SurfPointsPack &ps1=pr1.surfpack;
	SurfPointsPack &ps2=pr2.surfpack;
	SurfPointsPack &ps3=pr3.surfpack;
	SurfPointsPack &ps4=pr4.surfpack;
	SurfPointsPack &ps6=pr6.surfpack;

	VPoint2d pt0(0,0);
	VPoint2d pt1(1,1);
	VPoint2d pt2(2,2);
	VPoint2d pt3(3,3);
	VPoint2d pt4(4,4);	
	
	VPoint p0(0,0,0);
	VPoint p1(1,1,1);
	VPoint p2(2,2,2);
	VPoint p3(3,3,3);
	VPoint p4(4,4,4);

	//ps1==ps3,ps1!=ps6
	ps1.InsertPoint(pt0,p0,p0);
	ps3.InsertPoint(pt0,p0,p0);
	
	ps1.InsertPoint(pt1,p1,p1);
	ps3.InsertPoint(pt1,p1,p1);
	
	ps1.InsertPoint(pt2,p2,p2);
	ps3.InsertPoint(pt2,p2,p2);
	
	ps1.InsertPoint(pt3,p3,p3);
	ps3.InsertPoint(pt3,p3,p3);

	ps1.InsertPoint(pt1,p1,p1);
	ps3.InsertPoint(pt1,p1,p1);

	ps6.InsertPoint(pt1,p1,p1);
	ps6.InsertPoint(pt1,p1,p1);
	ps6.InsertPoint(pt1,p1,p1);
	ps6.InsertPoint(pt1,p1,p1);
	ps6.InsertPoint(pt1,p1,p1);
	
	//ps2==ps4,ps2!=ps1
	ps2.InsertPoint(pt0,p0,p0);
	ps2.InsertPoint(pt0,p0,p0);
	ps2.InsertPoint(pt0,p0,p0);
	ps2.InsertPoint(pt0,p0,p0);
	ps2.InsertPoint(pt0,p0,p0);

	ps4.InsertPoint(pt0,p0,p0);
	ps4.InsertPoint(pt0,p0,p0);
	ps4.InsertPoint(pt0,p0,p0);
	ps4.InsertPoint(pt0,p0,p0);
	ps4.InsertPoint(pt0,p0,p0);
	
	RefTriangle f0(0,1,2);
	RefTriangle f1(1,2,3);
	RefTriangle f2(2,3,4);
	RefTriangle f3(3,4,1);
	RefTriangle f4(4,0,1);

	//ps1==ps3,ps1!=ps6
	pr1.AddTriangle(f0);
	pr3.AddTriangle(f0);
	
	pr1.AddTriangle(f1);
	pr3.AddTriangle(f1);
	
	pr1.AddTriangle(f2);
	pr3.AddTriangle(f2);
	
	pr1.AddTriangle(f3);
	pr3.AddTriangle(f3);

	pr6.AddTriangle(f1);
	pr6.AddTriangle(f1);
	pr6.AddTriangle(f1);
	pr6.AddTriangle(f1);
	
	pr2.AddTriangle(f0);
	pr2.AddTriangle(f0);
	pr2.AddTriangle(f0);
	pr2.AddTriangle(f0);
	pr2.AddTriangle(f0);

	pr4.AddTriangle(f0);
	pr4.AddTriangle(f0);
	pr4.AddTriangle(f0);
	pr4.AddTriangle(f0);
	pr4.AddTriangle(f0);


	if(pr1!=pr3)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(pr1==pr3)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(pr1==pr6)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	
	if(pr1!=pr6)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	
	if(pr2==pr4)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(pr2!=pr4)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
	if(pr2!=pr1)
	{
		CorrectNoPlusPlus();
	}
	else
	{
		ErrorNoPlusPlus();
	}
	if(pr2==pr1)
	{
		ErrorNoPlusPlus();
	}
	else
	{
		CorrectNoPlusPlus();
	}
}

void TestGeometryContainers::ErrorNoPlusPlus()
{
	m_errorno++;
}

void TestGeometryContainers::CorrectNoPlusPlus()
{
	m_correctno++;
}

//	TestGeometryContainers mytest;
//	mytest.Testeqeq();
//	CString mystr1;
//	mystr1.Format(("correctno/errorno %d / %d"),mytest.m_correctno,mytest.m_errorno);
//	VpAfxMessageBox(mystr1);

	//for debug
//	TestGeometryContainers mytest;
//	mytest.TestPointList();

void TestGeometryContainers::TestPointList()
{
	PointList mypl;
	VPoint p1(-1.,0.,0.);
	VPoint p2(0.,3.,7.);
	VPoint p3(-7.,2.,1.);
	mypl.InsertPoint(p1);
	mypl.InsertPoint(p2);
	mypl.InsertPoint(p3);
	VPoint listmin=mypl.GetMin();
	VPoint listmax=mypl.GetMax();
	
	mypl.SortInX();
	PointList::cItr i;
	PointList::cItr lend=mypl.End();
	for(i=mypl.Begin();i!=lend;i++)
	{
		const VPoint &currp=*i;
		int tmp=0;
	}

	mypl.SortInY();
	lend=mypl.End();
	for(i=mypl.Begin();i!=lend;i++)
	{
		const VPoint &currp=*i;
		int tmp=0;
	}
	
	lend=mypl.End();
	mypl.SortInZ();
	for(i=mypl.Begin();i!=lend;i++)
	{
		const VPoint &currp=*i;
		int tmp=0;
	}

	return;
}

