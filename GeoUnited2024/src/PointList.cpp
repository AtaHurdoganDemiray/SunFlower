// PointList.cpp

#include "stdafx.h"
#include "PointList.h"
#include "ListSerializer.h"
#include <algorithm>


void PointList::InsertPoint (const VPoint &newpoint)
{
	m_list.push_back(newpoint);
}

void PointList::InsertPoint (const vreal newpoint[])
{
	VPoint p(newpoint);
	m_list.push_back(p);
}

PointList::Itr PointList::Begin()
{
	return m_list.begin();
}

PointList::Itr PointList::End()
{
	return m_list.end();
}

void PointList::InsertBefore (Itr itr,const VPoint &newp)
{
	m_list.insert(itr,newp);
}

bool PointList::operator == (const PointList &toCompare)const
{
	if(this==&toCompare)
		return true;

	if(m_list!=toCompare.m_list)
		return false;

	return true;
}

bool PointList::operator != (const PointList &toCompare)const
{
	return !(this->operator ==(toCompare));
}

void PointList::Serialize (Bofstream &outfile)const
{
	SerializeTList(outfile,m_list);		
}

void PointList::Serialize (Bifstream &infile)
{
	SerializeTList(infile,m_list);
}

VPoint PointList::GetMin()const
{	
	//cItr minx=std::min_element(m_list.begin(),m_list.end(),isLessInX);	
	//cItr miny=std::min_element(m_list.begin(),m_list.end(),isLessInY);
	//cItr minz=std::min_element(m_list.begin(),m_list.end(),isLessInZ);
	//VPoint result((*minx).x(),(*miny).y(),(*minz).z());
	//return result;
	VPoint mn (MAX_NUMBER_LIMIT);
	cItr i = m_list.begin();
	cItr e = m_list.end();
	for (i;i!=e;++i)
	{
		mn.ReplaceMin (*i);
	}
	return mn;
}

VPoint PointList::GetMax()const
{
	//cItr maxx=std::max_element(m_list.begin(),m_list.end(),isLessInX);	
	//cItr maxy=std::max_element(m_list.begin(),m_list.end(),isLessInY);
	//cItr maxz=std::max_element(m_list.begin(),m_list.end(),isLessInZ);
	//VPoint result((*maxx).x(),(*maxy).y(),(*maxz).z());
	//return result;
	VPoint mx (MIN_NUMBER_LIMIT);
	cItr i = m_list.begin();
	cItr e = m_list.end();
	for (i;i!=e;++i)
	{
		mx.ReplaceMax (*i);
	}
	return mx;
}

void PointList::SortInX()
{
	//m_list.sort(isLessInX);
	cItr i= m_list.begin();
	cItr e = m_list.end();
	std::map<double,VPoint> sm;
	for (i;i!=e;++i)
	{
		sm.insert (std::pair<double,VPoint>(i->x(),*i));
	}
	m_list.clear();
	std::map<double,VPoint>::const_iterator ii = sm.begin();
	std::map<double,VPoint>::const_iterator ee = sm.end();
	for (ii;ii!=ee;++ii)
	{
		m_list.push_back (ii->second);
	}
}

void PointList::SortInY()
{
	//m_list.sort(isLessInY);
		cItr i= m_list.begin();
	cItr e = m_list.end();
	std::map<double,VPoint> sm;
	for (i;i!=e;++i)
	{
		sm.insert (std::pair<double,VPoint>(i->y(),*i));
	}
	m_list.clear();
	std::map<double,VPoint>::const_iterator ii = sm.begin();
	std::map<double,VPoint>::const_iterator ee = sm.end();
	for (ii;ii!=ee;++ii)
	{
		m_list.push_back (ii->second);
	}
}

void PointList::SortInZ()
{
	//m_list.sort(isLessInZ);	
	cItr i= m_list.begin();
	cItr e = m_list.end();
	std::map<double,VPoint> sm;
	for (i;i!=e;++i)
	{
		sm.insert (std::pair<double,VPoint>(i->z(),*i));
	}
	m_list.clear();
	std::map<double,VPoint>::const_iterator ii = sm.begin();
	std::map<double,VPoint>::const_iterator ee = sm.end();
	for (ii;ii!=ee;++ii)
	{
		m_list.push_back (ii->second);
	}
}
