// PointList2d.cpp

#include "stdafx.h"
#include "PointList2d.h"
#include <algorithm>


void PointList2d::InsertPoint (const VPoint2d &newpoint)
{
	m_list.push_back(newpoint);
}

void PointList2d::InsertPoint (const vreal newpoint[])
{
	VPoint2d p(newpoint);
	m_list.push_back(p);
}

PointList2d::Itr PointList2d::Begin()
{
	return m_list.begin();
}

PointList2d::Itr PointList2d::End()
{
	return m_list.end();
}

void PointList2d::InsertBefore (Itr itr,const VPoint2d &newp)
{
	m_list.insert(itr,newp);
}

bool PointList2d::operator == (const PointList2d &toCompare)const
{
	if(this==&toCompare)
		return true;

	if(m_list!=toCompare.m_list)
		return false;

	return true;
}

bool PointList2d::operator != (const PointList2d &toCompare)const
{
	return !(this->operator ==(toCompare));
}

VPoint2d PointList2d::GetMin()const
{	
	VPoint2d mn (MAX_NUMBER_LIMIT);
	cItr i = m_list.begin();
	cItr e = m_list.end();
	for (i;i!=e;++i)
	{
		mn.ReplaceMin (*i);
	}
	return mn;
	//PointList2d::cItr minx=std::min_element(m_list.begin(),m_list.end(),isLessInX);	
	//PointList2d::cItr miny=std::min_element(m_list.begin(),m_list.end(),isLessInY);
	//VPoint2d result((*minx).x(),(*miny).y());
	//return result;
}

VPoint2d PointList2d::GetMax()const
{
	VPoint2d mx (MIN_NUMBER_LIMIT);
	cItr i = m_list.begin();
	cItr e = m_list.end();
	for (i;i!=e;++i)
	{
		mx.ReplaceMax (*i);
	}
	return mx;
	//PointList2d::cItr maxx=std::max_element(m_list.begin(),m_list.end(),isLessInX);	
	//PointList2d::cItr maxy=std::max_element(m_list.begin(),m_list.end(),isLessInY);
	//VPoint2d result((*maxx).x(),(*maxy).y());
	//return result;
}

void PointList2d::SortInX()
{
	std::map<double,VPoint2d> sm;
	cItr i = m_list.begin();
	cItr e = m_list.end();
	for (i;i!=e;++i)
	{
		sm.insert(std::pair<double,VPoint2d> (i->x(),*i));
	}
	std::map<double,VPoint2d>::const_iterator ii = sm.begin();
	std::map<double,VPoint2d>::const_iterator ee = sm.end();
	m_list.clear();
	for (ii;ii!=ee;++ii)
	{
		m_list.push_back (ii->second);
	}
}

void PointList2d::SortInY()
{
	std::map<double,VPoint2d> sm;
	cItr i = m_list.begin();
	cItr e = m_list.end();
	for (i;i!=e;++i)
	{
		sm.insert(std::pair<double,VPoint2d> (i->y(),*i));
	}
	std::map<double,VPoint2d>::const_iterator ii = sm.begin();
	std::map<double,VPoint2d>::const_iterator ee = sm.end();
	m_list.clear();
	for (ii;ii!=ee;++ii)
	{
		m_list.push_back (ii->second);
	}
}

