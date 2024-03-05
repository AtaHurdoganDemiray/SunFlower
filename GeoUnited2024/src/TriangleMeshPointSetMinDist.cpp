// TriangleMeshPointSetMinDist.cpp

#include "stdafx.h"
#include "TriangleMeshPointSetMinDist.h"
#include "MultiTriangles.h"
#include "MultiPoints.h"
#include "PosCubes.h"
#include "TrianglePointMinDist.h"
#include "VLine.h"
#include "LineSet.h"

TriangleMeshPointSetMinDist::TriangleMeshPointSetMinDist (const MultiTriangles &triangles , const MultiPoints &points)
:m_triangles (&triangles) 
, m_points (&points) 
{
	VPoint overallmin (m_points->GetMin());
	VPoint overallmax (m_points->GetMax());
	overallmin.ReplaceMin (m_triangles->GetMin());
	overallmax.ReplaceMax (m_triangles->GetMax());

	m_pointsCubes = new PosCubes (overallmin , overallmax);
	m_trianglesCubes = new PosCubes (overallmin , overallmax);

	//filling triangle pos cubes
	for (int i=0; i<m_triangles->Size(); i++)
	{
		const VTriangleE &currTri = m_triangles->operator [](i);
		m_trianglesCubes->AddItem (currTri , i);
	}
	//filling points pos cubes
	for (int j=0;j<m_points->Size();j++)
	{
		const VPoint &currPoint =m_points->operator [](j);
		m_pointsCubes->AddItem (currPoint , j);
	}
}
TriangleMeshPointSetMinDist::~TriangleMeshPointSetMinDist ()
{
	delete m_pointsCubes;
	delete m_trianglesCubes;
}
void TriangleMeshPointSetMinDist::MinDistAllTrianglesAndAllPoints	(std::vector <vreal> &allmindist 
																	, LineSet &minlines)const
{
	std::vector<vreal> md(m_points->Size());
	std::vector<VLine> ml(m_points->Size());

	//const PosCubesCore &trianglesCore = m_trianglesCubes.GetCore();
	
	int pointsCubeCount = m_pointsCubes->GetCubeCount ();

	for (int i=0;i<pointsCubeCount;i++)
	{
		const LongVec & currCube = m_pointsCubes->GetCube (i);
		
		if (currCube.Size() > 0)
		{
			// Get related triangles with current cube
			VPoint currCubeCenter = m_pointsCubes->GetCenterPointOfCube (i);		
			LongVec relind;
			VPoint temp;
			m_trianglesCubes->GetRelatedIndicesForMinDist (currCubeCenter , relind , temp ,2);
			// For all points in curr cube
			for (int j=0; j<currCube.Size();j++)
			{
				int cpi = currCube[j];
				VPoint p = m_points->operator [](cpi);
				VPoint witnessPoint;
				vreal mindist = GetMinDist (p , relind , witnessPoint);
				md[cpi]=mindist;
				VLine minl(p , witnessPoint);
				ml[cpi]=minl;
			}
		}			
	}
	allmindist = md;
	minlines = ml;
}

vreal TriangleMeshPointSetMinDist::GetMinDist (const VPoint &p 
												, const LongVec &relTriangleInd 
												, VPoint &witnessPoint)const
{
	TrianglePointMinDist calc;
	VPoint pointontri;
	vreal minDist = MAX_NUMBER_LIMIT;

	for (int i=0;i<relTriangleInd.Size();i++)
	{
		int currInd = relTriangleInd[i];
		VTriangleE currtri = m_triangles->operator [] (currInd);
		vreal currDist = calc.MinDist (currtri , p , pointontri);
		if (currDist < minDist)
		{
			minDist = currDist;
			witnessPoint = pointontri;
		}
	}
	
	return minDist;
}

int TriangleMeshPointSetMinDist::GetMinDist (const VPoint &p 
												, const LongVec &relTriangleInd 
												, vreal &minDist
												, VPoint &witnessPoint)const
{
	TrianglePointMinDist calc;
	VPoint pointontri;
	minDist = MAX_NUMBER_LIMIT;
	int triRef = -1;

	for (int i=0;i<relTriangleInd.Size();i++)
	{
		int currInd = relTriangleInd[i];
		VTriangleE currtri = m_triangles->operator [] (currInd);
		vreal currDist = calc.MinDist (currtri , p , pointontri);
		if (currDist < minDist)
		{
			minDist = currDist;
			triRef = currInd;
			witnessPoint = pointontri;
		}
	}
	
	return triRef;
}

void TriangleMeshPointSetMinDist::MinDistAllTrianglesAndAllPoints (std::vector <vreal> &allmindist 
																   , std::vector<int> &triangleind
																   , std::vector<VPoint> &witnessPoints)const
{
	int mysize = m_points->Size();
	std::vector<vreal> md(mysize);

	LongVec relindall;
	for (int i=0;i<m_triangles->Size();++i)
		relindall.AddLong(i);

	for (int i=0;i<mysize;i++)
	{
		VPoint currp = m_points->operator [](i);
		LongVec relind;
		VPoint temp;
		//m_trianglesCubes->GetRelatedIndicesForMinDist (currp , relind , temp ,2);
		VPoint witnessPoint;
		vreal mindist;
		int minRef = GetMinDist (currp , relindall , mindist , witnessPoint);
		md[i]=mindist;
		triangleind.push_back (minRef);
		witnessPoints.push_back (witnessPoint);
	}			
	allmindist = md;
}
