#include "stdafx.h"
#include "GhoshItem.h"
#include "PolarCalc2d.h"

GhoshItem::GhoshItem()
{

}
GhoshItem::GhoshItem(const GhoshItem& ghs)
{
	m_angle = ghs.m_angle;
	m_index = ghs.m_index;
	m_isTurningPoint = ghs.m_isTurningPoint;
	m_line = VLine(ghs.m_line);
	m_name = ghs.m_name;
	m_type = ghs.m_type;
}

GhoshItem::GhoshItem(const VLine& line, GhsType type, int index)
	:m_type(GhoshItem::Geo)
{
	m_line = line;
	m_index = index;
	m_isTurningPoint = false;
	m_type = type;
	VPoint v = line.GetV();
	m_angle = PolarCalc2d::GetPolarAngle(v.x(), v.y());
}
GhoshItem::~GhoshItem()
{

}

VLine GhoshItem::GetCurL()
{
	return m_line;
}
GhoshItem::GhsType GhoshItem::GetGType()
{
	return m_type;
}

int GhoshItem::GetIndex()
{
	return m_index;
}
vreal GhoshItem::GetAngle()
{
	return m_angle;
}
void GhoshItem::ReverseDirection(int dir)
{
	VPoint p1 = m_line.GetP1();
	VPoint p2 = m_line.GetP2();
	p1 *= dir;
	p2 *= dir;
	m_line.SetP1(p1);
	m_line.SetP2(p2);
	//if (dir == -1)
	//{
	//	m_line.Reverse();
	//}
}

void GhoshItem::Sort(std::vector<GhoshItem>& vec)
{
	std::sort(vec.begin(), vec.end(), GhoshItem::SortVec());
}
void GhoshItem::SetLine(const VLine& line)
{
	m_line = line;
}