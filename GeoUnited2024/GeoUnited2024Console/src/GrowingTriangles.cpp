// GrowingTriangles.cpp

#include "stdafx.h"
#include "GrowingTriangles.h"
#include "HalfEdge.h"

#include "SequentialLineSet2d.h"

double GrowingTriangles::m_epsilon = 1.e-10;

GrowingTriangles::GrowingTriangles (SurfPointsPackBucketed &surfPack)
:m_surfPack (surfPack)
{

}
GrowingTriangles::~GrowingTriangles()
{
	DestroyHalfEdges();
}

void GrowingTriangles::DestroyHalfEdges()
{
	size_t mysize = m_halfEdges.size();
	for (size_t i=0;i<mysize;++i)
	{
		HalfEdge *he = m_halfEdges.at(i);
		delete he;
	}
}

HalfEdge * GrowingTriangles::GetPointLocation(int p , bool &isOnEdge , bool &isOnVertex)
{
	return 0;
}
void GrowingTriangles::MakeInitialSquareTris (const std::vector<vreal> &ugrid , const std::vector<vreal> &vgrid)	// The square is from 0,0 to 1,1
{

}
void GrowingTriangles::SplitOneTriWithPointInside (HalfEdge *e , int p)
{

}
void GrowingTriangles::SplitOneEdgeWithPointOn (HalfEdge *e , int p)
{

}
void GrowingTriangles::InsertPoint (int p)
{

}
void GrowingTriangles::InsertOuterTrimLoop(const SequentialLineSet2d &sl)
{

}
void GrowingTriangles::InsertInnerTrimLoop(const SequentialLineSet2d &sl)
{

}
