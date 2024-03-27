// TriangleContainerBox.cpp

#include "stdafx.h"
#include "TriangleContainerBox.h"
#include "MultiTriangles.h"
#include "TriangleIntersection.h"

TriangleContainerBox::TriangleContainerBox()
{
	m_multiTri = 0;
	m_subBoxes2 = 0;
}
void TriangleContainerBox::Construct(MultiTriangles *multiTri, const std::list<int> &trianglesInside , const VPoint &min , const VPoint &max,vreal boxMaxLen)
{
	m_multiTri = multiTri;
	m_trianglesInside = trianglesInside;
	m_min = min;
	m_max = max;
	VPoint halfdelta = (m_max - m_min)*0.5;
	if (halfdelta.x() > boxMaxLen + EPSILON  && halfdelta.y() > boxMaxLen + EPSILON  && halfdelta.z() > boxMaxLen + EPSILON)
		DivideTo2(boxMaxLen);
}
MultiTriangles * TriangleContainerBox::GetMultiTriangles()
{
	return m_multiTri;
}
const MultiTriangles * TriangleContainerBox::GetMultiTriangles()const
{
	return m_multiTri;
}
std::list<int> & TriangleContainerBox::GetTrianglesInside()
{
	return m_trianglesInside;
}
const std::list<int> & TriangleContainerBox::GetTrianglesInside()const
{
	return m_trianglesInside;
}

void TriangleContainerBox::GetRelatedTriangles(const VPoint &boxmin , const VPoint &boxmax , const MultiTriangles *tri, const std::list<int> &initialIndexVec, std::list<int> &reducedResult)
{
	std::list<int>::const_iterator i = initialIndexVec.begin();
	std::list<int>::const_iterator e = initialIndexVec.end();
	for (i; i != e; ++i)
	{
		VTriangleE curTri = tri->operator[](*i);
		if (TriangleIntersection::IsTriCollidingToBox(curTri, boxmin, boxmax))
			reducedResult.push_back(*i);
	}
}

const VPoint & TriangleContainerBox::GetMin()const
{
	return m_min;
}
const VPoint & TriangleContainerBox::GetMax()const
{
	return m_max;
}
void TriangleContainerBox::DivideTo2(vreal boxMaxLen)
{
	m_subBoxes2 = new TriangleContainerBox[2];
	VPoint delta = (m_max - m_min);
	if (delta.x() >= delta.y() && delta.x() >= delta.z())
	{
		VPoint box1min(m_min);
		VPoint box1max(m_min.x() + delta.x()*0.5, m_max.y(), m_max.z());
		VPoint box2min(m_min.x() + delta.x()*0.5, m_min.y(), m_min.z());
		VPoint box2max(m_max);
		std::list<int> subRelBox1, subRelBox2;
		GetRelatedTriangles(box1min, box1max, m_multiTri, m_trianglesInside, subRelBox1);
		GetRelatedTriangles(box2min, box2max, m_multiTri, m_trianglesInside, subRelBox2);
		if (subRelBox1.begin() != subRelBox1.end())
			m_subBoxes2[0].Construct(m_multiTri, subRelBox1, box1min, box1max, boxMaxLen);
		if (subRelBox2.begin() != subRelBox2.end())
			m_subBoxes2[1].Construct(m_multiTri, subRelBox2, box2min, box2max, boxMaxLen);
	}
	else if (delta.y() >= delta.x() && delta.y() >= delta.z())
	{
		VPoint box1min(m_min);
		VPoint box1max(m_max.x(),m_min.y() + delta.y()*0.5, m_max.z());
		VPoint box2min(m_min.x(),m_min.y() + delta.y()*0.5, m_min.z());
		VPoint box2max(m_max);
		std::list<int> subRelBox1, subRelBox2;
		GetRelatedTriangles(box1min, box1max, m_multiTri, m_trianglesInside, subRelBox1);
		GetRelatedTriangles(box2min, box2max, m_multiTri, m_trianglesInside, subRelBox2);
		if (subRelBox1.begin() != subRelBox1.end())
			m_subBoxes2[0].Construct(m_multiTri, subRelBox1, box1min, box1max, boxMaxLen);
		if (subRelBox2.begin() != subRelBox2.end())
			m_subBoxes2[1].Construct(m_multiTri, subRelBox2, box2min, box2max,boxMaxLen);
	}
	else
	{
		VPoint box1min(m_min);
		VPoint box1max(m_max.x(),m_max.y(),m_min.z() + delta.z()*0.5);
		VPoint box2min(m_min.x(),m_min.y(), m_min.z() + delta.z()*0.5);
		VPoint box2max(m_max);
		std::list<int> subRelBox1, subRelBox2;
		GetRelatedTriangles(box1min, box1max, m_multiTri, m_trianglesInside, subRelBox1);
		GetRelatedTriangles(box2min, box2max, m_multiTri, m_trianglesInside, subRelBox2);
		if (subRelBox1.begin() != subRelBox1.end())
			m_subBoxes2[0].Construct(m_multiTri, subRelBox1, box1min, box1max, boxMaxLen);
		if (subRelBox2.begin() != subRelBox2.end())
			m_subBoxes2[1].Construct(m_multiTri, subRelBox2, box2min, box2max, boxMaxLen);
	}
}

void TriangleContainerBox::GetLastLevelContainerBoxes(std::vector<TriangleContainerBox *> &lastLevelBoxes)
{
	if (m_subBoxes2 != 0)
	{
		m_subBoxes2[0].GetLastLevelContainerBoxes(lastLevelBoxes);
		m_subBoxes2[1].GetLastLevelContainerBoxes(lastLevelBoxes);
	}
	else
	{
		if (m_multiTri != 0)
			lastLevelBoxes.push_back(this);
	}
}
