// AABB.cpp
#include "stdafx.h"
#include "AABB.h"
#include "VInterval.h"

void UpdateAABB (const VPoint &newp , AABB<3> &aabb)
{
	aabb.m_min.ReplaceMin (newp.GetArray());
	aabb.m_max.ReplaceMax (newp.GetArray());
}

void UpdateAABB (const VPoint2d &newp , AABB<2> &aabb)
{
	aabb.m_min.ReplaceMin (newp.GetArray());
	aabb.m_max.ReplaceMax (newp.GetArray());	
}

void UpdateAABB (const VLine &newl , AABB<3> &aabb)
{
	aabb.m_min.ReplaceMin (newl.GetP1().GetArray());
	aabb.m_min.ReplaceMin (newl.GetP2().GetArray());
	aabb.m_max.ReplaceMax (newl.GetP1().GetArray());
	aabb.m_max.ReplaceMax (newl.GetP2().GetArray());
}

void UpdateAABB (const VLine2d &newl , AABB<2> &aabb)
{
	aabb.m_min.ReplaceMin (newl.GetP1().GetArray());
	aabb.m_min.ReplaceMin (newl.GetP2().GetArray());
	aabb.m_max.ReplaceMax (newl.GetP1().GetArray());
	aabb.m_max.ReplaceMax (newl.GetP2().GetArray());
}

void UpdateAABB (const VTriangleE &newt , AABB<3> &aabb)
{
	aabb.m_min.ReplaceMin (newt.GetCorner1().GetArray());
	aabb.m_min.ReplaceMin (newt.GetCorner2().GetArray());
	aabb.m_min.ReplaceMin (newt.GetCorner3().GetArray());

	aabb.m_max.ReplaceMax (newt.GetCorner1().GetArray());
	aabb.m_max.ReplaceMax (newt.GetCorner2().GetArray());
	aabb.m_max.ReplaceMax (newt.GetCorner3().GetArray());
}

void UpdateAABB (const VTriangle2dE &newt , AABB<2> &aabb)
{
	aabb.m_min.ReplaceMin (newt.GetCorner1().GetArray());
	aabb.m_min.ReplaceMin (newt.GetCorner2().GetArray());
	aabb.m_min.ReplaceMin (newt.GetCorner3().GetArray());

	aabb.m_max.ReplaceMax (newt.GetCorner1().GetArray());
	aabb.m_max.ReplaceMax (newt.GetCorner2().GetArray());
	aabb.m_max.ReplaceMax (newt.GetCorner3().GetArray());
}