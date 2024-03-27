// ImprovedTriangleBoxContainer.cpp

#include "stdafx.h"
#include "ImprovedTriangleContainerBox.h"

ImprovedTriangleContainerBox::ImprovedTriangleContainerBox()
{
	m_multiTri = 0;
}
void ImprovedTriangleContainerBox::Construct(MultiTriangles *multiTri, const std::list<int> &trianglesInside,vreal bestBoxSearchAngleStepRough,vreal bestBoxSearchAngleStepFine)
{
	m_multiTri = multiTri;
	m_trianglesInside = trianglesInside;
	// calculate the optimum enclosing box for the related triangles
	SearchBestBox(bestBoxSearchAngleStepRough,bestBoxSearchAngleStepFine);
}
const MultiTriangles * ImprovedTriangleContainerBox::GetMultiTri()const
{
	return m_multiTri;
}
MultiTriangles * ImprovedTriangleContainerBox::GetMultiTri()
{
	return m_multiTri;
}
const std::list<int> & ImprovedTriangleContainerBox::GetTrianglesInside()const
{
	return m_trianglesInside;
}
std::list<int> & ImprovedTriangleContainerBox::GetTrianglesInside()
{
	return m_trianglesInside;
}
const CoordinateSystem & ImprovedTriangleContainerBox::GetCs()const
{
	return m_cs;
}
const VPoint & ImprovedTriangleContainerBox::GetMinInCs()const
{
	return m_minInCs;
}
const VPoint & ImprovedTriangleContainerBox::GetMaxInCs()const
{
	return m_maxInCs;
}
const TransformationMatrix & ImprovedTriangleContainerBox::GetTm()const
{
	return m_tm;
}
void ImprovedTriangleContainerBox::SetTransformationMatrix(const TransformationMatrix &tm)
{
	m_tm = tm;
}
bool ImprovedTriangleContainerBox::IsIntersectingOnlyBox(const ImprovedTriangleContainerBox *other)const
{
	return false;
}
bool ImprovedTriangleContainerBox::IsIntersectingDetailed(const ImprovedTriangleContainerBox *other)const
{
	return false;
}
void ImprovedTriangleContainerBox::SearchBestBox(vreal angleStepRough,vreal angleStepFine)
{

}