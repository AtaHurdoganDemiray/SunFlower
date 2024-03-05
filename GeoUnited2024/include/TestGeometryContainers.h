// TestGeometryContainers.h

#ifndef __TestGeometryContainers_h__
#define __TestGeometryContainers_h__

class TestGeometryContainers
{
public:
	TestGeometryContainers();
	void Testeqeq();
	void TesteqeqPointSet();
	void TesteqeqPointSet2d();
	void TesteqeqSequentialLineSet2d();
	void TesteqeqSequentialLineSet();
	void TesteqeqLineSet2d();
	void TesteqeqLineSet();
	void TesteqeqTriangleMesh2d();
	void TesteqeqTriangleMesh();
	void TesteqeqSurfPointsPack();
	void TesteqeqRefTriangleMesh();
	void TesteqeqVRefTriangleMesh();
	void CorrectNoPlusPlus();
	void ErrorNoPlusPlus();
	int m_errorno;
	int m_correctno;

	void TestPointList();
};

#endif
